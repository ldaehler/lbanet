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
class NxControllerManager;
class NxUserAllocator;
class NxVec3;
class NxController;
class NxActor;

#include <vector>
#include <boost/shared_ptr.hpp>


class ActorUserData
{
public:
	//! constructor
	ActorUserData(short ActType)
	: ActorType(ActType), Materials(NULL), 
		MaterialsSize(0), HittedFloorMaterial(0)
	{}

	//! destructor
	~ActorUserData()
	{
		if(Materials)
			delete Materials;
	}


	// Actor type
	// 1 = character
	// 2 = terrain
	// 3 = other
	short				ActorType;

	size_t				MaterialsSize;
	short *				Materials; 

	short				HittedFloorMaterial;
};




/***********************************************************************
 * Module:  PhysXEngine.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:59:34
 * Purpose: Declaration of the class PhysXEngine
 ***********************************************************************/
class PhysXEngine
{
public:

	//! constructor
	PhysXEngine();

	//! destructor
	~PhysXEngine();

	//! call to start physic calculation after having updated state from inputs
	void StartPhysics();

	//! call after render to get results of calculation
	void GetPhysicsResults();

	//! clear the scene of all actors
	void Clear();

	//! create actors
	// type: 1=static, 2=kinematic, 3=dynamic
	NxActor* CreateBox(const NxVec3 & StartPosition, float dimX, float dimY, float dimZ, 
						float density, int Type, ActorUserData * adata);
	NxActor* CreateSphere(const NxVec3 & StartPosition, float radius, float density, 
							int Type, ActorUserData * adata);
	NxActor* CreateCapsule(const NxVec3 & StartPosition, float radius, float height, 
							float density, int Type, ActorUserData * adata);

	NxController* CreateCharacter(const NxVec3 & StartPosition, float radius, float height,
									ActorUserData * adata);

	NxController* CreateCharacterBox(const NxVec3 & StartPosition, const NxVec3 & Extents,
									ActorUserData * adata);

	NxActor* CreateTriangleMesh(const NxVec3 & StartPosition, float *Vertexes, 
										size_t VertexesSize, unsigned int *Indices, size_t IndicesSize,
										ActorUserData * adata);

	//! destroy actors
	void DestroyActor(NxActor* actor);
	void DestroyCharacter(NxController* character);


	//! move character
	//! returned collision flags, collection of NxControllerFlag
	unsigned int MoveCharacter(NxController* character, const NxVec3& moveVector, bool checkcollision=true);
		
	//! set character position
	void SetCharacterPos(NxController* character, const NxVec3& posVector);

	//! get character position
	void GetCharacterPosition(NxController* character, float &posX, float &posY, float &posZ);

	//! get gravity
	void GetGravity(NxVec3 & Gravity);

	//! Load map physical shape to the engine
	//! important: can only load one map at a time
	void LoadMap(const std::string & filename);

protected:
	
	//! init function
	void Init();

	//! quit function
	void Quit();


private:

	// physX internal data
	NxPhysicsSDK*				gPhysicsSDK;
	NxScene*					gScene;
	NxControllerManager*		gManager;
	NxUserAllocator*			gAllocator;

	boost::shared_ptr<ActorUserData> _currmap_userdata;

	double						_lasttime;
};

#endif