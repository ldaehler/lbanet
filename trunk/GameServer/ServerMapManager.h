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

#ifndef _LBA_NET_SERVER_MAP_MGT_H_
#define _LBA_NET_SERVER_MAP_MGT_H_

#include <map>
#include <boost/shared_ptr.hpp>

#include "MapInfo.h"
#include "PlayerInfoHandler.h"
#include "MapInfoObject.h"
#include "GameClientCallbackBase.h"


class ZCom_Control;
class PhysXEngine;
class ActorObject;
class PlayerObject;
class PlayerServerWrapper;


/***********************************************************************
 * Module:  ServerMapManager.h
 * Author:  vivien
 * Modified: mardi 14 juillet 2009 17:41:03
 * Purpose: Class ServerMapManager
 ***********************************************************************/
class ServerMapManager : public GameClientCallbackBase
{

public:
	// constructor
	ServerMapManager(ZCom_Control *_control, unsigned int ZoidLevel, const MapInfo & MapInfo,
						boost::shared_ptr<PhysXEngine> pengine);


	// destructor
	~ServerMapManager();


	//! process server internal stuff
	void Process();


	//! return true if need to be deleted
	bool DeleteMe()
	{return _needdelete;}


	//! called when player enter the map
	void PlayerEnter(unsigned int PlayerId, boost::shared_ptr<PlayerInfoHandler> pinfo);

	//! called when player leave the map
	void PlayerLeave(unsigned int PlayerId);


	// get back the zoid level used
	unsigned int GetZoidLevel()
	{return _zoidlevel;}


	//get back physic engine
	boost::shared_ptr<PhysXEngine> GetPhysicEngine()
	{return _pengine;}

	//! return true if server is full
	bool IsFull();


	// add an actor object callback
	virtual boost::shared_ptr<PhysicalObjectHandlerBase>
			AddObject(unsigned int id, const ObjectInfo &desc, bool IsMainPlayer);

	// remove an actor object callback
	virtual void RemObject(unsigned int id){}

	// get last player inputs
	virtual Input GetLastPlayerInput(){return Input();}

	//apply inputs
	virtual void ApplyInputs(const Input & in){}


	//! start process physic
	void StartProcessPhysic();

	//! finish process physic
	void FinishProcessPhysic();

	//! process physic historic
	void ProcessPhysicHistoric();

private:
	// controller
	ZCom_Control *	_controler;
	unsigned int	_zoidlevel;
	boost::shared_ptr<PhysXEngine> _pengine;

	bool			_cycledone;
	bool			_needdelete;

	std::string						_mapname;

	//used to give info about map to clients
	MapInfoObject					_mapobject;


	std::map<long, boost::shared_ptr<ActorObject> >						_actors;
	std::map<unsigned int, boost::shared_ptr<PlayerObject> >			_players;
	std::map<unsigned int, boost::shared_ptr<PlayerServerWrapper> >		_playerHandlers;
};

#endif