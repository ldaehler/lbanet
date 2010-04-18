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
class NxTriangleMeshShape;
class NxQuat;
class PhysicalModification;
class PhysicalState;
struct NxExtendedVec3;
class SavedState;
class SimpleRotationHandler;
class PlayerCallbackBase;

#include <set>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "CommonTypes.h"


/***********************************************************************
 * Module:  PhysXEngine.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:59:34
 * Purpose: Declaration of the class ActorUserData
 ***********************************************************************/
class ActorUserData
{
public:
	//! constructor
	ActorUserData()
	: Materials(NULL), MaterialsSize(0), HittedFloorMaterial(0),
		CollisionUpFlag(false), CollisionDownFlag(false), CollisionSideFlag(false),
		InternalActor(NULL)
	{}

	//! destructor
	~ActorUserData()
	{
		if(Materials)
			delete Materials;
	}

	size_t				MaterialsSize;
	short *				Materials;
	NxActor *			InternalActor;
	PlayerCallbackBase *Callback;	


	short				HittedFloorMaterial;
	bool				CollisionUpFlag;
	bool				CollisionDownFlag;
	bool				CollisionSideFlag;

	boost::shared_ptr<SimpleRotationHandler>	RotH;
};



/***********************************************************************
 Comparaison functor
 ***********************************************************************/
struct classcomp 
{
	bool operator() (const boost::shared_ptr<PhysicalModification> &lhs, 
						const boost::shared_ptr<PhysicalModification> &rhs) const;
};

/***********************************************************************
 Comparaison functor
 ***********************************************************************/
struct statecomp 
{
	bool operator() (const boost::shared_ptr<PhysicalState> &lhs, 
						const boost::shared_ptr<PhysicalState> &rhs) const;
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
	//! return true if we have done one cycle
	bool GetPhysicsResults();

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

	NxActor* CreateTriangleMesh(const NxVec3 & StartPosition, float *Vertexes, 
										size_t VertexesSize, unsigned int *Indices, size_t IndicesSize,
										ActorUserData * adata, bool collidablemesh = true);

	NxController* CreateCharacter(const NxVec3 & StartPosition, float radius, float height,
									ActorUserData * adata);

	NxController* CreateCharacterBox(const NxVec3 & StartPosition, const NxVec3 & Extents,
									ActorUserData * adata);

	//! destroy actors
	void DestroyActor(NxActor* actor);
	void DestroyCharacter(NxController* character);


	//! move character
	//! returned collision flags, collection of NxControllerFlag
	static unsigned int MoveCharacter(NxController* character, const NxVec3& moveVector, bool checkcollision=true);


	//! get gravity
	void GetGravity(LbaVec3 & Gravity);


	//! Load triangle mesh shape to the engine from file
	NxActor* LoadTriangleMeshFile(const NxVec3 & StartPosition, const std::string Filename,
									boost::shared_ptr<ActorUserData> userdata);



	//! check if there is a roof up the 3d position in parameter
	//! if there is a roof, return the roof position along the y axis
	//! else return -1
	float CheckForRoof(float PositionX, float PositionY, float PositionZ);



	//! move an actor
	void MoveActorTo(unsigned int time, NxActor* act, const NxVec3 & targetPos, bool DirectApply);

	//! set an actor position
	void SetActorPos(unsigned int time, NxActor* act, const NxVec3 & targetPos);

	//! set an actor rotation
	void RotateActoryAxis(unsigned int time, NxActor* act, float Speed, bool DirectApply);

	//! set an actor rotation
	void SetActorRotation(unsigned int time, NxActor* act, const NxQuat & quat);

	//! move actor in rotation direction - only work for kinematic actors
	void MoveInDirectionActor(unsigned int time, NxActor* act, float MoveSpeed, bool AddGravity, bool DirectApply);

	//! inform engine an action has tkane place back in time 
	void RevertBack(unsigned int time);



	//! move character - return collision flag
	//! TODO check if step on water
	void DeltaMoveCharacter(unsigned int time, NxController* act, const NxVec3 & deltamove, 
								bool checkCollision, bool DirectApply);

	//! set character position - no collision checked
	void SetCharacterPosition(unsigned int time, NxController* act, const NxExtendedVec3 & targetPos);

	//! set an actor rotation
	void RotateCharacteryAxis(unsigned int time, NxController* act, float Speed, bool DirectApply);

	//! set an actor rotation
	void SetCharacterRotation(unsigned int time, NxController* act, const LbaQuaternion& Q);

	//! move actor in rotation direction
	void MoveInDirectionCharacter(unsigned int time, NxController* act, float MoveSpeed, 
									bool AddGravity, bool DirectApply);


	//! apply historic modifications
	void ApplyHistoricModifications();

protected:
	
	//! init function
	void Init();

	//! quit function
	void Quit();


	//! remove actors from history
	void RemoveActorFromHistory(NxActor* act);
	void RemoveCharFromHistory(NxController* act);

	//! add physicall modification
	void AddModification(boost::shared_ptr<PhysicalModification> mod);

	//! save current physical state
	SavedState * SaveCurrentState();

	//! load saved physical state
	void LoadState(SavedState * state);


private:

	// physX internal data
	NxPhysicsSDK*				gPhysicsSDK;
	NxScene*					gScene;
	NxControllerManager*		gManager;
	

	std::set<NxActor*>			_roofactors;

	std::multiset<boost::shared_ptr<PhysicalModification>, classcomp>	_curr_modifications;
	std::set<boost::shared_ptr<PhysicalState>, statecomp>				_savedstates;

	double						_lasttime;
	float						_lastduration;
	float						_currentsimduration;


	const float					_simtime;
};

#endif