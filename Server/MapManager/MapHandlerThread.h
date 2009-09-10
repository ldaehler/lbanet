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


#ifndef _MAP_HANDLER_THREAD_H
#define _MAP_HANDLER_THREAD_H


#include <ActorInfo.h>

#include <Ice/Ice.h>
#include <IceStorm/IceStorm.h>

#include "Actor.h"
#include <IceUtil/Thread.h>

#include "SharedData.h"

using namespace LbaNet;
class ServerSignaler;
class MapManagerServant;

class MapHandlerThread : public IceUtil::Mutex, public IceUtil::Thread

{
public:
	//! constructor
    MapHandlerThread(SharedData	* SD, ActorsObserverPrx	publisher,
						const std::string &mapName, MapManagerServant *	stopper);

	//! destructor
    ~MapHandlerThread();

	//! running thread
	virtual void run();


	//! send signal
	void SendSignal(long signal, const std::vector<long> &targets);


	//! get updated info
	LbaNet::UpdateSeq GetUpdatedInfo();

protected:
	// a player join a map
	void Join(Ice::Long PlayerId);

	// a player leave a map
	bool Leave(Ice::Long PlayerId);

	// callback function called when a message is received from IceStorm
	void UpdatedInfo(const LbaNet::ActorInfo& asi);

	// callback function called when an actor id activated
	void ActivateActor(const LbaNet::ActorActivationInfo& ai);

	// callback function called when an actor id signaled
	void SignalActor(const LbaNet::ActorSignalInfo& ai);

private:
	ActorsObserverPrx							_publisher;


	std::map<Ice::Long, LbaNet::ActorInfo>		_players;
	std::map<long, Actor *>						_actors;

	ServerSignaler *							_signaler;

	std::map<Ice::Long, Ice::Long>				_todeactivate;

	SharedData	*								_SD;

	double										_lasttime;

	double										_timerstart;
	bool										_stopping;
	MapManagerServant *							_stopper;

	std::string									_mapName;
};

#endif
