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
	m_last_info.MapName = m_map_changed_info.NewMapName;
	m_is_updated = true;
}

/***********************************************************
add request to work pile
***********************************************************/
void ThreadSafeWorkpile::ChangeMap(const MapChangedInformation & mi)
{
	IceUtil::Mutex::Lock lock(m_mutex_position_info);
	m_map_changed_info = mi;
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
bool ThreadSafeWorkpile::HasMapChanged(MapChangedInformation & mi)
{
	IceUtil::Mutex::Lock lock(m_mutex_position_info);
	bool res = m_map_changed;
	m_map_changed = false;
	if(res)
		mi = m_map_changed_info;

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
update single actor state
***********************************************************/
void ThreadSafeWorkpile::UpdateSingleActorState(const ActorStateInfo & newstate)
{
	IceUtil::Mutex::Lock lock(m_mutex_actor_state);
	m_new_actor_state = true;
	m_actors_states.push_back(newstate);
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
		newstate.swap(m_actors_states);
		m_new_actor_state = false;
	}

	return res;
}


/***********************************************************
update actor life
***********************************************************/
void ThreadSafeWorkpile::UpdateActorLife(const LbaNet::ActorLifeInfo& ali)
{
	IceUtil::Mutex::Lock lock(m_mutex_ext_actor_life_info);
	m_ext_life_info.push_back(ali);
}


/***********************************************************
get the last update info
***********************************************************/
void ThreadSafeWorkpile::GetExtActorLifeUpdate(std::vector<LbaNet::ActorLifeInfo> & vec)
{
	vec.clear();
	IceUtil::Mutex::Lock lock(m_mutex_ext_actor_life_info);
	m_ext_life_info.swap(vec);
}



/***********************************************************
add player hurted by another actor
***********************************************************/
void ThreadSafeWorkpile::AddPlayerHurt(long hurtingactorid)
{
	IceUtil::Mutex::Lock lock(m_mutex_player_hurt);
	m_player_hurts.push_back(hurtingactorid);
}


/***********************************************************
add player hurted by falling down
***********************************************************/
void ThreadSafeWorkpile::AddPlayerHurtFall(float fallingdistance)
{
	IceUtil::Mutex::Lock lock(m_mutex_player_hurt);
	m_player_hurt_falls.push_back(fallingdistance);
}


/***********************************************************
add player hurted by another actor
***********************************************************/
void ThreadSafeWorkpile::GetPlayerHurts(std::vector<long> & vec)
{
	vec.clear();
	IceUtil::Mutex::Lock lock(m_mutex_player_hurt);
	m_player_hurts.swap(vec);
}


/***********************************************************
add player hurted by falling down
***********************************************************/
void ThreadSafeWorkpile::GetPlayerHurtFalls(std::vector<float> & vec)
{
	vec.clear();
	IceUtil::Mutex::Lock lock(m_mutex_player_hurt);
	m_player_hurt_falls.swap(vec);
}


/***********************************************************
add a raised event
***********************************************************/
void ThreadSafeWorkpile::AddRaisedEvent()
{
	IceUtil::Mutex::Lock lock(m_mutex_player_raised);
	m_player_raised = true;
}

/***********************************************************
return true if there was a raised event
***********************************************************/
bool ThreadSafeWorkpile::IsRaised()
{
	bool res = false;
	IceUtil::Mutex::Lock lock(m_mutex_player_raised);
	if(m_player_raised)
		res = true;

	m_player_raised = false;
	return res;
}




/***********************************************************
changed the display color for char name
***********************************************************/
void ThreadSafeWorkpile::ChangeNameColor(std::string color)
{
	IceUtil::Mutex::Lock lock(m_mutex_name_color);
	m_name_color_changed = true;
	m_name_color = color;
}

/***********************************************************
return true if the color has changed
***********************************************************/
bool ThreadSafeWorkpile::NameColorChanged(std::string &color)
{
	IceUtil::Mutex::Lock lock(m_mutex_name_color);
	if(m_name_color_changed)
	{
		m_name_color_changed = false;
		color = m_name_color;
		return true;
	}

	return false;
}



/***********************************************************
called when someone joined or left list
***********************************************************/
void ThreadSafeWorkpile::ChatColorChanged(const std::string & nickname, const std::string & color)
{
	IceUtil::Mutex::Lock lock(m_mutex_color_changed);
	m_colors_changed.push_back(std::make_pair<std::string, std::string>(nickname, color));
}

/***********************************************************
return the color changed since last time
***********************************************************/
void ThreadSafeWorkpile::GetColorChanges(std::vector<std::pair<std::string, std::string> > & vec)
{
	IceUtil::Mutex::Lock lock(m_mutex_color_changed);
	vec.clear();
	m_colors_changed.swap(vec);
}




/***********************************************************
inform server of change world
***********************************************************/
void ThreadSafeWorkpile::InformChangeWorld(const std::string & NewWorld)
{
	IceUtil::Mutex::Lock lock(m_mutex_world_changed);
	m_world_changed = true;
	m_new_world_name = NewWorld;
	if(!IsServeron())
	{
		PlayerWorldPos pos;
		SetNewWorldPlayerPos(pos);
	}

}

/***********************************************************
check if world changed
***********************************************************/
bool ThreadSafeWorkpile::WorldChanged(std::string & NewWorld)
{
	IceUtil::Mutex::Lock lock(m_mutex_world_changed);
	if(m_world_changed)
	{
		m_world_changed = false;
		NewWorld = m_new_world_name;
		return true;
	}

	return false;
}

/***********************************************************
set the position of the player on the new world
***********************************************************/
void ThreadSafeWorkpile::SetNewWorldPlayerPos(const PlayerWorldPos & position)
{
	IceUtil::Monitor<IceUtil::Mutex>::Lock lock(m_monitor_player_pos_updated);

	m_player_pos_info_updated = true;
	m_player_pos_info = position;

	m_monitor_player_pos_updated.notifyAll();
}

/***********************************************************
wait for server to return with player position
***********************************************************/
const ThreadSafeWorkpile::PlayerWorldPos & ThreadSafeWorkpile::WaitForPlayerPosition()
{
	IceUtil::Monitor<IceUtil::Mutex>::Lock lock(m_monitor_player_pos_updated);

	if(!m_player_pos_info_updated)
		m_monitor_player_pos_updated.wait();

	if(!m_player_pos_info_updated)
		m_player_pos_info.MapName = "";

	m_player_pos_info_updated = false;
	return m_player_pos_info;
}



/***********************************************************
called when a shortcut is used
***********************************************************/
void ThreadSafeWorkpile::UseShortcut(int scnum)
{
	IceUtil::Mutex::Lock lock(m_mutex_shortcut_used);
	m_shortcuts_used.push_back(scnum);
}

/***********************************************************
called when a shortcut is used
***********************************************************/
void ThreadSafeWorkpile::GetUsedShorcut(std::vector<int> &scvec)
{
	scvec.clear();
	IceUtil::Mutex::Lock lock(m_mutex_shortcut_used);
	scvec.swap(m_shortcuts_used);
}



/***********************************************************
called when an object is used
***********************************************************/
void ThreadSafeWorkpile::InventoryObjectUsed(long itemid)
{
	IceUtil::Mutex::Lock lock(m_mutex_inventory_used);
	m_inv_item_used.push_back(itemid);
}

/***********************************************************
get list of used objects
***********************************************************/
void ThreadSafeWorkpile::GetListOfObjectUsed(std::vector<long> &objs)
{
	objs.clear();
	IceUtil::Mutex::Lock lock(m_mutex_inventory_used);
	objs.swap(m_inv_item_used);
}


/***********************************************************
ask for a container info
***********************************************************/
void ThreadSafeWorkpile::AskForContainerInfo(long containerid)
{
	IceUtil::Mutex::Lock lock(m_mutex_container_info);
	m_waiting_container_info = true;
	m_container_id = containerid;
}


/***********************************************************
check if player is waiting for container info
***********************************************************/
bool ThreadSafeWorkpile::HasAskedForContainer(long &containerid)
{
	IceUtil::Mutex::Lock lock(m_mutex_container_info);
	containerid = m_container_id;
	bool res = m_waiting_container_info;
	m_waiting_container_info = false;
	return res;
}





/***********************************************************
ask for a container info
***********************************************************/
void ThreadSafeWorkpile::OpenCloseContainer(long containerid, bool ForceClose)
{
	IceUtil::Mutex::Lock lock(m_mutex_container_close);
	m_closed_container = true;
	m_closed_container_id = containerid;
	m_cont_ForceClose = ForceClose;
}

/***********************************************************
check if player is waiting for container info
***********************************************************/
bool ThreadSafeWorkpile::HasOpenCloseContainer(long &containerid, bool &ForceClose)
{
	IceUtil::Mutex::Lock lock(m_mutex_container_close);
	containerid = m_closed_container_id;
	bool res = m_closed_container;
	m_closed_container = false;
	ForceClose = m_cont_ForceClose;
	return res;
}


/***********************************************************
called by server to give container update
***********************************************************/
void ThreadSafeWorkpile::UpdateContainer(const LbaNet::ContainerInfo & cinfo)
{
	IceUtil::Mutex::Lock lock(m_mutex_container_deal);
	m_updated_container = true;
	m_container = cinfo;
}

/***********************************************************
ask if we got an update
***********************************************************/
bool ThreadSafeWorkpile::IsUpdatedContainer(LbaNet::ContainerInfo & cinfo)
{
	IceUtil::Mutex::Lock lock(m_mutex_container_deal);
	cinfo = m_container;
	bool res = m_updated_container;
	m_updated_container = false;
	return res;
}


/***********************************************************
called by server to give container update from inventory
***********************************************************/
void ThreadSafeWorkpile::UpdateInvFromContainer(const UpdateInvContainer & cinfo)
{
	IceUtil::Mutex::Lock lock(m_mutex_container_exchange);
	m_exchanged_container = true;
	m_container_exchange = cinfo;
}

/***********************************************************
ask if we got an update
***********************************************************/
bool ThreadSafeWorkpile::IsUpdatedInvFromContainer(UpdateInvContainer & cinfo)
{
	IceUtil::Mutex::Lock lock(m_mutex_container_exchange);
	cinfo = m_container_exchange;
	bool res = m_exchanged_container;
	m_exchanged_container = false;
	return res;
}


/***********************************************************
add a whisper channel
***********************************************************/
void ThreadSafeWorkpile::AddWhisperChannel(const std::string & name)
{
	IceUtil::Mutex::Lock lock(m_mutex_whisper_chanel);
	m_asked_whispers.push_back(name);
}


/***********************************************************
get all queries for whisper channel
***********************************************************/
void ThreadSafeWorkpile::GetWhisperChannelQueries(std::vector<std::string> &scvec)
{
	scvec.clear();
	IceUtil::Mutex::Lock lock(m_mutex_whisper_chanel);
	m_asked_whispers.swap(scvec);
}



/***********************************************************
add a whisper channel
***********************************************************/
void ThreadSafeWorkpile::AddFriend(const std::string & name)
{
	IceUtil::Mutex::Lock lock(m_mutex_added_friend);
	m_added_friends.push_back(name);
}

/***********************************************************
get all queries for whisper channel
***********************************************************/
void ThreadSafeWorkpile::GetAddedFriend(std::vector<std::string> &scvec)
{
	scvec.clear();
	IceUtil::Mutex::Lock lock(m_mutex_added_friend);
	m_added_friends.swap(scvec);
}

/***********************************************************
add a whisper channel
***********************************************************/
void ThreadSafeWorkpile::RemoveFriend(const std::string & name)
{
	IceUtil::Mutex::Lock lock(m_mutex_removed_friend);
	m_removed_friends.push_back(name);
}

/***********************************************************
get all queries for whisper channel
***********************************************************/
void ThreadSafeWorkpile::GetRemovedFriend(std::vector<std::string> &scvec)
{
	scvec.clear();
	IceUtil::Mutex::Lock lock(m_mutex_removed_friend);
	m_removed_friends.swap(scvec);
}


/***********************************************************
set friend list
***********************************************************/
void ThreadSafeWorkpile::SetFriends(const std::vector<std::string> & friends)
{
	IceUtil::Mutex::Lock lock(m_mutex_friend);
	m_friend_list = friends;
}

/***********************************************************
get friend list
***********************************************************/
void ThreadSafeWorkpile::GetFriends(std::vector<std::string> & friends)
{
	IceUtil::Mutex::Lock lock(m_mutex_friend);
	friends.swap(m_friend_list);
}


/***********************************************************
add written letter to the server
***********************************************************/
void ThreadSafeWorkpile::AddWrittenLetter(const WrittenLetter & letter)
{
	IceUtil::Mutex::Lock lock(m_mutex_letters);
	m_letters.push_back(letter);
}

/***********************************************************
get written letter to the server
***********************************************************/
void ThreadSafeWorkpile::GetWrittenLetters(std::vector<WrittenLetter> & letters)
{
	letters.clear();
	IceUtil::Mutex::Lock lock(m_mutex_letters);
	letters.swap(m_letters);
}


/***********************************************************
add query for letter info
***********************************************************/
void ThreadSafeWorkpile::AddLetterInfoQuery(long itemid)
{
	IceUtil::Mutex::Lock lock(m_mutex_letter_queries);
	m_letter_queries.push_back(itemid);
}

/***********************************************************
get written letter to the server
***********************************************************/
void ThreadSafeWorkpile::GetLetterInfoQuerys(std::vector<long> & letterqueries)
{
	letterqueries.clear();
	IceUtil::Mutex::Lock lock(m_mutex_letter_queries);
	m_letter_queries.swap(letterqueries);
}



/***********************************************************
destroy an inventory item
***********************************************************/
void ThreadSafeWorkpile::DestroyItem(long ItemId)
{
	IceUtil::Mutex::Lock lock(m_mutex_destroy_items);
	m_destroyed_items.push_back(ItemId);
}

/***********************************************************
get destroyed items
***********************************************************/
void ThreadSafeWorkpile::GetDestroyedItems(std::vector<long> & destroyedItems)
{
	destroyedItems.clear();
	IceUtil::Mutex::Lock lock(m_mutex_destroy_items);
	destroyedItems.swap(m_destroyed_items);
}


/***********************************************************
set targetedacotr
***********************************************************/
void ThreadSafeWorkpile::SetTargetedActor(long actorid)
{
	IceUtil::Mutex::Lock lock(m_mutex_targeted_actors);
	m_targetedactors.push_back(actorid);
}

/***********************************************************
get destroyed items
***********************************************************/
void ThreadSafeWorkpile::GetTargetedActors(std::vector<long> & targetedactors)
{
	targetedactors.clear();
	IceUtil::Mutex::Lock lock(m_mutex_targeted_actors);
	targetedactors.swap(m_targetedactors);
}

/***********************************************************
set untargetedacotr
***********************************************************/
void ThreadSafeWorkpile::SetUntargetedActor(long actorid)
{
	IceUtil::Mutex::Lock lock(m_mutex_targeted_actors);
	m_untargetedactors.push_back(actorid);
}

/***********************************************************
get destroyed items
***********************************************************/
void ThreadSafeWorkpile::GetUntargetedActors(std::vector<long> & untargetedactors)
{
	untargetedactors.clear();
	IceUtil::Mutex::Lock lock(m_mutex_targeted_actors);
	untargetedactors.swap(m_untargetedactors);
}