//
//  XXDbConnect.h
//  XXOtl
//
//  Created by Javier Fuchs on 1/1/11.
//  Copyright (c) 2013 [Your company name here]. All rights reserved.
//
#ifndef __XXDBCONNECT_H__
#define __XXDBCONNECT_H__

#ifdef OTL_ODBC_MYSQL
#include <iostream>
#include "XXString.h"


// Default "numeric" NULLs to (0)
#define OTL_DEFAULT_NUMERIC_NULL_TO_VAL (0)

#include "otlv4.h" // include the OTL 4 header file

class TypeXXDate;

// print out error message (e.msg)
// print out SQL that caused the error (e.stm_text)
// print out the variable that caused the error (e.var_info)
// sleep = 60 segunds.
#define XX_OTL_EXCEPTION												\
{																		\
	wrn << "code = " << e.code << nl									\
		<< "[OTL/MYSQL] error-msg > " << e.msg 						    \
		<< " - [ " << e.stm_text << " ]" 								\
		<< " - Var: " << e.var_info << endl; 							\
    if (e.code == 1012 || e.code == 1033 || e.code == 1034 || e.code == 1041 || e.code == 1089 || e.code == 1092 || e.code == 3112 || e.code == 3113 || e.code == 3114 || e.code == 7429 || e.code == 24324) {	\
	wrn << "Reconectandose" << endl;									\
	XXDbConnect *conn = XXDbConnect::Instance();	\
	conn->destroyInstance();											\
	sleep(XXDDbConnect::getSleepTime());						\ 
	conn = XXDbConnect::Instance();							\
    }																	\
	XXDbConnect::otlExceptionCode = e.code;                   \
	XXDbConnect::otlExceptionMsg.copy((char*)e.msg);          \
}
#endif

class XXDbConnect 
{
public:

    static XXDbConnect* Instance();
    static void set(char* user, char* password, char* server);
    static void destroyInstance ();

    static bool existTable(const char* tablename);
    static bool check();
    static bool reconnect();
    static int otlExceptionCode;
    static XXString otlExceptionMsg;

#ifdef OTL_ODBC_MYSQL
    static void add2Query(XXString& query, otl_value<otl_datetime>& otlDate, const TypeXXDate& brkDate, const char *pBindVar);
    odbc::otl_connect db;
#else
    void* db;
#endif

protected:
    XXDbConnect();
    ~XXDbConnect();
    XXDbConnect(const XXDbConnect&);
    XXDbConnect& operator= (const XXDbConnect&);
    static char* username;
    static char* password;
    static char* database;

private:
    static XXDbConnect* pinstance;

};

#endif // __XXDBCONNECT_H__
