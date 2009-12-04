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


#include "ScriptedZoneActor.h"

#ifndef _LBANET_SERVER_SIDE_
#include "ThreadSafeWorkpile.h"
#include "GameEvents.h"
#include "MusicHandler.h"
#include "DataLoader.h"
#endif


/***********************************************************
	Constructor
***********************************************************/
ScriptedZoneActor::ScriptedZoneActor(float ZoneSizeX, float ZoneSizeY, float ZoneSizeZ, 
										const std::vector<PlayerScriptPart> & scripts, int activationtype,
										int NeededItemId, bool DestroyItem)
: ZoneActivableActor(ZoneSizeX, ZoneSizeY, ZoneSizeZ, activationtype), 
	_scripts(scripts), _NeededItemId(NeededItemId), _DestroyItem(DestroyItem)
{

}


/***********************************************************
	Destructor
***********************************************************/
ScriptedZoneActor::~ScriptedZoneActor()
{

}


/***********************************************************
process zone activation
***********************************************************/
void ScriptedZoneActor::ProcessActivation(float PlayerPosX, float PlayerPosY, float PlayerPosZ,
	float PlayerRotation)
{
#ifndef _LBANET_SERVER_SIDE_
	ThreadSafeWorkpile::getInstance()->AddEvent(new MainPlayerScriptedEvent(_scripts));
#endif
}



