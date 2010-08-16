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
#include "LogHandler.h"

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
: controller(contr), _tempkinec(false)
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
			if(_tempkinec || !controller->readBodyFlag(NX_BF_KINEMATIC))
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
- check if graphic need to be refresh from physic
--------------------------------------------------------------------------------------------------
*/
void ActorPositionHandler::SetKinematic(bool kinematic)
{
	if(controller)
	{
		
		if(kinematic)
		{
			_tempkinec = true;
			_savedkinematic = controller->readBodyFlag(NX_BF_KINEMATIC);
			controller->raiseBodyFlag(NX_BF_KINEMATIC);
		}
		else
		{
			_tempkinec = false;
			if(!_savedkinematic)
				controller->clearBodyFlag(NX_BF_KINEMATIC);
		}
	}
}







/*
--------------------------------------------------------------------------------------------------
- constructor
--------------------------------------------------------------------------------------------------
*/
MovableActorPositionHandler::MovableActorPositionHandler(NxController* contr, float X, float Y, float Z)
: _controller(contr), ActorPositionHandler(NULL, 0, 0, 0), _wasfalling(false)
{
	if(_controller)
		PhysXEngine::getInstance()->SetCharacterPos(_controller, NxVec3(X, Y, Z));
}


/*
--------------------------------------------------------------------------------------------------
- SetPosition
--------------------------------------------------------------------------------------------------
*/
void MovableActorPositionHandler::SetPosition(float X, float Y, float Z)
{
	float lX, lY, lZ;
	GetPosition(lX, lY, lZ);
	float speedX = X - lX;
	float speedY = Y - lY;
	float speedZ = Z - lZ;
	if(_controller)
		PhysXEngine::getInstance()->MoveCharacter(_controller, NxVec3(speedX, speedY, speedZ), false);

}

/*
--------------------------------------------------------------------------------------------------
- GetPosition
--------------------------------------------------------------------------------------------------
*/
void MovableActorPositionHandler::GetPosition(float &X, float &Y, float &Z)
{
	if(_controller)
		PhysXEngine::getInstance()->GetCharacterPosition(_controller, X, Y, Z);
}



/*
--------------------------------------------------------------------------------------------------
- hide
--------------------------------------------------------------------------------------------------
*/
void MovableActorPositionHandler::Hide()
{
	if(_controller)
		PhysXEngine::getInstance()->HideShowCharacter(_controller, false);
}

/*
--------------------------------------------------------------------------------------------------
- show
--------------------------------------------------------------------------------------------------
*/
void MovableActorPositionHandler::Show()
{
	if(_controller)
		PhysXEngine::getInstance()->HideShowCharacter(_controller, true);
}



/*
--------------------------------------------------------------------------------------------------
- check if graphic need to be refresh from physic
--------------------------------------------------------------------------------------------------
*/
bool MovableActorPositionHandler::GraphicsNeedUpdate()
{
	bool res = false;

	if(_controller)
	{
		ActorUserData * characterdata = (ActorUserData *)_controller->getActor()->userData;
		if(characterdata)
		{
			res = characterdata->GetShouldUpdate();
			characterdata->SetShouldUpdate(false);
		}
	}

	return res;
}



