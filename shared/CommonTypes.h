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
	bool UseShadow;
	bool StartOn;


	float LOnPosX;
	float LOnPosY;
	float LOnPosZ;

	float LOnAmbientR;
	float LOnAmbientG;
	float LOnAmbientB;

	float LOnDiffuseR;
	float LOnDiffuseG;
	float LOnDiffuseB;


	float LOffPosX;
	float LOffPosY;
	float LOffPosZ;

	float LOffAmbientR;
	float LOffAmbientG;
	float LOffAmbientB;

	float LOffDiffuseR;
	float LOffDiffuseG;
	float LOffDiffuseB;
};



// contain world description
struct WorldDesc
{
	std::string WorldName;
	std::string Description;
	std::string FileName;
};


// contain information about a teleport place
struct TPInfo
{
	// name
	std::string		Name;

	// arrival point
	std::string		NewMap;
	std::string		Spawning;
};



//struct ActorInfo
//{
//	long Time;
//	long ActorId;
//
//	float PosX;
//	float PosY;
//	float PosZ;
//
//	float Rotation;
//
//	int Model;
//	int Body;
//	int Animation;
//};

#endif
