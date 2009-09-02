#include "SessionServant.h"


//! constructor
SessionServant::SessionServant(const Ice::CommunicatorPtr& communicator)
: _communicator(communicator)
{

}

//! the client join a chat room
ChatRoomObserverPrx SessionServant::JoinChat(	const std::string& room, 
														const ChatRoomObserverPrx& observer, 
														const Ice::Current&)
{
	Lock sync(*this);

	// check if we already created this room
	std::map<std::string, IceStorm::TopicPrx>::const_iterator p = _chat_rooms.find(room);
	IceStorm::TopicPrx topic;


	// if not create the chat room
	if(p == _chat_rooms.end())
	{
		IceStorm::TopicManagerPrx manager;
		try
		{
			manager = IceStorm::TopicManagerPrx::uncheckedCast(
			_communicator->propertyToProxy("TopicManagerProxy"));
		}
		catch(const IceUtil::Exception& ex)
		{
			std::cout<<"Exception getting the topic manager: "<<ex.what()<<std::endl;
		}
		catch(...)
		{
			std::cout<<"Unknown exception getting the topic manager. "<<std::endl;
		}

		try
		{
			topic = manager->create(room);
		}
		catch(const IceStorm::TopicExists&)
		{
			topic = manager->retrieve(room);
		}
		catch(const IceUtil::Exception& ex)
		{
			std::cout<<"Exception creating topic "<<ex.what()<<std::endl;
		}
		catch(...)
		{
			std::cout<<"Unknown exception creating topic. "<<std::endl;
		}

		_chat_rooms.insert(make_pair(room, topic));
	}
	else
	{
		topic = p->second;
	}

	try
	{
		topic->subscribeAndGetPublisher(IceStorm::QoS(), observer);
	}
	catch(const IceUtil::Exception& ex)
	{
		std::cout<<"Exception subscribing "<<ex.what()<<std::endl;
	}
	catch(...)
	{
		std::cout<<"Unknown exception subscribing. "<<std::endl;
	}

	Ice::ObjectPrx pub; 

	try
	{
		pub = topic->getPublisher();
		pub = pub->ice_oneway();
	}
	catch(const IceUtil::Exception& ex)
	{
		std::cout<<"Exception getting the publisher "<<ex.what()<<std::endl;
	}
	catch(...)
	{
		std::cout<<"Unknown exception getting the publisher. "<<std::endl;
	}

	std::cout<<"hello de lu 4"<<std::endl;

	ChatRoomObserverPrx tmp = ChatRoomObserverPrx::uncheckedCast(pub);
	tmp->Message("Server", "Fart0");
	tmp->Message("Server", "Fart1");
	tmp->Message("Server", "Fart2");
	tmp->Message("Server", "Fart3");

	//Ice::Context context;
	//context["_fwd"] = "Oz";
	return ChatRoomObserverPrx::uncheckedCast(pub/*->ice_context(context)*/);
}

//! the client leave a chat room
void SessionServant::LeaveChat(	const std::string& room, const ChatRoomObserverPrx& observer, 
										const Ice::Current&)
{
	Lock sync(*this);

	try
	{
		std::map<std::string, IceStorm::TopicPrx>::iterator p = _chat_rooms.find(room);
		if(p != _chat_rooms.end())
		{
			p->second->unsubscribe(observer);
		}
	}
	catch(const IceUtil::Exception& ex)
	{
		std::cout<<"Exception unsubscribing "<<ex.what()<<std::endl;
	}
	catch(...)
	{
		std::cout<<"Unknown exception unsubscribing. "<<std::endl;
	}
}





//! the client join an actor information room
ActorsObserverPrx SessionServant::JoinActorRoom(const std::string& room, 
												const ActorsObserverPrx& observer)
{
	Lock sync(*this);

	// check if we already created this room
	std::map<std::string, IceStorm::TopicPrx>::const_iterator p = _actors_rooms.find(room);
	IceStorm::TopicPrx topic;

	// if not create the chat room
	if(p == _actors_rooms.end())
	{
		IceStorm::TopicManagerPrx manager;
		try
		{
			manager = IceStorm::TopicManagerPrx::uncheckedCast(
			_communicator->propertyToProxy("TopicManagerProxy"));
		}
		catch(const IceUtil::Exception& ex)
		{
			std::cout<<"Exception getting the actor topic manager: "<<ex.what()<<std::endl;
		}
		catch(...)
		{
			std::cout<<"Unknown exception getting the actor topic manager. "<<std::endl;
		}

		try
		{
			topic = manager->create(room);
		}
		catch(const IceStorm::TopicExists&)
		{
			topic = manager->retrieve(room);
		}
		catch(const IceUtil::Exception& ex)
		{
			std::cout<<"Exception creating actor topic "<<ex.what()<<std::endl;
		}
		catch(...)
		{
			std::cout<<"Unknown exception creating actor topic. "<<std::endl;
		}

		_actors_rooms.insert(make_pair(room, topic));
	}
	else
	{
		topic = p->second;
	}

	Ice::ObjectPrx pub;
	try
	{
		pub = topic->subscribeAndGetPublisher(IceStorm::QoS(), observer);
	}
	catch(const IceUtil::Exception& ex)
	{
		std::cout<<"Exception creating actor publisher "<<ex.what()<<std::endl;
	}
	catch(...)
	{
		std::cout<<"Unknown exception creating actor publisher. "<<std::endl;
	}
	//Ice::Context context;
	//context["_fwd"] = "Oz";
	return ActorsObserverPrx::uncheckedCast(pub/*->ice_context(context)*/);
}


//! the client leave an actor information room
void SessionServant::LeaveActorRoom(const std::string& room, const ActorsObserverPrx& observer)
{
	Lock sync(*this);
	try
	{
		std::map<std::string, IceStorm::TopicPrx>::iterator p = _actors_rooms.find(room);
		if(p != _actors_rooms.end())
		{
			p->second->unsubscribe(observer);
		}
	}
	catch(const IceUtil::Exception& ex)
	{
		std::cout<<"Exception unsubscribing actor"<<ex.what()<<std::endl;
	}
	catch(...)
	{
		std::cout<<"Unknown exception unsubscribing actor. "<<std::endl;
	}
}




//! the client change actor information room
ActorsObserverPrx SessionServant::ChangeRoom(	const std::string& oldroom, 
														const std::string& newroom, 
														const ActorsObserverPrx& observer, 
														const Ice::Current&)
{
	if(oldroom != "")
		LeaveActorRoom(oldroom, observer);

	return JoinActorRoom(newroom, observer);
}


//! destroy the session
void SessionServant::destroy(const Ice::Current& current)
{
	std::cout<<"dommage pas de chance"<<std::endl;

	current.adapter->remove(current.id);
}