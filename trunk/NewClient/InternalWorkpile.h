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


#if !defined(__LbaNetModel_1_InternalWorkpile_h)
#define __LbaNetModel_1_InternalWorkpile_h

#include <string>
#include <vector>
#include <map>

#include "ChatSubscriberBase.h"
#include "ClientListHandlerBase.h"
#include "GameServerCallbackBase.h"
class GameEvent;
class MainPlayerHandler;
class ExternalPlayersHandler;
class Player;

/***********************************************************************
 * Module:  InternalWorkpile.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:59:17
 * Purpose: Declaration of the class InternalWorkpile
 * Be carefull - this is not thread safe!!
 ***********************************************************************/
class InternalWorkpile : public ChatSubscriberBase, public ClientListHandlerBase, public GameServerCallbackBase
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

	//struct UpdateInvContainer
	//{
	//	long containerid;
	//	LbaNet::ItemList Taken;
	//	LbaNet::ItemList Put;
	//};

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
	~InternalWorkpile();

	// singleton pattern
	static InternalWorkpile * getInstance();

	//! inform the event handler to quit game
	void QuitGame();

	//! check if the game need to be quitted
	bool GameQuitted();

	//! add an event to the pile
	void AddEvent(GameEvent * gevent);

	//! get all pending events
	void GetPendingEvents(std::vector<GameEvent *> & events);


	//! add chat text coming from the server
	void ReceivedText(const std::string & Channel, const std::string & SenderName, 
																const std::string & Text);

	//! get all chat texts to be displayed on the GUI
	void GetChatData(std::vector<ChatTextData> & data);


	//! called when someone joined or left list
	void HappenedJoinEvent(const JoinEvent & ev);

	//! get all events
	void GetJoinEventData(std::vector<JoinEvent> & data);


	//! called when someone joined or left list
	void ChatColorChanged(const std::string & nickname, const std::string & color);

	//! return the color changed since last time
	void GetColorChanges(std::vector<std::pair<std::string, std::string> > & vec);

	// add chat text to be send to server
	void AddChatText(const std::string & text);

	// get the text to be sent
	void GetChatText(std::vector<std::string> & texts);


	// add a whisper channel
	void AddWhisperChannel(const std::string & name);

	// cget all queries for whisper channel
	void GetWhisperChannelQueries(std::vector<std::string> &scvec);




	// add a whisper channel
	void AddFriend(const std::string & name);

	// cget all queries for whisper channel
	void GetAddedFriend(std::vector<std::string> &scvec);

	// add a whisper channel
	void RemoveFriend(const std::string & name);

	// cget all queries for whisper channel
	void GetRemovedFriend(std::vector<std::string> &scvec);

	//! set friend list
	void SetFriends(const std::vector<std::string> & friends);

	//! get friend list
	void GetFriends(std::vector<std::string> & friends);



	// new client connected
	virtual void Connected(unsigned int id, const std::string & Name, 
							const std::string & Status, const std::string & Color);

	// client disconnected
	virtual void Disconnected(unsigned int id);

	// client changed status
	virtual void ChangedStatus(unsigned int id, const std::string & Status, const std::string & Color);

	// return the name given a client id
	virtual std::string GetName(unsigned int id);

	// return the name given a client id
	virtual unsigned int GetId(std::string Name);




	//! set if player moved
	void PlayerMoved();

	//! return true if player has moved
	bool HasPlayerMoved();


	// changed the display color for char name
	void ChangeNameColor(std::string color);

	// return true if the color has changed
	bool NameColorChanged(std::string &color);


	// received server address
	virtual void ReceivedAddress(const std::string & Server, const std::string & Address);

	// received aknowledgement
	virtual void Aknowldeged(bool Ok){}


	////! inform irc thread to quit
	//void QuitIrc();

	////! wait one cycle before reconencting to irc
	//bool WaitOneIrcCycle(long milliseconds);


	////! inform irc thread to quit
	//void QuitSending();

	////! wait one cycle before reconencting to irc
	//bool WaitOneSendingCycle();


	////! update info
	//void UpdateInfo(const LbaNet::ActorInfo & ai);

	////! add request to work pile
	//void ChangeMap(const MapChangedInformation & mi);

	////! process and empty work pile
	//bool HasUpdatedInfo(LbaNet::ActorInfo & ai);

	////! process and empty work pile
	//bool HasMapChanged(MapChangedInformation & mi);


	//// update actor info
	//void UpdateActor(const LbaNet::ActorInfo & ai);

	//// remove the actor from the list if present
	//void RemoveActor(long ActorName);

	//// get the last update info
	//void GetExtActorUpdate(std::vector<LbaNet::ActorInfo> & vec);

	//// get the last update info
	//void GetQuittedActors(std::vector<long> & vec);

	//// add an activation event
	//void AddActivationEvent(const LbaNet::ActorActivationInfo & ai);

	//// get all activation event
	//void GetAllActivationEvents(std::vector<LbaNet::ActorActivationInfo> & vec);

	//// add an activation event
	//void AddSignalEvent(const LbaNet::ActorSignalInfo & ai);

	//// get all activation event
	//void GetAllSignalEvents(std::vector<LbaNet::ActorSignalInfo> & vec);


	//// add an activation event
	//void Activated(const LbaNet::ActorActivationInfo & ai);

	//// get all activation event
	//void GetAllActivated(std::vector<LbaNet::ActorActivationInfo> & vec);

	//// add an activation event
	//void ActivationAborted(const LbaNet::ActorActivationInfo & ai);

	//// get all activation event
	//void GetAllActivationAborted(std::vector<LbaNet::ActorActivationInfo> & vec);

	//// set if the server is on or not
	//void SetServeron(bool serveron);

	//// check if the server is on or not
	//bool IsServeron();

	//// set id
	//void SetPlayerId(Ice::Long id)
	//{m_player_id = id;}

	//// get id
	//Ice::Long GetPlayerId()
	//{return m_player_id;}

	//// update single actor state
	//void UpdateSingleActorState(const ActorStateInfo & newstate);

	//// update actors state
	//void UpdateActorState(const std::vector<ActorStateInfo> & newstate);

	//// return true if the sate has been updated
	//bool ActorStatesUpdated(std::vector<ActorStateInfo> & newstate);

	//// update actor life
	//void UpdateActorLife(const LbaNet::ActorLifeInfo& ali);

	//// get the last update info
	//void GetExtActorLifeUpdate(std::vector<LbaNet::ActorLifeInfo> & vec);

	//// add player hurted by another actor
	//void AddPlayerHurt(long hurtingactorid);

	//// add player hurted by falling down
	//void AddPlayerHurtFall(float fallingdistance);

	//// add player hurted by another actor
	//void GetPlayerHurts(std::vector<long> & vec);

	//// add player hurted by falling down
	//void GetPlayerHurtFalls(std::vector<float> & vec);

	//// add a raised event
	//void AddRaisedEvent();

	//// return true if there was a raised event
	//bool IsRaised();



	//// inform server of change world
	//void InformChangeWorld(const std::string & NewWorld);

	//// check if world changed
	//bool WorldChanged(std::string & NewWorld);

	//// set the position of the player on the new world
	//void SetNewWorldPlayerPos(const PlayerWorldPos & position);

	//// wait for server to return with player position
	//const PlayerWorldPos & WaitForPlayerPosition();


	//// called when a shortcut is used
	//void UseShortcut(int scnum);

	//// called when a shortcut is used
	//void GetUsedShorcut(std::vector<int> &scvec);

	//// called when an object is used
	//void InventoryObjectUsed(long itemid);

	//// get list of used objects
	//void GetListOfObjectUsed(std::vector<long> &objs);


	//// ask for a container info
	//void AskForContainerInfo(long containerid);

	//// check if player is waiting for container info
	//bool HasAskedForContainer(long &containerid);

	//// ask for a container info
	//void OpenCloseContainer(long containerid, bool ForceClose);

	//// check if player is waiting for container info
	//bool HasOpenCloseContainer(long &containerid, bool &ForceClose);


	//// called by server to give container update
	//void UpdateContainer(const LbaNet::ContainerInfo & cinfo);

	//// ask if we got an update
	//bool IsUpdatedContainer(LbaNet::ContainerInfo & cinfo);


	//// called by server to give container update from inventory
	//void UpdateInvFromContainer(const UpdateInvContainer & cinfo);

	//// ask if we got an update
	//bool IsUpdatedInvFromContainer(UpdateInvContainer & cinfo);




	////! add written letter to the server
	//void AddWrittenLetter(const WrittenLetter & letter);

	////! get written letter to the server
	//void GetWrittenLetters(std::vector<WrittenLetter> & letters);

	////! add query for letter info
	//void AddLetterInfoQuery(long itemid);

	////! get written letter to the server
	//void GetLetterInfoQuerys(std::vector<long> & letterqueries);

	////! destroy an inventory item
	//void DestroyItem(long ItemId);

	////! get destroyed items
	//void GetDestroyedItems(std::vector<long> & destroyedItems);

	////! set targetedacotr
	//void SetTargetedActor(long actorid);

	////! get destroyed items
	//void GetTargetedActors(std::vector<long> & targetedactors);

	////! set untargetedacotr
	//void SetUntargetedActor(long actorid);

	////! get destroyed items
	//void GetUntargetedActors(std::vector<long> & untargetedactors);


	////! set pointer to main player
	//void SetMainPlayer(MainPlayerHandler * mplayer);

	////! set external players
	//void SetExternalPlayer(ExternalPlayersHandler * explayers);

	////! get player from id
	//Player * GetPlayer(long playerid);

	////! buy item 
	//void BuyItem(long FromActorId, long itemid);

	////! get all items bought
	//void GetBoughtItem(std::vector<std::pair<long,long> > & items);


	////! quest started
	//void AddQuestStarted(long QuestId);

	////! quest finished
	//void AddQuestFinished(long QuestId);


	////! quest started
	//void GetQuestStarted(std::vector<long> & vec);

	////! quest finished
	//void GetQuestFinished(std::vector<long> & vec);

	////! called when questbook needs an update
	//void NeedQuestBookUpdate(bool reset);

	////! check if update needed
	//QuestUpdate QuestBookUpdateNeeded();



