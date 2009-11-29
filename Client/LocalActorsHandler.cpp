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
#include "LocalActorsHandler.h"
#include "PhysicHandler.h"

/***********************************************************
	Constructor
***********************************************************/
LocalActorsHandler::LocalActorsHandler()
: _pH(NULL)
{

}

/***********************************************************
	destructor
***********************************************************/
LocalActorsHandler::~LocalActorsHandler()
{
	Cleanup();
}


/***********************************************************
render object
***********************************************************/
void LocalActorsHandler::Render(int RoomCut)
{
	std::map<long, Actor *>::iterator it =  _actors.begin();
	std::map<long, Actor *>::iterator end =  _actors.end();
	for(; it != end; ++it)
		it->second->Render(RoomCut);
}

/***********************************************************
do all check to be done when idle
***********************************************************/
int LocalActorsHandler::Process(double tnow, float tdiff)
{
	std::map<long, Actor *>::iterator it =  _actors.begin();
	std::map<long, Actor *>::iterator end =  _actors.end();
	for(; it != end; ++it)
		it->second->Process(tnow, tdiff);

	return 0;
}


/***********************************************************
cleanup allocated memory
***********************************************************/
void LocalActorsHandler::Cleanup()
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
void LocalActorsHandler::SetActors(const std::map<long, Actor *> & actors)
{
	Cleanup();
	_actors = actors;
}


/***********************************************************
get structure
***********************************************************/
short LocalActorsHandler::GetStructure(int X, int Y, int Z)
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
move movable objects
***********************************************************/
void LocalActorsHandler::Move(int X, int Y, int Z, float MoveX, float MoveZ)
{
	//std::map<long, Actor *>::iterator it =  _actors.begin();
	//std::map<long, Actor *>::iterator end =  _actors.end();
	//for(; it != end; ++it)
	//{
	//	int res=0;
	//	if((*it)->IsMovable())
	//	{
	//		if((*it)->GetStructure(X, Y, Z) != 0)
	//		{
	//			if(_pH)
	//			{
	//				_pH->Move((*it)->GetPosX(), (*it)->GetPosY(), (*it)->GetPosZ(),
	//					(*it)->GetSizeX()/2.0f, (*it)->GetSizeY(), (*it)->GetSizeZ()/2.0f,
	//							MoveX, MoveZ, false);

	//				(*it)->SetPosition((*it)->GetPosX()+MoveX, (*it)->GetPosY(), (*it)->GetPosZ()+MoveZ);
	//			}
	//		}
	//	}
	//}
}




/***********************************************************
try to activate actors
***********************************************************/
bool LocalActorsHandler::Activate(float PlayerPosX, float PlayerPosY, float PlayerPosZ, float PlayerRotation, 
								  int actionType)
{
	std::map<long, Actor *>::iterator it =  _actors.begin();
	std::map<long, Actor *>::iterator end =  _actors.end();
	for(; it != end; ++it)
	{
		if(it->second->Activate(PlayerPosX, PlayerPosY, PlayerPosZ, PlayerRotation, actionType))
			return true;
	}

	return false;
}


/***********************************************************
called to reload element when resizing screen
***********************************************************/
void LocalActorsHandler::Reload()
{
	std::map<long, Actor *>::iterator it =  _actors.begin();
	std::map<long, Actor *>::iterator end =  _actors.end();
	for(; it != end; ++it)
		it->second->Reload();
}


/***********************************************************
called to reload element when resizing screen
***********************************************************/
void LocalActorsHandler::CleanUp()
{
	std::map<long, Actor *>::iterator it =  _actors.begin();
	std::map<long, Actor *>::iterator end =  _actors.end();
	for(; it != end; ++it)
		it->second->CleanUp();
}


/***********************************************************
check zone activation
***********************************************************/
bool LocalActorsHandler::ActivateZone(float PlayerPosX, float PlayerPosY, float PlayerPosZ, 
										float PlayerRotation, MainPlayerHandler  * _mph)
{
	std::map<long, Actor *>::iterator it =  _actors.begin();
	std::map<long, Actor *>::iterator end =  _actors.end();
	for(; it != end; ++it)
	{
		if(it->second->ActivateZone(PlayerPosX, PlayerPosY, PlayerPosZ, PlayerRotation, _mph) == 1)
			return true;
	}

	return false;
}


/***********************************************************
called when a signal has been generated
***********************************************************/
void LocalActorsHandler::SignalEvent(long signal, const std::vector<long> &targets)
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
void LocalActorsHandler::RenderEditor()
{
	std::map<long, Actor *>::iterator it =  _actors.begin();
	std::map<long, Actor *>::iterator end =  _actors.end();
	for(; it != end; ++it)
		it->second->RenderEditor();
}


/***********************************************************
check if the actor should be attached
***********************************************************/
bool LocalActorsHandler::CheckAttach(Actor * act)
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
bool LocalActorsHandler::CheckDettach(Actor * act)
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
void LocalActorsHandler::UpdateActorStates(const std::vector<ActorStateInfo> & newstate)
{
	std::vector<ActorStateInfo>::const_iterator it = newstate.begin();
	std::vector<ActorStateInfo>::const_iterator end = newstate.end();
	for(; it != end; ++it)
	{
		std::map<long, Actor *>::iterator itmap = _actors.find(it->ActorId);
		if(itmap != _actors.end())
			itmap->second->Setstate(*it);
	}
}


/***********************************************************
attach player to actor
***********************************************************/
void LocalActorsHandler::ForcedAttach(Actor * act, long actorId)
{
	std::map<long, Actor *>::iterator it = _actors.find(actorId);
	if(it != _actors.end())
		it->second->Attach(act);
}

/***********************************************************
dettach player from actor
***********************************************************/
void LocalActorsHandler::ForcedDettach(Actor * act, long actorId)
{
	std::map<long, Actor *>::iterator it = _actors.find(actorId);
	if(it != _actors.end())
		it->second->Dettach(act);
}