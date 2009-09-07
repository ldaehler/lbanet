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
	inform.ValueA = GetId();
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

