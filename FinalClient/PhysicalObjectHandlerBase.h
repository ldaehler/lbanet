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


#ifndef _LBANET_PHYSICAL_OBJECT_HANDLER_BASE_H_
#define _LBANET_PHYSICAL_OBJECT_HANDLER_BASE_H_

#include "CommonTypes.h"



//*************************************************************************************************
//*                               class PhysicalObjectHandlerBase
//*************************************************************************************************
/**
* @brief Pure virtual class to get access to a physical object current transformation
* (e.g. translation, rotation, etc.)
*
*/
class PhysicalObjectHandlerBase
{
public:

	//! constructor
	PhysicalObjectHandlerBase()
		: _resetted(false)
	{}

	//! destructor
	virtual ~PhysicalObjectHandlerBase(){}

	//! get object position in the world
	virtual void GetPosition(float &X, float &Y, float &Z) = 0;

	//! get object rotation on all axis
	virtual void GetRotation(LbaQuaternion &Q) = 0;

	//! set object position in the world
	virtual void SetPosition(float X, float Y, float Z) = 0;

	//! set object rotation on all axis
	virtual void SetRotation(const LbaQuaternion& Q) = 0;

	//! move object in the world
	virtual void Move(float deltaX, float deltaY, float deltaZ) = 0;

	//! move object to a position in the world
	virtual void MoveTo(float X, float Y, float Z) = 0;

	//! rotate object in the world
	virtual void RotateTo(const LbaQuaternion& Q) = 0;

	//! call to see if the object was resetted in the physical world
	//! (e.g. object has been teleported) in this case the synchronization process
	//! would directly set display object to the new position without smoothing
	bool WasReseted()
	{
		bool res = _resetted;
		_resetted = false;
		return res;
	}

	//! destroy function - clear the object content
	virtual void Destroy(void) = 0;

protected:
	bool _resetted;
};


//*************************************************************************************************
//*                               class SimpleRotationHandler
//*************************************************************************************************
/**
* @brief Handle rotation directly if they do not have any influence on the physical shape
*
*/
class SimpleRotationHandler
{
public:

	//! constructor
	SimpleRotationHandler(const LbaQuaternion& Q)
		: _Q(Q)
	{}

	//! destructor
	~SimpleRotationHandler(){}

	//! get object rotation on all axis
	void GetRotation(LbaQuaternion& Q)
	{
		Q = _Q;
	}

	//! set object rotation on all axis
	void SetRotation(const LbaQuaternion& Q)
	{
		_Q = Q;
	}

	//! rotate object in the world
	void RotateTo(const LbaQuaternion& Q)
	{
		_Q = Q;
	}

private:
	LbaQuaternion _Q;
};



//*************************************************************************************************
//*                               class SimplePhysicalObjectHandler
//*************************************************************************************************
/**
* @brief Handle position and rotation directly with no physical shape
*
*/
class SimplePhysicalObjectHandler : public PhysicalObjectHandlerBase
{
public:

	//! constructor
	SimplePhysicalObjectHandler(float pX, float pY, float pZ,
									const LbaQuaternion& Q)
		: _PosX(pX), _PosY(pY), _PosZ(pZ), _rotH(Q)
	{}

	//! destructor
	virtual ~SimplePhysicalObjectHandler(){}



	//! get object position in the world
	virtual void GetPosition(float &X, float &Y, float &Z)
	{
		X = _PosX;
		Y = _PosY;
		Z = _PosZ;
	}


	//! set object position in the world
	virtual void SetPosition(float X, float Y, float Z)
	{
		_PosX = X;
		_PosY = Y;
		_PosZ = Z;
		_resetted = true;
	}


	//! move object in the world
	virtual void Move(float deltaX, float deltaY, float deltaZ)
	{
		_PosX += deltaX;
		_PosY += deltaY;
		_PosZ += deltaZ;
	}


	//! get object rotation on all axis
	virtual void GetRotation(LbaQuaternion& Q)
	{
		_rotH.GetRotation(Q);
	}

	//! set object rotation on all axis
	virtual void SetRotation(const LbaQuaternion& Q)
	{
		_rotH.SetRotation(Q);
		_resetted = true;
	}

	//! move object to a position in the world
	virtual void MoveTo(float X, float Y, float Z)
	{
		_PosX = X;
		_PosY = Y;
		_PosZ = Z;
	}


	//! rotate object in the world
	virtual void RotateTo(const LbaQuaternion& Q)
	{
		_rotH.RotateTo(Q);
	}


	//! destroy function - clear the object content
	virtual void Destroy(void){}

private:
	float _PosX;
	float _PosY;
	float _PosZ;

	SimpleRotationHandler _rotH;
};

#endif