//
//  XXDbStatus.cpp
//  XXOtl
//
//  Created by Javier Fuchs on 1/1/11.
//  Copyright (c) 2013 [Your company name here]. All rights reserved.
//

#include <assert.h>
#include "XXLog.h"
#include "XXString.h"
#include "XXStatus.h"
#include "XXSome.h"
#include "XXDbConnect.h"
#include "XXDbSome.h"
#include "XXDbStatus.h"

XXDbStatus::XXDbStatus() {}

XXDbStatus::~XXDbStatus() {}


bool XXDbStatus::select(XXDbConnect *conn, XXStatus::Vector& v)
{
    dbg << endl;
    bool rta = false;
    try {
		odbc::otl_stream sel; 

		XXString select;
		select = "SELECT id, note FROM status";
		dbg << "Select query = [" << select << ']' << endl;
		sel.open(1, select.c_str(), conn->db);

		while (!sel.eof()) {
			XXStatus status;

			sel >> status;
			v.push_back(status);

			rta = true;
		}
    } catch (odbc::otl_exception e) {
		XX_OTL_EXCEPTION;
		rta = false;
    }
    return rta;
}



bool XXDbStatus::insert(XXSome& some, XXStatus& status) 
{
	dbg << endl;
	bool rta = false;
	try {
		XXDbConnect *conn = XXDbConnect::Instance();
		odbc::otl_stream ins;

		XXString insert;
		insert = "INSERT INTO some_status ";
		insert += " ("
			 "status_id," // INTEGER NOT NULL,
			 "some_id" // INTEGER NOT NULL,
			") VALUES ("
			 ":status_id<int>," // INTEGER NOT NULL,
			 ":some_id<int>)"; // INTEGER NOT NULL,

		dbg << "Insert query = [" << insert << ']' << endl;
		ins.open(1, insert.c_str(), conn->db);

		ins << (int) status.id()
			<< (int) some.id();

		dbg << "Insert relacion some " << some << ", con status: " << status << endl;

		// Hizo algun insert?
		if (ins.get_rpc() != 0) {
			rta = true;
		}
		conn->db.commit();
	} catch (odbc::otl_exception e) {
		XX_OTL_EXCEPTION;
		rta = false;
	}
	return rta;
}






bool XXDbStatus::update(XXSome& some, XXStatus& status, XXString& table)
{
	dbg << endl;
	bool rta = false;
	try {
		XXDbConnect *conn = XXDbConnect::Instance();
		odbc::otl_stream upd;

		XXString update;
		update = "UPDATE ";
		update += table;
		update += "_status ";
		update += " SET "
		  		  "status_id = :status_id<int> "
				  "WHERE some_id = :some_id<int>";

		dbg << "update query = [" << update << ']' << endl;
		upd.open(1, update.c_str(), conn->db);

		upd << (int) status.id()
			<< (int) some.id();

		dbg << "Update some " << some << ", con status: " << status << endl;

		// Hizo algun insert?
		if (upd.get_rpc() != 0) {
			rta = true;
		}
		conn->db.commit();
	} catch (odbc::otl_exception e) {
		XX_OTL_EXCEPTION;
		rta = false;
	}
	return rta;
}

