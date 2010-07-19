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


#ifndef _LBANET_SENDING_LOOP_
#define _LBANET_SENDING_LOOP_

#include <IceUtil/Thread.h>
#include <ActorInfo.h>
#include <ChatInfo.h>
#include <ClientSession.h>
#include "ThreadSafeWorkpile.h"
#include <fstream>

class GameGUI;


class IceConnectionManager
{
public:
	// constructor
	IceConnectionManager(const Ice::ObjectAdapterPtr& adapter,
							const LbaNet::ClientSessionPrx& session,
							const std::string& category);

	// find a chat room if we already joined
	LbaNet::ChatRoomParticipantPrx FindChatRoom(const std::string& room) const;

	// join a chat channel
    bool JoinChat(const std::string& roomName);

	// leave a chat channel
    bool LeaveChat(const std::string& roomName);

	// change the room for actor info
	bool ChangeActorsRoom(const std::string& newRoom, const std::string& actorName);

	// get the publisher to the actors channel
	LbaNet::ActorsParticipantPrx GetActorPublisher();

	// activate an actor
	void ActivateActor(const LbaNet::ActorActivationInfo & info);

	// signal an actor
	void SignalActor(const LbaNet::ActorSignalInfo & info);

	// change player status
	void ChangeStatus(const std::string& status);

	// change player color name
	void ChangeColor(const std::string& color);

	// return current player life state
	LbaNet::ActorLifeInfo GetPlayerLife();

	// send player hurt event
	void AddPlayerHurt(long hurtingid);

	// send player hurt fall event
	void AddPlayerHurtFall(float fallingdistance);

	// player has been raised
	void PlayerRaised();

	// inform server drow
	void Drown();

	// player has changed world
	LbaNet::SavedWorldInfo ChangeWorld(const std::string& WorldName);

	// player update his current position in the world
	void UpdatePositionInWorld(const LbaNet::PlayerPosition& Position);

	// update inventory
	void UpdateInventory(const LbaNet::InventoryInfo &Inventory);


	// called to tell the server and obejct has been used
	void InventoryUsed(long ItemId);

	// ask for a container info
	void AskForContainerInfo(long containerid);

	// called by server to give container update from inventory
	void UpdateInvFromContainer(const ThreadSafeWorkpile::UpdateInvContainer & cinfo);

	//! a player wisper to another
    bool Whisper(const std::string& To, const std::string& Message);

	//! add friend
    void AddFriend(const std::string& Name);

	//! remove friend
    void RemoveFriend(long id);

	//! accept a friend request
	void AcceptFriend(long friendid);

	//! refresh friend list
	void RefreshFriend();

	//! send letter to the server
	void SendLetter(const ThreadSafeWorkpile::WrittenLetter &wl);

	//! ask for letter info
	void AskLetterInfo(long letterId);

	//! destroy item
	void DestroyItem(long Id);

	    
    // set player targeted by actor
    void SetTargeted(long ActorId);
    
	    
    // set player untargeted by actor
    void SetUnTargeted(long ActorId);    

	//! buy item 
	void BuyItem(long FromActorId, long itemid);


	//! start quest
	void StartQuest(long QuestId);

	//! end quest
	void EndQuest(long QuestId);

	//! when player throw MB
	void ThrowMagicBall(const LbaNet::LaunchInfo & linfo);

	//! when MB comeback
	void MagicBallEnd();

	//! MB hitted an actor
	void MbHittedActor(const std::vector<long> &vec);

	//! MB hitted an actor
	void MbHittedPlayer(const std::vector<long> &vec);


	//! ask server for pms
	void AskPms();

	//! send new pm
	void SendPM(const LbaNet::PMInfo &pm);

	//! delete pm
	void DeletePM(long pmid);

private:

    const Ice::ObjectAdapterPtr							_adapter;	// ice adapter
    const LbaNet::ClientSessionPrx						_session;	// glacier session

	std::map<std::string, LbaNet::ChatRoomParticipantPrx>	_chat_publishers;	// pointer to the chat rooms publishers
	std::map<std::string, LbaNet::ChatRoomObserverPrx>	_chat_observers;	// pointer to the chat rooms observers
	LbaNet::ActorsParticipantPrx						_actors_publisher;	// publisher of the actor
	LbaNet::ActorsObserverPrx							_actors_observer;	// observer of the actos info

	const std::string									_category; // category
};







/***********************************
*	Ice interface implementation
*************************************/
class SendingLoopThread : public IceUtil::Thread
{
public:
	//! constructor
	SendingLoopThread(	const Ice::ObjectAdapterPtr& adapter,
						const LbaNet::ClientSessionPrx& session,
						const std::string& category,
						const std::string & MainName);

	// running function of the thread
	virtual void run();

protected:
	// when the user change room
	bool ChangeMap(const ThreadSafeWorkpile::MapChangedInformation & NewMap);

	// when the engine update information
	void UpdateActorInfo(const LbaNet::ActorInfo & MainInfo);

	// when a text has been written on the chat
	void HandleChatText();

	// process a line of text
	void ProcessText(const std::string & Text);

	// when the engine update information
	void AddActivation(const LbaNet::ActorActivationInfo & info);

	// when the engine update information
	void AddSignal(const LbaNet::ActorSignalInfo & info);


private:
	std::string				_current_world;	// current playing world
	std::string				_current_map;	// current playing map
	std::string				_main_name;		// player name
	LbaNet::ActorInfo		_last_ai;		// last actor info
	long					_refresh_counter;
	long					_afk_counter;
	bool					_afked;
	bool					_previousworld;

	IceConnectionManager	_connectionMananger;	// ice connection manager

	//std::ofstream			_dumplog;
};

#endif
