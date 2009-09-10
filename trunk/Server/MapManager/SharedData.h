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
	void Join(Ice::Long PlayerId);

	//! a player leave a map
    void Leave(Ice::Long PlayerId);

	// callback function called when a message is received from IceStorm
	void UpdatedInfo(const LbaNet::ActorInfo& asi);

	// callback function called when an actor id activated
    void ActivateActor(const LbaNet::ActorActivationInfo& ai);

	// callback function called when an actor id signaled
    void SignalActor(const LbaNet::ActorSignalInfo& ai);

	// get joined/left players
	void GetJoined(std::vector<std::pair<Ice::Long, bool> > & joinedmap);

	// get player info
	void GetUpdatedinfo(std::vector<LbaNet::ActorInfo> & pinfos);

	// get actor info
	void GetActorinfo(std::vector<LbaNet::ActorActivationInfo> & pinfos);

	// get actor info
	void GetSignalinfo(std::vector<LbaNet::ActorSignalInfo> & pinfos);

	// make the thread wait for an amount of time
	// return false if the thread should quit
	bool WaitForTime(IceUtil::Time t);

	// tell threqd to quit
	void Quit();

protected:
	SharedData(const SharedData &);
	const SharedData & operator=(const SharedData &);

private:
	IceUtil::Mutex								m_mutex_players;
	IceUtil::Mutex								m_mutex_players_info;
	IceUtil::Mutex								m_mutex_actor_info;
	IceUtil::Mutex								m_mutex_actor_signal;
	IceUtil::Monitor<IceUtil::Mutex>			m_monitor_thread;

	std::vector<std::pair<Ice::Long, bool> >	m_joined_players;
	std::vector<LbaNet::ActorInfo>				m_players_info;
	std::vector<LbaNet::ActorActivationInfo>	m_actors_info;
	std::vector<LbaNet::ActorSignalInfo>		m_signals_info;

	bool										m_running;
};

#endif