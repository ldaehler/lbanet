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


#include "ConnectedTrackerServant.h"
#include <algorithm>


/***********************************************************
constructor
***********************************************************/
ConnectedTrackerServant::ConnectedTrackerServant(const Ice::CommunicatorPtr& communicator, 
													SharedData * shd, 
													DatabaseHandler &dbh)
: _communicator(communicator), _shd(shd), _dbh(dbh)
{

}

/***********************************************************
add a connected person
***********************************************************/
Ice::Long ConnectedTrackerServant::Connect(const std::string& Nickname, const Ice::Current&)
{
	return _shd->GetId(Nickname);
}

/***********************************************************
remove a disconnected person
***********************************************************/
void ConnectedTrackerServant::Disconnect(Ice::Long Id, const Ice::Current&)
{
	_shd->Disconnect(Id);
	_dbh.DisconnectUser(Id);
}

/***********************************************************
get list of connected people
***********************************************************/
LbaNet::ConnectedL ConnectedTrackerServant::GetConnected(const Ice::Current&)
{
	return _shd->GetConnected();
}


/***********************************************************
change status of one player
***********************************************************/
void ConnectedTrackerServant::ChangeStatus(const std::string& Nickname, 
												   const std::string& NewStatus, const Ice::Current&)
{
	_shd->ChangeStatus(Nickname, NewStatus);
}


/***********************************************************
change player name display color
***********************************************************/
void ConnectedTrackerServant::ChangeNameColor(const std::string& Nickname, const std::string& Color, 
												const Ice::Current&)
{
	_shd->ChangeNameColor(Nickname, Color);
}