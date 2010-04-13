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
#include "PhysXEngine.h"
#include "UserAllocatorHandler.h"
#include "SynchronizedTimeHandler.h"
#include "NxPhysics.h"
#include "NxControllerManager.h"
#include "NxCapsuleController.h"
#include "NxBoxController.h"
#include "UserAllocator.h"
#include "NxCooking.h"
#include "Stream.h"
#include "LogHandler.h"
#include "CommonTypes.h"
#include "PhysicalModification.h"

#include <limits>

#include <fstream>

#define SKINWIDTH	0.2f
#define GRAVITY		-9.8f
#define MAX_HISTORY_TIME	1000

enum GameGroup
{
	GROUP_NON_COLLIDABLE,
	GROUP_COLLIDABLE_NON_PUSHABLE,
	GROUP_COLLIDABLE_PUSHABLE,
};

#define COLLIDABLE_MASK	(1<<GROUP_COLLIDABLE_NON_PUSHABLE) | (1<<GROUP_COLLIDABLE_PUSHABLE)



/***********************************************************************
* comparison functor
***********************************************************************/
bool classcomp::operator() (const boost::shared_ptr<PhysicalModification> &lhs, 
								const boost::shared_ptr<PhysicalModification> &rhs) const
{
	return (*lhs<*rhs);
}

/***********************************************************************
* comparison functor
***********************************************************************/
bool statecomp::operator() (const boost::shared_ptr<PhysicalState> &lhs, 
								const boost::shared_ptr<PhysicalState> &rhs) const
{
	return (*lhs<*rhs);
}


/***********************************************************************
 * Declaration of the class ControllerHitReport
 ***********************************************************************/
class ControllerHitReport : public NxUserControllerHitReport
{
public:
	virtual NxControllerAction  onShapeHit(const NxControllerShapeHit& hit)
	{
		if(1 && hit.shape)
		{
			// i need to know which triangle of the mesh the controller has hit
			// i did not find any other way yet...
			NxTriangleMeshShape* tmesh = hit.shape->isTriangleMesh();
			if(tmesh)
			{
				NxActor& actor = hit.shape->getActor();
				if(actor.userData)
				{
					ActorUserData * mstorage = (ActorUserData *)actor.userData;
					if(mstorage)
					{
						NxRaycastHit hitinfo;
						NxVec3 pos((float)hit.worldPos.x,(float)hit.worldPos.y+0.01f,(float)hit.worldPos.z);
						NxVec3 vec(0, -1,0);

						if(tmesh->raycast(NxRay(pos, vec), 0.02f, 
													NX_RAYCAST_FACE_INDEX, hitinfo, false))
						{
							if(hitinfo.faceID < mstorage->MaterialsSize)
							{
								ActorUserData * characterdata = (ActorUserData *)hit.controller->getActor()->userData;
								if(characterdata)
									characterdata->HittedFloorMaterial = mstorage->Materials[hitinfo.faceID];
							}
						}
					}
				}
			}


			NxCollisionGroup group = hit.shape->getGroup();
			if(group!=GROUP_COLLIDABLE_NON_PUSHABLE)
			{
				NxActor& actor = hit.shape->getActor();
				if(actor.isDynamic())
				{
					// We only allow horizontal pushes. Vertical pushes when we stand on dynamic objects creates
					// useless stress on the solver. It would be possible to enable/disable vertical pushes on
					// particular objects, if the gameplay requires it.
					if(hit.dir.y==0.0f)
					{
						NxF32 coeff = actor.getMass() * hit.length * 2.0f;
						actor.addForceAtLocalPos(hit.dir*coeff, NxVec3(0,0,0), NX_IMPULSE);
					}
				}
			}
		}

		return NX_ACTION_NONE;
	}

	virtual NxControllerAction  onControllerHit(const NxControllersHit& hit)
	{
		return NX_ACTION_NONE;
	}

} gControllerHitReport;






/***********************************************************
	Constructor
***********************************************************/
PhysXEngine::PhysXEngine()
: gPhysicsSDK(NULL), gScene(NULL), _lastduration(0),
	_simtime(1.0f/SIM_UPDATE_RATE)
{
	Init();
}


/***********************************************************
	Destructor
***********************************************************/
PhysXEngine::~PhysXEngine()
{
	Quit();
}



