#include "ExitActor.h"

#ifndef _LBANET_SERVER_SIDE_
	#include "ThreadSafeWorkpile.h"
	#include "GameEvents.h"
	#include <math.h>

	#ifndef M_PI
	#define M_PI    3.14159265358979323846f
	#endif
#endif

/***********************************************************
	Constructor
***********************************************************/
ExitActor::ExitActor(float activationdistance, float deltaX, float deltaY, float deltaZ, int direction)
: _deltaX(deltaX), _deltaY(deltaY), _deltaZ(deltaZ), _direction(direction), ActivableActor(activationdistance)
{

}


/***********************************************************
	Destructor
***********************************************************/
ExitActor::~ExitActor()
{

}

/***********************************************************
process activation
***********************************************************/
void ExitActor::ProcessActivation(float PlayerPosX, float PlayerPosY, float PlayerPosZ, float PlayerRotation)
{
	SendSignal(_outputsignal, _targets);

#ifndef _LBANET_SERVER_SIDE_
	float distX = _posX-PlayerPosX;
	float distY = _posY-PlayerPosY;
	float distZ = _posZ-PlayerPosZ;

	double distance = sqrt((distX * distX) + (distZ * distZ));

	float angle = (float)(180 * acos(distZ / distance) / M_PI);
	if(distX < 0)
		angle = 360-angle;



	float remX=0, remZ=0;
	switch(_direction)
	{
		case 0:
			remZ = -1.f;
		break;
		case 90:
			remX = -1.f;
		break;
		case 180:
			remZ = 1.f;
		break;
		case 270:
			remX = 1.f;
		break;
	}



	std::vector<PlayerScriptPart> script;

	// add first rotation toward point position
	{
	PlayerScriptPart rot1;
	rot1.Type = 0;
	rot1.Animation = (angle > PlayerRotation) ? 3 : 4;
	rot1.ValueA = angle;
	rot1.Speed = 0.08f;
	script.push_back(rot1);
	}

	// add translation to point
	{
	PlayerScriptPart trans1;
	trans1.Type = 1;
	trans1.Animation = 1;
	trans1.ValueA = _posX+remX;
	trans1.ValueB = _posY;
	trans1.ValueC = _posZ+remZ;
	trans1.Speed = 0.003f;
	script.push_back(trans1);
	}

	// add rotation before animate
	{
	PlayerScriptPart rot2;
	rot2.Type = 0;
	rot2.Animation =  (_direction > angle) ? 3 : 4;
	rot2.ValueA = (float)_direction;
	rot2.Speed = 0.08f;
	script.push_back(rot2);
	}

	// play short down animation
	{
	PlayerScriptPart anim1;
	anim1.Type = 2;
	anim1.Animation = 24;
	anim1.ValueA = 3;
	script.push_back(anim1);
	}


	// do translation to exit
	{
	PlayerScriptPart trans2;
	trans2.Type = 1;
	trans2.Animation = 24;
	trans2.ValueA = _posX+remX+_deltaX;
	trans2.ValueB = _posY+_deltaY;
	trans2.ValueC = _posZ+remZ+_deltaZ;
	trans2.Speed = 0.003f;
	script.push_back(trans2);
	}

	ThreadSafeWorkpile::getInstance()->AddEvent(new MainPlayerScriptedEvent(script));
#endif
}