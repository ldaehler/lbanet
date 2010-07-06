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


#ifndef _SESSION_SERVANT_H
#define _SESSION_SERVANT_H

#include <ClientSession.h>
#include <RoomManager.h>
#include <ActorInfo.h>
#include <ConnectedTracker.h>
#include <MapManager.h>

#include <map>

#include "ChatParticipantServant.h"
#include "ActorsParticipantServant.h"
#include "DatabaseHandler.h"
#include "WorldInfo.h"

#include "InventoryHandlerBase.h"
#include "QuestHandler.h"	


using namespace LbaNet;


class SessionServant : public ClientSession, public IceUtil::Mutex, public InventoryHandlerBase
{
public:
	//! constructor
	SessionServant(const std::string& userId, const RoomManagerPrx& manager,
					const ConnectedTrackerPrx& ctracker, const MapManagerPrx& map_manager,
					std::string	version, DatabaseHandler & dbh);

	//! the client join a chat room
    virtual ChatRoomParticipantPrx JoinChat(const std::string& room, const ChatRoomObserverPrx& observer,
												const Ice::Current&);

	//! the client leave a chat room
    virtual void LeaveChat(const std::string& room, const ChatRoomObserverPrx& observer,
								const Ice::Current&);


	//! the client change actor information room
	virtual ActorsParticipantPrx ChangeRoom(	const std::string& newroom,
												const std::string & actorname,
												const ActorsObserverPrx& observer, const Ice::Current&);


	//! destroy the session
	virtual void destroy(const Ice::Current&);

	//! get the list of people connected
	LbaNet::ConnectedL GetConnected(Ice::Long & ownid, const Ice::Current&);


	// callback function called when an actor id activated
    virtual void ActivateActor(const LbaNet::ActorActivationInfo& ai, const Ice::Current&);

	// callback function called when an actor id signaled
    virtual void SignalActor(const LbaNet::ActorSignalInfo& ai, const Ice::Current&);

	//get updated info
	virtual LbaNet::UpdateSeq GetUpdatedInfo(const Ice::Current&);

	//! get player info
	virtual LbaNet::PlayerSeq GetPlayersInfo(const Ice::Current&);

	// change player status 
	virtual void ChangeStatus(const std::string& Status, const Ice::Current&);

	//! get server time
	Ice::Long GetTime(const Ice::Current&);

	//! get server version
	std::string GetVersion(const Ice::Current&);

	//! return the current life state
    virtual LbaNet::ActorLifeInfo GetLifeInfo(const Ice::Current&);

	//! called when actor have been hurt
    virtual void GotHurtByActor(Ice::Long hurtingactorid, const Ice::Current&);

	//! called when actor have been hurt
    virtual void GotHurtByFalling(Ice::Float fallingdistance, const Ice::Current&);

	//! player is dead and reborn
    virtual void PlayerRaisedFromDead(const Ice::Current&);
	       
    //called when player is drowning
    virtual void PlayerDrowning(const Ice::Current&);

	// change name display color
	virtual void ChangeNameColor(const std::string& Color, const Ice::Current&);

	// player has changed world
	virtual LbaNet::SavedWorldInfo ChangeWorld(const std::string& WorldName, const Ice::Current&);

	// player update his current position in the world
	virtual void UpdatePositionInWorld(const LbaNet::PlayerPosition& Position, const Ice::Current&);

	// player update his current inventory gui info
	virtual void UpdateInventory(const InventoryInfo &Inventory, const Ice::Current&);

	// player use an item from inventory 
	virtual void UseItem(Ice::Long ItemId, const Ice::Current&);
	    
	// get container content  
	virtual void AskForContainerContent(Ice::Long ContainerId, const Ice::Current&);

	//update player inventory from container content
	virtual void UpdateInventoryFromContainer(Ice::Long ContainerId, const ItemList &Taken, const ItemList &Put, const Ice::Current&);

   	//check if we have item in inventory
    virtual bool HasItem(Ice::Long ItemId, int QUantity, const Ice::Current&);
   
   // callback functions to apply inventory changes
    virtual void ApplyInventoryChanges(const UpdatedItemSeq &InventoryChanges, const Ice::Current&);

    // callback functions to update container info
    virtual  void UpdateContainerInfo(const ContainerInfo &container, const Ice::Current&);	

