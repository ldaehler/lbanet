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


#include "ActorsParticipantServant.h"


/***********************************************************
constructor
***********************************************************/
ActorsParticipantServant::ActorsParticipantServant(const std::string& room, long actor_id,
													const std::string &actor_name,
													const ActorsObserverPrx& observer,
													const RoomManagerPrx& manager)
:	 _room(room), _actor_id(actor_id),
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
		_publish->Quitted(_actor_id);
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
		ActorInfo asin(asi);
		asin.ActorId = _actor_id;
		_publish->UpdatedInfo(asin);
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



/***********************************************************
update ghost
***********************************************************/
void ActorsParticipantServant::UpdateGhost(const GhostActorInfo& asi, const Ice::Current &current)
{
    try
    {
		GhostActorInfo asin(asi);
		asin.ControllingPlayerId = _actor_id;
		_publish->UpdatedGhost(asi);
	}
	catch(const IceUtil::Exception& ex)
	{
		std::cout<<"Exception updating ghost info: "<<ex.what()<<std::endl;
	}
	catch(...)
	{
		std::cout<<"Unknown exception updating ghost info. "<<std::endl;
	}
}
