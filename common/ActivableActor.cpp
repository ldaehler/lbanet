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


#include "ActivableActor.h"
#include <math.h>

#ifndef M_PI
#define M_PI    3.14159265358979323846f
#endif

/***********************************************************
	Constructor
***********************************************************/
ActivableActor::ActivableActor(float activationdistance, int activationtype)
: _activationdistance(activationdistance), _activationtype(activationtype)
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
								int actionType, bool DirectActivation)
{
	float distX = _posX-PlayerPosX;
	float distY = _posY-PlayerPosY;
	float distZ = _posZ-PlayerPosZ;

	if(_activationtype != actionType)
		return false;

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

