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


#include "AreaSwitch.h"

#ifndef _LBANET_SERVER_SIDE_
#include "ThreadSafeWorkpile.h"
#include "GameEvents.h"
#include "MusicHandler.h"
#include "DataLoader.h"
#include "QuestHandler.h"
#endif

/***********************************************************
	Constructor
***********************************************************/
AreaSwitch::AreaSwitch(float zoneSizeX, float zoneSizeY, float zoneSizeZ, long QuestToTriggerEnd)
: ZoneActor(zoneSizeX, zoneSizeY, zoneSizeZ), _QuestToTriggerEnd(QuestToTriggerEnd)
{

}

/***********************************************************
	Destructor
***********************************************************/
AreaSwitch::~AreaSwitch()
{

}

/***********************************************************
	process zone activation
***********************************************************/
void AreaSwitch::ProcessActivation(float PlayerPosX, float PlayerPosY, float PlayerPosZ,
									  float PlayerRotation)
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

#ifndef _LBANET_SERVER_SIDE_
	if(_QuestToTriggerEnd >= 0)
	{
		if(QuestHandler::getInstance()->QuestConditionFulfilled(_QuestToTriggerEnd))
			ThreadSafeWorkpile::getInstance()->AddQuestFinished(_QuestToTriggerEnd);
	}
#endif
}

/***********************************************************
	process zone desactivation
***********************************************************/
void AreaSwitch::ProcessDesactivation(float PlayerPosX, float PlayerPosY, float PlayerPosZ,
										 float PlayerRotation)
{
	SendSignal(_outputsignal, _targets);
}