/***********************************************************
	init function
***********************************************************/
void PhysXEngine::Init()
{
	// Create the physics SDK
    gPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, UserAllocatorHandler::getInstance()->GetAllocator());
    if (!gPhysicsSDK)  
		return;

	if (gPhysicsSDK->getFoundationSDK().getRemoteDebugger())
		gPhysicsSDK->getFoundationSDK().getRemoteDebugger()->connect("localhost", 5425);


	// Set the physics parameters
	gPhysicsSDK->setParameter(NX_SKIN_WIDTH, 0.01f);

	// Set the debug visualization parameters
	gPhysicsSDK->setParameter(NX_VISUALIZATION_SCALE, 1);
	gPhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1);
	gPhysicsSDK->setParameter(NX_VISUALIZE_ACTOR_AXES, 1);

    // Create the scene
    NxSceneDesc sceneDesc;
 	sceneDesc.simType				= NX_SIMULATION_SW;
    sceneDesc.gravity               = NxVec3(0,GRAVITY,0);
    gScene = gPhysicsSDK->createScene(sceneDesc);	
	if(!gScene)
	{ 
		sceneDesc.simType			= NX_SIMULATION_SW; 
		gScene = gPhysicsSDK->createScene(sceneDesc);  
		if(!gScene) 
			return;
	}


	// Create the default material
	NxMaterial* defaultMaterial = gScene->getMaterialFromIndex(0); 
	defaultMaterial->setRestitution(0.5);
	defaultMaterial->setStaticFriction(0.5);
	defaultMaterial->setDynamicFriction(0.5);


	// init time
	_lasttime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeSync()*0.001;


	// init character controllers
	gManager = NxCreateControllerManager(UserAllocatorHandler::getInstance()->GetAllocator());


	// set timing to X timestep per second
	gScene->setTiming(_simtime);


	// Start the first frame of the simulation
	if (gScene)  
		StartPhysics();
}

/***********************************************************
	quit function
***********************************************************/
void PhysXEngine::Quit()
{
    if (gScene)
	{
		//clear physic scene
		Clear();

		// clean up character controllers
		NxReleaseControllerManager(gManager);
		gManager = NULL;

		// clean up scene
		gPhysicsSDK->releaseScene(*gScene);
		gScene = NULL;
	}

	if (gPhysicsSDK)  
		gPhysicsSDK->release();

	gPhysicsSDK = NULL;
}


/***********************************************************
	call to start physic calculation after having updated state from inputs
***********************************************************/
void PhysXEngine::StartPhysics()
{


	// Update the time step
	double currentime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeSync()*0.001;
	float diff = (float)(currentime - _lasttime); // time in seconds
	_lastduration = diff; //+= diff;
	_lasttime = currentime;

	// Start collision and dynamics for delta time since the last frame
    gScene->simulate(diff);
	gScene->flushStream();
}


/***********************************************************
	call after render to get results of calculation
***********************************************************/
bool PhysXEngine::GetPhysicsResults()
{
	// Get results from gScene->simulate
	gScene->fetchResults(NX_RIGID_BODY_FINISHED, true);

	// update controllers position
	gManager->updateControllers();

	// only save state every real update
	//if(_lastduration > _simtime)
	//{
		//save state
		boost::shared_ptr<PhysicalState> sstate
			(new PhysicalState(SynchronizedTimeHandler::getInstance()->GetCurrentTimeSync(), _lastduration));
		sstate->SetSavedState(SaveCurrentState());
		_savedstates.insert(sstate);

		//while(_lastduration > _simtime)
		//	_lastduration -= _simtime;

		//remove too old historic entries
		unsigned int currentime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeSync();
		std::set<boost::shared_ptr<PhysicalState>, statecomp>::iterator it = _savedstates.begin();
		while(it != _savedstates.end())
		{
			if((currentime - (*it)->GetTime()) > MAX_HISTORY_TIME)
				it = _savedstates.erase(it);
			else
				break;
		}

		return true;
	//}

	return false;

}



/***********************************************************
	clear the scene of all actors
***********************************************************/
void PhysXEngine::Clear()
{

	if(gScene)
	{

		// Make sure to fetchResults() before shutting down
		GetPhysicsResults();  


		// clean up character controllers
		if(gManager)
			gManager->purgeControllers();


		// clean up other actors
		NxActor** actors = gScene->getActors();
		for(NxU32 i=0; i<gScene->getNbActors(); ++i, ++actors)
			gScene->releaseActor(**actors);
	}

	_roofactors.clear();
}


