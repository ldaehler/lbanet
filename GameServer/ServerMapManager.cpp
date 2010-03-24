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

#include <zoidcom.h>

#include "ServerMapManager.h"
#include "LogHandler.h"



/************************************************************************/
/* constructor                                        
/************************************************************************/
ServerMapManager::ServerMapManager(ZCom_Control *_control, unsigned int ZoidLevel, const MapInfo & MapInfo)
: _controler(_control), _zoidlevel(ZoidLevel), _needdelete(false)
{
}



/************************************************************************/
/* destructor                                      
/************************************************************************/
ServerMapManager::~ServerMapManager()
{

}


/************************************************************************/
/* process server internal stuff                                  
/************************************************************************/
void ServerMapManager::Process()
{

}



/************************************************************************/
/* called when player enter the map                             
/************************************************************************/
void ServerMapManager::PlayerEnter(unsigned int PlayerId, boost::shared_ptr<PlayerInfoHandler> pinfo)
{

}

/************************************************************************/
/* called when player leave the map                         
/************************************************************************/
void ServerMapManager::PlayerLeave(unsigned int PlayerId)
{

}




/************************************************************************/
/* return true if server is full                        
/************************************************************************/
bool ServerMapManager::IsFull()
{
	return false;
}