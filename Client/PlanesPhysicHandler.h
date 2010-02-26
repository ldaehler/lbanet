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

#ifndef __LBA_NET_PLANES_PHYSIC_HANDLER_H__
#define __LBA_NET_PLANES_PHYSIC_HANDLER_H__

#include <string>
#include <vector>

#include "PhysicHandlerBase.h"

class LocalActorsHandler;
class ExternalActorsHandler;



//*************************************************************************************************
//*                                      class PlanesPhysicHandler
//*************************************************************************************************

class PlanesPhysicHandler : public PhysicHandlerBase
{
public:

	//! constructor
	PlanesPhysicHandler(const std::string filename, LocalActorsHandler * LAH, ExternalActorsHandler * EAH);

	//! destructor
	~PlanesPhysicHandler();


	// check if it is possible to move from one position to another
	// the actor has a bounding box centered on currX, currZ
	// and which goes up from currY
	// actorSizeX and actorSizeZ are 1/2 the diameter from the center
	// the physic engine will apply a gravity force to the move if not flying
	// so that the actor might fall down if needed
	virtual MoveOutput MoveActor(long ActorId, float ActorPosX, float ActorPosY, float ActorPosZ,
									float ActorSizeX, float ActorSizeY, float ActorSizeZ,
									float SpeedX, float SpeedY, float SpeedZ);


	// return int > 0 if there is a roof on top of the given position
	// function used to know when to cut the room display in half
	virtual int IsUnderRoof(float ActorPosX, float ActorPosY, float ActorPosZ);


	// get closest floor from the actor position
	virtual float GetClosestFloor(float ActorPosX, float ActorPosY, float ActorPosZ);


	// return a positive number giving the speed of the gravity on the Y axis
	virtual float GetGravitySpeed();

protected:

private:

	LocalActorsHandler*		_localAH;
	ExternalActorsHandler*	_externalAH;

};

#endif
