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
				:  _mysqlH(NULL), _db(db), _server(server), _user(user), _password(password)
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
		Clear();
		_mysqlH = new mysqlpp::Connection(false);

		if (!_mysqlH->connect(_db.c_str(), _server.c_str(), _user.c_str(), _password.c_str()))
		{
			std::cerr<<IceUtil::Time::now()<<": Connected tracker - DB connection failed: " << _mysqlH->error() << std::endl;
		}
	}
	catch(...)
	{
		std::cerr<<IceUtil::Time::now()<<": unknown execption during DB connection" << std::endl;
	}
}

/***********************************************************
check login
return -1 if login incorrect - else return the user id
***********************************************************/
long DatabaseHandler::CheckLogin(const std::string & PlayerName, const std::string & Password)
{
	Lock sync(*this);
	if(!_mysqlH || !_mysqlH->connected())
	{
		Connect();
		if(!_mysqlH->connected())
		{
			std::cerr<<IceUtil::Time::now()<<": Connected tracker - CheckLoginfailed for user "<<PlayerName<<std::endl;
			Clear();
			return -1;
		}
	}

	mysqlpp::Query query(_mysqlH, false);
	query << "SELECT ju.id FROM jos_users ju, jos_comprofiler jc WHERE jc.confirmed = '1' AND ju.username COLLATE utf8_bin = '"<<PlayerName;
	query << "' AND ju.password = '"<<Password<<"' AND ju.id = jc.user_id";
	if (mysqlpp::StoreQueryResult res = query.store())
	{
		if(res.size() > 0)
		{
			long lbaid = -1;
			long juid = res[0][0];

			// check if user is already in the lbanet table
			query.clear();
			query << "SELECT id FROM lba_users WHERE josiid = '"<<juid<<"';
			if (res = query.store())
			{
				lbaid = res[0][0];

				//set the user as connected
				query.clear();
				query << "UPDATE lba_users SET lastconnected = UTC_TIMESTAMP(), connected = '1' WHERE id = '"<<lbaid<<"'";
				if(!query.exec())
					std::cerr<<IceUtil::Time::now()<<": Connected tracker - Update lastconnected failed for user id "<<lbaid<<" : "<<query.error()<<std::endl;
			}
			else
			{
				// if not exist then create it
				query.clear();
				query << "INSERT  INTO lba_users (josiid, lastconnected, connected) VALUES('"<<juid<<"', UTC_TIMESTAMP(), '1');
				if(!query.exec())
					std::cerr<<IceUtil::Time::now()<<": Connected tracker - Can not create new lba user entry for id "<<juid<<" : "<<query.error()<<std::endl;
				else
				{
					// get the id afterwards
					lbaid = query.insert_id();
				}

				//query.clear();
				//query << "SELECT id FROM lba_users WHERE josiid = '"<<juid<<"';
				//if (res = query.store())
				//{
				//	lbaid = res[0][0];	
				//}
				//else
				//{
				//	std::cerr<<IceUtil::Time::now()<<": Connected tracker - CheckLoginfailed on create for user "<<PlayerName<<" : "<<query.error()<<std::endl;
				//}
			
			} 

			return lbaid;
		}
	}

	std::cerr<<IceUtil::Time::now()<<": Connected tracker - CheckLoginfailed for user "<<PlayerName<<" : "<<query.error()<<std::endl;
	Clear();
	return -1;
}



/***********************************************************
set the user as disconnected in the database
***********************************************************/
void DatabaseHandler::DisconnectUser(long Id)
{
	Lock sync(*this);
	if(!_mysqlH || !_mysqlH->connected())
	{
		Connect();
		if(!_mysqlH->connected())
		{
			std::cerr<<IceUtil::Time::now()<<": Connected tracker - Update DisconnectUser failed for user id "<<Id<<std::endl;
			Clear();
			return;
		}
	}

	mysqlpp::Query query(_mysqlH, false);
	query << "UPDATE lba_users SET playedtimemin = playedtimemin + TIMESTAMPDIFF(MINUTE, lastconnected, UTC_TIMESTAMP()), connected = '0' WHERE id = '"<<Id<<"'";
	if(!query.exec())
	{
		std::cerr<<IceUtil::Time::now()<<": Connected tracker - Update timeplayed failed for user id "<<Id<<" : "<<query.error()<<std::endl;
		Clear();
	}
}



/***********************************************************
clear db connection
***********************************************************/
void DatabaseHandler::Clear()
{
	if(_mysqlH)
	{
		if(_mysqlH->connected())
			_mysqlH->disconnect();

		delete _mysqlH;
		_mysqlH = NULL;
	}
}
