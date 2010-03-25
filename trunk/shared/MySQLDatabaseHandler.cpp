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


#include "MySQLDatabaseHandler.h"
#include "LogHandler.h"

#include <mysql++.h>


/***********************************************************
constructor
***********************************************************/
MySQLDatabaseHandler::MySQLDatabaseHandler(const std::string &db, const std::string &server,
											const std::string &user, const std::string &password)
				:  _mysqlH(NULL), _db(db), _server(server), _user(user), _password(password)
{
	_mysqlH = new mysqlpp::Connection(false);
	Connect();
}


/***********************************************************
destrutor
***********************************************************/
MySQLDatabaseHandler::~MySQLDatabaseHandler()
{
	if(_mysqlH)
	{
		if(_mysqlH->connected())
			_mysqlH->disconnect();

		delete _mysqlH;
	}
}


/***********************************************************
connect to database
***********************************************************/
void MySQLDatabaseHandler::Connect()
{
	try
	{
		if (!_mysqlH->connect(_db.c_str(), _server.c_str(), _user.c_str(), _password.c_str()))
		{	
			std::stringstream strs;
			strs<<"DB: Connected tracker - DB connection failed: " << _mysqlH->error();
			LogHandler::getInstance()->LogToFile(strs.str());  
		}
	}
	catch(...){}
}

/***********************************************************
check login
return -1 if login incorrect - else return the user id
***********************************************************/
long MySQLDatabaseHandler::CheckLogin(const std::string & PlayerName, const std::string & Password)
{
	boost::mutex::scoped_lock lock(m_mutex);

	if(!_mysqlH->connected())
	{
		Connect();
		if(!_mysqlH->connected())
		{
			std::stringstream strs;
			strs<<"DB: Connected tracker - CheckLoginfailed for user "<<PlayerName;
			LogHandler::getInstance()->LogToFile(strs.str());  
			return -1;
		}
	}

	mysqlpp::Query query(const_cast<mysqlpp::Connection *>(_mysqlH), false);
	query << "SELECT id FROM users WHERE status = '1' AND username COLLATE utf8_bin = '"<<PlayerName;
	query << "' AND password = '"<<Password<<"'";
	if (mysqlpp::StoreQueryResult res = query.store())
	{
		if(res.size() > 0)
			return res[0][0];
		else
			return -1;
	}

	std::stringstream strs;
	strs<<"DB: Connected tracker - CheckLoginfailed for user "<<PlayerName<<" : "<<query.error();
	LogHandler::getInstance()->LogToFile(strs.str());  

	if(_mysqlH->connected())
		_mysqlH->disconnect();

	return -1;
}



/***********************************************************
set the user as disconnected in the database
***********************************************************/
void MySQLDatabaseHandler::SetUserConnected(long Id)
{
	boost::mutex::scoped_lock lock(m_mutex);

	if(!_mysqlH->connected())
	{
		Connect();
		if(!_mysqlH->connected())
		{
			std::stringstream strs;
			strs<<"DB: Connected tracker - CheckLoginfailed for user "<<Id;
			LogHandler::getInstance()->LogToFile(strs.str()); 
			return;
		}
	}

	mysqlpp::Query query(const_cast<mysqlpp::Connection *>(_mysqlH), false);
	query << "UPDATE users SET lastconnected = UTC_TIMESTAMP(), connected = '1' WHERE id = '"<<Id<<"'";	
	if(!query.exec())
	{
		std::stringstream strs;
		strs<<"DB: Connected tracker - Update connected failed for user id "<<Id<<" : "<<query.error();
		LogHandler::getInstance()->LogToFile(strs.str());  

		if(_mysqlH->connected())
			_mysqlH->disconnect();
	}
}

/***********************************************************
set the user as disconnected in the database
***********************************************************/
void MySQLDatabaseHandler::DisconnectUser(long Id)
{
	boost::mutex::scoped_lock lock(m_mutex);

	if(!_mysqlH->connected())
	{
		Connect();
		if(!_mysqlH->connected())
		{
			std::stringstream strs;
			strs<<"DB: Connected tracker - CheckLoginfailed for user "<<Id;
			LogHandler::getInstance()->LogToFile(strs.str()); 
			return;
		}
	}

	mysqlpp::Query query(const_cast<mysqlpp::Connection *>(_mysqlH), false);
	query << "UPDATE users SET playedtimemin = playedtimemin + TIMESTAMPDIFF(MINUTE, lastconnected, UTC_TIMESTAMP()), connected = '0' WHERE id = '"<<Id<<"'";
	if(!query.exec())
	{
		std::stringstream strs;
		strs<<"DB: Connected tracker - Update timeplayed failed for user id "<<Id<<" : "<<query.error();
		LogHandler::getInstance()->LogToFile(strs.str());  

		if(_mysqlH->connected())
			_mysqlH->disconnect();
	}
}



