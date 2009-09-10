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


#include "SharedData.h"


/***********************************************************
a player join a map
***********************************************************/
void SharedData::Join(Ice::Long PlayerId)
{
	IceUtil::Mutex::Lock lock(m_mutex_players);
	m_joined_players.push_back(std::make_pair<Ice::Long, bool>(PlayerId, true));
}

/***********************************************************
a player leave a map
***********************************************************/
void SharedData::Leave(Ice::Long PlayerId)
{
	IceUtil::Mutex::Lock lock(m_mutex_players);
	m_joined_players.push_back(std::make_pair<Ice::Long, bool>(PlayerId, false));
}

/***********************************************************
callback function called when a message is received from IceStorm
***********************************************************/
void SharedData::UpdatedInfo(const LbaNet::ActorInfo& asi)
{
	IceUtil::Mutex::Lock lock(m_mutex_players_info);
	m_players_info.push_back(asi);
}

/***********************************************************
callback function called when an actor id activated
***********************************************************/
void SharedData::ActivateActor(const LbaNet::ActorActivationInfo& ai)
{
	IceUtil::Mutex::Lock lock(m_mutex_actor_info);
	m_actors_info.push_back(ai);
}

/***********************************************************
callback function called when an actor id signaled
***********************************************************/
void SharedData::SignalActor(const LbaNet::ActorSignalInfo& ai)
{
	IceUtil::Mutex::Lock lock(m_mutex_actor_signal);
	m_signals_info.push_back(ai);
}

/***********************************************************
get joined/left players
***********************************************************/
void SharedData::GetJoined(std::vector<std::pair<Ice::Long, bool> > & joinedmap)
{
	joinedmap.clear();
	IceUtil::Mutex::Lock lock(m_mutex_players);
	m_joined_players.swap(joinedmap);
}

/***********************************************************
get player info
***********************************************************/
void SharedData::GetUpdatedinfo(std::vector<LbaNet::ActorInfo> & pinfos)
{
	pinfos.clear();
	IceUtil::Mutex::Lock lock(m_mutex_players_info);
	m_players_info.swap(pinfos);
}

/***********************************************************
get actor info
***********************************************************/
void SharedData::GetActorinfo(std::vector<LbaNet::ActorActivationInfo> & pinfos)
{
	pinfos.clear();
	IceUtil::Mutex::Lock lock(m_mutex_actor_info);
	m_actors_info.swap(pinfos);
}

/***********************************************************
get signal info
***********************************************************/
void SharedData::GetSignalinfo(std::vector<LbaNet::ActorSignalInfo> & pinfos)
{
	pinfos.clear();
	IceUtil::Mutex::Lock lock(m_mutex_actor_signal);
	m_signals_info.swap(pinfos);
}


/***********************************************************
make the thread wait for an amount of time
return false if the thread should quit
***********************************************************/
bool SharedData::WaitForTime(IceUtil::Time t)
{
	IceUtil::Monitor<IceUtil::Mutex>::Lock lock(m_monitor_thread);
	if(m_running)
		m_monitor_thread.timedWait(t);

	return m_running;
}

/***********************************************************
tell thread to quit
***********************************************************/
void SharedData::Quit()
{
	IceUtil::Monitor<IceUtil::Mutex>::Lock lock(m_monitor_thread);
	m_running = false;
	m_monitor_thread.notifyAll();
}