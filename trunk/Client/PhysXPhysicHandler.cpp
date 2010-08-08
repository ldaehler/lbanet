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

#include "PhysXPhysicHandler.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <math.h>
#include <set>
#include <algorithm>

#include "LocalActorsHandler.h"
#include "ExternalActorsHandler.h"
#include "PhysXEngine.h"
#include "NxVec3.h"
#include "NxController.h"
#include "NxActor.h"
#include "Actor.h"

#include <windows.h>    // Header File For Windows
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#include <GL/glu.h>     // Header File For The GLu32 Library


#define _OFFSET_MISS_Y	2.499f
#define _OFFSET_MISS_2_Y	0.0f


/*
--------------------------------------------------------------------------------------------------
- constructor
--------------------------------------------------------------------------------------------------
*/
ActorPositionHandler::ActorPositionHandler(NxActor* contr, float X, float Y, float Z)
: controller(contr)
{
	if(controller)
		controller->setGlobalPosition(NxVec3(X, Y+_OFFSET_MISS_2_Y, Z));
}


/*
--------------------------------------------------------------------------------------------------
- SetPosition
--------------------------------------------------------------------------------------------------
*/
void ActorPositionHandler::SetPosition(float X, float Y, float Z)
{
	if(controller)
		controller->moveGlobalPosition(NxVec3(X, Y+_OFFSET_MISS_2_Y, Z));
}

/*
--------------------------------------------------------------------------------------------------
- GetPosition
--------------------------------------------------------------------------------------------------
*/
void ActorPositionHandler::GetPosition(float &X, float &Y, float &Z)
{
	if(controller)
	{
		NxVec3 vec = controller->getGlobalPosition();
		X = vec.x;
		Y = vec.y-_OFFSET_MISS_2_Y;
		Z = vec.z;
	}
}



/*
--------------------------------------------------------------------------------------------------
- hide
--------------------------------------------------------------------------------------------------
*/
void ActorPositionHandler::Hide()
{
	if(controller)
		controller->raiseActorFlag(NX_AF_DISABLE_COLLISION);
}

/*
--------------------------------------------------------------------------------------------------
- show
--------------------------------------------------------------------------------------------------
*/
void ActorPositionHandler::Show()
{
	if(controller)
		ActorPositionHandler::controller->clearActorFlag(NX_AF_DISABLE_COLLISION);
}



/*
--------------------------------------------------------------------------------------------------
- check if graphic need to be refresh from physic
--------------------------------------------------------------------------------------------------
*/
bool ActorPositionHandler::GraphicsNeedUpdate()
{
	bool res = false;

	if(controller)
	{
		
		if(controller->isDynamic())
		{
			if(!controller->readBodyFlag(NX_BF_KINEMATIC))
			{
				if(!controller->isSleeping())
				{
					res = true;
				}
			}
		}
	}

	return res;
}




