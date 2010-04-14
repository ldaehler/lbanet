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
#include "NxVec3.h"
#include "ObjectsDescription.h"
#include "LogHandler.h"

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
PhysXActorsHandler::PhysXActorsHandler(boost::shared_ptr<PhysXEngine> Pengine,
													boost::shared_ptr<ActorUserData> UserData,
													NxActor* Actor, const LbaQuaternion& rotation)
		: PhysXObjectHandlerBase(Pengine, UserData), _Actor(Actor)
{
	_Actor->setGlobalOrientationQuat(NxQuat(NxVec3(rotation.X, rotation.Y, rotation.Z), rotation.W));


	#ifdef _DEBUG
		LogHandler::getInstance()->LogToFile("Created new PhysXActor.");
	#endif
}


/***********************************************************
	destructor
***********************************************************/
PhysXActorsHandler::~PhysXActorsHandler()
{

	#ifdef _DEBUG
		LogHandler::getInstance()->LogToFile("Destroyed PhysXActor.");
	#endif

	_Pengine->DestroyActor(_Actor);
}

/***********************************************************
get object position in the world
***********************************************************/
void PhysXActorsHandler::GetPosition(float &X, float &Y, float &Z)
{	
	if(!_Actor) return;

	NxVec3 vec = _Actor->getGlobalPosition();
	X = vec.x;
	Y = vec.y;
	Z = vec.z;
}

/***********************************************************
get object rotation on all axis
***********************************************************/
void PhysXActorsHandler::GetRotation(LbaQuaternion& Q)
{	
	if(!_Actor) return;

	NxQuat quat = _Actor->getGlobalOrientationQuat();
	Q.X = quat.x;
	Q.Y = quat.y;
	Q.Z = quat.z;
	Q.W = quat.w;
}

/***********************************************************
get object rotation on all axis
***********************************************************/
float PhysXActorsHandler::GetRotationSingleAngle()
{
	NxQuat quat = _Actor->getGlobalOrientationQuat();
	return quat.getAngle();
}

/***********************************************************
set object position in the world
***********************************************************/
void PhysXActorsHandler::SetPosition(unsigned int time, float X, float Y, float Z)
{	
	_Pengine->SetActorPos(time, _Actor, NxVec3(X, Y, Z));
	_resetted = true;
}

/***********************************************************
set object rotation on all axis
***********************************************************/
void PhysXActorsHandler::SetRotation(unsigned int time, const LbaQuaternion& Q)
{	
	_Pengine->SetActorRotation(time, _Actor, NxQuat(NxVec3(Q.X, Q.Y, Q.Z), Q.W));
	_resetted = true;
}

/***********************************************************
move object in the world
***********************************************************/
void PhysXActorsHandler::Move(unsigned int time, float deltaX, float deltaY, float deltaZ)
{
	float currPosX, currPosY, currPosZ;
	GetPosition(currPosX, currPosY, currPosZ);
	MoveTo(time, currPosX+deltaX, currPosY+deltaY, currPosZ+deltaZ);
}

/***********************************************************
move object to a position in the world
***********************************************************/
void PhysXActorsHandler::MoveTo(unsigned int time, float X, float Y, float Z)
{	
	_Pengine->MoveActorTo(time, _Actor, NxVec3(X, Y, Z));
}


/***********************************************************
rotate object in the world
***********************************************************/
void PhysXActorsHandler::RotateYAxis(unsigned int time, float Speed)
{	
	_Pengine->RotateActoryAxis(time, _Actor, Speed);
}

/***********************************************************
rotate object in the world
***********************************************************/
void PhysXActorsHandler::RotateTo(unsigned int time, const LbaQuaternion& Q)
{	
	_Pengine->SetActorRotation(time, _Actor, NxQuat(NxVec3(Q.X, Q.Y, Q.Z), Q.W));
}



/***********************************************************
move object in the world
***********************************************************/
void PhysXActorsHandler::MoveInDirection(unsigned int time, float MoveSpeed, bool AddGravity)
{
	_Pengine->MoveInDirectionActor(time, _Actor, MoveSpeed, AddGravity);
}









/***********************************************************
	Constructor
***********************************************************/
PhysXControllerHandler::PhysXControllerHandler(boost::shared_ptr<PhysXEngine> Pengine,
												boost::shared_ptr<ActorUserData> UserData,
												NxController* Controller,
												boost::shared_ptr<SimpleRotationHandler> rotH)
		: PhysXObjectHandlerBase(Pengine, UserData), _Controller(Controller)
{
	_UserData->RotH = rotH;

	#ifdef _DEBUG
		LogHandler::getInstance()->LogToFile("Created new PhysXController.");
	#endif
}



