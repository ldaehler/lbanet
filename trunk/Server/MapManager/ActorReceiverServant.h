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


#ifndef __ACTORS_RECEIVER_SERVANT_H_
#define __ACTORS_RECEIVER_SERVANT_H_

#include <MapObserver.h>
#include <string>

#include "SharedData.h"
class MapHandlerThread;

/***********************************************************
  Wrapper class containing the callback function used when a
  message is received in IceStorm
***********************************************************/
class ActorReceiverServant : public LbaNet::MapObserver  
{
public:

	explicit ActorReceiverServant(SharedData * SD, MapHandlerThread *mthread)
		: _SD(SD), _mthread(mthread)
	{}

	// callback function called when an actor id activated
    virtual void ActivateActor(const LbaNet::ActorActivationInfo& ai, const Ice::Current&);

	// callback function called when an actor id signaled
    virtual void SignalActor(const LbaNet::ActorSignalInfo& ai, const Ice::Current&);

	//! get updated info
	virtual LbaNet::UpdateSeq GetUpdatedInfo(const Ice::Current&);

	//! get player info
	virtual LbaNet::PlayerSeq GetPlayersInfo(const Ice::Current&);

	//! called when an actor has been hurt
    virtual void GotHurtByActor(Ice::Long ActorId, Ice::Long HurtingId, const Ice::Current&);

	//! called when an actor has been hurt
    virtual void GotHurtByFalling(Ice::Long ActorId, Ice::Float FallingDistance, const Ice::Current&);

	//! called when an actor has been dead and reborn
    virtual void RaisedFromDead(Ice::Long ActorId, const Ice::Current&);

	//! item used
	virtual void UpdateLifeMana(Ice::Long ActorId, int LifeDelta, int ManaDelta, 
									const Ice::Current&);

	//! AskForContainer
    virtual void AskForContainer(Ice::Long ActorId, Ice::Long ContainerId, const Ice::Current&);
    
	//! UpdateContainer
   virtual  void UpdateContainer(Ice::Long ContainerId, Ice::Long ActorId, const LbaNet::ItemList &Taken, 
							const LbaNet::ItemList &Put, const Ice::Current&);
	    
	    
    // set player targeted by actor
   virtual void SetTargeted(Ice::Long PlayerId, Ice::Long ActorId, const Ice::Current&);
    	    
    // set player untargeted by actor
    virtual void SetUnTargeted(Ice::Long PlayerId, Ice::Long ActorId, const Ice::Current&);   
	    
	    
    // called when player throw magic ball
    virtual void MagicBallPlayed(Ice::Long PlayerId, const LbaNet::LaunchInfo& Linfo, const Ice::Current&);
    
    // called when magic ball touch an actor
    virtual void MagicBallTouchActor(Ice::Long PlayerId, Ice::Long ActorId, const Ice::Current&);
    
    // called when magic ball touch a player
    virtual void MagicBallTouchPlayer(Ice::Long PlayerId, Ice::Long ActorId, const Ice::Current&);	

private:
	SharedData * _SD;
	MapHandlerThread *	_mthread;
};

#endif
