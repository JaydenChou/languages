#!/bin/ksh
# Program:    configure_otl_postgresql.sh
# Type:       Shell Script
# Arguments:  -
# Obs.:       Install postgresql odbc + otl. Configure the conexion and test.
# Requisites: you must be root
#
# Aug/2008: Javier Fuchs                                    |\_/|   
#           Phone (+54 2944) 525173                        /     \  
#           http://southfox.servehttp.com                 /_.~ ~,_\ 
#                                                            \@/    

POSTGRES_USERNAME=postgres
POSTGRES_PASSWORD=postgres

otl()
{
	cd /tmp/
	wget http://otl.sourceforge.net/otlv4_h.zip
	unzip -o otlv4_h.zip
	rm otlv4_h.zip
	cd -
}


compileta()
{
otl

cat > /tmp/simple.cpp - <<BUFFER
#include <iostream>
#include <string>

#define OTL_STL // Turn on STL features and otl_value<T>
#include "otlv4.h" // include the OTL 4.0 header file


int main()
{
    	otl_connect db;
	try {
		otl_connect::otl_initialize(); // initialize OCI environment
		db.rlogon("${POSTGRES_USERNAME}/${POSTGRES_PASSWORD}@PostgreSQL");

		std::cout << "CONECTADO A DB" << '\n';

		otl_stream myselect; 

		char query[300];
		strcpy(query, "SELECT ");
		strcat(query, "TO_CHAR(now(),'yyyy-mm-dd hh24:mi:ss') ");

		myselect.open(1, query, db); // open del cursor

		while (!myselect.eof()) {
 			otl_value<std::string> Ahora;
			myselect >> Ahora; // fetch
			std::cout << "fecha hora es: " << Ahora << '\n';
		}

		db.logoff();

    	} catch (otl_exception e) {
		std::cerr << "[OTL/POSTGRESQL] error-msg > " << e.msg 	// print out error message
			<< " - [ " << e.stm_text << " ]" 		// print out SQL that caused the error
			<< " - Var: " << e.var_info << '\n'; 		// print out the vari 
		exit(1);
    	}

	exit(0);
}
BUFFER
	g++ -Wno-deprecated "-DOTL_ODBC_UNIX" "-DOTL_ODBC_POSTGRESQL" "-DOTL_UNCAUGHT_EXCEPTION_ON" -g -Wall -W -Wunused -I/tmp /tmp/simple.cpp /usr/lib/psqlodbcw.so -o /tmp/simple 
	if [ $? -ne 0 ]; then
		echo "$0: compilation FAIL";
		return 1;
	fi
	/tmp/simple
	if [ $? -eq 0 ]; then
		echo "$0: Example c++ otl + postresql + odbc works correctly. OK";
	else
		echo "$0: ERROR - Example c++ otl + postresql + odbc. FAIL";
		return 1;
	fi
	return 0;
}

install()
{
	# user root
	ID=`id -u`
	if [ ${ID} -ne 0 ]; then
		echo "$0: you must be root";
		return 1;
	fi

	# INSTALL LIBRARIES ODBC POSTGRESQL

	# check installed library
	rpm -q postgresql-odbc
	if [ $? -ne 0 ]; then
		# install
		yum install -y postgresql-odbc

		# verify installation
		rpm -q postgresql-odbc
		if [ $? -ne 0 ]; then
			echo "$0: postgresql-odbc cannot be installed. STFW, RTFM";
			return 1;
		fi
	fi
	#
	# Verifying link of postgresql odbc
	#
	cd /usr/lib
	# postgresql odbc must exist
	if [ ! -f psqlodbc.so ]; then
		echo "$0: libreria psqlodbc.so does not exist. Something fail in PostgreSQL installation or ODBC installation. STFW, RTFM";
		return 1;
	fi
	# Check if link exist, if not, create link
	if [ ! -L psqlodbcw.so ]; then
		ln -s psqlodbc.so psqlodbcw.so
		if [ $? -ne 0 ]; then
			echo "$0: cannot create a simple link.";
			return 1;
		fi
	fi

	# Generate postgresql ODBC Unix configuration (see http://www.unixodbc.org/odbcinst.html):
	echo "#PostgreSQL
[PostgreSQL]
Description             = ODBC for PostgreSQL
Driver          = /usr/lib/libodbcpsql.so
Setup           = /usr/lib/libodbcpsqlS.so
FileUsage               = 1
UsageCount              = 3" > /etc/odbcinst.ini

	# now create /etc/odbc.ini using odbcinst
	odbcinst -i -s -l -f /etc/odbcinst.ini
	# check if config file exist
	if [ ! -f /etc/odbc.ini ]; then
		echo "$0: el archivo /etc/odbc.ini no se pudo crear. Consulte web unixodbc";
		echo "$0: file /etc/odbc.ini cannot be created. Search unixodbc website";
		return 1;
	fi
	# Adding database, servername and username to /etc/odbc.ini
	echo "Database            = showposition" >> /etc/odbc.ini
	echo "Servername          = localhost" >> /etc/odbc.ini
	echo "UserName            = ${POSTGRES_USERNAME}" >> /etc/odbc.ini

	# Testing connect with OBC + Postgres:
	isql -v PostgreSQL<<BUFFER
select now();
quit;
BUFFER
	if [ $? -ne 0 ]; then
		echo "$0: isql FAIL. STFW, RTFM."
		return 1;
		# Expecting:
		# +---------------------------------------+
		# | Connected!                            |
		# |                                       |
		# | sql-statement                         |
		# | help [tablename]                      |
		# | quit                                  |
		# |                                       |
		# +---------------------------------------+
		# SQL> select now();
		# +--------------------+
		# | now                |
		# +--------------------+
		# | 2008-09-07 01:06:29|
		# +--------------------+
		# SQLRowCount returns 1
		# 1 rows fetched
	fi


	return 0;
}

main()
{
	echo "First Install";
	install;
	if [ $? -eq 0 ]; then
		echo "$0: INSTALL OK";
	else
		echo "$0: INSTALL ERROR. Some fail. Check";
		return 1;
	fi
	echo "Ahora ver que funciona la compilacion que nusa OTL + ODBCUnix + PostgreSQL";
	echo "Now, testing compilation using OTL + ODBCUnix + PostgreSQL";
	compileta;
	if [ $? -eq 0 ]; then
		echo "$0: COMPILE OK";
	else
		echo "$0: COMPILE ERROR. Some fail. Check";
		return 1;
	fi
	return 0;
}

main
if [ $? -eq 0 ]; then
	echo "$0: OK"
else
	echo "$0: ERROR. Some fail. Check";
fi
# eof