/***********************************************************
	destructor
***********************************************************/
PhysXControllerHandler::~PhysXControllerHandler()
{
	#ifdef _DEBUG
		LogHandler::getInstance()->LogToFile("Destroyed PhysXController.");
	#endif

	_Pengine->DestroyCharacter(_Controller);
}


/***********************************************************
get object position in the world
***********************************************************/
void PhysXControllerHandler::GetPosition(float &X, float &Y, float &Z)
{	
	if(!_Controller) return;

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
	_UserData->RotH->GetRotation(Q);
}

/***********************************************************
get object rotation on all axis
***********************************************************/
float PhysXControllerHandler::GetRotationSingleAngle()
{
	return _UserData->RotH->GetRotationSingleAngle();
}

/***********************************************************
set object position in the world
***********************************************************/
void PhysXControllerHandler::SetPosition(unsigned int time, float X, float Y, float Z)
{	
	if(!_Controller) return;

	NxExtendedVec3 pos;
	pos.x = X;
	pos.y = Y;
	pos.z = Z;
	_Pengine->SetCharacterPosition(time, _Controller, pos);
	_resetted = true;
}

/***********************************************************
set object rotation on all axis
***********************************************************/
void PhysXControllerHandler::SetRotation(unsigned int time, const LbaQuaternion& Q)
{
	_Pengine->SetCharacterRotation(time, _Controller, Q);
	_resetted = true;
}


/***********************************************************
rotate object in the world
***********************************************************/
void PhysXControllerHandler::RotateYAxis(unsigned int time, float Speed)
{	
	_Pengine->RotateCharacteryAxis(time, _Controller, Speed);
}

/***********************************************************
move object in the world
***********************************************************/
void PhysXControllerHandler::Move(unsigned int time, float deltaX, float deltaY, float deltaZ)
{
	_Pengine->DeltaMoveCharacter(time, _Controller, NxVec3(deltaX, deltaY, deltaZ), true);
}

/***********************************************************
move object to a position in the world
***********************************************************/
void PhysXControllerHandler::MoveTo(unsigned int time, float X, float Y, float Z)
{
	if(!_Controller) 
		return;

	NxExtendedVec3 vec = _Controller->getPosition();
	_Pengine->DeltaMoveCharacter(time, _Controller, 
									NxVec3(X - (float)vec.x, Y - (float)vec.y, Z - (float)vec.z), false);
}

/***********************************************************
rotate object in the world
***********************************************************/
void PhysXControllerHandler::RotateTo(unsigned int time, const LbaQuaternion& Q)
{
	_Pengine->SetCharacterRotation(time, _Controller, Q);
}


/***********************************************************
move object in the world
***********************************************************/
void PhysXControllerHandler::MoveInDirection(unsigned int time, float MoveSpeed, bool AddGravity)
{
	_Pengine->MoveInDirectionCharacter(time, _Controller, MoveSpeed, AddGravity);
}









/***********************************************************
	Constructor
***********************************************************/
PhysicalDescriptionBox::PhysicalDescriptionBox(float posX, float posY, float posZ,
												int Otype, float Odensity,
												const LbaQuaternion &rot,
												float sX, float sY, float sZ)
	:PhysicalDescriptionWithShape(posX, posY, posZ, Otype, Odensity, rot),
			sizeX(sX), sizeY(sY), sizeZ(sZ)
{

}

/***********************************************************
	destructor
***********************************************************/
PhysicalDescriptionBox::~PhysicalDescriptionBox()
{

}


/***********************************************************
 build description into a reald physic object
***********************************************************/
boost::shared_ptr<PhysicalObjectHandlerBase> PhysicalDescriptionBox::BuildSelf(
												boost::shared_ptr<PhysXEngine> _PEngine) const
{
	boost::shared_ptr<ActorUserData> udata = boost::shared_ptr<ActorUserData>(new ActorUserData());

	if(type != 4)
	{
		NxActor* act = _PEngine->CreateBox(NxVec3(positionX, positionY, positionZ), sizeX, sizeY, sizeZ, 
													density, type, udata.get());
		
		return boost::shared_ptr<PhysicalObjectHandlerBase>(new PhysXActorsHandler(_PEngine, udata, act, rotation));
	}
	else
	{
		NxController* controller = _PEngine->CreateCharacterBox(NxVec3(positionX, positionY, positionZ), 
															NxVec3(sizeX, sizeY, sizeZ), udata.get());

		return boost::shared_ptr<PhysicalObjectHandlerBase>(new PhysXControllerHandler(_PEngine, udata, controller, 
								boost::shared_ptr<SimpleRotationHandler>(new SimpleRotationHandler(rotation))));
	}
}

