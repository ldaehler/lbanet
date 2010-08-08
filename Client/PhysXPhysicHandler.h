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

#ifndef __LBA_NET_PHYSX_PHYSIC_HANDLER_H__
#define __LBA_NET_PHYSX_PHYSIC_HANDLER_H__

#include <string>
#include <vector>
#include <map>


#include "PhysicHandlerBase.h"


class LocalActorsHandler;
class ExternalActorsHandler;
class NxActor;
class NxController;


class ActorPositionHandler
{
public:
	//! constructor
	ActorPositionHandler(NxActor* contr, float X, float Y, float Z);

	void SetPosition(float X, float Y, float Z);
	void GetPosition(float &X, float &Y, float &Z);

	NxActor* GetActor()
	{ return controller; }

	void Hide();
	void Show();

	// check if graphic need to be refresh from physic
	bool GraphicsNeedUpdate();

private:
	NxActor* controller;
};


//*************************************************************************************************
//*                                      class PlanesPhysicHandler
//*************************************************************************************************

class PhysXPhysicHandler : public PhysicHandlerBase
{
public:

	//! constructor
	PhysXPhysicHandler(const std::string filename, LocalActorsHandler * LAH, 
						ExternalActorsHandler * EAH,
						float posX, float posY, float posZ);

	//! destructor
	~PhysXPhysicHandler();


	// check if it is possible to move from one position to another
	// the actor has a bounding box centered on currX, currZ
	// and which goes up from currY
	// actorSizeX and actorSizeZ are 1/2 the diameter from the center
	// the physic engine will apply a gravity force to the move if not flying
	// so that the actor might fall down if needed
	virtual MoveOutput MoveActor(long ActorId, const AABB & actorBB,
									const VECTOR &Speed, bool checkcolision = true);


	// return int > 0 if there is a roof on top of the given position
	// function used to know when to cut the room display in half
	virtual int IsUnderRoof(const VECTOR & ActorPos);


	// get closest floor from the actor position
	virtual int GetClosestFloor(const VECTOR & ActorPos);


	// return a positive number giving the speed of the gravity on the Y axis
	virtual float GetGravitySpeed();

	//! set actor position
	virtual void SetActorPos(long ActorId, const VECTOR &NewPos);

	// render physic shapes
	virtual void Render();

	// set if actor is allowed to move objects
	virtual void SetAllowedMoving(bool allowed);

protected:

	struct StairPlane
	{
		VECTOR C1;
		VECTOR C2;
		VECTOR C3;
		VECTOR C4;

		VECTOR Normal;
	};

	struct CornerStairPlane
	{
		VECTOR C1;
		VECTOR C2;
		VECTOR C3;

		VECTOR Normal;
	};


	struct NormalPlane
	{
		int Layer;
		Square2D Square;

		bool IsWater;
	};




private:

	LocalActorsHandler*				_localAH;
	ExternalActorsHandler*			_externalAH;


	std::map<int, std::vector<NormalPlane> >		_floors;
	std::map<int, std::vector<NormalPlane> >		_wallsX;
	std::map<int, std::vector<NormalPlane> >		_wallsZ;

	std::vector<StairPlane>							_stairs;
	std::vector<CornerStairPlane>					_corner_stairs;

	bool											_shiftcheck;

	float											_lastposX;
	float											_lastposY;
	float											_lastposZ;

	NxController*									_controller;
	std::vector<NxActor*>							_actors;
	NxActor*										_map;
};

#endif
