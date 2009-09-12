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

#include "ThreadSafeWorkpile.h"


ThreadSafeWorkpile* ThreadSafeWorkpile::_singletonInstance = NULL;


/***********************************************************
	Destructor
***********************************************************/
ThreadSafeWorkpile::~ThreadSafeWorkpile()
{

}




/***********************************************************
singleton pattern
***********************************************************/
ThreadSafeWorkpile * ThreadSafeWorkpile::getInstance()
{
    if(!_singletonInstance)
    {
        _singletonInstance = new ThreadSafeWorkpile();
		return _singletonInstance;
    }
    else
    {
		return _singletonInstance;
    }

}


/***********************************************************
inform the event handler to quit game
***********************************************************/
void ThreadSafeWorkpile::QuitGame()
{
	IceUtil::Mutex::Lock lock(m_mutex_quitG);
	m_game_quitted = true;
}


/***********************************************************
check if the game need to be quitted
***********************************************************/
bool ThreadSafeWorkpile::GameQuitted()
{
	IceUtil::Mutex::Lock lock(m_mutex_quitG);
	return m_game_quitted;
}


/***********************************************************
add an event to the pile
***********************************************************/
void ThreadSafeWorkpile::AddEvent(GameEvent * gevent)
{
	IceUtil::Mutex::Lock lock(m_mutex_events);
	m_events.push_back(gevent);
}

/***********************************************************
get all pending events
***********************************************************/
void ThreadSafeWorkpile::GetPendingEvents(std::vector<GameEvent *> & events)
{
	IceUtil::Mutex::Lock lock(m_mutex_events);
	events.clear();
	m_events.swap(events);
}


/***********************************************************
add chat text
***********************************************************/
void ThreadSafeWorkpile::AddChatData(const ChatTextData & Data)
{
	IceUtil::Mutex::Lock lock(m_mutex_chat_data);
	m_chat_data.push_back(Data);
}

/***********************************************************
get all chat texts
***********************************************************/
void ThreadSafeWorkpile::GetChatData(std::vector<ChatTextData> & data)
{
	IceUtil::Mutex::Lock lock(m_mutex_chat_data);
	data.clear();
	m_chat_data.swap(data);
}


/***********************************************************
called when someone joined or left list
***********************************************************/
void ThreadSafeWorkpile::HappenedJoinEvent(const JoinEvent & ev)
{
	IceUtil::Mutex::Lock lock(m_mutex_join_event);
	m_join_event.push_back(ev);
}

/***********************************************************
get all events
***********************************************************/
void ThreadSafeWorkpile::GetJoinEventData(std::vector<JoinEvent> & data)
{
	IceUtil::Mutex::Lock lock(m_mutex_join_event);
	data.clear();
	m_join_event.swap(data);
}


/***********************************************************
inform irc thread to quit
***********************************************************/
void ThreadSafeWorkpile::QuitIrc()
{
	IceUtil::Monitor<IceUtil::Mutex>::Lock lock(m_monitor_irc);
	m_irc_quitted = true;
	m_monitor_irc.notifyAll();
}


/***********************************************************
wait one cycle before reconencting to irc
***********************************************************/
bool ThreadSafeWorkpile::WaitOneIrcCycle(long milliseconds)
{
	IceUtil::Monitor<IceUtil::Mutex>::Lock lock(m_monitor_irc);
	if(!m_irc_quitted)
	{
		IceUtil::Time t = IceUtil::Time::milliSeconds(milliseconds);
		m_monitor_irc.timedWait(t);
	}


	if(m_irc_quitted)
	{
		m_irc_quitted = false;
		return false;
	}
	return true;
}



/***********************************************************
add chat text to be send to server
***********************************************************/
void ThreadSafeWorkpile::AddChatText(const std::string & text)
{
	IceUtil::Mutex::Lock lock(m_mutex_chat_send);
	m_chat_texts.push_back(text);
}


/***********************************************************
get the text to be sent
***********************************************************/
void ThreadSafeWorkpile::GetChatText(std::vector<std::string> & texts)
{
	IceUtil::Mutex::Lock lock(m_mutex_chat_send);
	texts.clear();
	m_chat_texts.swap(texts);
}



/***********************************************************
inform irc thread to quit
***********************************************************/
void ThreadSafeWorkpile::QuitSending()
{
	IceUtil::Monitor<IceUtil::Mutex>::Lock lock(m_monitor_sending_loop);
	m_sending_quitted = true;
	m_monitor_sending_loop.notifyAll();
}

/***********************************************************
wait one cycle before reconencting to irc
***********************************************************/
bool ThreadSafeWorkpile::WaitOneSendingCycle()
{
	IceUtil::Monitor<IceUtil::Mutex>::Lock lock(m_monitor_sending_loop);
	if(!m_sending_quitted)
	{
		IceUtil::Time t = IceUtil::Time::milliSeconds(m_send_cycle_time);
		m_monitor_sending_loop.timedWait(t);
	}

	if(m_sending_quitted)
	{
		m_sending_quitted = false;
		return false;
	}
	return true;
}


/***********************************************************
update info
***********************************************************/
void ThreadSafeWorkpile::UpdateInfo(const LbaNet::ActorInfo & ai)
{
	IceUtil::Mutex::Lock lock(m_mutex_position_info);
	m_last_info = ai;
	m_last_info.MapName = m_new_map_name;
	m_is_updated = true;
}