/***********************************************************
add friend function
***********************************************************/
void MySQLDatabaseHandler::AddFriend(long myId, const std::string&  name)
{
	boost::mutex::scoped_lock lock(m_mutex);

	if(!_mysqlH->connected())
	{
		Connect();
		if(!_mysqlH->connected())
		{
			std::stringstream strs;
			strs<<"DB: Connected tracker - AddFriend for user "<<myId;
			LogHandler::getInstance()->LogToFile(strs.str()); 	
			return;
		}
	}

	mysqlpp::Query query(const_cast<mysqlpp::Connection *>(_mysqlH), false);

	query << "SELECT id FROM users";
	query << " WHERE username = '"<<name<<"'";
	if (mysqlpp::StoreQueryResult res = query.store())
	{
		if(res.size() > 0)
		{
			query.clear();
			query << "INSERT INTO friends (userid, friendid) VALUES('";
			query << myId << "', '" << res[0][0] << "')";
			if(!query.exec())
			{
				std::stringstream strs;
				strs<<"DB: Update INSERT friends failed for user id  "<<myId<<" : "<<query.error();
				LogHandler::getInstance()->LogToFile(strs.str()); 	
			}
		}
	}
	else
	{
		if(_mysqlH->connected())
			_mysqlH->disconnect();
	}
}

/***********************************************************
remove friend function
***********************************************************/
void MySQLDatabaseHandler::RemoveFriend(long myId, const std::string&  name)
{
	boost::mutex::scoped_lock lock(m_mutex);

	if(!_mysqlH->connected())
	{
		Connect();
		if(!_mysqlH->connected())
		{
			std::stringstream strs;
			strs<<"DB: Connected tracker - RemoveFriend for user "<<myId;
			LogHandler::getInstance()->LogToFile(strs.str()); 	
			return;
		}
	}

	mysqlpp::Query query(const_cast<mysqlpp::Connection *>(_mysqlH), false);

	query << "SELECT id FROM users";
	query << " WHERE username = '"<<name<<"'";
	if (mysqlpp::StoreQueryResult res = query.store())
	{
		if(res.size() > 0)
		{
			query.clear();
			query << "DELETE FROM friends";
			query << " WHERE userid = '"<<myId<<"' AND friendid = '"<<res[0][0]<<"'";
			if(!query.exec())
			{
				std::stringstream strs;
				strs<<"DB: Update DELETE friends failed for user id  "<<myId<<" : "<<query.error();
				LogHandler::getInstance()->LogToFile(strs.str()); 	
			}
		}
	}
	else
	{
		if(_mysqlH->connected())
			_mysqlH->disconnect();
	}
}

/***********************************************************
get friends function
***********************************************************/
std::vector<std::string> MySQLDatabaseHandler::GetFriends(long myId)
{
	std::vector<std::string> resF;

	boost::mutex::scoped_lock lock(m_mutex);

	if(!_mysqlH->connected())
	{
		Connect();
		if(!_mysqlH->connected())
		{
			std::stringstream strs;
			strs<<"DB: Connected tracker - GetFriends for user "<<myId;
			LogHandler::getInstance()->LogToFile(strs.str()); 	
			return resF;
		}
	}

	mysqlpp::Query query(const_cast<mysqlpp::Connection *>(_mysqlH), false);

	query << "SELECT users.username FROM users, friends";
	query << " WHERE users.id = friends.friendid";
	query << " AND friends.userid = '"<<myId<<"'";
	if (mysqlpp::StoreQueryResult res = query.store())
	{
		if(res.size() > 0)
		{
			for(size_t i=0; i<res.size(); ++i)
				resF.push_back(res[i][0].c_str());
		}
	}
	else
	{
		std::stringstream strs;
		strs<<"DB: GetFriends failed for user id  "<<myId<<" : "<<query.error();
		LogHandler::getInstance()->LogToFile(strs.str()); 	

		if(_mysqlH->connected())
			_mysqlH->disconnect();
	}

	return resF;
}

