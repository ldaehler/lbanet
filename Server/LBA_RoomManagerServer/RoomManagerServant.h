#ifndef _ROOM_MANAGER_SERVANT_H
#define _ROOM_MANAGER_SERVANT_H

#include <RoomManager.h>

#include <Ice/Ice.h>
#include <IceStorm/IceStorm.h>

using namespace LbaNet;


class RoomManagerServant : public RoomManager, public IceUtil::Mutex
{
public:
	//! constructor
    RoomManagerServant(const Ice::CommunicatorPtr& communicator);

	//! the client join a chat room
    virtual ChatRoomObserverPrx JoinChat(const std::string& room, const ChatRoomObserverPrx& observer, 
												const Ice::Current&);

	//! the client leave a chat room
    virtual void LeaveChat(const std::string& room, const ChatRoomObserverPrx& observer, 
								const Ice::Current&);



	//! the client join an actor information room
    ActorsObserverPrx JoinActorRoom(const std::string& room, const std::string& actorName, const ActorsObserverPrx& observer, 
										const Ice::Current&);

	//! the client leave an actor information room
    void LeaveActorRoom(const std::string& room, const ActorsObserverPrx& observer, 
							const Ice::Current&);


private:
	std::map<std::string, IceStorm::TopicPrx> _chat_rooms;
	std::map<std::string, IceStorm::TopicPrx> _actors_rooms;
	const Ice::CommunicatorPtr& _communicator;
};

#endif