/***********************************************************
	create box actor
***********************************************************/
NxActor* PhysXEngine::CreateBox(const NxVec3 & StartPosition, float dimX, float dimY, float dimZ, 
								float density, int Type, ActorUserData * adata)
{
	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;

	// The actor has one shape, a box, 1m on a side
	NxBoxShapeDesc boxDesc;
	boxDesc.dimensions.set(dimX, dimY, dimZ);
	boxDesc.localPose.t = NxVec3(0, 0, 0);

	if(Type != 1)
	{
		if(Type == 2)
			bodyDesc.flags |= NX_BF_KINEMATIC;

		actorDesc.body	= &bodyDesc;
		actorDesc.density = density;

		if(Type == 3)
			boxDesc.group = GROUP_COLLIDABLE_PUSHABLE;
		else
			boxDesc.group = GROUP_COLLIDABLE_NON_PUSHABLE;
	}
	else
		boxDesc.group = GROUP_COLLIDABLE_NON_PUSHABLE;

	actorDesc.shapes.pushBack(&boxDesc);
	actorDesc.userData = adata;

	actorDesc.globalPose.t	= StartPosition;	
	assert(actorDesc.isValid());
	NxActor *pActor = gScene->createActor(actorDesc);	
	assert(pActor);

	#ifdef _DEBUG
		LogHandler::getInstance()->LogToFile("Added box actor to physic engine");
	#endif

	return pActor;
}

/***********************************************************
	create sphere actor
***********************************************************/
NxActor* PhysXEngine::CreateSphere(const NxVec3 & StartPosition, float radius, float density, 
									int Type, ActorUserData * adata)
{
	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;

	// The actor has one shape, a sphere, 1m on radius
	NxSphereShapeDesc sphereDesc;
	sphereDesc.radius		= radius;
	sphereDesc.localPose.t	= NxVec3(0, 0, 0);

	if(Type != 1)
	{
		if(Type == 2)
			bodyDesc.flags |= NX_BF_KINEMATIC;

		actorDesc.body	= &bodyDesc;
		actorDesc.density		= density;

		if(Type == 3)
			sphereDesc.group = GROUP_COLLIDABLE_PUSHABLE;
		else
			sphereDesc.group = GROUP_COLLIDABLE_NON_PUSHABLE;
	}
	else
		sphereDesc.group = GROUP_COLLIDABLE_NON_PUSHABLE;

	actorDesc.shapes.pushBack(&sphereDesc);
	actorDesc.userData = adata;

	actorDesc.globalPose.t	= StartPosition;	
	assert(actorDesc.isValid());
	NxActor *pActor = gScene->createActor(actorDesc);	
	assert(pActor);

	#ifdef _DEBUG
		LogHandler::getInstance()->LogToFile("Added sphere actor to physic engine");
	#endif

	return pActor;
}

/***********************************************************
	create capsule actor
***********************************************************/
NxActor* PhysXEngine::CreateCapsule(const NxVec3 & StartPosition, float radius, float height, float density, 
										int Type, ActorUserData * adata)
{
	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;

	// The actor has one shape, a sphere, 1m on radius
	NxCapsuleShapeDesc capsuleDesc;
	capsuleDesc.radius		= radius;
	capsuleDesc.height		= height;
	capsuleDesc.localPose.t = NxVec3(0, 0, 0);

	if(Type != 1)
	{
		if(Type == 2)
			bodyDesc.flags |= NX_BF_KINEMATIC;

		actorDesc.body	= &bodyDesc;
		actorDesc.density = density;

		if(Type == 3)
			capsuleDesc.group = GROUP_COLLIDABLE_PUSHABLE;
		else
			capsuleDesc.group = GROUP_COLLIDABLE_NON_PUSHABLE;
	}
	else
		capsuleDesc.group = GROUP_COLLIDABLE_NON_PUSHABLE;


	actorDesc.shapes.pushBack(&capsuleDesc);
	actorDesc.userData = adata;

	actorDesc.globalPose.t	= StartPosition;	
	assert(actorDesc.isValid());

	NxActor *pActor = gScene->createActor(actorDesc);	
	assert(pActor);

	#ifdef _DEBUG
		LogHandler::getInstance()->LogToFile("Added capsule actor to physic engine");
	#endif

	return pActor;
}