/***********************************************************
add request to work pile
***********************************************************/
void ThreadSafeWorkpile::ChangeMap(const std::string & NewWorldName, const std::string & NewMapName)
{
	IceUtil::Mutex::Lock lock(m_mutex_position_info);
	m_new_map_name = NewMapName;
	m_NewWorldName = NewWorldName;
	m_map_changed = true;
}




/***********************************************************
process and empty work pile
***********************************************************/
bool ThreadSafeWorkpile::HasUpdatedInfo(LbaNet::ActorInfo & ai)
{
	IceUtil::Mutex::Lock lock(m_mutex_position_info);
	bool res = m_is_updated;
	m_is_updated = false;
	if(res)
		ai = m_last_info;
	return res;
}

/***********************************************************
process and empty work pile
***********************************************************/
bool ThreadSafeWorkpile::HasMapChanged(std::string & NewWorldName, std::string & NewMapName)
{
	IceUtil::Mutex::Lock lock(m_mutex_position_info);
	bool res = m_map_changed;
	m_map_changed = false;
	if(res)
	{
		NewMapName = m_new_map_name;
		NewWorldName = m_NewWorldName;
	}
	return res;
}



/***********************************************************
// update actor info
***********************************************************/
void ThreadSafeWorkpile::UpdateActor(const LbaNet::ActorInfo & ai)
{
	IceUtil::Mutex::Lock lock(m_mutex_ext_actor_info);
	m_ext_info.push_back(ai);
}

/***********************************************************
remove the actor from the list if present
***********************************************************/
void ThreadSafeWorkpile::RemoveActor(const std::string & ActorName)
{
	IceUtil::Mutex::Lock lock(m_mutex_ext_actor_info);
	m_quitted_actors.push_back(ActorName);
}

/***********************************************************
get the last update info
***********************************************************/
void ThreadSafeWorkpile::GetExtActorUpdate(std::vector<LbaNet::ActorInfo> & vec)
{
	IceUtil::Mutex::Lock lock(m_mutex_ext_actor_info);
	vec.clear();
	m_ext_info.swap(vec);
}

/***********************************************************
get the last update info
***********************************************************/
void ThreadSafeWorkpile::GetQuittedActors(std::vector<std::string> & vec)
{
	IceUtil::Mutex::Lock lock(m_mutex_ext_actor_info);
	vec.clear();
	m_quitted_actors.swap(vec);
}


/***********************************************************
add an activation event
***********************************************************/
void ThreadSafeWorkpile::AddActivationEvent(const LbaNet::ActorActivationInfo & ai)
{
	IceUtil::Mutex::Lock lock(m_mutex_activation);
	m_activation_events.push_back(ai);
}

/***********************************************************
get all activation event
***********************************************************/
void ThreadSafeWorkpile::GetAllActivationEvents(std::vector<LbaNet::ActorActivationInfo> & vec)
{
	IceUtil::Mutex::Lock lock(m_mutex_activation);
	vec.clear();
	m_activation_events.swap(vec);
}


/***********************************************************
add an activation event
***********************************************************/
void ThreadSafeWorkpile::AddSignalEvent(const LbaNet::ActorSignalInfo & ai)
{
	IceUtil::Mutex::Lock lock(m_mutex_activation);
	m_signal_events.push_back(ai);
}


/***********************************************************
get all activation event
***********************************************************/
void ThreadSafeWorkpile::GetAllSignalEvents(std::vector<LbaNet::ActorSignalInfo> & vec)
{
	IceUtil::Mutex::Lock lock(m_mutex_activation);
	vec.clear();
	m_signal_events.swap(vec);
}



/***********************************************************
add an activation event
***********************************************************/
void ThreadSafeWorkpile::Activated(const LbaNet::ActorActivationInfo & ai)
{
	IceUtil::Mutex::Lock lock(m_mutex_external_activation);
	m_ext_activations.push_back(ai);
}


/***********************************************************
get all activation event
***********************************************************/
void ThreadSafeWorkpile::GetAllActivated(std::vector<LbaNet::ActorActivationInfo> & vec)
{
	IceUtil::Mutex::Lock lock(m_mutex_external_activation);
	vec.clear();
	m_ext_activations.swap(vec);
}


/***********************************************************
set if the server is on or not
***********************************************************/
void ThreadSafeWorkpile::SetServeron(bool serveron)
{
	IceUtil::Mutex::Lock lock(m_mutex_server_on);
	m_server_on = serveron;
}

/***********************************************************
check if the server is on or not
***********************************************************/
bool ThreadSafeWorkpile::IsServeron()
{
	IceUtil::Mutex::Lock lock(m_mutex_server_on);
	return m_server_on;
}



/***********************************************************
update actors state
***********************************************************/
void ThreadSafeWorkpile::UpdateActorState(const std::vector<ActorStateInfo> & newstate)
{
	IceUtil::Mutex::Lock lock(m_mutex_actor_state);
	m_new_actor_state = true;
	m_actors_states = newstate;
}

/***********************************************************
return true if the sate has been updated
***********************************************************/
bool ThreadSafeWorkpile::ActorStatesUpdated(std::vector<ActorStateInfo> & newstate)
{
	bool res = false;
	IceUtil::Mutex::Lock lock(m_mutex_actor_state);
	if(m_new_actor_state)
	{
		res = true;
		newstate = m_actors_states;
		m_new_actor_state = false;
	}

	return res;
}

