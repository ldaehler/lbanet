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


#ifndef __LBA_NET_COMMON_TYPES_H__
#define __LBA_NET_COMMON_TYPES_H__

#include <string>


//simulation update rates
#define SIM_UPDATE_RATE 50                              // hz
#define SIMULATION_TIME_PER_UPDATE (1000/SIM_UPDATE_RATE) // msecs per update
#define MAX_HISTORY_TIME	1000

class LbaVec3
{
public:
	//! constructor
	LbaVec3()
		: x(0), y(0), z(0)
	{}

	//! constructor
	LbaVec3(float X, float Y, float Z)
		: x(X), y(Y), z(Z)
	{}

	float x;
	float y;
	float z;
};

/***********************************************************************
Internal quaternion class
 ***********************************************************************/
class LbaQuaternion
{
public:
	//!constructor
	LbaQuaternion();

	//!constructor
	LbaQuaternion(float x, float y, float z, float w)
		: X(x), Y(y), Z(z), W(w){}


	//!constructor with angle in degree and axis vec
	LbaQuaternion(float angle, LbaVec3 vec);


	//! add rotation to quaternion
	void AddRotation(float angle, LbaVec3 vec);

	//! get direction vector
	LbaVec3 GetDirection(const LbaVec3 &vec);

	//! get object rotation on a single angle
	float GetRotationSingleAngle();

	float X;
	float Y;
	float Z;
	float W;
};


/***********************************************************************
This class describe main light information for the scene
 ***********************************************************************/
class LbaMainLightInfo
{
public:

	//! constructor
	LbaMainLightInfo()
		: UseLight(false)
	{}


	//! members
	bool UseLight;

	float LOnPosX;
	float LOnPosY;
	float LOnPosZ;

	float LOnAmbientR;
	float LOnAmbientG;
	float LOnAmbientB;

	float LOnDiffuseR;
	float LOnDiffuseG;
	float LOnDiffuseB;
};




/***********************************************************************
the input class
 ***********************************************************************/
struct Input
{
	//! constructor
	Input() 
	{ 
		up = down = left = right = false; 
	}

	// comparaison
	bool operator==(const Input& _in) 
	{
		return (up == _in.up && down == _in.down && left == _in.left && right == _in.right);
	}

	// assignment
	Input& operator=(const Input& _rhs) 
	{
		up = _rhs.up; down = _rhs.down; left = _rhs.left; right = _rhs.right;
		return *this;
	}

	// members
	bool up;
	bool down;
	bool left;
	bool right;
};


#endif
