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
#include "PlayerObject.h"


#define MAX_PLAYERS_IN_MAP 2


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
	{
		_actors[itmap->second.GetId()] = 
			boost::shared_ptr<ActorObject>(new ActorObject(_controler, _zoidlevel, 0, itmap->second, this));
	}
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
	{
		std::map<long, boost::shared_ptr<ActorObject> >::iterator itmap = _actors.begin();
		std::map<long, boost::shared_ptr<ActorObject> >::iterator endmap = _actors.end();
		for(;itmap != endmap; ++itmap)
			itmap->second->Process();
	}

	//process players
	{
		std::map<unsigned int, boost::shared_ptr<PlayerObject> >::iterator itmap = _players.begin();
		std::map<unsigned int, boost::shared_ptr<PlayerObject> >::iterator endmap = _players.end();
		for(;itmap != endmap; ++itmap)
			itmap->second->Process();
	}
}



/************************************************************************/
/* called when player enter the map                             
/************************************************************************/
void ServerMapManager::PlayerEnter(unsigned int PlayerId, boost::shared_ptr<PlayerInfoHandler> pinfo)
{
	//create player object info
	boost::shared_ptr<DisplayObjectDescriptionBase> Ds(new OsgOrientedCapsuleDescription(4, 0.5, 0.5, 0, 0, 1));
	boost::shared_ptr<DisplayTransformation> Tr(new DisplayTransformation());
	Tr->rotation = LbaQuaternion(90, LbaVec3(1, 0, 0));
	boost::shared_ptr<DisplayInfo> DInfo(new DisplayInfo(Tr, Ds));
	boost::shared_ptr<PhysicalDescriptionBase> Pyd(new PhysicalDescriptionCapsule(20, 30, 40, 4, 1, LbaQuaternion(), 0.5, 4));
	ObjectInfo objinfo(0, DInfo, Pyd, false, true);

	// create player object
	boost::shared_ptr<PlayerObject> newplayer(new PlayerObject(_controler, _zoidlevel, 0, objinfo, this));
	newplayer->GetNode()->setOwner(PlayerId, true);
	_players[PlayerId] = newplayer;
}

/************************************************************************/
/* called when player leave the map                         
/************************************************************************/
void ServerMapManager::PlayerLeave(unsigned int PlayerId)
{
	std::map<unsigned int, boost::shared_ptr<PlayerObject> >::iterator it =	_players.find(PlayerId);
	if(it != _players.end())
		_players.erase(it);
}



/************************************************************************/
/* return true if server is full                        
/************************************************************************/
bool ServerMapManager::IsFull()
{
	return (_players.size() >= MAX_PLAYERS_IN_MAP);
}


/************************************************************************/
/* add an actor object callback                                  
/************************************************************************/
boost::shared_ptr<PhysicalObjectHandlerBase>
			ServerMapManager::AddObject(unsigned int id, const ObjectInfo &desc, bool IsMainPlayer)
{
	return desc.BuildSelfServer(_pengine);
}