/***********************************************************
	create character controller
***********************************************************/
NxController* PhysXEngine::CreateCharacter(const NxVec3 & StartPosition, float radius, float height,
											ActorUserData * adata)
{
	NxCapsuleControllerDesc desc;
	desc.position.x		= StartPosition.x;
	desc.position.y		= StartPosition.y;
	desc.position.z		= StartPosition.z;
	desc.radius			= radius;
	desc.height			= height;
	desc.upDirection	= NX_Y;
	desc.slopeLimit		= cosf(NxMath::degToRad(50.0f));
	desc.skinWidth		= SKINWIDTH;
	desc.stepOffset		= 0.5f;
	desc.callback		= &gControllerHitReport;

	NxController* res = gManager->createController(gScene, desc);
	res->getActor()->userData = adata;

	#ifdef _DEBUG
		LogHandler::getInstance()->LogToFile("Added character to physic engine");
	#endif

	return res;
}


/***********************************************************
	create character controller
***********************************************************/
NxController* PhysXEngine::CreateCharacterBox(const NxVec3 & StartPosition, const NxVec3 & Extents,
												ActorUserData * adata)
{
	NxBoxControllerDesc desc;
	desc.position.x		= StartPosition.x;
	desc.position.y		= StartPosition.y;
	desc.position.z		= StartPosition.z;
	desc.extents		= Extents;
	desc.upDirection	= NX_Y;
	desc.slopeLimit		= cosf(NxMath::degToRad(50.0f));
	desc.skinWidth		= SKINWIDTH;
	desc.stepOffset		= 0.5f;
	desc.callback		= &gControllerHitReport;

	NxController* res = gManager->createController(gScene, desc);
	res->getActor()->userData = adata;

	#ifdef _DEBUG
		LogHandler::getInstance()->LogToFile("Added character box to physic engine");
	#endif

	return res;
}

/***********************************************************
	CreateTriangleMesh
***********************************************************/
NxActor* PhysXEngine::CreateTriangleMesh(const NxVec3 & StartPosition, float *Vertexes, size_t VertexesSize, 
											unsigned int *Indices, size_t IndicesSize,
											ActorUserData * adata, bool collidablemesh)
{
	#ifdef _DEBUG
		LogHandler::getInstance()->LogToFile("Added triangle mesh actor to physic engine");
	#endif

	// Create descriptor for triangle mesh
	NxTriangleMeshDesc triangleMeshDesc;
	triangleMeshDesc.numVertices = VertexesSize/3;
	triangleMeshDesc.pointStrideBytes = 3 * sizeof(float);
	triangleMeshDesc.points	= Vertexes;
	triangleMeshDesc.numTriangles = IndicesSize/3;
	triangleMeshDesc.flags = 0;
	triangleMeshDesc.triangles = Indices;
	triangleMeshDesc.triangleStrideBytes = 3 * sizeof(unsigned int);


	// The actor has one shape, a triangle mesh
	NxInitCooking();
	MemoryWriteBuffer buf;

	bool status = NxCookTriangleMesh(triangleMeshDesc, buf);
	NxTriangleMesh* pMesh;
	if (status)
	{
		pMesh = gPhysicsSDK->createTriangleMesh(MemoryReadBuffer(buf.data));
	}
	else
	{
		assert(false);
		pMesh = NULL;
	}
	NxCloseCooking();

	// Create TriangleMesh above code segment.
	NxTriangleMeshShapeDesc tmsd;
	tmsd.meshData			= pMesh;
	tmsd.localPose.t		= NxVec3(0, 0, 0);
	tmsd.meshPagingMode 	= NX_MESH_PAGING_AUTO;

	if(collidablemesh)
		tmsd.group = GROUP_COLLIDABLE_NON_PUSHABLE;
	else
		tmsd.group = GROUP_NON_COLLIDABLE;

	NxActorDesc actorDesc;
	NxBodyDesc  bodyDesc;

	assert(tmsd.isValid());
	actorDesc.shapes.pushBack(&tmsd);
	actorDesc.body		= NULL;		
	actorDesc.globalPose.t	= StartPosition;
	actorDesc.userData = adata;

	if (pMesh)
	{
		assert(actorDesc.isValid());
		NxActor *actor = gScene->createActor(actorDesc);
		assert(actor);

		return actor;
	}

	return NULL;
}



