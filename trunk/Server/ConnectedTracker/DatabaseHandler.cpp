/*
------------------------[ Lbanet Source ]-------------------------
Copyright (C) 2009
Author: Vivien Delage [Rincevent_123]
Email : vdelage@gmail.com

-------------------------------[ GNU License ]-------------------------------

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

-----------------------------------------------------------------------------
*/


#include "DatabaseHandler.h"
#include <IceUtil/Time.h>


/***********************************************************
constructor
***********************************************************/
DatabaseHandler::DatabaseHandler(const std::string db, const std::string server,
									const std::string user, const std::string password)
				:  _mysqlH(false), _db(db), _server(server), _user(user), _password(password)
{
	Connect();
}


/***********************************************************
connect to database
***********************************************************/
void DatabaseHandler::Connect()
{
	try
	{
		if (!_mysqlH.connect(_db.c_str(), _server.c_str(), _user.c_str(), _password.c_str()))
		{
			std::cerr<<IceUtil::Time::now()<<": Connected tracker - DB connection failed: " << _mysqlH.error() << std::endl;
		}
	}
	catch(...){}
}

/***********************************************************
check login
return -1 if login incorrect - else return the user id
***********************************************************/
long DatabaseHandler::CheckLogin(const std::string & PlayerName, const std::string & Password)
{
	Lock sync(*this);
	if(!_mysqlH.connected())
	{
		Connect();
		if(!_mysqlH.connected())
		{
			std::cerr<<IceUtil::Time::now()<<": Connected tracker - CheckLoginfailed for user "<<PlayerName<<std::endl;
			return -1;
		}
	}

	mysqlpp::Query query(const_cast<mysqlpp::Connection *>(&_mysqlH), false);
	query << "SELECT id FROM users WHERE status = '1' AND username COLLATE utf8_bin = '"<<PlayerName;
	query << "' AND password = '"<<Password<<"'";
	if (mysqlpp::StoreQueryResult res = query.store())
	{
		if(res.size() > 0)
		{
			//set the user as connected
			query.clear();
			query << "UPDATE users SET lastconnected = UTC_TIMESTAMP(), connected = '1' WHERE id = '"<<res[0][0]<<"'";
			if(!query.exec())
				std::cerr<<IceUtil::Time::now()<<": Connected tracker - Update lastconnected failed for user id "<<res[0][0]<<" : "<<query.error()<<std::endl;
			return res[0][0];
		}
	}

	std::cerr<<IceUtil::Time::now()<<": Connected tracker - CheckLoginfailed for user "<<PlayerName<<" : "<<query.error()<<std::endl;
	if(_mysqlH.connected())
		_mysqlH.disconnect();

	return -1;
}



/***********************************************************
set the user as disconnected in the database
***********************************************************/
void DatabaseHandler::DisconnectUser(long Id)
{
	Lock sync(*this);
	if(!_mysqlH.connected())
	{
		Connect();
		if(!_mysqlH.connected())
		{
			std::cerr<<IceUtil::Time::now()<<": Connected tracker - Update DisconnectUser failed for user id "<<Id<<std::endl;
			return;
		}
	}

	mysqlpp::Query query(const_cast<mysqlpp::Connection *>(&_mysqlH), false);
	query << "UPDATE users SET playedtimemin = playedtimemin + TIMESTAMPDIFF(MINUTE, lastconnected, UTC_TIMESTAMP()), connected = '0' WHERE id = '"<<Id<<"'";
	if(!query.exec())
	{
		std::cerr<<IceUtil::Time::now()<<": Connected tracker - Update timeplayed failed for user id "<<Id<<" : "<<query.error()<<std::endl;
		if(_mysqlH.connected())
			_mysqlH.disconnect();
	}
}
