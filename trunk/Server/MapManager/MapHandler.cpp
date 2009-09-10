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


#include "MapHandler.h"
#include "InfosReceiverServant.h"
#include "ActorReceiverServant.h"
#include "MapInfoXmlReader.h"
#include "ServerSignaler.h"
#include "MapHandlerThread.h"


/***********************************************************
	Constructor
***********************************************************/
MapHandler::MapHandler(const Ice::CommunicatorPtr& communicator, const Ice::ObjectAdapterPtr & adapter,
								const std::string mapName, MapManagerServant *	stopper)
: _communicator(communicator), _adapter(adapter), _mapName(mapName), _publisher(NULL)
{
	IcestormSubscribe();

	MapHandlerThread *thread = new MapHandlerThread(&_SD, _publisher, mapName, stopper);
	_threadC = thread->start();

	Ice::ObjectPrx proxy = _adapter->addWithUUID(new ActorReceiverServant(&_SD, thread));
	_mapproxy = LbaNet::MapObserverPrx::uncheckedCast(proxy);

	std::cout<<IceUtil::Time::now().toDateTime()<<": thread created for map: "<<_mapName<<std::endl;
}


/***********************************************************
	destructor
***********************************************************/
MapHandler::~MapHandler()
{
	std::cout<<IceUtil::Time::now().toDateTime()<<": destroying thread for map: "<<_mapName<<std::endl;

	IcestormUnsubscribe();

	try
	{
		_adapter->remove(_mapproxy->ice_getIdentity());
		_adapter->remove(_observer->ice_getIdentity());
	}
	catch(...)
	{
		std::cout<<IceUtil::Time::now().toDateTime()<<"MapHandler - Exception unregistering map. "<<std::endl;
	}

	// stop thread
	_SD.Quit();

	_threadC.join();
	std::cout<<IceUtil::Time::now().toDateTime()<<": thread destroyed for map: "<<_mapName<<std::endl;
}


/***********************************************************
	get proxy to the map observer
***********************************************************/
LbaNet::MapObserverPrx MapHandler::GetMapProxy()
{
	return _mapproxy;
}


/***********************************************************
	a player join a map
***********************************************************/
void MapHandler::Join(Ice::Long PlayerId)
{
	_SD.Join(PlayerId);
	std::cout<<IceUtil::Time::now().toDateTime()<<": player id "<<PlayerId<<" joined map "<<_mapName<<_mapName<<std::endl;
}

/***********************************************************
 a player leave a map
***********************************************************/
void MapHandler::Leave(Ice::Long PlayerId)
{
	_SD.Leave(PlayerId);
	std::cout<<IceUtil::Time::now().toDateTime()<<": player id "<<PlayerId<<" left map "<<_mapName<<_mapName<<std::endl;
}

/***********************************************************
subscribe to icestorm
***********************************************************/
void MapHandler::IcestormSubscribe()
{
	Ice::ObjectPrx proxy = _adapter->addWithUUID(new InfosReceiverServant(&_SD));
	_observer = LbaNet::ActorsObserverPrx::uncheckedCast(proxy);

	IceStorm::TopicManagerPrx manager;
	try
	{
		manager = IceStorm::TopicManagerPrx::uncheckedCast(
		_communicator->propertyToProxy("TopicManagerProxy"));
	}
	catch(const IceUtil::Exception& ex)
	{
		std::cout<<"MapHandler - Exception getting the actor topic manager: "<<ex.what()<<std::endl;
	}
	catch(...)
	{
		std::cout<<"MapHandler - Unknown exception getting the actor topic manager. "<<std::endl;
	}

	try
	{
		_topic = manager->create(_mapName);
	}
	catch(const IceStorm::TopicExists&)
	{
		_topic = manager->retrieve(_mapName);
	}
	catch(const IceUtil::Exception& ex)
	{
		std::cout<<"MapHandler - Exception creating actor topic "<<ex.what()<<std::endl;
	}
	catch(...)
	{
		std::cout<<"MapHandler - Unknown exception creating actor topic. "<<std::endl;
	}



	try
	{
		_topic->subscribeAndGetPublisher(IceStorm::QoS(), _observer);
	}
	catch(const IceUtil::Exception& ex)
	{
		std::cout<<"MapHandler - Exception creating actor publisher "<<ex.what()<<std::endl;
	}
	catch(...)
	{
		std::cout<<"MapHandler - Unknown exception creating actor publisher. "<<std::endl;
	}



	Ice::ObjectPrx pub;
	try
	{
		pub = _topic->getPublisher();
		_publisher = ActorsObserverPrx::uncheckedCast(pub->ice_oneway());
	}
	catch(const IceUtil::Exception& ex)
	{
		std::cout<<"MapHandler - Exception getting the publisher "<<ex.what()<<std::endl;
	}
	catch(...)
	{
		std::cout<<"MapHandler - Unknown exception getting the publisher. "<<std::endl;
	}
}


/***********************************************************
unsubscribe to icestorm
***********************************************************/
void MapHandler::IcestormUnsubscribe()
{
	try
	{
		_topic->unsubscribe(_observer);
	}
	catch(const IceUtil::Exception& ex)
	{
		std::cout<<"MapHandler - Exception unsubscribing actor"<<ex.what()<<std::endl;
	}
	catch(...)
	{
		std::cout<<"MapHandler - Unknown exception unsubscribing actor. "<<std::endl;
	}
}



