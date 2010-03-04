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

#if !defined(__LbaNetModel_1_PhysXEngine_h)
#define __LbaNetModel_1_PhysXEngine_h

class NxPhysicsSDK;
class NxScene;
class NxActor;
class NxVec3;

/***********************************************************************
 * Module:  PhysXEngine.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:59:34
 * Purpose: Declaration of the class PhysXEngine
 ***********************************************************************/
class PhysXEngine
{
public:

	// singleton pattern
	static PhysXEngine * getInstance();

	//! destructor
	~PhysXEngine();


	//! init function
	void Init();

	//! quit function
	void Quit();

	//! call to start physic calculation after having updated state from inputs
	void StartPhysics();

	//! call after render to get results of calculation
	void GetPhysicsResults();

	//! create actors
	NxActor* CreatePlane(const NxVec3 & StartPosition, const NxVec3 & PlaneNormal);
	NxActor* CreateBox(const NxVec3 & StartPosition, float dimX, float dimY, float dimZ, float density);
	NxActor* CreateSphere(const NxVec3 & StartPosition, float radius, float density);
	NxActor* CreateCapsule(const NxVec3 & StartPosition, float radius, float height, float density);


	//! render actors
	void RenderActors();


protected:
	//! constructor
	PhysXEngine();


private:
	// singleton
	static PhysXEngine * _singletonInstance;

	// physX internal data
	NxPhysicsSDK*     gPhysicsSDK;
	NxScene*          gScene;

	double			_lasttime;
};

#endif