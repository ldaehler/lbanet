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

#include "Actor.h"
#include "ExternalActorsHandler.h"
#include "PhysicHandler.h"
#include "ThreadSafeWorkpile.h"

/***********************************************************
	Constructor
***********************************************************/
ExternalActorsHandler::ExternalActorsHandler()
: _needupdate(false)
{

}

/***********************************************************
	destructor
***********************************************************/
ExternalActorsHandler::~ExternalActorsHandler()
{
	Cleanup();
}


/***********************************************************
render object
***********************************************************/
void ExternalActorsHandler::Render(int RoomCut)
{
	if(_needupdate)
		return;

	std::map<long, Actor *>::iterator it =  _actors.begin();
	std::map<long, Actor *>::iterator end =  _actors.end();
	for(; it != end; ++it)
		it->second->Render(RoomCut);
}

/***********************************************************
do all check to be done when idle
***********************************************************/
int ExternalActorsHandler::Process(double tnow, float tdiff)
{
	// update from external info
	ThreadSafeWorkpile * wp = ThreadSafeWorkpile::getInstance();
	std::vector<LbaNet::ActorActivationInfo> vecai;
	wp->GetAllActivated(vecai);
	for(size_t i=0; i<vecai.size(); ++i)
	{
		const LbaNet::ActorActivationInfo & ai = vecai[i];

		std::map<long, Actor *>::iterator it = _actors.find((long)ai.ActivatedId);
		if(it != _actors.end())
		{
			if(ai.Activate)
				it->second->ProcessActivation(ai.X, ai.Y, ai.Z, ai.Rotation);
			else
				it->second->ProcessDesactivation(ai.X, ai.Y, ai.Z, ai.Rotation);
		}
	}

	// update aborted activation
	std::vector<LbaNet::ActorActivationInfo> vecaiab;
	wp->GetAllActivationAborted(vecaiab);
	for(size_t i=0; i<vecaiab.size(); ++i)
	{
		const LbaNet::ActorActivationInfo & ai = vecaiab[i];
		std::map<long, Actor *>::iterator it = _actors.find((long)ai.ActivatedId);
		if(it != _actors.end())
			it->second->InformActivationAborted();
	}

	std::map<long, Actor *>::iterator it =  _actors.begin();
	std::map<long, Actor *>::iterator end =  _actors.end();
	for(; it != end; ++it)
		it->second->Process(tnow, tdiff);

	return 0;
}


/***********************************************************
cleanup allocated memory
***********************************************************/
void ExternalActorsHandler::Cleanup()
{
	std::map<long, Actor *>::iterator it =  _actors.begin();
	std::map<long, Actor *>::iterator end =  _actors.end();
	for(; it != end; ++it)
		delete it->second;

	_actors.clear();
}


/***********************************************************
set actors
***********************************************************/
void ExternalActorsHandler::SetActors(const std::map<long, Actor *> & actors)
{
	Cleanup();
	_actors = actors;

	if(ThreadSafeWorkpile::getInstance()->IsServeron())
		_needupdate = true;
}


/***********************************************************
get structure
***********************************************************/
short ExternalActorsHandler::GetStructure(int X, int Y, int Z)
{
	std::map<long, Actor *>::iterator it =  _actors.begin();
	std::map<long, Actor *>::iterator end =  _actors.end();
	for(; it != end; ++it)
	{
		int res=0;
		if(res=it->second->GetStructure(X, Y, Z) != 0)
			return res;
	}

	return 0;
}




/***********************************************************
try to activate actors
***********************************************************/
bool ExternalActorsHandler::Activate(float PlayerPosX, float PlayerPosY, float PlayerPosZ,
									 float PlayerRotation, int actionType)
{
	bool serveron = ThreadSafeWorkpile::getInstance()->IsServeron();

	std::map<long, Actor *>::iterator it =  _actors.begin();
	std::map<long, Actor *>::iterator end =  _actors.end();
	for(; it != end; ++it)
	{
		if(it->second->Activate(PlayerPosX, PlayerPosY, PlayerPosZ, PlayerRotation, actionType, !serveron))
		{
			if(serveron)
			{
				LbaNet::ActorActivationInfo ae;
				ae.Activate = true;
				ae.ActivatedId = it->first;
				ae.X = PlayerPosX;
				ae.Y = PlayerPosY;
				ae.Z = PlayerPosZ;
				ae.Rotation = PlayerRotation;
				ThreadSafeWorkpile::getInstance()->AddActivationEvent(ae);
			}
			return true;
		}
	}

	return false;
}


