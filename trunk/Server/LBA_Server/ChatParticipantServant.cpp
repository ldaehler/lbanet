#include "ChatParticipantServant.h"


/***********************************************************
constructor
***********************************************************/
ChatParticipantServant::ChatParticipantServant(const std::string& room, const ChatRoomObserverPrx& observer,
												const RoomManagerPrx& manager) 
:	 _room(room),
    _manager(manager),
    _observer(observer),
    _publish(manager->JoinChat(room, observer))
{

}

/***********************************************************
destructor
***********************************************************/
ChatParticipantServant::~ChatParticipantServant()
{
    try
    {
		_manager->LeaveChat(_room, _observer);
    }
    catch(...)
    {
		// Ignore. The ice mediated invocation can throw an
		// exception on shutdown.
    }
}

/***********************************************************
say something
***********************************************************/
void ChatParticipantServant::Say(const std::string& sender, const std::string& text, const Ice::Current&)
{
	try
	{
		_publish->Message(sender, text);
	}
	catch(const IceUtil::Exception& ex)
	{
		std::cout<<"Exception updating actor info: "<<ex.what()<<std::endl;
	}
	catch(...)
	{
		std::cout<<"Unknown exception updating actor info. "<<std::endl;
	}
}