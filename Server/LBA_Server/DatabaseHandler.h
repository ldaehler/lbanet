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


#if !defined(__Agent_database_handler_h)
#define __Agent_database_handler_h

#include <string>
#include <vector>
#include <map>

#include <IceUtil/Mutex.h>

#include <mysql++.h>

#include <ClientSession.h>


/***********************************************************************
 * Module:  DatabaseHandler.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:59:17
 * Purpose: Declaration of the class SharedData
 ***********************************************************************/
class DatabaseHandler : public IceUtil::Mutex
{
public:
	//! default construtor
	DatabaseHandler(const std::string db, const std::string server,
						const std::string user, const std::string password);

	// player has changed world
	LbaNet::SavedWorldInfo ChangeWorld(const std::string& NewWorldName, long PlayerId);

	// player update his current position in the world
	void UpdatePositionInWorld(const LbaNet::PlayerPosition& Position, 
								const std::string& WorldName,long PlayerId);

	// quit current world
	void QuitWorld(const std::string& LastWorldName,long PlayerId);

	// update player inventory structure
	void UpdateInventory(const LbaNet::InventoryInfo &Inventory, const std::string& WorldName,long PlayerId);


    // add friend function
    void AddFriend(long myId, const std::string&  name);

    // remove friend function
	void RemoveFriend(long myId, const std::string&  name);

    // get friends function
    LbaNet::FriendsSeq GetFriends(long myId);


    // store letter to the server and return the letter id
    long AddLetter(long myId, const std::string& title, const std::string& message);
    
    // return letter info
    LbaNet::LetterInfo GetLetterInfo(Ice::Long LetterId);


protected:
	DatabaseHandler(const DatabaseHandler &);
	const DatabaseHandler & operator=(const DatabaseHandler &);

	// connect to database
	void Connect();

private:
	// mysql connection handler
	mysqlpp::Connection	_mysqlH;

	std::string			_db;
	std::string			_server;
	std::string			_user;
	std::string			_password;
};

#endif