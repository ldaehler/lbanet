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
#include "PhysXEngine.h"
#include "ActorObject.h"

/************************************************************************/
/* constructor                                        
/************************************************************************/
ServerMapManager::ServerMapManager(ZCom_Control *_control, unsigned int ZoidLevel, const MapInfo & MapInfo,
										boost::shared_ptr<PhysXEngine> pengine)
: _controler(_control), _zoidlevel(ZoidLevel), _needdelete(false), _pengine(pengine), 
	_mapobject(_control, MapInfo), _mapname(MapInfo.Name)
{
	std::stringstream strs;
	strs<<"Map instance created for map: "<<_mapname;
	LogHandler::getInstance()->LogToFile(strs.str(), 2);    

	std::map<long, ObjectInfo>::const_iterator itmap = MapInfo.Actors.begin();
	std::map<long, ObjectInfo>::const_iterator endmap = MapInfo.Actors.end();

	//create actors
	for(;itmap != endmap; ++itmap)
		_actors[itmap->second.GetId()] = new ActorObject(_control, _zoidlevel, 0, itmap->second, this);
}



/************************************************************************/
/* destructor                                      
/************************************************************************/
ServerMapManager::~ServerMapManager()
{
	std::stringstream strs;
	strs<<"Map instance deleted for map: "<<_mapname;
	LogHandler::getInstance()->LogToFile(strs.str(), 2);    
}


/************************************************************************/
/* process server internal stuff                                  
/************************************************************************/
void ServerMapManager::Process()
{
	//process map info
	_mapobject.Process();

	//process actors
	std::map<long, ActorObject *>::iterator itmap = _actors.begin();
	std::map<long, ActorObject *>::iterator endmap = _actors.end();
	for(;itmap != endmap; ++itmap)
		itmap->second->Process();
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


/************************************************************************/
/* add an actor object callback                                  
/************************************************************************/
boost::shared_ptr<PhysicalObjectHandlerBase>
			ServerMapManager::AddObject(unsigned int id, const ObjectInfo &desc, bool IsMainPlayer)
{
	return desc.BuildSelfServer(_pengine);
}
