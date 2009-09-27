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


#include "FloorSwitch.h"

#ifndef _LBANET_SERVER_SIDE_
#include "ThreadSafeWorkpile.h"
#include "GameEvents.h"
#include "MusicHandler.h"
#include "DataLoader.h"
#endif


/***********************************************************
	Constructor
***********************************************************/
FloorSwitch::FloorSwitch(float ZoneSizeX, float ZoneSizeY, float ZoneSizeZ)
: ZoneActivableActor(ZoneSizeX, ZoneSizeY, ZoneSizeZ)
{

}


/***********************************************************
	Destructor
***********************************************************/
FloorSwitch::~FloorSwitch()
{

}


/***********************************************************
process zone activation
***********************************************************/
void FloorSwitch::ProcessActivation(float PlayerPosX, float PlayerPosY, float PlayerPosZ,
	float PlayerRotation)
{
#ifndef _LBANET_SERVER_SIDE_
	std::vector<PlayerScriptPart> script;

	// play short switch animation
	{
	PlayerScriptPart anim1;
	anim1.Type = 2;
	anim1.Animation = 23;
	anim1.ValueA = -1;
	script.push_back(anim1);
	}

	{
	PlayerScriptPart inform;
	inform.Type = 3;
	inform.ValueA = (float)GetId();
	inform.ValueB = 3;	// event id
	script.push_back(inform);
	}

	ThreadSafeWorkpile::getInstance()->AddEvent(new MainPlayerScriptedEvent(script));
#endif
}


/***********************************************************
	called on signal
***********************************************************/
bool FloorSwitch::OnSignal(long SignalNumber)
{
	if(SignalNumber == 3)	// animation finished signal
	{
		#ifndef _LBANET_SERVER_SIDE_
		if(_attachedsound >= 0)
		{
			std::string soundp = DataLoader::getInstance()->GetSoundPath(_attachedsound);
			if(soundp != "")
				MusicHandler::getInstance()->PlaySample(soundp, 0);
		}
		#endif

		SendSignal(_outputsignal, _targets);

		return true;
	}

	return false;
}

