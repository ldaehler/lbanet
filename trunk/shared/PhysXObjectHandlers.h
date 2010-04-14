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


#ifndef __LBA_NET_PHYSX_OBJECTS_HANDLERS_H__
#define __LBA_NET_PHYSX_OBJECTS_HANDLERS_H__

#include <boost/shared_ptr.hpp>
#include "PhysicalObjectHandlerBase.h"


class PhysXEngine;
class ActorUserData;
class NxActor;
class NxController;

/***********************************************************************
 * Handler on a physX object
 ***********************************************************************/
class PhysXObjectHandlerBase : public PhysicalObjectHandlerBase
{
public:
	//! constructor
	PhysXObjectHandlerBase(boost::shared_ptr<PhysXEngine> Pengine,
								boost::shared_ptr<ActorUserData> UserData);

	//! destructor
	virtual ~PhysXObjectHandlerBase(){}

protected:
	boost::shared_ptr<PhysXEngine>		_Pengine;
	boost::shared_ptr<ActorUserData>	_UserData;
};



/***********************************************************************
 * Handler on a physX dynamic actor - this actor controll itself its rotation
 ***********************************************************************/
class PhysXActorsHandler : public PhysXObjectHandlerBase
{
public:
	//! constructor
	PhysXActorsHandler(boost::shared_ptr<PhysXEngine> Pengine,
						boost::shared_ptr<ActorUserData> UserData,
						NxActor* Actor, const LbaQuaternion& rotation);

	//! destructor
	virtual ~PhysXActorsHandler();

	//! get object position in the world
	virtual void GetPosition(float &X, float &Y, float &Z);

	//! get object rotation on all axis
	virtual void GetRotation(LbaQuaternion& Q);

	//! get object rotation on a single angle
	virtual float GetRotationSingleAngle();

	//! set object position in the world
	virtual void SetPosition(unsigned int time, float X, float Y, float Z);

	//! set object rotation on all axis
	virtual void SetRotation(unsigned int time, const LbaQuaternion& Q);

	//! move object in the world
	virtual void Move(unsigned int time, float deltaX, float deltaY, float deltaZ);

	//! move object to a position in the world
	virtual void MoveTo(unsigned int time, float X, float Y, float Z);

	//! rotate object in the world
	virtual void RotateYAxis(unsigned int time, float Speed);

	//! rotate object in the world
	virtual void RotateTo(unsigned int time, const LbaQuaternion& Q);

	//! move object in the world
	virtual void MoveInDirection(unsigned int time, float MoveSpeed, bool AddGravity);

protected:
	NxActor*	_Actor;
};



/***********************************************************************
 * Handler on a physX controller
 ***********************************************************************/
class PhysXControllerHandler : public PhysXObjectHandlerBase
{
public:
	//! constructor
	PhysXControllerHandler(boost::shared_ptr<PhysXEngine> Pengine,
								boost::shared_ptr<ActorUserData> UserData,
								NxController* Controller,
								boost::shared_ptr<SimpleRotationHandler> rotH);

	//! destructor
	virtual ~PhysXControllerHandler();

	//! get object position in the world
	virtual void GetPosition(float &X, float &Y, float &Z);

	//! get object rotation on all axis
	virtual void GetRotation(LbaQuaternion& Q);

	//! get object rotation on a single angle
	virtual float GetRotationSingleAngle();

	//! set object position in the world
	virtual void SetPosition(unsigned int time, float X, float Y, float Z);

	//! set object rotation on all axis
	virtual void SetRotation(unsigned int time, const LbaQuaternion& Q);

	//! move object in the world
	virtual void Move(unsigned int time, float deltaX, float deltaY, float deltaZ);

	//! move object to a position in the world
	virtual void MoveTo(unsigned int time, float X, float Y, float Z);

	//! rotate object in the world
	virtual void RotateYAxis(unsigned int time, float Speed);

	//! rotate object in the world
	virtual void RotateTo(unsigned int time, const LbaQuaternion& Q);

	//! move object in the world
	virtual void MoveInDirection(unsigned int time, float MoveSpeed, bool AddGravity);

protected:
	NxController*								_Controller;
};

#endif