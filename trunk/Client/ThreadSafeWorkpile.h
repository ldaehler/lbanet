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


#if !defined(__LbaNetModel_1_ThreadSafeWorkpile_h)
#define __LbaNetModel_1_ThreadSafeWorkpile_h

#include <string>
#include <vector>
#include <IceUtil/Mutex.h>
#include <IceUtil/Monitor.h>
#include <ActorInfo.h>
#include "Actor.h"

class GameEvent;

/***********************************************************************
 * Module:  ThreadSafeWorkpile.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:59:17
 * Purpose: Declaration of the class ThreadSafeWorkpile
 ***********************************************************************/
class ThreadSafeWorkpile
{
public:

	struct ChatTextData
	{
		std::string Channel;
		std::string Sender;
		std::string Text;
	};

	struct JoinEvent
	{
		std::string ListName;
		bool Joined;
		std::string Nickname;
		bool Clear;
		std::string Status;
		std::string Color;
	};

	struct PlayerWorldPos
	{
		std::string MapName;
		float X;
		float Y;
		float Z;
		float Rotation;
	};

	struct MapChangedInformation
	{
		std::string NewWorldName;
		std::string NewMapName;
		float X;
		float Y;
		float Z;
		float Rotation;
	};


	//! destructor
	~ThreadSafeWorkpile();

	// singleton pattern
	static ThreadSafeWorkpile * getInstance();

	//! inform the event handler to quit game
	void QuitGame();

	//! check if the game need to be quitted
	bool GameQuitted();

	//! add an event to the pile
	void AddEvent(GameEvent * gevent);

	//! get all pending events
	void GetPendingEvents(std::vector<GameEvent *> & events);


	//! add chat text
	void AddChatData(const ChatTextData & Data);

	//! get all chat texts
	void GetChatData(std::vector<ChatTextData> & data);


	//! called when someone joined or left list
	void HappenedJoinEvent(const JoinEvent & ev);

	//! get all events
	void GetJoinEventData(std::vector<JoinEvent> & data);


	//! called when someone joined or left list
	void ChatColorChanged(const std::string & nickname, const std::string & color);

	//! return the color changed since last time
	void GetColorChanges(std::vector<std::pair<std::string, std::string> > & vec);


	//! inform irc thread to quit
	void QuitIrc();

	//! wait one cycle before reconencting to irc
	bool WaitOneIrcCycle(long milliseconds);


	// add chat text to be send to server
	void AddChatText(const std::string & text);

	// get the text to be sent
	void GetChatText(std::vector<std::string> & texts);

	//! inform irc thread to quit
	void QuitSending();

	//! wait one cycle before reconencting to irc
	bool WaitOneSendingCycle();



	//! update info
	void UpdateInfo(const LbaNet::ActorInfo & ai);

	//! add request to work pile
	void ChangeMap(const MapChangedInformation & mi);

	//! process and empty work pile
	bool HasUpdatedInfo(LbaNet::ActorInfo & ai);

	//! process and empty work pile
	bool HasMapChanged(MapChangedInformation & mi);


	// update actor info
	void UpdateActor(const LbaNet::ActorInfo & ai);

	// remove the actor from the list if present
	void RemoveActor(const std::string & ActorName);

	// get the last update info
	void GetExtActorUpdate(std::vector<LbaNet::ActorInfo> & vec);

	// get the last update info
	void GetQuittedActors(std::vector<std::string> & vec);

	// add an activation event
	void AddActivationEvent(const LbaNet::ActorActivationInfo & ai);

	// get all activation event
	void GetAllActivationEvents(std::vector<LbaNet::ActorActivationInfo> & vec);

	// add an activation event
	void AddSignalEvent(const LbaNet::ActorSignalInfo & ai);

	// get all activation event
	void GetAllSignalEvents(std::vector<LbaNet::ActorSignalInfo> & vec);


	// add an activation event
	void Activated(const LbaNet::ActorActivationInfo & ai);

	// get all activation event
	void GetAllActivated(std::vector<LbaNet::ActorActivationInfo> & vec);

	// set if the server is on or not
	void SetServeron(bool serveron);

	// check if the server is on or not
	bool IsServeron();

	// set id
	void SetPlayerId(Ice::Long id)
	{m_player_id = id;}

	// get id
	Ice::Long GetPlayerId()
	{return m_player_id;}

	// update actors state
	void UpdateActorState(const std::vector<ActorStateInfo> & newstate);

	// return true if the sate has been updated
	bool ActorStatesUpdated(std::vector<ActorStateInfo> & newstate);

	// update actor life
	void UpdateActorLife(const LbaNet::ActorLifeInfo& ali);

	// get the last update info
	void GetExtActorLifeUpdate(std::vector<LbaNet::ActorLifeInfo> & vec);

	// add player hurted by another actor
	void AddPlayerHurt(long hurtingactorid);

