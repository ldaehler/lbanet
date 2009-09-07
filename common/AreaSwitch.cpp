#include "AreaSwitch.h"

#ifndef _LBANET_SERVER_SIDE_
#include "ThreadSafeWorkpile.h"
#include "GameEvents.h"
#include "MusicHandler.h"
#include "DataLoader.h"
#endif

/***********************************************************
	Constructor
***********************************************************/
AreaSwitch::AreaSwitch(float zoneSizeX, float zoneSizeY, float zoneSizeZ)
: ZoneActor(zoneSizeX, zoneSizeY, zoneSizeZ)
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
}
	
/***********************************************************
	process zone desactivation
***********************************************************/
void AreaSwitch::ProcessDesactivation(float PlayerPosX, float PlayerPosY, float PlayerPosZ, 
										 float PlayerRotation)
{
	SendSignal(_outputsignal, _targets);
}

	