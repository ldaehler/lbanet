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

#ifndef __LBA_NET_PHYSIC_HANDLER_BASE_H__
#define __LBA_NET_PHYSIC_HANDLER_BASE_H__



#include "vector.h"


struct MoveOutput
{
	VECTOR	NewSpeed;		// modified actor speed
	bool	TouchingWater;	// flag indicate if the actor is touchign water
	bool	TouchingGround;	// flag indicate if the actor is on the ground or if he is flying/falling down
};


//*************************************************************************************************
//*                                      class PhysicHandlerBase
//*************************************************************************************************

class PhysicHandlerBase
{
public:

	//! constructor
	PhysicHandlerBase(){}

	//! destructor
	virtual ~PhysicHandlerBase(){}


	// check if it is possible to move from one position to another
	// the actor has a bounding box centered on currX, currZ
	// and which goes up from currY
	// actorSizeX and actorSizeZ are 1/2 the diameter from the center
	// the physic engine will apply a gravity force to the move if not flying
	// so that the actor might fall down if needed
	virtual MoveOutput MoveActor(long ActorId, const AABB & actorBB,
									const VECTOR &Speed, bool checkcolision = true) = 0;


	// return int > 0 if there is a roof on top of the given position
	// function used to know when to cut the room display in half
	virtual int IsUnderRoof(const VECTOR & ActorPos) = 0;


	// get closest floor from the actor position
	virtual float GetClosestFloor(const VECTOR & ActorPos) = 0;

	// return a positive number giving the speed of the gravity on the Y axis
	virtual float GetGravitySpeed() =0;

	//! set actor position
	virtual void SetActorPos(long ActorId, const VECTOR &NewPos) = 0;

	// render physic shapes
	virtual void Render() =0;
};

#endif
