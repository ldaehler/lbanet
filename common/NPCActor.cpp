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


#include "NPCActor.h"
#include <math.h>
#include <algorithm>

#ifndef _LBANET_SERVER_SIDE_
#include "ThreadSafeWorkpile.h"
#include "DataLoader.h"
#include "MusicHandler.h"
#include "CharacterRenderer.h"
#include "GameEvents.h"
#endif

#ifndef M_PI
#define M_PI    3.14159265358979323846f
#endif


/***********************************************************
	Constructor
***********************************************************/
NPCActor::NPCActor(const std::vector<PlayerScriptPart> & scripts, bool IsLift, 
					int NPCType, float activationdistance, std::string Name)
: ScriptableActor(scripts, IsLift), _NPCType(NPCType), 
	_activationdistance(activationdistance), _activated(false), _Name(Name)
{
}

/***********************************************************
	Destructor
***********************************************************/
NPCActor::~NPCActor()
{

}

/***********************************************************
do all check to be done when idle
***********************************************************/
int NPCActor::Process(double tnow, float tdiff)
{
	if(_actiontargets.size() > 0)
	{

		return Actor::Process(tnow, tdiff);
	}

	return ScriptableActor::Process(tnow, tdiff);
}



/***********************************************************
activate an actor
***********************************************************/
bool NPCActor::Activate(float PlayerPosX, float PlayerPosY, float PlayerPosZ, float PlayerRotation,
								int actionType, bool DirectActivation)
{
	if(actionType != 1)
		return false;

	float distX = _posX-PlayerPosX;
	float distY = _posY-PlayerPosY;
	float distZ = _posZ-PlayerPosZ;

	double distance = (distX * distX) + (distY * distY) + (distZ * distZ);
	if(distance > _activationdistance)
		return false;

	distance = sqrt((distX * distX) + (distZ * distZ));
	float angle = (float)(180 * acos(distZ / distance) / M_PI);
	if(distX < 0)
		angle = 360-angle;

	if(PlayerRotation < (angle-80))
		return false;

	if(PlayerRotation > (angle+80))
		return false;


#ifndef _LBANET_SERVER_SIDE_
	if(actionType == 1 || actionType == 2)
	{
		ThreadSafeWorkpile::getInstance()->SetTargetedActor(_ID);
		ThreadSafeWorkpile::getInstance()->AddEvent(new DisplayDialogEvent(_ID, _Name, true));
		_activated = true;
	}
#endif


	return false;
}


/***********************************************************
check zone activation
***********************************************************/
int NPCActor::ActivateZone(float PlayerPosX, float PlayerPosY, float PlayerPosZ, float PlayerRotation,
								MainPlayerHandler  * _mph, bool DirectActivation)
{

#ifndef _LBANET_SERVER_SIDE_
	if(_activated)
	{
		float distX = _posX-PlayerPosX;
		float distY = _posY-PlayerPosY;
		float distZ = _posZ-PlayerPosZ;

		double distance = (distX * distX) + (distY * distY) + (distZ * distZ);
		if(distance > _activationdistance)
		{
			ThreadSafeWorkpile::getInstance()->SetUntargetedActor(_ID);
			ThreadSafeWorkpile::getInstance()->AddEvent(new DisplayDialogEvent(_ID, _Name, false));
			_activated = false;
		}
	}
#endif


	return 0;
}

/***********************************************************
update actor to target a player
***********************************************************/
void NPCActor::UpdateTargetedActor(long playerid, bool target)
{
	std::pair<long, long> p(std::make_pair<long, long>(-1, playerid));
	std::vector<std::pair<long, long> >::iterator it = 
					std::find(_actiontargets.begin(), _actiontargets.end(), p);

	if(target)
	{
		if(it == _actiontargets.end())
		{
			_actiontargets.push_back(p);
			_needs_update = true;
		}
	}
	else
	{
		if(it != _actiontargets.end())
		{
			_actiontargets.erase(it);
			_needs_update = true;
		}
	}
}



