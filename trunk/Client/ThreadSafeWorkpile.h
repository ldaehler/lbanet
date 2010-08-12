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
#include <LbaTypes.h>
#include "Actor.h"

class GameEvent;
class MainPlayerHandler;
class ExternalPlayersHandler;
class Player;

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

	struct UpdateInvContainer
	{
		long containerid;
		LbaNet::ItemList Taken;
		LbaNet::ItemList Put;
	};

	struct WrittenLetter
	{
		std::string subject;
		std::string message;
	};

	struct QuestUpdate
	{
		bool NeedUpdate;
		bool NeedReset;
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
	void RemoveActor(long ActorName);

	// get the last update info
	void GetExtActorUpdate(std::vector<LbaNet::ActorInfo> & vec);


	// update actor info
	void UpdateGhost(const LbaNet::GhostActorInfo & ai);

	// get the last update info
	void GetExtGhostUpdate(std::vector<LbaNet::GhostActorInfo> & vec);



	// get the last update info
	void GetQuittedActors(std::vector<long> & vec);

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

	// add an activation event
	void ActivationAborted(const LbaNet::ActorActivationInfo & ai);

	// get all activation event
	void GetAllActivationAborted(std::vector<LbaNet::ActorActivationInfo> & vec);

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

	// update single actor state
	void UpdateSingleActorState(const ActorStateInfo & newstate);

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


	// ask for a container info
	void AskForContainerInfo(long containerid);

	// check if player is waiting for container info
	bool HasAskedForContainer(long &containerid);

	// ask for a container info
	void OpenCloseContainer(long containerid, bool ForceClose);

	// check if player is waiting for container info
	bool HasOpenCloseContainer(long &containerid, bool &ForceClose);


	// called by server to give container update
	void UpdateContainer(const LbaNet::ContainerInfo & cinfo);

	// ask if we got an update
	bool IsUpdatedContainer(LbaNet::ContainerInfo & cinfo);


	// called by server to give container update from inventory
	void UpdateInvFromContainer(const UpdateInvContainer & cinfo);

	// ask if we got an update
	bool IsUpdatedInvFromContainer(UpdateInvContainer & cinfo);



	// add a whisper channel
	void AddWhisperChannel(const std::string & name);

	// cget all queries for whisper channel
	void GetWhisperChannelQueries(std::vector<std::string> &scvec);


	// add a whisper channel
	void AddFriend(const std::string & name);

	// cget all queries for whisper channel
	void GetAddedFriend(std::vector<std::string> &scvec);

	// add a whisper channel
	void RemoveFriend(long id);

	// cget all queries for whisper channel
	void GetRemovedFriend(std::vector<long> &scvec);

	//! set friend list
	void SetFriends(const LbaNet::FriendsSeq & friends);

	//! get friend list
	bool GetFriends(LbaNet::FriendsSeq & friends);

	//! accept a friend request
	void AcceptFriend(long id);

	// cget all queries for whisper channel
	void GetAcceptedFriend(std::vector<long> &scvec);

	//! ask server to refresh friend list
	void RefreshFriend();

	//! check if list should be refreshed
	bool ShouldRefreshFriends();


	//! add written letter to the server
	void AddWrittenLetter(const WrittenLetter & letter);

	//! get written letter to the server
	void GetWrittenLetters(std::vector<WrittenLetter> & letters);

	//! add query for letter info
	void AddLetterInfoQuery(long itemid);

	//! get written letter to the server
	void GetLetterInfoQuerys(std::vector<long> & letterqueries);

	//! destroy an inventory item
	void DestroyItem(long ItemId);

	//! get destroyed items
	void GetDestroyedItems(std::vector<long> & destroyedItems);

	//! set targetedacotr
	void SetTargetedActor(long actorid);

	//! get destroyed items
	void GetTargetedActors(std::vector<long> & targetedactors);

	//! set untargetedacotr
	void SetUntargetedActor(long actorid);

	//! get destroyed items
	void GetUntargetedActors(std::vector<long> & untargetedactors);


	//! set pointer to main player
	void SetMainPlayer(MainPlayerHandler * mplayer);

	//! set external players
	void SetExternalPlayer(ExternalPlayersHandler * explayers);

	//! get player from id
	Player * GetPlayer(long playerid);

	//! buy item 
	void BuyItem(long FromActorId, long itemid);

	//! get all items bought
	void GetBoughtItem(std::vector<std::pair<long,long> > & items);


	//! quest started
	void AddQuestStarted(long QuestId);

	//! quest finished
	void AddQuestFinished(long QuestId);


	//! quest started
	void GetQuestStarted(std::vector<long> & vec);

	//! quest finished
	void GetQuestFinished(std::vector<long> & vec);

	//! called when questbook needs an update
	void NeedQuestBookUpdate(bool reset);

	//! check if update needed
	QuestUpdate QuestBookUpdateNeeded();

	//! a magic ball was played by a player
	void MagicBallPlayed(long PlayerId, const LbaNet::LaunchInfo & linfo);

	//! get all magic ball played
	void GetMagicBallPlayed(std::vector<std::pair<long, LbaNet::LaunchInfo> > & vec);

	//! a magic ball come back
	void MagicBallComeback(long PlayerId);

	//! get all magic ball played
	void GetMagicBallComeback(std::vector<long> & vec);

	//! when player throw MB
	void ThrowMagicBall(const LbaNet::LaunchInfo & linfo);

	//! when MB comeback
	void MagicBallEnd();

	//! check if MB throwed
	bool MagicBallThrowed(LbaNet::LaunchInfo & linfo);

	//! check if MB Ended
	bool MagicBallEnded();

	//! MB hitted an actor
	void MbHittedActor(long ActorId);

	//! MB hitted an actor
	void MbHittedPlayer(long ActorId);

	//! MB hitted an actor
	void GetMbHittedActor(std::vector<long> &vec);

	//! MB hitted an actor
	void GetMbHittedPlayer(std::vector<long> &vec);

	//! inform server that player drow
	void InformDrown();

	//! check if player drowning
	bool IsDrowning();


	//! ask to get all pms
	void AskPMs();

	//! check if PM is asked
	bool IsPMAsked();

	//! ask to get all pms
	void ClosePMs();

	//! check if PM is asked
	bool IsPMClosed();

	//! send new pm
	void SendPM(const LbaNet::PMInfo &pm);

	//! get all pm to send
	void GetPMToSend(std::vector<LbaNet::PMInfo> &vec);

	//! delete a pm
	void DeletePM(long pmid);

	//! get all pm to send
	void GetPMToDelete(std::vector<long> &vec);

	//! delete a pm
	void MarkReadPM(long pmid);

	//! get all pm to send
	void GetPMMarkRead(std::vector<long> &vec);

	//! get next ghost id
	int GetNextGhostId();

	//! update ghost actor
	void UpdateIntGhost(LbaNet::GhostActorInfo ginfo);

	//! get last ghost updates
	void GetIntUpdateGhosts(std::vector<LbaNet::GhostActorInfo> & vec);

protected:

	//! construtor
	ThreadSafeWorkpile()
		: m_game_quitted(false), m_irc_quitted(false), m_sending_quitted(false),
			m_send_cycle_time(10), m_is_updated(false), m_map_changed(false), m_server_on(false),
			m_player_id(-1), m_new_actor_state(false), m_name_color_changed(false),
			m_world_changed(false), m_player_pos_info_updated(false), m_waiting_container_info(false),
			m_updated_container(false), m_exchanged_container(false), m_closed_container(false),
			m_mplayer(NULL), m_explayers(NULL), mb_throwed(false), mb_ended(false), m_drowning(false),
			m_newfriendlist(false), m_askpms(false), m_ghostid(0)
	{
		m_update_questbook.NeedUpdate = false;
	}

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
	IceUtil::Mutex								m_mutex_container_info;
	IceUtil::Mutex								m_mutex_container_updated;
	IceUtil::Mutex								m_mutex_container_deal;
	IceUtil::Mutex								m_mutex_container_exchange;
	IceUtil::Mutex								m_mutex_container_close;
	IceUtil::Mutex								m_mutex_whisper_chanel;
	IceUtil::Mutex								m_mutex_added_friend;
	IceUtil::Mutex								m_mutex_removed_friend;
	IceUtil::Mutex								m_mutex_accept_friend;
	IceUtil::Mutex								m_mutex_refresh_friend;
	IceUtil::Mutex								m_mutex_friend;
	IceUtil::Mutex								m_mutex_letters;
	IceUtil::Mutex								m_mutex_letter_queries;
	IceUtil::Mutex								m_mutex_destroy_items;
	IceUtil::Mutex								m_mutex_targeted_actors;
	IceUtil::Mutex								m_mutex_buy_items;
	IceUtil::Mutex								m_mutex_act_aborted;

	IceUtil::Mutex								m_mutex_start_quest;
	IceUtil::Mutex								m_mutex_end_quest;
	IceUtil::Mutex								m_mutex_questbook_update;

	IceUtil::Mutex								m_mutex_magic_ball_played;
	IceUtil::Mutex								m_mutex_magic_ball_comeback;
	IceUtil::Mutex								m_mutex_throw_MB;
	IceUtil::Mutex								m_mutex_ended_MB;
	IceUtil::Mutex								m_mutex_hitted_actors;
	IceUtil::Mutex								m_mutex_hitted_players;

	IceUtil::Mutex								m_mutex_drowing;

	IceUtil::Mutex								m_mutex_askpm;
	IceUtil::Mutex								m_mutex_sendpm;
	IceUtil::Mutex								m_mutex_deletepm;
	IceUtil::Mutex								m_mutex_markreadpm;

	IceUtil::Mutex								m_mutex_ghost;
	IceUtil::Mutex								m_mutex_ghostid;
	IceUtil::Mutex								m_mutex_ghostupd;

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
	std::vector<long>							m_quitted_actors;
	std::vector<LbaNet::ActorLifeInfo>			m_ext_life_info;
	std::vector<LbaNet::GhostActorInfo>			m_ext_ghost_info;
	std::vector<LbaNet::GhostActorInfo>			m_ghost_updates;

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

	bool										m_waiting_container_info;
	long										m_container_id;

	bool										m_updated_container;
	LbaNet::ContainerInfo						m_container;

	bool										m_exchanged_container;
	UpdateInvContainer							m_container_exchange;

	bool										m_closed_container;
	long										m_closed_container_id;
	bool										m_cont_ForceClose;


	std::vector<std::string>					m_asked_whispers;

	std::vector<std::string>					m_added_friends;
	std::vector<long>							m_removed_friends;
	std::vector<long>							m_accept_friends;
	LbaNet::FriendsSeq							m_friend_list;
	bool										m_refresh_friends;
	bool										m_newfriendlist;

	std::vector<WrittenLetter>					m_letters;

	std::vector<long>							m_letter_queries;
	std::vector<long>							m_destroyed_items;

	std::vector<long>							m_targetedactors;
	std::vector<long>							m_untargetedactors;

	MainPlayerHandler *							m_mplayer;
	ExternalPlayersHandler *					m_explayers;

	std::vector<std::pair<long,long> >			m_bought_items;

	std::vector<LbaNet::ActorActivationInfo>	m_act_aborted;

	std::vector<long>							m_quest_started;
	std::vector<long>							m_quest_ended;
	QuestUpdate									m_update_questbook;

	std::vector<std::pair<long, LbaNet::LaunchInfo> >	m_mb_played;
	std::vector<long>							m_mb_comeback;

	bool										mb_throwed;
	LbaNet::LaunchInfo							mb_info;

	bool										mb_ended;

	std::vector<long>							m_mb_hitted_actors;
	std::vector<long>							m_mb_hitted_players;

	bool										m_drowning;

	bool										m_askpms;
	std::vector<LbaNet::PMInfo>					m_pmtosend;
	std::vector<long>							m_pmtodelete;
	std::vector<long>							m_pmtomarkread;

	static ThreadSafeWorkpile *					_singletonInstance;

	int											m_ghostid;
};

#endif