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
	virtual void GetRotation(float &X, float &Y, float &Z) = 0;

	//! set object position in the world
	virtual void SetPosition(float X, float Y, float Z) = 0;

	//! set object rotation on all axis
	virtual void SetRotation(float X, float Y, float Z) = 0;

	//! move object in the world
	virtual void Move(float deltaX, float deltaY, float deltaZ) = 0;

	//! rotate object in the world
	virtual void Rotate(float deltaX, float deltaY, float deltaZ) = 0;

	//! move object to a position in the world
	virtual void MoveTo(float X, float Y, float Z);

	//! rotate object in the world
	virtual void RotateTo(float X, float Y, float Z) = 0;

	//! call to see if the object was resetted in the physical world
	//! (e.g. object has been teleported) in this case the synchronization process
	//! would directly set display object to the new position without smoothing
	bool WasReseted()
	{
		bool res = _resetted;
		_resetted = false;
		return res;
	}

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
	SimpleRotationHandler(float X, float Y, float Z)
		: _RotX(X), _RotY(Y), _RotZ(Z)
	{}

	//! destructor
	~SimpleRotationHandler(){}

	//! get object rotation on all axis
	void GetRotation(float &X, float &Y, float &Z)
	{
		X = _RotX;
		Y = _RotY;
		Z = _RotZ;
	}

	//! set object rotation on all axis
	void SetRotation(float X, float Y, float Z)
	{
		_RotX = X;
		_RotY = Y;
		_RotZ = Z;
	}


	//! move object in the world
	void Rotate(float deltaX, float deltaY, float deltaZ)
	{
		_RotX += deltaX;
		_RotY += deltaY;
		_RotZ += deltaZ;
	}

	//! rotate object in the world
	void RotateTo(float X, float Y, float Z)
	{
		_RotX = X;
		_RotY = Y;
		_RotZ = Z;
	}

private:
	float _RotX;
	float _RotY;
	float _RotZ;
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
									float rX, float rY, float rZ)
		: _PosX(pX), _PosY(pY), _PosZ(pZ),
			_RotX(rX), _RotY(rY), _RotZ(rZ)
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
	virtual void GetRotation(float &X, float &Y, float &Z)
	{
		X = _RotX;
		Y = _RotY;
		Z = _RotZ;
	}

	//! set object rotation on all axis
	virtual void SetRotation(float X, float Y, float Z)
	{
		_RotX = X;
		_RotY = Y;
		_RotZ = Z;
		_resetted = true;
	}


	//! move object in the world
	virtual void Rotate(float deltaX, float deltaY, float deltaZ)
	{
		_RotX += deltaX;
		_RotY += deltaY;
		_RotZ += deltaZ;
	}

	//! move object to a position in the world
	virtual void MoveTo(float X, float Y, float Z)
	{
		_PosX = X;
		_PosY = Y;
		_PosZ = Z;
	}


	//! rotate object in the world
	virtual void RotateTo(float X, float Y, float Z)
	{
		_RotX = X;
		_RotY = Y;
		_RotZ = Z;
	}


private:
	float _PosX;
	float _PosY;
	float _PosZ;

	float _RotX;
	float _RotY;
	float _RotZ;
};

#endif