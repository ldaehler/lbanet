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


#include "ActorReceiverServant.h"
#include "MapHandlerThread.h"


/***********************************************************
	callback function called when an actor id activated
***********************************************************/
void ActorReceiverServant::ActivateActor(const LbaNet::ActorActivationInfo& ai, 
										 const LbaNet::ClientSessionPrx &clientPtr,
										 const Ice::Current&)
{
	ActorActivationInfoWithCallback aic;
	aic.ainfo = ai;
	aic.clientPtr = clientPtr;
	_SD->ActivateActor(aic);
}

/***********************************************************
	callback function called when an actor id signaled
***********************************************************/
void ActorReceiverServant::SignalActor(const LbaNet::ActorSignalInfo& ai, const Ice::Current&)
{
	_SD->SignalActor(ai);
}


/***********************************************************
	get updated info
***********************************************************/
LbaNet::UpdateSeq ActorReceiverServant::GetUpdatedInfo(const Ice::Current&)
{
	return _mthread->GetUpdatedInfo();
}


/***********************************************************
	get player info
***********************************************************/
LbaNet::PlayerSeq ActorReceiverServant::GetPlayersInfo(const Ice::Current&)
{
	return _mthread->GetPlayersInfo();
}


/***********************************************************
	called when an actor has been hurt
***********************************************************/
void ActorReceiverServant::GotHurtByActor(Ice::Long ActorId, Ice::Long HurtingId, const Ice::Current&)
{
	_SD->GotHurtByActor(ActorId, HurtingId);
}


/***********************************************************
	called when an actor has been hurt
***********************************************************/
void ActorReceiverServant::GotHurtByFalling(Ice::Long ActorId, Ice::Float FallingDistance, const Ice::Current&)
{
	_SD->GotHurtByFalling(ActorId, FallingDistance);
}


/***********************************************************
called when an actor has been dead and reborn
***********************************************************/
void ActorReceiverServant::RaisedFromDead(Ice::Long ActorId, const Ice::Current&)
{
	_SD->RaisedFromDead(ActorId);
}


/***********************************************************
item used
***********************************************************/
void ActorReceiverServant::UpdateLifeMana(Ice::Long ActorId, int LifeDelta, int ManaDelta, 
												const Ice::Current&)
{
	LifeManaInfo itinfo;
	itinfo.ActorId = ActorId;
	itinfo.LifeDelta = LifeDelta;
	itinfo.ManaDelta = ManaDelta;
	_SD->UpdateLifeMana(itinfo);
}



/***********************************************************
AskForContainer
***********************************************************/
void ActorReceiverServant::AskForContainer(Ice::Long ActorId, Ice::Long ContainerId, 
										   const LbaNet::ClientSessionPrx &callback, 
											const Ice::Current&)
{
	ContainerQueryInfo qinfo;
	qinfo.ActorId = ActorId;
	qinfo.ContainerId = ContainerId;
	qinfo.clientPtr = callback;
	_SD->UpdateContainerQuery(qinfo);
}

/***********************************************************
UpdateContainer
***********************************************************/
void ActorReceiverServant::UpdateContainer(Ice::Long ContainerId, Ice::Long ActorId, const LbaNet::ItemList &Taken, 
											const LbaNet::ItemList &Put, const LbaNet::ClientSessionPrx &callback,
											const Ice::Current&)
{
	ContainerUpdateInfo qinfo;
	qinfo.ActorId = ActorId;
	qinfo.ContainerId = ContainerId;
	qinfo.Taken = Taken;
	qinfo.Put = Put;
	qinfo.clientPtr = callback;
	_SD->UpdateContainerUpdate(qinfo);
}


    
/***********************************************************
set player targeted by actor
***********************************************************/
void ActorReceiverServant::SetTargeted(Ice::Long PlayerId, 
									   Ice::Long ActorId, const Ice::Current&)
{
	TargetedActorPlayer info;
	info.ActorId = ActorId;
	info.PlayerId = PlayerId;
	_SD->UpdateTargetedActor(info);
}

/***********************************************************
set player untargeted by actor
***********************************************************/
void ActorReceiverServant::SetUnTargeted(Ice::Long PlayerId, 
										 Ice::Long ActorId, const Ice::Current&)
{
	TargetedActorPlayer info;
	info.ActorId = ActorId;
	info.PlayerId = PlayerId;
	_SD->UpdateUntargetedActor(info);
}  

    
/***********************************************************
called when player throw magic ball
***********************************************************/
void ActorReceiverServant::MagicBallPlayed(Ice::Long PlayerId, const LbaNet::LaunchInfo& Linfo, const Ice::Current&)
{
    _SD->MagicBallPlayed(PlayerId, Linfo);
}

/***********************************************************
called when magic ball touch an actor
***********************************************************/
void ActorReceiverServant::MagicBallTouchActor(Ice::Long PlayerId, Ice::Long ActorId, const Ice::Current&)
{
   _SD->MagicBallTouchActor(PlayerId, ActorId);
}

/***********************************************************
called when magic ball touch a player
***********************************************************/
void ActorReceiverServant::MagicBallTouchPlayer(Ice::Long PlayerId, Ice::Long ActorId, const Ice::Current&)
{
    _SD->MagicBallTouchPlayer(PlayerId, ActorId);
}	