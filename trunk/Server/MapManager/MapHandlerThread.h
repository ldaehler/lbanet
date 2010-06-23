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
#include <time.h>

using namespace LbaNet;
class ServerSignaler;
class MapManagerServant;

class MapHandlerThread : public IceUtil::Mutex, public IceUtil::Thread

{
public:
	//! constructor
    MapHandlerThread(SharedData	* SD, ActorsObserverPrx	publisher,
						const std::string &mapName, MapManagerServant *	stopper,
						const std::map<long, Actor *> & actors);

	//! destructor
    ~MapHandlerThread();

	//! running thread
	virtual void run();


	//! send signal
	void SendSignal(long signal, const std::vector<long> &targets);


	//! get updated info
	LbaNet::UpdateSeq GetUpdatedInfo();

	//! get player info
	LbaNet::PlayerSeq GetPlayersInfo();

	//! get player life
	LbaNet::ActorLifeInfo GetPlayerLife(Ice::Long PlayerId);



protected:
	// a player join a map
	void Join(const ActorLifeInfo &PlayerId);

	// a player leave a map
	bool Leave(const ActorLifeInfo &PlayerId);

	// callback function called when a message is received from IceStorm
	void UpdatedInfo(const LbaNet::ActorInfo& asi);

	// callback function called when an actor id activated
	void ActivateActor(const ActorActivationInfoWithCallback& ai);

	// callback function called when an actor id signaled
	void SignalActor(const LbaNet::ActorSignalInfo& ai);

	// called when a player is hurted
	void Hurt(Ice::Long PlayerId, Ice::Long hurtingid);

	// called when a player is hurted
	void HurtFall(Ice::Long PlayerId, Ice::Float fallingdistance);

	// called when a player id dead and raised
	void Raised(Ice::Long PlayerId);

	//! object used
	void UpdateLifeMana(const LifeManaInfo &itinfo);

	void UpdateContainerQuery(const ContainerQueryInfo &itinfo);
	void UpdateContainerUpdate(const ContainerUpdateInfo &itinfo);

	void UpdateTargetedActor(const TargetedActorPlayer & info, bool targeted);

	void MagicBallTouchedActor(long PlayerId, long ActorId);
	void MagicBallTouchedPlayer(long PlayerId, long ActorId);


private:
	ActorsObserverPrx							_publisher;


	std::map<Ice::Long, std::pair<ActorInfo, ActorLifeInfo> >		_players;
	std::map<long, Actor *>						_actors;

	std::map<Ice::Long, std::vector<Ice::Long> > _unlocked;

	ServerSignaler *							_signaler;

	std::map<Ice::Long, Ice::Long>				_todeactivate;
	std::map<Ice::Long, std::pair<Ice::Long, time_t> >	_lockedContainers;

	SharedData	*								_SD;

	double										_lasttime;

	double										_timerstart;
	bool										_stopping;
	MapManagerServant *							_stopper;

	std::string									_mapName;

	std::map<Ice::Long, Ice::Long>				_targetedactor;
};

#endif
