#ifndef _SESSION_SERVANT_H
#define _SESSION_SERVANT_H

#include <ClientSession.h>

#include <Ice/Ice.h>
#include <IceStorm/IceStorm.h>

using namespace LbaNet;


class SessionServant : public ClientSession, public IceUtil::Mutex
{
public:
	//! constructor
    SessionServant(const Ice::CommunicatorPtr& communicator);

	//! the client join a chat room
    virtual ChatRoomObserverPrx JoinChat(const std::string& room, const ChatRoomObserverPrx& observer, 
												const Ice::Current&);

	//! the client leave a chat room
    virtual void LeaveChat(const std::string& room, const ChatRoomObserverPrx& observer, 
								const Ice::Current&);


	//! the client change actor information room
	virtual ActorsObserverPrx ChangeRoom(const std::string& oldroom, const std::string& newroom, 
												const ActorsObserverPrx& observer, const Ice::Current&);

	//! destroy the session
	virtual void destroy(const Ice::Current&);

protected:

	//! the client join an actor information room
    ActorsObserverPrx JoinActorRoom(const std::string& room, const ActorsObserverPrx& observer);

	//! the client leave an actor information room
    void LeaveActorRoom(const std::string& room, const ActorsObserverPrx& observer);


private:
	std::map<std::string, IceStorm::TopicPrx> _chat_rooms;
	std::map<std::string, IceStorm::TopicPrx> _actors_rooms;
	const Ice::CommunicatorPtr& _communicator;
};

#endif