/***********************************************************
 move character
 returned collision flags, collection of NxControllerFlag
***********************************************************/
unsigned int PhysXEngine::MoveCharacter(NxController* character, const NxVec3& moveVector, bool checkcollision)
{
	NxU32 collisionFlags;
	if(character)
		character->move(moveVector, (checkcollision?COLLIDABLE_MASK:0), 0.000001f, collisionFlags);

	return collisionFlags;
}



/***********************************************************
get gravity
***********************************************************/
void PhysXEngine::GetGravity(LbaVec3 & Gravity)
{
	if(gScene)
	{
		NxVec3 Gr;
		gScene->getGravity(Gr);
		Gravity.x = Gr.x;
		Gravity.y = Gr.y;
		Gravity.z = Gr.z;
	}
}


/***********************************************************
DestroyActor
***********************************************************/
void PhysXEngine::DestroyActor(NxActor* actor)
{
	#ifdef _DEBUG
		LogHandler::getInstance()->LogToFile("Remove Actor from physic engine");
	#endif

	//destroy internal actor if there is one
	ActorUserData * udata = (ActorUserData *)actor->userData;
	if(udata)
	{
		if(udata->InternalActor)
		{
			DestroyActor(udata->InternalActor);
			std::set<NxActor*>::iterator it = _roofactors.find(udata->InternalActor);
			if(it != _roofactors.end())
				_roofactors.erase(it);
		}
	}

	//remove from history
	RemoveActorFromHistory(actor);

	if(gScene && actor)
		gScene->releaseActor(*actor);
}


/***********************************************************
DestroyCharacter
***********************************************************/
void PhysXEngine::DestroyCharacter(NxController* character)
{
	#ifdef _DEBUG
		LogHandler::getInstance()->LogToFile("Remove Character from physic engine");
	#endif

	//remove from history
	RemoveCharFromHistory(character);

	if(gManager && character)
		gManager->releaseController(*character);
}






/***********************************************************
//! Load triangle mesh shape to the engine
***********************************************************/
NxActor* PhysXEngine::LoadTriangleMeshFile(const NxVec3 & StartPosition, const std::string Filename,
											boost::shared_ptr<ActorUserData> userdata)
{
	// load data from binary file and set it into a triangle mesh
	std::ifstream file(Filename.c_str(), std::ifstream::binary);
	if(!file.is_open())
	{
		LogHandler::getInstance()->LogToFile("Can not read file "+Filename, 1);
		return NULL;
	}

	unsigned int sizevertex;
	unsigned int sizeindices;
	unsigned int sizematerials;
	unsigned int sizevertexroof;
	unsigned int sizeindicesroof;

	file.read((char*)&sizevertex, sizeof(unsigned int));
	file.read((char*)&sizeindices, sizeof(unsigned int));
	file.read((char*)&sizematerials, sizeof(unsigned int));
	file.read((char*)&sizevertexroof, sizeof(unsigned int));
	file.read((char*)&sizeindicesroof, sizeof(unsigned int));

	float *buffervertex = new float[sizevertex];
	unsigned int *bufferindices = new unsigned int[sizeindices];
	short *buffermaterials = new short[sizematerials];

	file.read((char*)buffervertex, sizevertex*sizeof(float));
	file.read((char*)bufferindices, sizeindices*sizeof(unsigned int));
	file.read((char*)buffermaterials, sizematerials*sizeof(short));


	userdata->MaterialsSize = sizematerials;
	userdata->Materials =  buffermaterials;

	NxActor* act = CreateTriangleMesh(StartPosition, buffervertex, sizevertex, bufferindices, sizeindices, 
											userdata.get());
	//NxActor* act = NULL;
	delete[] buffervertex;
	delete[] bufferindices;


	// create shape for roof if necessary
	if(sizevertexroof > 0)
	{
		float *buffervertexroof = NULL;
		unsigned int *bufferindicesroof = NULL;

		buffervertexroof = new float[sizevertexroof];
		bufferindicesroof = new unsigned int[sizeindicesroof];
		file.read((char*)buffervertexroof, sizevertexroof*sizeof(float));
		file.read((char*)bufferindicesroof, sizeindicesroof*sizeof(unsigned int));

		NxActor* roofact = CreateTriangleMesh(StartPosition, buffervertexroof, sizevertexroof, 
												bufferindicesroof, sizeindicesroof, NULL, false);

		_roofactors.insert(roofact);
		userdata->InternalActor = roofact;

		if(buffervertexroof)
			delete buffervertexroof;

		if(bufferindicesroof)
			delete bufferindicesroof;
	}



	return act;
}



