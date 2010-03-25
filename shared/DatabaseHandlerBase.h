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


#if !defined(__Agent_database_handler_base_h)
#define __Agent_database_handler_base_h

#include <string>
#include <vector>
#include <map>


/***********************************************************************
 * Module:  DatabaseHandlerBase.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:59:17
 * Purpose: Declaration of the class DatabaseHandlerBase
 ***********************************************************************/
class DatabaseHandlerBase
{
public:
	//! default construtor
	DatabaseHandlerBase(){}


	//! destrutor
	virtual ~DatabaseHandlerBase(){}

	//! check login
	//! return -1 if login incorrect - else return the user id
	virtual long CheckLogin(const std::string & PlayerName, const std::string & Password)=0;


	//! set the user as connected in the database
	virtual void SetUserConnected(long Id)=0;

	//! set the user as disconnected in the database
	virtual void DisconnectUser(long Id)=0;


    // add friend function
    virtual void AddFriend(long myId, const std::string&  name)=0;

    // remove friend function
	virtual void RemoveFriend(long myId, const std::string&  name)=0;

    // get friends function
	virtual std::vector<std::string> GetFriends(long myId)=0;

};

#endif