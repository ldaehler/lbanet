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
void PhysXActorHandler::GetRotation(float &X, float &Y, float &Z)
{
	_rotH->GetRotation(X, Y, Z);
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
void PhysXActorHandler::SetRotation(float X, float Y, float Z)
{
	_rotH->SetRotation(X, Y, Z);
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
move object in the world
***********************************************************/
void PhysXActorHandler::Rotate(float deltaX, float deltaY, float deltaZ)
{
	_rotH->Rotate(deltaX, deltaY, deltaZ);
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
void PhysXActorHandler::RotateTo(float X, float Y, float Z)
{
	_rotH->RotateTo(X, Y, Z);
}


/***********************************************************
	Constructor
***********************************************************/
PhysXDynamicActorHandler::PhysXDynamicActorHandler(boost::shared_ptr<PhysXEngine> Pengine,
													boost::shared_ptr<ActorUserData> UserData,
													NxActor* Actor)
		: PhysXObjectHandlerBase(Pengine, UserData), _Actor(Actor)
{}


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
void PhysXDynamicActorHandler::GetRotation(float &X, float &Y, float &Z)
{
	// TODO
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
void PhysXDynamicActorHandler::SetRotation(float X, float Y, float Z)
{
	NxQuat quatx;
	quatx.fromAngleAxis(X, NxVec3(1, 0, 0));
	NxMat33 matx;
	matx.fromQuat(quatx);

	NxQuat quaty;
	quaty.fromAngleAxis(Y, NxVec3(0, 1, 0));
	NxMat33 maty;
	maty.fromQuat(quaty);

	NxQuat quatz;
	quatz.fromAngleAxis(Z, NxVec3(0, 0, 1));
	NxMat33 matz;
	matz.fromQuat(quatz);

	_Actor->setGlobalOrientation(matx*maty*matz);
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
move object in the world
***********************************************************/
void PhysXDynamicActorHandler::Rotate(float deltaX, float deltaY, float deltaZ)
{
	float currRX, currRY, currRZ;
	GetRotation(currRX, currRY, currRZ);
	RotateTo(currRX+deltaX, currRY+deltaY, currRZ+deltaZ);
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
void PhysXDynamicActorHandler::RotateTo(float X, float Y, float Z)
{
	NxQuat quatx;
	quatx.fromAngleAxis(X, NxVec3(1, 0, 0));
	NxMat33 matx;
	matx.fromQuat(quatx);

	NxQuat quaty;
	quaty.fromAngleAxis(Y, NxVec3(0, 1, 0));
	NxMat33 maty;
	maty.fromQuat(quaty);

	NxQuat quatz;
	quatz.fromAngleAxis(Z, NxVec3(0, 0, 1));
	NxMat33 matz;
	matz.fromQuat(quatz);

	_Actor->moveGlobalPosition(NxVec3(X, Y, Z));
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
void PhysXControllerHandler::GetRotation(float &X, float &Y, float &Z)
{
	_rotH->GetRotation(X, Y, Z);
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
void PhysXControllerHandler::SetRotation(float X, float Y, float Z)
{
	_rotH->SetRotation(X, Y, Z);
	_resetted = true;
}

/***********************************************************
move object in the world
***********************************************************/
void PhysXControllerHandler::Move(float deltaX, float deltaY, float deltaZ)
{
	_UserData->CollisionFlag =
			_Pengine->MoveCharacter(_Controller, NxVec3(deltaX, deltaY, deltaZ), true);
}

/***********************************************************
move object in the world
***********************************************************/
void PhysXControllerHandler::Rotate(float deltaX, float deltaY, float deltaZ)
{
	_rotH->Rotate(deltaX, deltaY, deltaZ);
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
void PhysXControllerHandler::RotateTo(float X, float Y, float Z)
{
	_rotH->RotateTo(X, Y, Z);
}