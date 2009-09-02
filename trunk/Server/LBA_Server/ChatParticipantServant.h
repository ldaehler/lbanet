#ifndef _CHAT_PARTICIPANT_SERVANT_H
#define _CHAT_PARTICIPANT_SERVANT_H

#include <ChatInfo.h>
#include <RoomManager.h>

#include <string>

using namespace LbaNet;

class ChatParticipantServant : public ChatRoomParticipant
{
public:
	//! constructor
	ChatParticipantServant(const std::string& room, const ChatRoomObserverPrx& observer,
							const RoomManagerPrx& manager);
	
	//! destructor
    ~ChatParticipantServant();

	// say something
    virtual void Say(const std::string& sender, const std::string& text, const Ice::Current&);

private:
	const std::string _room;
    const RoomManagerPrx _manager;
    const ChatRoomObserverPrx _observer;
    const ChatRoomObserverPrx _publish;
};

#endif
