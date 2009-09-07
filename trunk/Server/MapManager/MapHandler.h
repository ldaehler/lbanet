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

#include <MapObserver.h>
#include <ActorInfo.h>

#include <Ice/Ice.h>
#include <IceStorm/IceStorm.h>

#include "Actor.h"
#include <IceUtil/Thread.h>

using namespace LbaNet;
class ServerSignaler;

class MapHandler : public MapObserver,  public IceUtil::Mutex, public IceUtil::Thread

{
public:
	//! constructor
    MapHandler(const Ice::CommunicatorPtr& communicator, const Ice::ObjectAdapterPtr & adapter,
					const std::string mapName);

	//! destructor
    ~MapHandler();

	// callback function called when an actor id activated
    virtual void ActivateActor(const LbaNet::ActorActivationInfo& ai, const Ice::Current&);

	// callback function called when an actor id signaled
    virtual void SignalActor(const LbaNet::ActorSignalInfo& ai, const Ice::Current&);


	// callback function called when a message is received from IceStorm
	void UpdatedInfo(const LbaNet::ActorInfo& asi);

	// get proxy to the map observer
	LbaNet::MapObserverPrx GetMapProxy();

	//! a player join a map
	void Join(Ice::Long PlayerId);

	//! a player leave a map
    bool Leave(Ice::Long PlayerId);

	//! send signal
	void SendSignal(long signal, const std::vector<long> &targets);

	//! running thread
	virtual void run();

	//! get updated info
	LbaNet::UpdateSeq GetUpdatedInfo(const Ice::Current&);

	//! destroy the map handler
	void Destroy();

protected:
	//! subscribe to icestorm
	void IcestormSubscribe();

	//! subscribe to icestorm
	void IcestormUnsubscribe();


private:
	IceUtil::Mutex								m_mutex_players;

	const Ice::CommunicatorPtr&					_communicator;
	Ice::ObjectAdapterPtr						_adapter;
	std::string									_mapName;
	LbaNet::MapObserverPrx						_proxy;

	IceStorm::TopicPrx							_topic;
	ActorsObserverPrx							_publisher;
	ActorsObserverPrx							_observer;

	std::map<Ice::Long, LbaNet::ActorInfo>		_players;

	std::map<long, Actor *>						_actors;

	ServerSignaler *							_signaler;

	std::map<Ice::Long, Ice::Long>				_todeactivate;

	IceUtil::ThreadControl						_threadC;
	bool										_stop;
	double										_lasttime;
};

#endif