/***********************************************************
//! check if there is a roof up the 3d position in parameter
//! if there is a roof, return the roof position along the y axis
//! else return -1
***********************************************************/
float PhysXEngine::CheckForRoof(float PositionX, float PositionY, float PositionZ)
{
	std::set<NxActor*>::iterator it = _roofactors.begin();
	std::set<NxActor*>::iterator end = _roofactors.end();
	for(;it != end; ++it)
	{
		if((*it)->getNbShapes() > 0)
		{
			NxTriangleMeshShape* _currmap = (*(*it)->getShapes())->isTriangleMesh();

			NxRaycastHit hitinfo;
			NxVec3 vec(0, 1, 0);
			NxVec3 Position(PositionX, PositionY, PositionZ);

			if(_currmap && _currmap->raycast(NxRay(Position, vec), 100.0f, NX_RAYCAST_DISTANCE, hitinfo, false))
			{
				return (Position.y + hitinfo.distance);
			}
		}
	}

	return -1;
}	



/***********************************************************
//! apply historic modifications
***********************************************************/
void PhysXEngine::ApplyHistoricModifications()
{
	// do something only if we got modification during this timestep
	if(_curr_modifications.size() > 0)
	{
		//get oldest modification time
		unsigned int modtime = (*_curr_modifications.begin())->GetTime();	

		//get the saved state for this time
		std::set<boost::shared_ptr<PhysicalState>, statecomp>::reverse_iterator it = _savedstates.rbegin();
		for(; it != _savedstates.rend(); ++it)
		{
			if((*it)->GetTime() < modtime)
				break;
		}
	
		// if we found a corresponding saved state
		if(it != _savedstates.rend())
		{
			//load the saved state at this time
			if(it != _savedstates.rbegin())
				LoadState((*it)->GetSavedState());

			(*it)->ApplyModification((*it)->GetSimDuration());
			--it;


			//reapply all modification since this time
			for(; it != _savedstates.rend(); --it)
			{
				// do the simulation step
				gScene->simulate((*it)->GetSimDuration());
				gScene->flushStream();
				gScene->fetchResults(NX_RIGID_BODY_FINISHED, true);
				gManager->updateControllers();

				//save back the new calculated state
				(*it)->SetSavedState(SaveCurrentState());

				// apply mods
				(*it)->ApplyModification((*it)->GetSimDuration());
			}
		}


		//! clear current mod vector
		_curr_modifications.clear();
	}
}

/***********************************************************
//! remove actors from history
***********************************************************/
void PhysXEngine::RemoveActorFromHistory(NxActor* act)
{
	std::multiset<boost::shared_ptr<PhysicalModification>, classcomp>::iterator it = _curr_modifications.begin();
	while(it != _curr_modifications.end())
	{
		if((*it)->SameActor(act))
			it = _curr_modifications.erase(it);
		else
			++it;
	}

	std::set<boost::shared_ptr<PhysicalState>, statecomp>::iterator itst =	_savedstates.begin();
	std::set<boost::shared_ptr<PhysicalState>, statecomp>::iterator endst =	_savedstates.end();
	for(; itst != endst; ++itst)
		(*itst)->RemoveActor(act);

}

/***********************************************************
//! remove actors from history
***********************************************************/
void PhysXEngine::RemoveCharFromHistory(NxController* act)
{
	std::multiset<boost::shared_ptr<PhysicalModification>, classcomp>::iterator it = _curr_modifications.begin();
	while(it != _curr_modifications.end())
	{
		if((*it)->SameCharacter(act))
			it = _curr_modifications.erase(it);
		else
			++it;
	}

	std::set<boost::shared_ptr<PhysicalState>, statecomp>::iterator itst =	_savedstates.begin();
	std::set<boost::shared_ptr<PhysicalState>, statecomp>::iterator endst =	_savedstates.end();
	for(; itst != endst; ++itst)
		(*itst)->RemoveCharacter(act);
}