/*
--------------------------------------------------------------------------------------------------
- process physci if needed
--------------------------------------------------------------------------------------------------
*/
void MovableActorPositionHandler::Process(bool isattached)
{
	if(isattached)
		return;

	if(_controller)
	{
		NxVec3 vecg;
		PhysXEngine::getInstance()->GetGravity(vecg);

		unsigned int flags = PhysXEngine::getInstance()->MoveCharacter(_controller, 
																	NxVec3(0, vecg.y/10.0f, 0), true);

		// we are falling
		if(!(flags & NXCC_COLLISION_DOWN))
		{
			_wasfalling = true;
			ActorUserData * characterdata = (ActorUserData *)_controller->getActor()->userData;
			if(characterdata)
				characterdata->SetShouldUpdate(true);
		}
		else
		{
			if(_wasfalling)
			{
				ActorUserData * characterdata = (ActorUserData *)_controller->getActor()->userData;
				if(characterdata)
					characterdata->SetShouldUpdate(true);
			}
			_wasfalling = false;
		}
	}
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

				if(sizex > 0)
				{
					sizey /= 2;
					posy += sizey;
					ActorUserData * usdata = new ActorUserData(1, it->first, NULL);
					usdata->SetAllowFreeMove(it->second->GetAllowFreeMove());

					if(it->second->IsMovable())
					{
						NxController* cont = PhysXEngine::getInstance()->CreateCharacterBox(NxVec3(posx, posy, posz), 
																							NxVec3(sizex, sizey, sizez), usdata);

						it->second->SetPhysController(new MovableActorPositionHandler(cont, posx, posy, posz));
						_mactors.push_back(cont);
					}
					else
					{				
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

				if(sizex > 0)
				{
					sizey /= 2;
					posy += sizey;
					ActorUserData * usdata = new ActorUserData(1, it->first, NULL);
					usdata->SetAllowFreeMove(it->second->GetAllowFreeMove());

					if(it->second->IsMovable())
					{
						NxController* cont = PhysXEngine::getInstance()->CreateCharacterBox(NxVec3(posx, posy, posz), 
																							NxVec3(sizex, sizey, sizez), usdata);

						it->second->SetPhysController(new MovableActorPositionHandler(cont, posx, posy, posz));
						_mactors.push_back(cont);
					}
					else
					{

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
}

/*
--------------------------------------------------------------------------------------------------
- denstructor
--------------------------------------------------------------------------------------------------
*/
PhysXPhysicHandler::~PhysXPhysicHandler()
{
	LogHandler::getInstance()->LogToFile("Destroying physical scene...");

	if(PhysXEngine::getInstance()->IsInitialized())
	{
		ActorUserData * mstorage = (ActorUserData *)_map->userData;
		ActorUserData * characterdata = (ActorUserData *)_controller->getActor()->userData;

		LogHandler::getInstance()->LogToFile("Destroying map...");
		PhysXEngine::getInstance()->DestroyActor(_map);

		LogHandler::getInstance()->LogToFile("Destroying player...");
		PhysXEngine::getInstance()->DestroyCharacter(_controller);

		if(mstorage)
			delete mstorage;
		if(characterdata)
			delete characterdata;

		LogHandler::getInstance()->LogToFile("Destroying actors...");

		//destroy actors
		{
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

		LogHandler::getInstance()->LogToFile("Destroying movable actors...");

		//destroy movable actors
		{
			std::vector<NxController*>::iterator it = _mactors.begin();
			std::vector<NxController*>::iterator end = _mactors.end();
			for(; it != end; ++it)
			{
				ActorUserData * adata = (ActorUserData *)(*it)->getActor()->userData;
				PhysXEngine::getInstance()->DestroyCharacter(*it);
				
				if(adata)
					delete adata;
			}
		}
		
	}

	LogHandler::getInstance()->LogToFile("Physical destroyed.");
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
	res.AllowFreeMove = false;

	ActorUserData * characterdata = (ActorUserData *)_controller->getActor()->userData;
	if(characterdata)
	{
		characterdata->SetCurrentMoveX(Speed.x);
		characterdata->SetCurrentMoveY(Speed.y);
		characterdata->SetCurrentMoveZ(Speed.z);
	}

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
		if(characterdata)
			res.TouchingWater = (characterdata->GetHittedFloorMaterial() > 16);
	}

	if(characterdata)
	{
		res.MovingObject = characterdata->GetMovingObject();
		characterdata->SetMovingObject(false);
		res.MovingDirection = characterdata->GetMovingDirection();
		res.AllowFreeMove = characterdata->GetAllowFreeMove();
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
void PhysXPhysicHandler::SetAllowedMoving(bool allowedX, bool allowedZ)
{
	ActorUserData * characterdata = (ActorUserData *)_controller->getActor()->userData;
	if(characterdata)
	{
		characterdata->SetAllowedMovingX(allowedX);
		characterdata->SetAllowedMovingZ(allowedZ);
	}
}