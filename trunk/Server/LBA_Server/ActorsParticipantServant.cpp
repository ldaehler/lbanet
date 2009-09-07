#include "ActorsParticipantServant.h"


/***********************************************************
constructor
***********************************************************/
ActorsParticipantServant::ActorsParticipantServant(const std::string& room, std::string actor_name,
													const ActorsObserverPrx& observer,
													const RoomManagerPrx& manager)
:	 _room(room), _actor_name(actor_name),
    _manager(manager),
    _observer(observer),
    _publish(manager->JoinActorRoom(room, actor_name, observer))
{

}
  
/***********************************************************
destructor
***********************************************************/  
ActorsParticipantServant::~ActorsParticipantServant()
{
    try
    {
		_publish->Quitted(_actor_name);
		_manager->LeaveActorRoom(_room, _observer);
    }
    catch(...)
    {
		// Ignore. The ice mediated invocation can throw an
		// exception on shutdown.
    }
}


  
/***********************************************************
update
***********************************************************/  
void ActorsParticipantServant::Update(const ActorInfo& asi, const Ice::Current &current)
{
    try
    {
		_publish->UpdatedInfo(asi);
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