/***********************************************************
called to reload element when resizing screen
***********************************************************/
void ExternalActorsHandler::Reload()
{
	std::map<long, Actor *>::iterator it =  _actors.begin();
	std::map<long, Actor *>::iterator end =  _actors.end();
	for(; it != end; ++it)
		it->second->Reload();
}


/***********************************************************
called to reload element when resizing screen
***********************************************************/
void ExternalActorsHandler::CleanUp()
{
	std::map<long, Actor *>::iterator it =  _actors.begin();
	std::map<long, Actor *>::iterator end =  _actors.end();
	for(; it != end; ++it)
		it->second->CleanUp();
}

/***********************************************************
check zone activation
***********************************************************/
bool ExternalActorsHandler::ActivateZone(float PlayerPosX, float PlayerPosY, float PlayerPosZ,
										 float PlayerRotation, MainPlayerHandler  * _mph)
{
	bool serveron = ThreadSafeWorkpile::getInstance()->IsServeron();

	std::map<long, Actor *>::iterator it =  _actors.begin();
	std::map<long, Actor *>::iterator end =  _actors.end();
	for(; it != end; ++it)
	{
		int res = 0;
		if((res = it->second->ActivateZone(PlayerPosX, PlayerPosY, PlayerPosZ, PlayerRotation, 
											_mph, !serveron)) != 0)
		{
			if(serveron)
			{
				LbaNet::ActorActivationInfo ae;
				ae.Activate = (res == 1);
				ae.ActivatedId = it->first;
				ae.X = PlayerPosX;
				ae.Y = PlayerPosY;
				ae.Z = PlayerPosZ;
				ThreadSafeWorkpile::getInstance()->AddActivationEvent(ae);
			}

			if(res == 1)
				return true;
		}
	}

	return false;
}


/***********************************************************
called when a signal has been generated
***********************************************************/
void ExternalActorsHandler::SignalEvent(long signal, const std::vector<long> &targets)
{
	for(size_t i=0; i<targets.size(); ++i)
	{
		std::map<long, Actor *>::iterator it = _actors.find(targets[i]);
		if(it != _actors.end())
			it->second->OnSignal(signal);
	}
}


/***********************************************************
render editor part
***********************************************************/
void ExternalActorsHandler::RenderEditor()
{
	std::map<long, Actor *>::iterator it =  _actors.begin();
	std::map<long, Actor *>::iterator end =  _actors.end();
	for(; it != end; ++it)
		it->second->RenderEditor();
}



/***********************************************************
check if the actor should be attached
***********************************************************/
bool ExternalActorsHandler::CheckAttach(Actor * act)
{
	std::map<long, Actor *>::iterator it =  _actors.begin();
	std::map<long, Actor *>::iterator end =  _actors.end();
	for(; it != end; ++it)
	{
		if(it->second->CheckAttach(act))
			return true;
	}

	return false;
}

/***********************************************************
check if the actor should be dettached
***********************************************************/
bool ExternalActorsHandler::CheckDettach(Actor * act)
{
	std::map<long, Actor *>::iterator it =  _actors.begin();
	std::map<long, Actor *>::iterator end =  _actors.end();
	for(; it != end; ++it)
	{
		if(it->second->CheckDettach(act))
			return true;
	}

	return false;
}


/***********************************************************
update actor state
***********************************************************/
void ExternalActorsHandler::UpdateActorStates(const std::vector<ActorStateInfo> & newstate)
{
	std::vector<ActorStateInfo>::const_iterator it = newstate.begin();
	std::vector<ActorStateInfo>::const_iterator end = newstate.end();
	for(; it != end; ++it)
	{
		std::map<long, Actor *>::iterator itmap = _actors.find(it->ActorId);
		if(itmap != _actors.end())
			itmap->second->Setstate(*it);
	}

	_needupdate = false;
}


/***********************************************************
attach player to actor
***********************************************************/
void ExternalActorsHandler::ForcedAttach(Actor * act, long actorId)
{
	std::map<long, Actor *>::iterator it = _actors.find(actorId);
	if(it != _actors.end())
		it->second->Attach(act);
}

/***********************************************************
dettach player from actor
***********************************************************/
void ExternalActorsHandler::ForcedDettach(Actor * act, long actorId)
{
	std::map<long, Actor *>::iterator it = _actors.find(actorId);
	if(it != _actors.end())
		it->second->Dettach(act);
}