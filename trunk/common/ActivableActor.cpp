#include "ActivableActor.h"
#include <math.h>

#ifndef M_PI
#define M_PI    3.14159265358979323846f
#endif

/***********************************************************
	Constructor
***********************************************************/
ActivableActor::ActivableActor(float activationdistance)
: _activationdistance(activationdistance)
{

}



/***********************************************************
	Destructor
***********************************************************/
ActivableActor::~ActivableActor()
{

}

/***********************************************************
activate an actor
***********************************************************/
bool ActivableActor::Activate(float PlayerPosX, float PlayerPosY, float PlayerPosZ, float PlayerRotation,
								bool DirectActivation)
{
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

	if(PlayerRotation < (angle-40))
		return false;

	if(PlayerRotation > (angle+40))
		return false;

	if(DirectActivation)
 		ProcessActivation(PlayerPosX, PlayerPosY, PlayerPosZ, PlayerRotation);

	return true;
}

