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


/***********************************************************
constructor
***********************************************************/
DatabaseHandler::DatabaseHandler(const std::string db, const std::string server,
									const std::string user, const std::string password)
				: _connected(false), _mysqlH(false)
{
	if (_mysqlH.connect(db.c_str(), server.c_str(), user.c_str(), password.c_str()))
	{
		_connected = true;
	}
	else
	{
		std::cerr << "DB connection failed: " << _mysqlH.error() << std::endl;
		_connected = false;
	}
}

/***********************************************************
check login
return -1 if login incorrect - else return the user id
***********************************************************/
long DatabaseHandler::CheckLogin(const std::string & PlayerName, const std::string & Password) const
{
	if(!_connected)
		return -1;

	Lock sync(*this);

	//static long id = 1;
	//++id;
	//return id;

	mysqlpp::Query query(const_cast<mysqlpp::Connection *>(&_mysqlH), false);
	query << "SELECT id FROM users WHERE status = '1' AND username = '"<<PlayerName;
	query << "' AND password = '"<<Password<<"'";
	if (mysqlpp::StoreQueryResult res = query.store())
	{
		if(res.size() > 0)
		{
			//set the user as connected
			query.clear();
			query << "UPDATE users SET lastconnected = UTC_TIMESTAMP(), connected = '1' WHERE id = '"<<res[0][0]<<"'";
			if(!query.exec())
				std::cout<<"Connected tracker - Update lastconnected failed for user id "<<res[0][0]<<" : "<<query.error()<<std::endl;
			return res[0][0];
		}
	}

	return -1;
}



/***********************************************************
set the user as disconnected in the database
***********************************************************/
void DatabaseHandler::DisconnectUser(long Id)
{
	if(!_connected)
		return;

	Lock sync(*this);


	mysqlpp::Query query(const_cast<mysqlpp::Connection *>(&_mysqlH), false);
	query << "UPDATE users SET playedtimemin = playedtimemin + TIMESTAMPDIFF(MINUTE, lastconnected, UTC_TIMESTAMP()), connected = '0' WHERE id = '"<<Id<<"'";
	if(!query.exec())
		std::cout<<"Connected tracker - Update timeplayed failed for user id "<<Id<<" : "<<query.error()<<std::endl;

}