	// add player hurted by falling down
	void AddPlayerHurtFall(float fallingdistance);

	// add player hurted by another actor
	void GetPlayerHurts(std::vector<long> & vec);

	// add player hurted by falling down
	void GetPlayerHurtFalls(std::vector<float> & vec);

	// add a raised event
	void AddRaisedEvent();

	// return true if there was a raised event
	bool IsRaised();

	// changed the display color for char name
	void ChangeNameColor(std::string color);

	// return true if the color has changed
	bool NameColorChanged(std::string &color);


	// inform server of change world
	void InformChangeWorld(const std::string & NewWorld);

	// check if world changed
	bool WorldChanged(std::string & NewWorld);

	// set the position of the player on the new world
	void SetNewWorldPlayerPos(const PlayerWorldPos & position);

	// wait for server to return with player position
	const PlayerWorldPos & WaitForPlayerPosition();


	// called when a shortcut is used
	void UseShortcut(int scnum);

	// called when a shortcut is used
	void GetUsedShorcut(std::vector<int> &scvec);

	// called when an object is used
	void InventoryObjectUsed(long itemid);

	// get list of used objects
	void GetListOfObjectUsed(std::vector<long> &objs);

protected:

	//! construtor
	ThreadSafeWorkpile()
		: m_game_quitted(false), m_irc_quitted(false), m_sending_quitted(false),
			m_send_cycle_time(20), m_is_updated(false), m_map_changed(false), m_server_on(false),
			m_player_id(-1), m_new_actor_state(false), m_name_color_changed(false),
			m_world_changed(false), m_player_pos_info_updated(false)
	{}

	ThreadSafeWorkpile(const ThreadSafeWorkpile &);
	const ThreadSafeWorkpile & operator=(const ThreadSafeWorkpile &);

private:
	IceUtil::Mutex								m_mutex_quitG;
	IceUtil::Mutex								m_mutex_events;
	IceUtil::Mutex								m_mutex_chat_send;
	IceUtil::Mutex								m_mutex_chat_data;
	IceUtil::Mutex								m_mutex_join_event;
	IceUtil::Mutex								m_mutex_position_info;
	IceUtil::Mutex								m_mutex_ext_actor_info;
	IceUtil::Mutex								m_mutex_activation;
	IceUtil::Mutex								m_mutex_external_activation;
	IceUtil::Mutex								m_mutex_server_on;
	IceUtil::Mutex								m_mutex_actor_state;
	IceUtil::Mutex								m_mutex_ext_actor_life_info;
	IceUtil::Mutex								m_mutex_player_hurt;
	IceUtil::Mutex								m_mutex_player_raised;
	IceUtil::Mutex								m_mutex_name_color;
	IceUtil::Mutex								m_mutex_color_changed;
	IceUtil::Mutex								m_mutex_world_changed;
	IceUtil::Mutex								m_mutex_shortcut_used;
	IceUtil::Mutex								m_mutex_inventory_used;

	IceUtil::Monitor<IceUtil::Mutex>			m_monitor_irc;
	IceUtil::Monitor<IceUtil::Mutex>			m_monitor_sending_loop;
	IceUtil::Monitor<IceUtil::Mutex>			m_monitor_player_pos_updated;

	bool										m_game_quitted;
	bool										m_irc_quitted;
	bool										m_sending_quitted;
	long										m_send_cycle_time;

	std::vector<GameEvent *>					m_events;
	std::vector<ChatTextData>					m_chat_data;
	std::vector<JoinEvent>						m_join_event;
	std::vector<std::string>					m_chat_texts;

	std::vector<LbaNet::ActorActivationInfo>	m_activation_events;
	std::vector<LbaNet::ActorActivationInfo>	m_ext_activations;
	std::vector<LbaNet::ActorSignalInfo>		m_signal_events;

	std::vector<long>							m_player_hurts;
	std::vector<float>							m_player_hurt_falls;

	bool										m_is_updated;
	LbaNet::ActorInfo							m_last_info;
	bool										m_map_changed;
	MapChangedInformation						m_map_changed_info;								

	std::vector<LbaNet::ActorInfo>				m_ext_info;
	std::vector<std::string>					m_quitted_actors;
	std::vector<LbaNet::ActorLifeInfo>			m_ext_life_info;

	bool										m_server_on;

	Ice::Long									m_player_id;

	bool										m_new_actor_state;
	std::vector<ActorStateInfo>					m_actors_states;

	bool										m_player_raised;

	std::string									m_name_color;
	bool										m_name_color_changed;

	std::vector<std::pair<std::string, std::string> >	m_colors_changed;

	bool										m_world_changed;
	std::string									m_new_world_name;


	bool										m_player_pos_info_updated;
	PlayerWorldPos								m_player_pos_info;


	std::vector<int>							m_shortcuts_used;
	std::vector<long>							m_inv_item_used;

	
	static ThreadSafeWorkpile *					_singletonInstance;
};

#endif