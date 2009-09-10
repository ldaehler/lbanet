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


#include "SharedData.h"


/***********************************************************
constructor
***********************************************************/
SharedData::SharedData()
: m_current_id(1)
{

}

/***********************************************************
get player id
***********************************************************/
Ice::Long SharedData::GetId(const std::string & PlayerName)
{
	Lock sync(*this);
	LbaNet::ConnectedL::iterator it = m_connected_users.find(PlayerName);
	if(it != m_connected_users.end())
		return -1;

	LbaNet::PlayerInfo pi;
	pi.Id = m_current_id;
	m_connected_users[PlayerName] = pi;
	++m_current_id;

	std::cout<<IceUtil::Time::now().toDateTime()<<": "<<PlayerName<<" connected"<<std::endl;

	return pi.Id;
}

/***********************************************************
check if user already logged in
if not log him in
***********************************************************/
bool SharedData::TryLogin(const std::string & PlayerName)
{
	Lock sync(*this);
	LbaNet::ConnectedL::iterator it = m_connected_users.find(PlayerName);
	if(it != m_connected_users.end())
		return false;


	return true;
}

/***********************************************************
get connected list
***********************************************************/
const LbaNet::ConnectedL & SharedData::GetConnected()
{
	Lock sync(*this);
	return m_connected_users;
}

/***********************************************************
disconnect player
***********************************************************/
bool SharedData::Disconnect(Ice::Long playerid)
{
	Lock sync(*this);

	LbaNet::ConnectedL::iterator it = m_connected_users.begin();
	LbaNet::ConnectedL::iterator end = m_connected_users.end();

	for(; it != end; ++it)
	{
		if(it->second.Id == playerid)
		{
			std::cout<<IceUtil::Time::now().toDateTime()<<": "<<it->first<<" disconnected"<<std::endl;
			m_connected_users.erase(it);
			return true;
		}
	}

	return false;
}


/***********************************************************
change player status
***********************************************************/
void SharedData::ChangeStatus(const std::string& Nickname, const std::string& NewStatus)
{
	Lock sync(*this);
	LbaNet::ConnectedL::iterator it = m_connected_users.find(Nickname);
	if(it != m_connected_users.end())
		it->second.Status = NewStatus;
}