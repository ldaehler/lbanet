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

#include "PhysXObjectHandlers.h"
#include "NxPhysics.h"
#include "NxCapsuleController.h"
#include "PhysXEngine.h"


/***********************************************************
	Constructor
***********************************************************/
PhysXObjectHandlerBase::PhysXObjectHandlerBase(boost::shared_ptr<PhysXEngine> Pengine,
													boost::shared_ptr<ActorUserData> UserData)
		: _Pengine(Pengine), _UserData(UserData)
{}




/***********************************************************
	Constructor
***********************************************************/
PhysXActorHandler::PhysXActorHandler(boost::shared_ptr<PhysXEngine> Pengine,
						boost::shared_ptr<ActorUserData> UserData,
						NxActor* Actor,
						boost::shared_ptr<SimpleRotationHandler> rotH)
		: PhysXObjectHandlerBase(Pengine, UserData), _Actor(Actor), _rotH(rotH)
{}



/***********************************************************
get object position in the world
***********************************************************/
void PhysXActorHandler::GetPosition(float &X, float &Y, float &Z)
{
	NxVec3 vec = _Actor->getGlobalPosition();
	X = vec.x;
	Y = vec.y;
	Z = vec.z;
}

/***********************************************************
get object rotation on all axis
***********************************************************/
void PhysXActorHandler::GetRotation(LbaQuaternion& Q)
{
	_rotH->GetRotation(Q);
}

/***********************************************************
set object position in the world
***********************************************************/
void PhysXActorHandler::SetPosition(float X, float Y, float Z)
{
	_Actor->setGlobalPosition(NxVec3(X, Y, Z));
	_resetted = true;
}

/***********************************************************
set object rotation on all axis
***********************************************************/
void PhysXActorHandler::SetRotation(const LbaQuaternion& Q)
{
	_rotH->SetRotation(Q);
	_resetted = true;
}

/***********************************************************
move object in the world
***********************************************************/
void PhysXActorHandler::Move(float deltaX, float deltaY, float deltaZ)
{
	float currPosX, currPosY, currPosZ;
	GetPosition(currPosX, currPosY, currPosZ);
	MoveTo(currPosX+deltaX, currPosY+deltaY, currPosZ+deltaZ);
}

/***********************************************************
move object to a position in the world
***********************************************************/
void PhysXActorHandler::MoveTo(float X, float Y, float Z)
{
	_Actor->moveGlobalPosition(NxVec3(X, Y, Z));
}


/***********************************************************
rotate object in the world
***********************************************************/
void PhysXActorHandler::RotateTo(const LbaQuaternion& Q)
{
	_rotH->RotateTo(Q);
}


/***********************************************************
	Constructor
***********************************************************/
PhysXDynamicActorHandler::PhysXDynamicActorHandler(boost::shared_ptr<PhysXEngine> Pengine,
													boost::shared_ptr<ActorUserData> UserData,
													NxActor* Actor, const LbaQuaternion& rotation)
		: PhysXObjectHandlerBase(Pengine, UserData), _Actor(Actor)
{
	SetRotation(rotation);
}


/***********************************************************
get object position in the world
***********************************************************/
void PhysXDynamicActorHandler::GetPosition(float &X, float &Y, float &Z)
{
	NxVec3 vec = _Actor->getGlobalPosition();
	X = vec.x;
	Y = vec.y;
	Z = vec.z;
}

/***********************************************************
get object rotation on all axis
***********************************************************/
void PhysXDynamicActorHandler::GetRotation(LbaQuaternion& Q)
{
	NxQuat quat = _Actor->getGlobalOrientationQuat();
	Q.X = quat.x;
	Q.Y = quat.y;
	Q.Z = quat.z;
	Q.W = quat.w;
}

/***********************************************************
set object position in the world
***********************************************************/
void PhysXDynamicActorHandler::SetPosition(float X, float Y, float Z)
{
	_Actor->setGlobalPosition(NxVec3(X, Y, Z));
	_resetted = true;
}

/***********************************************************
set object rotation on all axis
***********************************************************/
void PhysXDynamicActorHandler::SetRotation(const LbaQuaternion& Q)
{
	_Actor->setGlobalOrientationQuat(NxQuat(NxVec3(Q.X, Q.Y, Q.Z), Q.W));
	_resetted = true;
}

/***********************************************************
move object in the world
***********************************************************/
void PhysXDynamicActorHandler::Move(float deltaX, float deltaY, float deltaZ)
{
	float currPosX, currPosY, currPosZ;
	GetPosition(currPosX, currPosY, currPosZ);
	MoveTo(currPosX+deltaX, currPosY+deltaY, currPosZ+deltaZ);
}

/***********************************************************
move object to a position in the world
***********************************************************/
void PhysXDynamicActorHandler::MoveTo(float X, float Y, float Z)
{
	_Actor->moveGlobalPosition(NxVec3(X, Y, Z));
}


/***********************************************************
rotate object in the world
***********************************************************/
void PhysXDynamicActorHandler::RotateTo(const LbaQuaternion& Q)
{
	_Actor->moveGlobalOrientationQuat(NxQuat(NxVec3(Q.X, Q.Y, Q.Z), Q.W));
}


/***********************************************************
	Constructor
***********************************************************/
PhysXControllerHandler::PhysXControllerHandler(boost::shared_ptr<PhysXEngine> Pengine,
												boost::shared_ptr<ActorUserData> UserData,
												NxController* Controller,
												boost::shared_ptr<SimpleRotationHandler> rotH)
		: PhysXObjectHandlerBase(Pengine, UserData), _Controller(Controller), _rotH(rotH)
{}



/***********************************************************
get object position in the world
***********************************************************/
void PhysXControllerHandler::GetPosition(float &X, float &Y, float &Z)
{
	NxExtendedVec3 vec = _Controller->getPosition();
	X = (float)vec.x;
	Y = (float)vec.y;
	Z = (float)vec.z;
}

/***********************************************************
get object rotation on all axis
***********************************************************/
void PhysXControllerHandler::GetRotation(LbaQuaternion& Q)
{
	_rotH->GetRotation(Q);
}

/***********************************************************
set object position in the world
***********************************************************/
void PhysXControllerHandler::SetPosition(float X, float Y, float Z)
{
	NxExtendedVec3 pos;
	pos.x = X;
	pos.y = Y;
	pos.z = Z;
	_Controller->setPosition(pos);
	_resetted = true;
}

/***********************************************************
set object rotation on all axis
***********************************************************/
void PhysXControllerHandler::SetRotation(const LbaQuaternion& Q)
{
	_rotH->SetRotation(Q);
	_resetted = true;
}

/***********************************************************
move object in the world
***********************************************************/
void PhysXControllerHandler::Move(float deltaX, float deltaY, float deltaZ)
{
	unsigned int CollisionFlag =
			_Pengine->MoveCharacter(_Controller, NxVec3(deltaX, deltaY, deltaZ), true);

	_UserData->CollisionUpFlag = (CollisionFlag == NXCC_COLLISION_UP);
	_UserData->CollisionDownFlag = (CollisionFlag == NXCC_COLLISION_DOWN);
	_UserData->CollisionSideFlag = (CollisionFlag == NXCC_COLLISION_SIDES);
}

/***********************************************************
move object to a position in the world
***********************************************************/
void PhysXControllerHandler::MoveTo(float X, float Y, float Z)
{
	SetPosition(X, Y, Z);
}

/***********************************************************
rotate object in the world
***********************************************************/
void PhysXControllerHandler::RotateTo(const LbaQuaternion& Q)
{
	_rotH->RotateTo(Q);
}