/***********************************************************
	Constructor
***********************************************************/
PhysicalDescriptionCapsule::PhysicalDescriptionCapsule(float posX, float posY, float posZ,
														int Otype, float Odensity,
														const LbaQuaternion &rot,
														float rad, float ht)
	:PhysicalDescriptionWithShape(posX, posY, posZ, Otype, Odensity, rot),
		radius(rad), height(ht)
{

}

/***********************************************************
	destructor
***********************************************************/
PhysicalDescriptionCapsule::~PhysicalDescriptionCapsule()
{

}


/***********************************************************
 build description into a reald physic object
***********************************************************/
boost::shared_ptr<PhysicalObjectHandlerBase> PhysicalDescriptionCapsule::BuildSelf(
												boost::shared_ptr<PhysXEngine> _PEngine) const
{
	boost::shared_ptr<ActorUserData> udata = boost::shared_ptr<ActorUserData>(new ActorUserData());

	if(type != 4)
	{
		NxActor* act = _PEngine->CreateCapsule(NxVec3(positionX, positionY, positionZ), radius, height,
													density, type, udata.get());
		
		return boost::shared_ptr<PhysicalObjectHandlerBase>(new PhysXActorsHandler(_PEngine, udata, act, rotation));
	}
	else
	{
		NxController* controller = _PEngine->CreateCharacter(NxVec3(positionX, positionY, positionZ), 
															radius, height, udata.get());

		return boost::shared_ptr<PhysicalObjectHandlerBase>(new PhysXControllerHandler(_PEngine, udata, controller, 
								boost::shared_ptr<SimpleRotationHandler>(new SimpleRotationHandler(rotation))));
	}
}

/***********************************************************
	Constructor
***********************************************************/
PhysicalDescriptionSphere::PhysicalDescriptionSphere(float posX, float posY, float posZ,
														int Otype, float Odensity,
														const LbaQuaternion &rot,
														float rad)
	:PhysicalDescriptionWithShape(posX, posY, posZ, Otype, Odensity, rot),
		radius(rad)
{

}

/***********************************************************
	destructor
***********************************************************/
PhysicalDescriptionSphere::~PhysicalDescriptionSphere()
{

}


/***********************************************************
 build description into a reald physic object
***********************************************************/
boost::shared_ptr<PhysicalObjectHandlerBase> PhysicalDescriptionSphere::BuildSelf(
												boost::shared_ptr<PhysXEngine> _PEngine) const
{
	boost::shared_ptr<ActorUserData> udata = boost::shared_ptr<ActorUserData>(new ActorUserData());

	if(type != 4)
	{
		NxActor* act = _PEngine->CreateSphere(NxVec3(positionX, positionY, positionZ), radius, 
													density, type, udata.get());

		return boost::shared_ptr<PhysicalObjectHandlerBase>(new PhysXActorsHandler(_PEngine, udata, act, rotation));
	}
	else
	{
		NxController* controller = _PEngine->CreateCharacter(NxVec3(positionX, positionY, positionZ), 
															radius, 0, udata.get());

		return boost::shared_ptr<PhysicalObjectHandlerBase>(new PhysXControllerHandler(_PEngine, udata, controller, 
								boost::shared_ptr<SimpleRotationHandler>(new SimpleRotationHandler(rotation))));
	}
}

/***********************************************************
	Constructor
***********************************************************/
PhysicalDescriptionTriangleMesh::PhysicalDescriptionTriangleMesh(float posX, float posY, float posZ,
																	const std::string &FileName)
	:PhysicalDescriptionWithShape(posX, posY, posZ, 1, 0, LbaQuaternion()),
		MeshInfoDataFileName(FileName)
{

}

/***********************************************************
	destructor
***********************************************************/
PhysicalDescriptionTriangleMesh::~PhysicalDescriptionTriangleMesh()
{

}



/***********************************************************
 build description into a reald physic object
***********************************************************/
boost::shared_ptr<PhysicalObjectHandlerBase> PhysicalDescriptionTriangleMesh::BuildSelf(
												boost::shared_ptr<PhysXEngine> _PEngine) const
{


	boost::shared_ptr<ActorUserData> udata = boost::shared_ptr<ActorUserData>(new ActorUserData());

	NxActor* actor = _PEngine->LoadTriangleMeshFile(NxVec3(positionX, positionY, positionZ), 
														"Data/"+MeshInfoDataFileName, udata);

	return boost::shared_ptr<PhysicalObjectHandlerBase>(new PhysXActorsHandler(_PEngine, udata, actor, rotation));
}