/*
--------------------------------------------------------------------------------------------------
- constructor
--------------------------------------------------------------------------------------------------
*/
PhysXPhysicHandler::PhysXPhysicHandler(const std::string filename, 
										 LocalActorsHandler * LAH, ExternalActorsHandler * EAH,
										float posX, float posY, float posZ)
	: _localAH(LAH), _externalAH(EAH)
{

	_lastposX = posX;
	_lastposY = posY+_OFFSET_MISS_Y;
	_lastposZ = posZ;


	ActorUserData * usdata = new ActorUserData(3, -1, NULL);
	_controller = PhysXEngine::getInstance()->CreateCharacter(NxVec3(_lastposX, _lastposY, _lastposZ), 
																0.4f, 3.8f, usdata);


	ActorUserData * mstorage = new ActorUserData(2, -1, NULL);
	_map = PhysXEngine::getInstance()->LoadTriangleMeshFile(NxVec3(0,0,0), filename, mstorage);


	if(LAH)
	{	 
		std::map<long, Actor *> * acts = LAH->GetActors();
		if(acts)
		{
			std::map<long, Actor *>::iterator it = acts->begin();
			std::map<long, Actor *>::iterator end = acts->end();
			for(;it != end; ++it)
			{
				float sizex = it->second->GetSizeX();
				float sizey = it->second->GetSizeY();
				float sizez = it->second->GetSizeZ();

				float posx = it->second->GetPosX();
				float posy = it->second->GetPosY() + it->second->GetOffsetSizeY();
				float posz = it->second->GetPosZ();


				int ctype = 2; //kynematic by default
				if(it->second->IsMovable())
					ctype = 3; // dynamic if movable

				if(sizex > 0)
				{
					sizey /= 2;
					posy += sizey;



					ActorUserData * usdata = new ActorUserData(1, it->first, NULL);
					NxActor* cont = PhysXEngine::getInstance()->CreateBox(NxVec3(posx, posy, posz), 
																		sizex, sizey, sizez, 1.0, ctype, usdata,
																		it->second->GetCollidable(), it->second->IsMovable());

					it->second->SetPhysController(new ActorPositionHandler(cont, posx, posy, posz));
					_actors.push_back(cont);
				}
			}
		}
	}

	if(EAH)
	{	 
		std::map<long, Actor *> * acts = EAH->GetActors();
		if(acts)
		{
			std::map<long, Actor *>::iterator it = acts->begin();
			std::map<long, Actor *>::iterator end = acts->end();
			for(;it != end; ++it)
			{
				float sizex = it->second->GetSizeX();
				float sizey = it->second->GetSizeY();
				float sizez = it->second->GetSizeZ();

				float posx = it->second->GetPosX();
				float posy = it->second->GetPosY() + it->second->GetOffsetSizeY();
				float posz = it->second->GetPosZ();

				int ctype = 2; //kynematic by default
				if(it->second->IsMovable())
					ctype = 3; // dynamic if movable

				if(sizex > 0)
				{
					sizey /= 2;
					posy += sizey;

					ActorUserData * usdata = new ActorUserData(1, it->first, NULL);
					NxActor* cont = PhysXEngine::getInstance()->CreateBox(NxVec3(posx, posy, posz), 
																		sizex, sizey, sizez, 1.0, ctype, usdata,
																		it->second->GetCollidable(), it->second->IsMovable());

					it->second->SetPhysController(new ActorPositionHandler(cont, posx, posy, posz));
					_actors.push_back(cont);
				}
			}
		}
	}
}

/*
--------------------------------------------------------------------------------------------------
- denstructor
--------------------------------------------------------------------------------------------------
*/
PhysXPhysicHandler::~PhysXPhysicHandler()
{
	if(PhysXEngine::getInstance()->IsInitialized())
	{
		ActorUserData * mstorage = (ActorUserData *)_map->userData;
		ActorUserData * characterdata = (ActorUserData *)_controller->getActor()->userData;

		PhysXEngine::getInstance()->DestroyActor(_map);
		PhysXEngine::getInstance()->DestroyCharacter(_controller);

		if(mstorage)
			delete mstorage;
		if(characterdata)
			delete characterdata;

		std::vector<NxActor*>::iterator it = _actors.begin();
		std::vector<NxActor*>::iterator end = _actors.end();
		for(; it != end; ++it)
		{
			ActorUserData * adata = (ActorUserData *)(*it)->userData;
			PhysXEngine::getInstance()->DestroyActor(*it);
			
			if(adata)
				delete adata;
		}
	}



}

