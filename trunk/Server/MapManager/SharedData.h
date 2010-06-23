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
#if !defined(__Agent_shared_data_h)
#define __Agent_shared_data_h

#include <string>
#include <vector>
#include <map>

#include <IceUtil/Mutex.h>
#include <IceUtil/Monitor.h>
#include <Ice/Config.h>

#include <MapObserver.h>
#include <ActorInfo.h>

using namespace LbaNet;

struct LifeManaInfo
{
	Ice::Long ActorId;
	int LifeDelta;
	int ManaDelta;
};

struct ActorActivationInfoWithCallback
{
	LbaNet::ActorActivationInfo ainfo;
	LbaNet::ClientSessionPrx clientPtr;
};

struct ContainerQueryInfo
{
	Ice::Long ActorId;
	Ice::Long ContainerId; 
	LbaNet::ClientSessionPrx clientPtr;
};


struct ContainerUpdateInfo
{
	Ice::Long ContainerId;
	Ice::Long ActorId;
	LbaNet::ItemList Taken; 
	LbaNet::ItemList Put;
	LbaNet::ClientSessionPrx clientPtr;
};


struct TargetedActorPlayer
{
	Ice::Long ActorId;
	Ice::Long PlayerId;
};

/***********************************************************************
 * Module:  SharedData.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:59:17
 * Purpose: Declaration of the class SharedData
 ***********************************************************************/
class SharedData
{
public:
	//! default construtor
	SharedData()
		: m_running(true)
	{}

	//! a player join a map
	void Join(Ice::Long PlayerId, const ActorLifeInfo & ali);

	//! a player leave a map
    void Leave(Ice::Long PlayerId);

	// callback function called when a message is received from IceStorm
	void UpdatedInfo(const LbaNet::ActorInfo& asi);

	// callback function called when an actor id activated
    void ActivateActor(const ActorActivationInfoWithCallback& ai);

	// callback function called when an actor id signaled
    void SignalActor(const LbaNet::ActorSignalInfo& ai);

	// get joined/left players
	void GetJoined(std::vector<std::pair<ActorLifeInfo, bool> > & joinedmap);

	// get player info
	void GetUpdatedinfo(std::vector<LbaNet::ActorInfo> & pinfos);

	// get actor info
	void GetActorinfo(std::vector<ActorActivationInfoWithCallback> & pinfos);

	// get actor info
	void GetSignalinfo(std::vector<LbaNet::ActorSignalInfo> & pinfos);

	// make the thread wait for an amount of time
	// return false if the thread should quit
	bool WaitForTime(IceUtil::Time t);

	// tell threqd to quit
	void Quit();

	//! called when an actor has been hurt
    void GotHurtByActor(Ice::Long ActorId, Ice::Long HurtingId);

	//! called when an actor has been hurt
    void GotHurtByFalling(Ice::Long ActorId, Ice::Float FallingDistance);

	// get actor info
	void GetHurtedPlayer(std::vector<std::pair<Ice::Long, Ice::Long> > & pinfos);

	// get actor info
	void GetHurtedFallPlayer(std::vector<std::pair<Ice::Long, Ice::Float> > & pinfos);

	// actor dead and reborn
	void RaisedFromDead(Ice::Long ActorId);

	// actor dead and reborn
	void GetRaisedFromDead(std::vector<Ice::Long> & pinfos);

	// called when an item is used by a player
	void UpdateLifeMana(const LifeManaInfo & itinfo);

	// get all items used
	void GetAllUpdateLifeMana(std::vector<LifeManaInfo> & vec);


	// called when an item is used by a player
	void UpdateContainerQuery(const ContainerQueryInfo & itinfo);

	// get all items used
	void GetAllContainerQuerys(std::vector<ContainerQueryInfo> & vec);

	// called when an item is used by a player
	void UpdateContainerUpdate(const ContainerUpdateInfo & itinfo);

	// get all items used
	void GetAllContainerUpdates(std::vector<ContainerUpdateInfo> & vec);

	// update targeted actors
	void UpdateTargetedActor(const TargetedActorPlayer & tinfo);

	// update targeted actors
	void UpdateUntargetedActor(const TargetedActorPlayer & tinfo);

	// get all items used
	void GetAllTargetedActors(std::vector<TargetedActorPlayer> & vec);

	// get all items used
	void GetAllUntargetedActors(std::vector<TargetedActorPlayer> & vec);

		    
    // called when player throw magic ball
    void MagicBallPlayed(long PlayerId, const LbaNet::LaunchInfo& Linfo);
    
    // called when magic ball touch an actor
    void MagicBallTouchActor(long PlayerId, long ActorId);
    
    // called when magic ball touch a player
    void MagicBallTouchPlayer(long PlayerId, long ActorId);	

		    
    // called when player throw magic ball
    void GetMagicBallPlayed(std::vector<std::pair<long, LbaNet::LaunchInfo>	> & vec);
    
    // called when magic ball touch an actor
	void GetMagicBallTouchActor(std::vector<std::pair<long, long> > & vec);
    
    // called when magic ball touch a player
    void GetMagicBallTouchPlayer(std::vector<std::pair<long, long> >  & vec);	


protected:
	SharedData(const SharedData &);
	const SharedData & operator=(const SharedData &);

private:
	IceUtil::Mutex								m_mutex_players;
	IceUtil::Mutex								m_mutex_players_info;
	IceUtil::Mutex								m_mutex_actor_info;
	IceUtil::Mutex								m_mutex_actor_signal;
	IceUtil::Mutex								m_mutex_actor_hurt;
	IceUtil::Mutex								m_mutex_actor_raised;
	IceUtil::Monitor<IceUtil::Mutex>			m_monitor_thread;
	IceUtil::Mutex								m_mutex_item_used;
	IceUtil::Mutex								m_mutex_container_queries;
	IceUtil::Mutex								m_mutex_container_updates;
	IceUtil::Mutex								m_mutex_targeted_actors;
	IceUtil::Mutex								m_mutex_magicball_played;
	IceUtil::Mutex								m_mutex_magicball_touchedactor;
	IceUtil::Mutex								m_mutex_magicball_touchedplayer;

	std::vector<std::pair<ActorLifeInfo, bool> >	m_joined_players;
	std::vector<LbaNet::ActorInfo>				m_players_info;
	std::vector<ActorActivationInfoWithCallback>	m_actors_info;
	std::vector<LbaNet::ActorSignalInfo>		m_signals_info;
	std::vector<std::pair<Ice::Long, Ice::Long> >	m_hurt_players;
	std::vector<std::pair<Ice::Long, Ice::Float> >	m_hurt_fall_players;
	std::vector<Ice::Long>						m_raised_info;

	std::vector<LifeManaInfo>					m_items_used;

	std::vector<ContainerQueryInfo>				m_container_queries;
	std::vector<ContainerUpdateInfo>			m_container_updates;

	std::vector<TargetedActorPlayer>			m_targeted_actors;
	std::vector<TargetedActorPlayer>			m_untargeted_actors;

	std::vector<std::pair<long, LbaNet::LaunchInfo>	> m_mb_played;
    std::vector<std::pair<long, long> >			m_mb_touched_actor;
    std::vector<std::pair<long, long> >			m_mb_touched_player;



	bool										m_running;
};

#endif