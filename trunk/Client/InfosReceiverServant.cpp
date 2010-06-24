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
#include "QuestHandler.h"


// callback function called when a message is received from IceStorm
void InfosReceiverServant::UpdatedInfo(const LbaNet::ActorInfo& asi, const Ice::Current&)
{
	ThreadSafeWorkpile::getInstance()->UpdateActor(asi);
}


// callback function called when a message is received from IceStorm
void InfosReceiverServant::Quitted(Ice::Long PlayerId, const Ice::Current&)
{
	ThreadSafeWorkpile::getInstance()->RemoveActor(PlayerId);
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

	if(InventoryChanges.size() == 1)
	{
		if(InventoryChanges[0].NewCount < 0)
		{
			ThreadSafeWorkpile::ChatTextData cdata;
			cdata.Channel = "All";
			cdata.Sender = "info";
			cdata.Text = "Used object.." ;
			ThreadSafeWorkpile::getInstance()->AddChatData(cdata);
		}
	}


}


// update container
void InfosReceiverServant::UpdateContainerInfo(const LbaNet::ContainerInfo &container, const Ice::Current&)
{
	ThreadSafeWorkpile::getInstance()->UpdateContainer(container);
}



// apply inventory changes
 void InfosReceiverServant::InformInventoryChanges(const LbaNet::UpdatedItemSeq &InventoryChanges, const Ice::Current&)
 {
	for(size_t i=0; i<InventoryChanges.size(); ++i)
	{
		if(InventoryChanges[i].NewCount < 0)
		{
			ThreadSafeWorkpile::getInstance()->AddEvent(new ObjectUpdateEvent(InventoryChanges[i].ItemId, 
																		false, -InventoryChanges[i].NewCount));
		}
		else
		{
			ThreadSafeWorkpile::getInstance()->AddEvent(new ObjectUpdateEvent(InventoryChanges[i].ItemId, 
																		true, InventoryChanges[i].NewCount));
		}
	}
 }

    
// Update Actor State
void InfosReceiverServant::UpdateActorState(const LbaNet::ActorUpdateInfo &newinfo, const Ice::Current&)
{
	ActorStateInfo asi;
	asi.ActorId = newinfo.ActorId;

	// used for switch
	asi.On = newinfo.On;

	//used for door
	asi.Open = newinfo.Open;
	asi.Counter = newinfo.Counter;
	asi.SignalOn = newinfo.SignalOn;

	//used for lift
	asi.CurrentScript = newinfo.CurrentScript;
	LbaNet::TargetSeq::const_iterator itcs = newinfo.CurrentSignals.begin();
	LbaNet::TargetSeq::const_iterator endcs = newinfo.CurrentSignals.end();
	for(; itcs != endcs; ++itcs)
		asi.CurrentSignals.push_back(*itcs);

	asi.X = newinfo.X;
	asi.Y = newinfo.Y;
	asi.Z = newinfo.Z;
	asi.Rotation = newinfo.Rotation;

	// used for NPC
	asi.Visible = newinfo.Visible;

	LbaNet::TargetedSeq::const_iterator ittar = newinfo.Targets.begin();
	LbaNet::TargetedSeq::const_iterator endtar = newinfo.Targets.end();
	for(; ittar != endtar; ++ittar)
		asi.Targets.push_back(std::make_pair<long, long>(ittar->TargetActorId, ittar->TargetPlayerId));


	ThreadSafeWorkpile::getInstance()->UpdateSingleActorState(asi);
}


// Update Actor State
void InfosReceiverServant::ActivationAborted(const LbaNet::ActorActivationInfo &ai, const Ice::Current&)
{
	ThreadSafeWorkpile::getInstance()->ActivationAborted(ai);
}


// InitQuestStartedFinished  
void InfosReceiverServant::InitQuestStartedFinished(const LbaNet::QuestSeq &Started, 
													const LbaNet::QuestSeq &Finished, 
													const Ice::Current&)
{
	std::vector<long> questStarted;
	for(size_t i=0; i<Started.size(); ++i)
		questStarted.push_back(Started[i]);

	std::vector<long> questFinished;
	for(size_t i=0; i<Finished.size(); ++i)
		questFinished.push_back(Finished[i]);

	QuestHandler::getInstance()->SetStartedFinished(questStarted, questFinished);
	ThreadSafeWorkpile::getInstance()->NeedQuestBookUpdate(true);
}

// InformQuestStarted
void InfosReceiverServant::InformQuestStarted(Ice::Long QuestId, const Ice::Current&)
{
	QuestHandler::getInstance()->TriggerQuestStart(QuestId);
	ThreadSafeWorkpile::getInstance()->NeedQuestBookUpdate(false);

	QuestInfo qi = QuestHandler::getInstance()->GetQuestInfo(QuestId);

	if(qi.Visible)
	{
		std::stringstream strs;
		strs<<"New quest: " << qi.Tittle; 
		ThreadSafeWorkpile::ChatTextData cdata;
		cdata.Channel = "All";
		cdata.Sender = "info";
		cdata.Text = strs.str();
		ThreadSafeWorkpile::getInstance()->AddChatData(cdata);
	}
}

// InformQuestFinished  
void InfosReceiverServant::InformQuestFinished(Ice::Long QuestId, const Ice::Current&)
{
	QuestHandler::getInstance()->TriggerQuestEnd(QuestId);
	ThreadSafeWorkpile::getInstance()->NeedQuestBookUpdate(false);
}

		    
// called when player throw magic ball
void InfosReceiverServant::MagicBallPlayed(Ice::Long PlayerId, const LbaNet::LaunchInfo & linfo, const Ice::Current&)
{
	ThreadSafeWorkpile::getInstance()->MagicBallPlayed(PlayerId, linfo);
}


//called when actor should die
void InfosReceiverServant::Die(const Ice::Current&)
{
	ThreadSafeWorkpile::getInstance()->AddEvent(new DieEvent());
}
