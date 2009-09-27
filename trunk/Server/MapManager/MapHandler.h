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


#ifndef _MAP_HANDLER_H
#define _MAP_HANDLER_H


#include <ActorInfo.h>

#include <Ice/Ice.h>
#include <IceStorm/IceStorm.h>

#include "Actor.h"
#include <IceUtil/Thread.h>

#include "SharedData.h"

using namespace LbaNet;
class MapManagerServant;
class MapHandlerThread;

class MapHandler

{
public:
	//! constructor
    MapHandler(const Ice::CommunicatorPtr& communicator, const Ice::ObjectAdapterPtr & adapter,
					const std::string mapName, MapManagerServant *	stopper);

	//! destructor
    ~MapHandler();


	// get proxy to the map observer
	LbaNet::MapObserverPrx GetMapProxy();

	//! a player join a map
	void Join(Ice::Long PlayerId, const ActorLifeInfo & ali);

	//! a player leave a map
    ActorLifeInfo Leave(Ice::Long PlayerId);


protected:
	//! subscribe to icestorm
	void IcestormSubscribe();

	//! subscribe to icestorm
	void IcestormUnsubscribe();


private:

	const Ice::CommunicatorPtr&					_communicator;
	Ice::ObjectAdapterPtr						_adapter;
	std::string									_mapName;


	IceStorm::TopicPrx							_topic;
	ActorsObserverPrx							_publisher;
	ActorsObserverPrx							_observer;
	MapObserverPrx								_mapproxy;


	IceUtil::ThreadControl						_threadC;
	SharedData									_SD;

	MapHandlerThread *							_mapthread;
};

#endif