/***********************************************************
//! add physicall modification
***********************************************************/
void PhysXEngine::AddModification(boost::shared_ptr<PhysicalModification> mod)
{
	std::set<boost::shared_ptr<PhysicalState>, statecomp>::reverse_iterator it = _savedstates.rbegin();
	for(; it != _savedstates.rend(); ++it)
	{
		if((*it)->GetTime() < mod->GetTime())
		{
			(*it)->AddModification(mod);
			_curr_modifications.insert(mod);
			return;
		}
	}
}



/***********************************************************
//! move an actor
***********************************************************/
void PhysXEngine::MoveActorTo(unsigned int time, NxActor* act, const NxVec3 & targetPos)
{
	AddModification(boost::shared_ptr<PhysicalModification>(new MoveActorToModification(time, act, targetPos)));
}

/***********************************************************
//! set an actor position
***********************************************************/
void PhysXEngine::SetActorPos(unsigned int time, NxActor* act, const NxVec3 & targetPos)
{
	AddModification(boost::shared_ptr<PhysicalModification>(new SetActorPosModification(time, act, targetPos)));
}

/***********************************************************
//! set an actor rotation
***********************************************************/
void PhysXEngine::SetActorRotation(unsigned int time, NxActor* act, const NxQuat & quat)
{
	AddModification(boost::shared_ptr<PhysicalModification>(new SetActorRotationModification(time, act, quat)));
}

/***********************************************************
//! move character - return collision flag
***********************************************************/
void PhysXEngine::DeltaMoveCharacter(unsigned int time, NxController* act, 
											 boost::shared_ptr<ActorUserData> udata,
												const NxVec3 & deltamove, bool checkCollision)
{
	AddModification(boost::shared_ptr<PhysicalModification>(new DeltaMoveCharacterModification(time, act, 
																			udata, deltamove, checkCollision)));
}

/***********************************************************
//! set character position - no collision checked
***********************************************************/
void PhysXEngine::SetCharacterPosition(unsigned int time, NxController* act, 
									   boost::shared_ptr<ActorUserData> udata,
										const NxExtendedVec3 & targetPos)
{
	AddModification(boost::shared_ptr<PhysicalModification>(new SetCharacterPositionModification(time, act, 
																							udata, targetPos)));
}



/***********************************************************
//! move actor in rotation direction
***********************************************************/
void PhysXEngine::MoveInDirectionActor(unsigned int time, NxActor* act, boost::shared_ptr<SimpleRotationHandler> rotH,
							float RotationYBeforeMove, float MoveSpeed,	bool AddGravity)
{
	LbaVec3 Gravity(0, 0, 0);
	if(AddGravity)
		GetGravity(Gravity);

	AddModification(boost::shared_ptr<PhysicalModification>(new MoveInDirectionActorModification(time, act, 
																rotH, RotationYBeforeMove, MoveSpeed, Gravity)));

}

/***********************************************************
//! move actor in rotation direction
***********************************************************/
void PhysXEngine::MoveInDirectionCharacter(unsigned int time, NxController* act, boost::shared_ptr<ActorUserData> udata,
								boost::shared_ptr<SimpleRotationHandler> rotH,
								float RotationYBeforeMove, float MoveSpeed,	bool AddGravity)
{
	LbaVec3 Gravity(0, 0, 0);
	if(AddGravity)
		GetGravity(Gravity);

	AddModification(boost::shared_ptr<PhysicalModification>(new MoveInDirectionCharacterModification(time, act, 
																udata, rotH, RotationYBeforeMove, MoveSpeed, Gravity)));
}


/***********************************************************
save current physical state
***********************************************************/
SavedState * PhysXEngine::SaveCurrentState()
{
	SavedState * res = new SavedState();

	//save actors
	unsigned int nbactors = gScene->getNbActors();
	NxActor** actors = gScene->getActors();
	for(unsigned int i=0; i<nbactors; ++i, ++actors)
		res->AddActor(*actors);

	//save characters
	unsigned int nbcontrols = gManager->getNbControllers();
	for(unsigned int i=0; i<nbcontrols; ++i)
		res->AddCharacter(gManager->getController(i));

	return res;
}

/***********************************************************
load saved physical state
***********************************************************/
void PhysXEngine::LoadState(SavedState * state)
{
	state->Load();
}