	// set self pointer
	 virtual void setSelfPointer(const LbaNet::ClientSessionPrx &ptr){_selfptr = ptr;}

	//! set player wisper interface
    virtual void SetWhisperInterface(const LbaNet::ChatRoomObserverPrx& winterface, const Ice::Current&);

	//! a player wisper to another
    virtual bool Whisper(const std::string& To, const std::string& Message, const ::Ice::Current&);


    // add friend function
    virtual void AskFriend(const std::string &friendname, const ::Ice::Current&);

	// accept friend invitation
    virtual void AcceptFriend(Ice::Long friendid, const ::Ice::Current&);

    // remove friend function
	virtual void RemoveFriend(Ice::Long friendid, const ::Ice::Current&);

    // get friends function
    virtual LbaNet::FriendsSeq GetFriends(const ::Ice::Current&);

    
    // store letter to the server and return the letter id
    virtual void AddLetter(const std::string& title, const std::string& message, const ::Ice::Current&);
    
    // return letter info
    virtual LbaNet::LetterInfo GetLetterInfo(Ice::Long LetterId, const ::Ice::Current&);
	    
	// destroy an inventory item
	virtual void DestroyItem(Ice::Long  Id, const ::Ice::Current&);
	    
    // set player targeted by actor
    virtual void SetTargeted(Ice::Long ActorId, const ::Ice::Current&);
        
    // set player untargeted by actor
    virtual void SetUnTargeted(Ice::Long ActorId, const ::Ice::Current&);
  
    // buy item
    virtual void BuyItem(Ice::Long FromActorId, Ice::Long Itemid, const ::Ice::Current&);
	    
	// tell client only if actor is activated
	virtual void ActivatedActor(const LbaNet::ActorActivationInfo &ai, bool succeded, const ::Ice::Current&);

	// start a quest
    virtual void StartQuest(Ice::Long QuestId, const ::Ice::Current&);
	
	// end a quest
    virtual void EndQuest(Ice::Long QuestId, const ::Ice::Current&);


	//! get the number of item in inventory
	virtual int GetItemNumber(long id);

	//! update item number
	virtual void UpdateItemNumber(long itemid, int deltaCount);	


	//! inform class that a quest has been started
	virtual void InformQuestStarted(long Questid);

	//! inform class that a quest has been finished
	virtual void InformQuestFinished(long Questid);

    
    // called when player throw magic ball
    virtual void MagicBallStart(const LaunchInfo & linfo, const ::Ice::Current&);
    
    // called when magic ball is back
    virtual void MagicBallEnd(const ::Ice::Current&);
    
    // called when magic ball touch an actor
    virtual void MagicBallTouchActor(Ice::Long ActorId, const ::Ice::Current&);
    
    // called when magic ball touch a player
    virtual void MagicBallTouchPlayer(Ice::Long ActorId, const ::Ice::Current&);
	    
	// update current life of player - called by map server
	virtual void UpdatedLife(const ActorLifeInfo &ali, const ::Ice::Current&);

protected:
	void ApplyInternalInventoryChanges(const UpdatedItemSeq &InventoryChanges);

	void QuitCurrentWorld();
	void cleanEphemereItems();

	// init client with quests started and finished
	void InitializeClientQuests(std::vector<long> questStarted, std::vector<long> questFinished);

private:
	std::string							_userId;
	Ice::Long							_userNum;

	std::string							_currStatus;
	std::string							_currColor;

	std::map<std::string, ChatRoomParticipantPrx>	_chat_rooms;

	std::string							_curr_actor_room;
	ActorsParticipantPrx				_actors_room;
	LbaNet::MapObserverPrx				_actors_manager;

	const RoomManagerPrx				_manager;
	const ConnectedTrackerPrx			_ctracker;
	const MapManagerPrx					_map_manager;

    ActorsObserverPrx					_client_observer;

	std::string							_version;

	LbaNet::ActorLifeInfo				_lifeinfo;

	DatabaseHandler &					_dbh;
	std::string							_currWorldName;


	InventoryInfo						_playerInventory;
	std::map<long, ItemInfo>			_inventory_db;
	std::map<std::string, std::string>	_session_world_inventory_files;
	std::map<std::string, std::string>	_session_world_quest_files;

	LbaNet::ClientSessionPrx			_selfptr;

	QuestHandler						_QH;
	bool								_needquestupdate;

	bool								_magicballused;

	bool								_playerdead;
};

#endif
