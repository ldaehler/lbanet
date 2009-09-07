#include "SwitchActor.h"

#ifndef _LBANET_SERVER_SIDE_
#include "ThreadSafeWorkpile.h"
#include "GameEvents.h"
#include "SpriteRenderer.h"
#include "MusicHandler.h"
#include "DataLoader.h"
#endif

/***********************************************************
	Constructor
***********************************************************/
SwitchActor::SwitchActor(float activationdistance)
: ActivableActor(activationdistance), _switchOn(false)
{

}


/***********************************************************
	Destructor
***********************************************************/
SwitchActor::~SwitchActor()
{

}

/***********************************************************
process activation
***********************************************************/
void SwitchActor::ProcessActivation(float PlayerPosX, float PlayerPosY, float PlayerPosZ, float PlayerRotation)
{
#ifndef _LBANET_SERVER_SIDE_
	std::vector<PlayerScriptPart> script;

	// play short switch animation
	{
	PlayerScriptPart anim1;
	anim1.Type = 2;
	anim1.Animation = 20;
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
bool SwitchActor::OnSignal(long SignalNumber)
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

		if(_Renderer && _renderertype == 0)
		{
			SpriteRenderer * spr = static_cast<SpriteRenderer *>(_Renderer);
			spr->Next();
		}
		#endif

		_switchOn = !_switchOn;
		SendSignal(_outputsignal, _targets);

		return true;
	}

	return false;
}

/***********************************************************
get current actor state
return false if the actor is stateless
***********************************************************/
bool SwitchActor::Getstate(ActorStateInfo & currState)
{
	currState.On = 	_switchOn;
	return true;
}

/***********************************************************
set the actor state
***********************************************************/
void SwitchActor::Setstate(const ActorStateInfo & currState)
{
	if(currState.On)
	{
		_switchOn = true;

		#ifndef _LBANET_SERVER_SIDE_
		if(_Renderer && _renderertype == 0)
		{
			SpriteRenderer * spr = static_cast<SpriteRenderer *>(_Renderer);
			spr->Next();
		}
		#endif
	}
}
