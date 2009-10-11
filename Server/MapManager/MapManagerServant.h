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


#ifndef _MAP_MANAGER_SERVANT_H
#define _MAP_MANAGER_SERVANT_H

#include "MapHandler.h"

#include <MapManager.h>
#include <Ice/Ice.h>

using namespace LbaNet;


class MapManagerServant : public MapManager, public IceUtil::Mutex
{
public:
	//! constructor
    MapManagerServant(const Ice::CommunicatorPtr& communicator, const Ice::ObjectAdapterPtr & adapter);

	//! destructor
    ~MapManagerServant();

	//! a player join a map
	virtual LbaNet::MapObserverPrx JoinMap(const std::string& mapName,
											Ice::Long PlayerId, const ActorLifeInfo &ali, const Ice::Current&);

	//! a player leave a map
    virtual ActorLifeInfo LeaveMap(const std::string& mapName, Ice::Long PlayerId, const Ice::Current&);


	//! stop the thread for map mapName
	void StopThread(const std::string& mapName);

protected:
	//! clean thread to be stopped
	void CleanThreads();

private:
	std::map<std::string, MapHandler *> _running_maps;
	const Ice::CommunicatorPtr&			_communicator;
	Ice::ObjectAdapterPtr				_adapter;

	std::vector<std::string>			_tostop;

	std::map<std::string, std::map<long, Actor *> > _actors_maps;
};

#endif