/*
--------------------------------------------------------------------------------------------------
// check if it is possible to move from one position to another
// the actor has a bounding box centered on currX, currZ
// and which goes up from currY
// actorSizeX and actorSizeZ are 1/2 the diameter from the center
// the physic engine will apply a gravity force to the move if not flying
// so that the actor might fall down if needed
--------------------------------------------------------------------------------------------------
*/
MoveOutput PhysXPhysicHandler::MoveActor(long ActorId, const AABB & actorBB,
											const VECTOR &Speed, bool checkcolision)
{
	MoveOutput res;
	res.NewSpeed = Speed;
	res.TouchingWater = false;
	res.TouchingGround = false;
	res.MovingObject = false;
	res.Collisionx = false;
	res.Collisionz = false;
	res.CollisionUp = false;
	

	unsigned int flags = PhysXEngine::getInstance()->MoveCharacter(_controller, 
													NxVec3(Speed.x, Speed.y, Speed.z), checkcolision);

	float posX, posY, posZ;
	PhysXEngine::getInstance()->GetCharacterPosition(_controller, posX, posY, posZ);

	res.NewSpeed.x = posX - _lastposX;
	res.NewSpeed.y = posY - _lastposY;
	res.NewSpeed.z = posZ - _lastposZ;

	if((bool)(flags & NXCC_COLLISION_SIDES))
	{
		if(abs(res.NewSpeed.x - Speed.x) > 0.00001)
			res.Collisionx = true;

		if(abs(res.NewSpeed.z - Speed.z) > 0.00001)
			res.Collisionz = true;
	}

	res.CollisionUp = (bool)(flags & NXCC_COLLISION_UP);

	res.TouchingGround = (bool)(flags & NXCC_COLLISION_DOWN);
	if(res.TouchingGround)
	{
		ActorUserData * characterdata = (ActorUserData *)_controller->getActor()->userData;
		if(characterdata)
		{
			res.TouchingWater = (characterdata->HittedFloorMaterial > 16);

			res.MovingObject = characterdata->MovingObject;
			characterdata->MovingObject = false;
			res.MovingDirection = characterdata->MovingDirection;
		}
	}

	_lastposX = posX;
	_lastposY = posY;
	_lastposZ = posZ;

	return res;
}


/*
--------------------------------------------------------------------------------------------------
// set actor position
--------------------------------------------------------------------------------------------------
*/
void PhysXPhysicHandler::SetActorPos(long ActorId, const VECTOR &NewPos)
{
	_lastposX = NewPos.x;
	_lastposY = NewPos.y+_OFFSET_MISS_Y;
	_lastposZ = NewPos.z;
	PhysXEngine::getInstance()->SetCharacterPos(_controller, NxVec3(_lastposX, _lastposY, _lastposZ));
}


/*
--------------------------------------------------------------------------------------------------
- return int > 0 if there is a roof on top of the given position
- function used to know when to cut the room display in half
--------------------------------------------------------------------------------------------------
*/
int PhysXPhysicHandler::IsUnderRoof(const VECTOR & ActorPos)
{
	return PhysXEngine::getInstance()->CheckForRoof(ActorPos.x, ActorPos.y, ActorPos.z);
}



/*
--------------------------------------------------------------------------------------------------
- get closest floor from the actor position
--------------------------------------------------------------------------------------------------
*/
int PhysXPhysicHandler::GetClosestFloor(const VECTOR & ActorPos)
{
	return PhysXEngine::getInstance()->GetClosestFloor(ActorPos.x, ActorPos.y, ActorPos.z);
}



/*
--------------------------------------------------------------------------------------------------
- return a positive number giving the speed of the gravity on the Y axis
--------------------------------------------------------------------------------------------------
*/
float PhysXPhysicHandler::GetGravitySpeed()
{
	NxVec3 vec;
	PhysXEngine::getInstance()->GetGravity(vec);
	return vec.y;
}


/*
--------------------------------------------------------------------------------------------------
- render physic shapes
--------------------------------------------------------------------------------------------------
*/
void PhysXPhysicHandler::Render()
{
	PhysXEngine::getInstance()->RenderActors();
}


/*
--------------------------------------------------------------------------------------------------
- set if actor is allowed to move objects
--------------------------------------------------------------------------------------------------
*/
void PhysXPhysicHandler::SetAllowedMoving(bool allowed)
{
	ActorUserData * characterdata = (ActorUserData *)_controller->getActor()->userData;
	if(characterdata)
	{
		characterdata->AllowedMoving = allowed;
	}
}