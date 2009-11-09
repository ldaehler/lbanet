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

#include "InfosReceiverServant.h"
#include "ThreadSafeWorkpile.h"
#include "GameEvents.h"
#include "InventoryHandler.h"

// callback function called when a message is received from IceStorm
void InfosReceiverServant::UpdatedInfo(const LbaNet::ActorInfo& asi, const Ice::Current&)
{
	ThreadSafeWorkpile::getInstance()->UpdateActor(asi);
}


// callback function called when a message is received from IceStorm
void InfosReceiverServant::Quitted(const std::string& ActorName, const Ice::Current&)
{
	ThreadSafeWorkpile::getInstance()->RemoveActor(ActorName);
}


// callback function called when a message is received from IceStorm
void InfosReceiverServant::ActivatedActor(const LbaNet::ActorActivationInfo &ai, const Ice::Current&)
{
	ThreadSafeWorkpile::getInstance()->Activated(ai);
}


// callback function called when a message is received from IceStorm
void InfosReceiverServant::SignaledActor(const LbaNet::ActorSignalInfo &ai, const Ice::Current&)
{
	std::vector<long> vectar;

	LbaNet::TargetSeq::const_iterator it = ai.Targets.begin();
	LbaNet::TargetSeq::const_iterator end = ai.Targets.end();
	for(; it != end; ++it)
		vectar.push_back((long)*it);

	long targetsignal = (long)ai.SignalId;
	ThreadSafeWorkpile::getInstance()->AddEvent(new GameSignalvent(targetsignal, vectar));
}


// update actor life
void InfosReceiverServant::UpdatedLife(const LbaNet::ActorLifeInfo& ali, const Ice::Current&)
{
	ThreadSafeWorkpile::getInstance()->UpdateActorLife(ali);
}


// apply inventory changes
void InfosReceiverServant::ApplyInventoryChanges(const LbaNet::UpdatedItemSeq &InventoryChanges, 
												 const Ice::Current&)
{
	LbaNet::UpdatedItemSeq::const_iterator it = InventoryChanges.begin();
	LbaNet::UpdatedItemSeq::const_iterator end = InventoryChanges.end();
	for(;it != end; ++it)
		InventoryHandler::getInstance()->UpdateInventoryItem(it->ItemId, it->NewCount);
}


// update container
void InfosReceiverServant::UpdateContainerInfo(const LbaNet::ContainerInfo &container, const Ice::Current&)
{
	ThreadSafeWorkpile::getInstance()->UpdateContainer(container);
}


