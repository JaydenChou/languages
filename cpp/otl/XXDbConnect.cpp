//
//  XXDbConnect.cpp
//  XXOtl
//
//  Created by Javier Fuchs on 1/1/11.
//  Copyright (c) 2013 [Your company name here]. All rights reserved.
//
#include "XXDbConnect.h"
#include "XXString.h"
#include "XXDate.h"
#include "XXLog.h"


/**
 * flag que indica conexion (static global) :)
 */
XXDbConnect* XXDbConnect::pinstance = 0;// inicializar un puntero
char *XXDbConnect::username = NULL;
char *XXDbConnect::password = NULL;
char *XXDbConnect::database = NULL;
int XXDbConnect::otlExceptionCode = 0;
XXString XXDbConnect::otlExceptionMsg = "";

/**
 * Crea una unica conexion a la DB, si no retorna la existente 
 * @return XXDbConnect*
 */
XXDbConnect* XXDbConnect::Instance () 
{
	if (pinstance == 0) { // es el primer llamado
		pinstance = new XXDbConnect(); // crear una sola instancia
	}
	return pinstance; // direccion e la instancia
}

/**
 * Destruye la instancia de mySqlcle
 */
void XXDbConnect::destroyInstance()
{
	delete pinstance;
	pinstance = 0L;
}


/**
 * Destructor. Se desconecta de la base
 */
XXDbConnect::~XXDbConnect() {
	slog.setStdOutput();
	dbg << endl;
	inf << "desconectandose!!" << '\n';
	try {
		db.logoff();
	} catch (odbc::otl_exception e) {
		slog.setStdOutput();
		err << "[OTL/MYSQL] error-msg > " << e.msg 	// print out error message
			<< " - [ " << e.stm_text << " ]" 		// print out SQL that caused the error
			<< " - Var: " << e.var_info << endl; 		// print out the vari 
	}
}


/**
 * Constructor. Se conecta a la base
 */
XXDbConnect::XXDbConnect()
{ 
	dbg << endl;
	assert(XXDbConnect::username != NULL);
	assert(XXDbConnect::password != NULL);

	XXString credentials;
	credentials = XXDbConnect::username;
	credentials += "/";
	credentials += XXDbConnect::password;
	if (XXDbConnect::database != NULL && strlen(XXDbConnect::database) != 0) {
		credentials += "@";
		credentials += XXDbConnect::database;
	}
	inf << "Conexion a DB: (" << credentials << ')' << endl;

	try {
		odbc::otl_connect::otl_initialize(); // initialize OCI environment
		db.rlogon(credentials.c_str());	// connect to Oracle
		inf << "CONECTADO A DB" << endl;
	} catch (odbc::otl_exception e) {
		slog.setStdOutput();
		slog[XXLog::LOGFULL];
		err << "NO SE PUDO CONECTAR A DB" << endl;
		err << "[OTL/MYSQL] error-msg > " << e.msg 	// print out error message
			<< " - [ " << e.stm_text << " ]" 		// print out SQL that caused the error
			<< " - Var: " << e.var_info << endl; 		// print out the vari 
	}
}

/**
 * Setea usuario y pass de mySqlcle
 * @param user
 * @param password
 * @param server
 */
void XXDbConnect::set(char* user, char* password, char* server) {
	dbg << endl;
	if (XXDbConnect::username == NULL) {
		XXDbConnect::username = new char[XXSTRINGMAX];
		memset(XXDbConnect::username, '\0', XXSTRINGMAX);
		strcpy(XXDbConnect::username, user);
	}
	if (XXDbConnect::password == NULL) {
		XXDbConnect::password = new char[XXSTRINGMAX];
		memset(XXDbConnect::password, '\0', XXSTRINGMAX);
		strcpy(XXDbConnect::password, password);
	}
	if (XXDbConnect::database == NULL) {
		XXDbConnect::database = new char[XXSTRINGMAX];
		memset(XXDbConnect::database, '\0', XXSTRINGMAX);
		strcpy(XXDbConnect::database, server);
	}
}

/**
 * Funcion para agregar una fecha a un query
 * @param query
 * @param otlDate
 * @param brkDate
 * @param pBindVar
 */
void XXDbConnect::add2Query(XXString& query,
									  otl_value<otl_datetime>& otlDate,
									  const TypeXXDate& brkDate,
									  const char *pBindVar) 
{
	dbg << endl;
	if (brkDate.isSysdate()) {
		query += "SYSDATE";
	} else {
		if (brkDate.isNull()) {
			otlDate = otl_null();
		} else {
			brkDate.toOtl(otlDate);
		}
		query += " :";
		query += pBindVar;
		query += "<timestamp>";
	}
}

/**
 * Busca la tabla en USER_TABLES
 * @param tablename
 * 
 * @return bool
 */
bool XXDbConnect::existTable(const char* tablename) {
	dbg << endl;
	bool rta = false;
	try {
		XXDbConnect *conn = XXDbConnect::Instance();
		odbc::otl_stream sel; 

		inf << "(tablename: " << tablename 
			<< ")" << endl;

		sel.open(1,
			 "SELECT COUNT(*) FROM USER_TABLES "
			 "WHERE TABLE_NXXE = :tablename<char[30]>",
			 conn->db);
		int count;

		sel << tablename;

		if (!sel.eof()) {
			sel >> count;
			inf << "hay: " << count << endl;
			if (count == 0) {
				rta = false;
			} else {
				rta = true;
			}
		}
	} catch (odbc::otl_exception e) {
		XX_OTL_EXCEPTION;
		rta = false;
	}
	return rta;
}


/**
 * Chequea la conexion, si no hay, se reconecta a la base de
 * datos
 * 
 * @return bool
 */
bool XXDbConnect::reconnect() {
	dbg << endl;
	bool rta = false;
	do {
		XXDbConnect *conn = XXDbConnect::Instance();
		if (conn->check() == true) {
			rta = true;
			break;
		}
		wrn << "Desconectando" << endl;
		conn->destroyInstance();
		wrn << "Conectando" << endl;
		conn = XXDbConnect::Instance();
		rta = conn->check();
	} while (false);
	
	return rta;
}




/**
 * Chequea la conexion y obtiene la fecha de la base: Si hay retorna true
 * 
 * @return bool
 */
bool XXDbConnect::check() {
	dbg << endl;
	bool rta = false;
	try {
		XXDbConnect *conn = XXDbConnect::Instance();
		odbc::otl_stream sel; 

		sel.open(1, "SELECT 1, NOW() FROM DUAL", conn->db);
		int count;
		otl_value<otl_datetime> otlNow;

		if (!sel.eof()) {
			sel >> count >> otlNow;
			inf << "hay: " << count << endl;
			inf << "fecha: " << otlNow << endl;
			if (count == 0) {
				rta = false;
			} else {
				rta = true;
			}
		}
	} catch (odbc::otl_exception e) {
		wrn << "code = " << e.code << nl
			<< "[OTL/MYSQL] error-msg > " << e.msg
			<< " - [ " << e.stm_text << " ]"
			<< " - Var: " << e.var_info << endl;
		rta = false;
	}
	return rta;
}

