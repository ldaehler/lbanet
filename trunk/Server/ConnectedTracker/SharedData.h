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


#if !defined(__Agent_shared_data_h)
#define __Agent_shared_data_h

#include <string>
#include <vector>
#include <map>

#include <IceUtil/Mutex.h>
#include <IceUtil/Monitor.h>
#include <Ice/Config.h>

/***********************************************************************
 * Module:  SharedData.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:59:17
 * Purpose: Declaration of the class SharedData
 ***********************************************************************/
class SharedData : public IceUtil::Mutex
{
public:
	//! default construtor
	SharedData();

	//! get player id
	Ice::Long GetId(const std::string & PlayerName);

	//! check if user already logged in
	//! if not log him in
	bool TryLogin(const std::string & PlayerName);

	//! get connected list
	const std::map<std::string, Ice::Long> & GetConnected();

	//! disconnect player
	bool Disconnect(Ice::Long playerid);

protected:
	SharedData(const SharedData &);
	const SharedData & operator=(const SharedData &);

private:
	Ice::Long								m_current_id;
	std::map<std::string, Ice::Long>		m_connected_users;

};

#endif