protected:

	//! construtor
	InternalWorkpile()
		: m_game_quitted(false)//, m_irc_quitted(false), m_sending_quitted(false),
		//	m_send_cycle_time(20), m_is_updated(false), m_map_changed(false), m_server_on(false),
		//	m_player_id(-1), m_new_actor_state(false), m_name_color_changed(false),
		//	m_world_changed(false), m_player_pos_info_updated(false), m_waiting_container_info(false),
		//	m_updated_container(false), m_exchanged_container(false), m_closed_container(false),
		//	m_mplayer(NULL), m_explayers(NULL)
	{
		//m_update_questbook.NeedUpdate = false;
	}

	InternalWorkpile(const InternalWorkpile &);
	const InternalWorkpile & operator=(const InternalWorkpile &);

private:
	bool										m_game_quitted;

	std::vector<GameEvent *>					m_events;
	std::vector<ChatTextData>					m_chat_data;
	std::vector<JoinEvent>						m_join_event;
	std::vector<std::string>					m_chat_texts;
	std::vector<std::string>					m_asked_whispers;
	std::vector<std::pair<std::string, std::string> >	m_colors_changed;


	std::vector<std::string>					m_added_friends;
	std::vector<std::string>					m_removed_friends;
	std::vector<std::string>					m_friend_list;
	std::map<unsigned int, std::string>			m_clientmap;

	bool										m_player_moved;

	std::string									m_name_color;
	bool										m_name_color_changed;


	//bool										m_irc_quitted;
	//bool										m_sending_quitted;
	//long										m_send_cycle_time;

	//std::vector<LbaNet::ActorActivationInfo>	m_activation_events;
	//std::vector<LbaNet::ActorActivationInfo>	m_ext_activations;
	//std::vector<LbaNet::ActorSignalInfo>		m_signal_events;

	//std::vector<long>							m_player_hurts;
	//std::vector<float>							m_player_hurt_falls;

	//bool										m_is_updated;
	//LbaNet::ActorInfo							m_last_info;
	//bool										m_map_changed;
	//MapChangedInformation						m_map_changed_info;							

	//std::vector<LbaNet::ActorInfo>				m_ext_info;
	//std::vector<long>					m_quitted_actors;
	//std::vector<LbaNet::ActorLifeInfo>			m_ext_life_info;

	//bool										m_server_on;

	//Ice::Long									m_player_id;

	//bool										m_new_actor_state;
	//std::vector<ActorStateInfo>					m_actors_states;

	//bool										m_player_raised;




	//bool										m_world_changed;
	//std::string									m_new_world_name;


	//bool										m_player_pos_info_updated;
	//PlayerWorldPos								m_player_pos_info;


	//std::vector<int>							m_shortcuts_used;
	//std::vector<long>							m_inv_item_used;

	//bool										m_waiting_container_info;
	//long										m_container_id;

	//bool										m_updated_container;
	//LbaNet::ContainerInfo						m_container;

	//bool										m_exchanged_container;
	//UpdateInvContainer							m_container_exchange;

	//bool										m_closed_container;
	//long										m_closed_container_id;
	//bool										m_cont_ForceClose;




	//std::vector<WrittenLetter>					m_letters;

	//std::vector<long>							m_letter_queries;
	//std::vector<long>							m_destroyed_items;

	//std::vector<long>							m_targetedactors;
	//std::vector<long>							m_untargetedactors;

	//MainPlayerHandler *							m_mplayer;
	//ExternalPlayersHandler *					m_explayers;

	//std::vector<std::pair<long,long> >			m_bought_items;

	//std::vector<LbaNet::ActorActivationInfo>	m_act_aborted;

	//std::vector<long>							m_quest_started;
	//std::vector<long>							m_quest_ended;
	//QuestUpdate									m_update_questbook;

	static InternalWorkpile *					_singletonInstance;
};

#endif