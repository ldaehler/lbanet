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
#include "SynchronizedTimeHandler.h"
#include "NxPhysics.h"
#include "NxControllerManager.h"
#include "NxCapsuleController.h"
#include "NxBoxController.h"
#include "UserAllocator.h"
#include "NxCooking.h"
#include "Stream.h"
#include <limits>
#include <fstream>


#include <windows.h>    // Header File For Windows
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#include <GL/glu.h>     // Header File For The GLu32 Library


#define SKINWIDTH	0.2f



enum GameGroup
{
	GROUP_NON_COLLIDABLE,
	GROUP_COLLIDABLE_NON_PUSHABLE,
	GROUP_COLLIDABLE_PUSHABLE,
};

#define COLLIDABLE_MASK	(1<<GROUP_COLLIDABLE_NON_PUSHABLE) | (1<<GROUP_COLLIDABLE_PUSHABLE)


class MyContactReport : public NxUserContactReport    
{        
	void onContactNotify(NxContactPair& pair, NxU32 events)        
	{  
		if(events & NX_NOTIFY_ON_START_TOUCH)
		{
			NxActor* n1 = pair.actors[0];
			NxActor* n2 = pair.actors[1];
			if(n1 && n2)
			{
				ActorUserData * acd1 = (ActorUserData *) n1->userData;
				ActorUserData * acd2 = (ActorUserData *) n2->userData;
				if(acd1 && acd2)
				{
					if(acd1->Callback)
						acd1->Callback->CallbackOnContact(acd2->ActorType, acd2->ActorId);

					if(acd2->Callback)
						acd2->Callback->CallbackOnContact(acd1->ActorType, acd1->ActorId);
				}
			}
		} 


		//if(events & NX_NOTIFY_ON_START_TOUCH)
		//{
		//	NxActor* n1 = pair.actors[0];
		//	NxActor* n2 = pair.actors[1];
		//	if(n1 && n2)
		//	{
		//		ActorUserData * acd1 = (ActorUserData *) n1->userData;
		//		ActorUserData * acd2 = (ActorUserData *) n2->userData;
		//		if(acd1 && acd2)
		//		{
		//			if(	(acd1->ActorType == 1 && acd2->ActorType == 2) ||
		//				(acd2->ActorType == 1 && acd1->ActorType == 2))
		//			{
		//				// Iterate through contact points
		//				NxContactStreamIterator i(pair.stream);
		//				//user can call getNumPairs() here
		//				while(i.goNextPair())
		//				{
		//					//user can also call getShape() and getNumPatches() here
		//					while(i.goNextPatch())
		//					{
		//						while(i.goNextPoint())
		//						{
		//							//user can also call getPoint() and getSeparation() here
		//							if(i.getSeparation()<0.0f)
		//							{
		//								const NxVec3& contactPoint = i.getPoint();

		//								NxU32 faceIndex = i.getFeatureIndex0();
		//								if(faceIndex==0xffffffff)	
		//									faceIndex = i.getFeatureIndex1();

		//								if(faceIndex!=0xffffffff)
		//								{
		//									std::cout<<faceIndex<<std::endl;
		//									//gTouchedTris.pushBack(faceIndex);
		//								}
		//							}
		//						}
		//					}
		//				}		
		//			}
		//		}
		//	}
		//} 
	}    
} myReport;


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







PhysXEngine* PhysXEngine::_singletonInstance = NULL;


/***********************************************************
singleton pattern
***********************************************************/
PhysXEngine * PhysXEngine::getInstance()
{
    if(!_singletonInstance)
        _singletonInstance = new PhysXEngine();

	return _singletonInstance;
}


/***********************************************************
	Constructor
***********************************************************/
PhysXEngine::PhysXEngine()
: gAllocator(NULL), _flooractor(NULL)
{
	gAllocator = new UserAllocator();
}


/***********************************************************
	Destructor
***********************************************************/
PhysXEngine::~PhysXEngine()
{
	if(gAllocator!=NULL)
	{
			delete gAllocator;
			gAllocator=NULL;
	}
}



/***********************************************************
	init function
***********************************************************/
void PhysXEngine::Init(float gravity)
{
	// Create the physics SDK
    gPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION);
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
    sceneDesc.gravity               = NxVec3(0,gravity,0);
    gScene = gPhysicsSDK->createScene(sceneDesc);	
	if(!gScene)
	{ 
		sceneDesc.simType			= NX_SIMULATION_SW; 
		gScene = gPhysicsSDK->createScene(sceneDesc);  
		if(!gScene) 
			return;
	}

	gScene->setUserContactReport(&myReport);


	// Create the default material
	NxMaterial* defaultMaterial = gScene->getMaterialFromIndex(0); 
	defaultMaterial->setRestitution(0.5);
	defaultMaterial->setStaticFriction(0.5);
	defaultMaterial->setDynamicFriction(0.5);
	defaultMaterial->setFrictionCombineMode(NX_CM_MIN);
	defaultMaterial->setRestitutionCombineMode(NX_CM_MAX);


	// init time
	_lasttime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeDouble();


	// init character controllers
	gManager = NxCreateControllerManager(gAllocator);


	//float TimeStep = 1.0f / 60.0f;
	gScene->setTiming();


	// Start the first frame of the simulation
	if (gScene)  
		StartPhysics();

	_isInitialized = true;
}

/***********************************************************
	quit function
***********************************************************/
void PhysXEngine::Quit()
{
	_isInitialized = false;

    if (gScene)
	{
		// Make sure to fetchResults() before shutting down
		GetPhysicsResults();  

		// clean up character controllers
		gManager->purgeControllers();
		NxReleaseControllerManager(gManager);
		gManager = NULL;

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
	double currentime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeDouble();
	float tdiff = (float)(currentime - _lasttime);

	// Start collision and dynamics for delta time since the last frame
    gScene->simulate(tdiff);
	gScene->flushStream();

	_lasttime = currentime;
}


/***********************************************************
	call after render to get results of calculation
***********************************************************/
void PhysXEngine::GetPhysicsResults()
{
	// Get results from gScene->simulate
	gScene->fetchResults(NX_RIGID_BODY_FINISHED, true);

	NxReal maxTimestep;
	NxTimeStepMethod method;
	NxU32 maxIter;
	NxU32 numSubSteps;
	gScene->getTiming(maxTimestep, maxIter, method, &numSubSteps);
	if(numSubSteps)	
		gManager->updateControllers();
}


/***********************************************************
	create plane actor
***********************************************************/
NxActor* PhysXEngine::CreatePlane(const NxVec3 & StartPosition, const NxVec3 & PlaneNormal)
{
    // Create a plane with default descriptor
    NxPlaneShapeDesc planeDesc;
	planeDesc.normal = PlaneNormal;
	planeDesc.group = GROUP_COLLIDABLE_NON_PUSHABLE;

    NxActorDesc actorDesc;
    actorDesc.shapes.pushBack(&planeDesc);
	actorDesc.globalPose.t	= StartPosition;	

    return gScene->createActor(actorDesc);
}


/***********************************************************
	create box actor
***********************************************************/
NxActor* PhysXEngine::CreateBox(const NxVec3 & StartPosition, float dimX, float dimY, float dimZ, 
								float density, int Type, ActorUserData * adata, bool collidable)
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

	if(!collidable)
		boxDesc.group = GROUP_NON_COLLIDABLE;

	actorDesc.shapes.pushBack(&boxDesc);
	actorDesc.userData = adata;

	actorDesc.globalPose.t	= StartPosition;	
	assert(actorDesc.isValid());
	NxActor *pActor = gScene->createActor(actorDesc);	
	assert(pActor);

	return pActor;
}

/***********************************************************
	create sphere actor
***********************************************************/
NxActor* PhysXEngine::CreateSphere(const NxVec3 & StartPosition, float radius, float density, 
									int Type,  ActorUserData * adata,
									float staticFriction, float dynamicFriction, float restitution)
{
	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;

	// The actor has one shape, a sphere, 1m on radius
	NxSphereShapeDesc sphereDesc;
	sphereDesc.radius		= radius;
	sphereDesc.localPose.t	= NxVec3(0, 0, 0);
	

    NxMaterialDesc materialDesc;    
	materialDesc.restitution = restitution;    
	materialDesc.staticFriction = staticFriction;    
	materialDesc.dynamicFriction = dynamicFriction; 
	materialDesc.frictionCombineMode = NX_CM_MIN;
	materialDesc.restitutionCombineMode = NX_CM_MAX;
    NxMaterial *newMaterial=gScene->createMaterial(materialDesc); 

	sphereDesc.materialIndex = newMaterial->getMaterialIndex();

	
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
	actorDesc.globalPose.t	= StartPosition;	
	actorDesc.userData = adata;

	return gScene->createActor(actorDesc);
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
	actorDesc.globalPose.t	= StartPosition;
	actorDesc.userData = adata;

	return gScene->createActor(actorDesc);
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
	res->setCollision(true);

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
	res->setCollision(true);

	return res;
}

/***********************************************************
	CreateTriangleMesh
***********************************************************/
NxActor* PhysXEngine::CreateTriangleMesh(const NxVec3 & StartPosition, float *Vertexes, size_t VertexesSize, 
											unsigned int *Indices, size_t IndicesSize,
											ActorUserData * adata, bool collidablemesh)
{
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
//! Load triangle mesh shape to the engine
***********************************************************/
NxActor* PhysXEngine::LoadTriangleMeshFile(const NxVec3 & StartPosition, const std::string Filename,
												ActorUserData * userdata)
{
	// load data from binary file and set it into a triangle mesh
	std::ifstream file(Filename.c_str(), std::ifstream::binary);
	if(!file.is_open())
	{
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

	_flooractor = CreateTriangleMesh(StartPosition, buffervertex, sizevertex, bufferindices, sizeindices, 
											userdata);
	//NxActor* act = NULL;
	delete[] buffervertex;
	delete[] bufferindices;


	// create shape for roof if necessary
	//_sizebuff = sizeindicesroof;
	if(sizevertexroof > 0)
	{
		float *buffervertexroof = NULL;
		unsigned int *bufferindicesroof = NULL;

		buffervertexroof = new float[sizevertexroof];
		bufferindicesroof = new unsigned int[sizeindicesroof];
		file.read((char*)buffervertexroof, sizevertexroof*sizeof(float));
		file.read((char*)bufferindicesroof, sizeindicesroof*sizeof(unsigned int));

		//_buffervertexroof = new float[sizevertexroof];
		//memcpy(_buffervertexroof, buffervertexroof, sizevertexroof*sizeof(float));
		//_bufferindiceroof = new unsigned int[sizeindicesroof];
		//memcpy(_bufferindiceroof, bufferindicesroof, sizeindicesroof*sizeof(unsigned int));


		NxActor* roofact = CreateTriangleMesh(StartPosition, buffervertexroof, sizevertexroof, 
												bufferindicesroof, sizeindicesroof, NULL, false);

		_roofactors.insert(roofact);
		userdata->InternalActor = roofact;

		if(buffervertexroof)
			delete buffervertexroof;

		if(bufferindicesroof)
			delete bufferindicesroof;
	}



	return _flooractor;
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
set actor position (teleport)
***********************************************************/
void PhysXEngine::SetCharacterPos(NxController* character, const NxVec3& posVector)
{
	NxExtendedVec3 pos;
	pos.x = posVector.x;
	pos.y = posVector.y;
	pos.z = posVector.z;
	character->setPosition(pos);
}

/***********************************************************
get gravity
***********************************************************/
void PhysXEngine::GetGravity(NxVec3 & Gravity)
{
	if(gScene)
		gScene->getGravity(Gravity);
}

/***********************************************************
DestroyActor
***********************************************************/
void PhysXEngine::DestroyActor(NxActor* actor)
{
	if(!_isInitialized)
		return;

	if(actor == _flooractor)
		_flooractor = NULL;

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

		udata->released = true;
	}

	if(gScene && actor)
		gScene->releaseActor(*actor);
}

/***********************************************************
DestroyCharacter
***********************************************************/
void PhysXEngine::DestroyCharacter(NxController* character)
{
	if(gManager && character)
		gManager->releaseController(*character);
}




/***********************************************************
GetCharacterPosition
***********************************************************/
void PhysXEngine::GetCharacterPosition(NxController* character, float &posX, float &posY, float &posZ)
{
	NxExtendedVec3 vec = character->getPosition();
	posX = (float)vec.x;
	posY = (float)vec.y;
	posZ = (float)vec.z;
}



/***********************************************************
//! check if there is a roof up the 3d position in parameter
//! if there is a roof, return the roof position along the y axis
//! else return -1
***********************************************************/
int PhysXEngine::CheckForRoof(float PositionX, float PositionY, float PositionZ)
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
			NxVec3 Position(PositionX, PositionY+1, PositionZ);

			if(_currmap && _currmap->raycast(NxRay(Position, vec), 100.0f, NX_RAYCAST_DISTANCE, hitinfo, false))
			{
				return (int)(Position.y + hitinfo.distance + 0.5);
			}
		}
	}

	return -1;
}	

/***********************************************************
//! Get Closest Floor
***********************************************************/
int PhysXEngine::GetClosestFloor(float PositionX, float PositionY, float PositionZ)
{
	if(_flooractor)
	{
		if(_flooractor->getNbShapes() > 0)
		{
			NxTriangleMeshShape* _currmap = (*_flooractor->getShapes())->isTriangleMesh();

			NxRaycastHit hitinfo;
			NxVec3 vec(0, -1, 0);
			NxVec3 Position(PositionX, PositionY+1, PositionZ);

			if(_currmap && _currmap->raycast(NxRay(Position, vec), 100.0f, NX_RAYCAST_DISTANCE, hitinfo, false))
			{
				return (int)(Position.y - hitinfo.distance + 0.5);
			}
		}
	}

	return -1;
}



/***********************************************************
ignore collision for a pair of actor
***********************************************************/
void PhysXEngine::IgnoreActorContact(NxActor* actor1, NxActor* actor2)
{
	gScene->setActorPairFlags(*actor1, *actor2, NX_IGNORE_PAIR);
}


/***********************************************************
	Render actors
***********************************************************/
void PhysXEngine::RenderActors()
{
	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glLineWidth(2.0f);

    // Render all the actors in the scene
    NxU32 nbActors = gScene->getNbActors();
    NxActor** actors = gScene->getActors();
    while (nbActors--)
    {
        NxActor* actor = *actors++;

		glPushMatrix();

		glScalef(1, 0.5f, 1);
		NxMat34 pose = actor->getShapes()[0]->getGlobalPose();
		//glTranslated(pose.t.x, pose.t.y/2. + 0.5, pose.t.z);

		float glmat[16];	//4x4 column major matrix for OpenGL.
		pose.M.getColumnMajorStride4(&(glmat[0]));
		pose.t.get(&(glmat[12]));
		//clear the elements we don't need:
		glmat[3] = glmat[7] = glmat[11] = 0.0f;
		glmat[15] = 1.0f;
		glMultMatrixf(&(glmat[0]));

		NxBoxShape* boxshap = actor->getShapes()[0]->isBox();
		if(boxshap)
		{
			glColor4f(0.0f,0.0f,1.0f, 1.f);

			NxVec3 boxDim = boxshap->getDimensions();
			glScalef(boxDim.x, boxDim.y, boxDim.z);

			float _sizeX = 1, _sizeY = 1, _sizeZ = 1;
			glBegin(GL_LINES);
				glVertex3f(-_sizeX,-_sizeY,-_sizeZ);
				glVertex3f(_sizeX,-_sizeY,-_sizeZ);
				glVertex3f(_sizeX,-_sizeY,-_sizeZ);
				glVertex3f(_sizeX,-_sizeY,_sizeZ);
				glVertex3f(_sizeX,-_sizeY,_sizeZ);
				glVertex3f(-_sizeX,-_sizeY,_sizeZ);
				glVertex3f(-_sizeX,-_sizeY,_sizeZ);
				glVertex3f(-_sizeX,-_sizeY,-_sizeZ);

				glVertex3f(-_sizeX,_sizeY,-_sizeZ);
				glVertex3f(_sizeX,_sizeY,-_sizeZ);
				glVertex3f(_sizeX,_sizeY,-_sizeZ);
				glVertex3f(_sizeX,_sizeY,_sizeZ);
				glVertex3f(_sizeX,_sizeY,_sizeZ);
				glVertex3f(-_sizeX,_sizeY,_sizeZ);
				glVertex3f(-_sizeX,_sizeY,_sizeZ);
				glVertex3f(-_sizeX,_sizeY,-_sizeZ);

				glVertex3f(-_sizeX,-_sizeY,-_sizeZ);
				glVertex3f(-_sizeX,_sizeY,-_sizeZ);

				glVertex3f(_sizeX,-_sizeY,-_sizeZ);
				glVertex3f(_sizeX,_sizeY,-_sizeZ);

				glVertex3f(_sizeX,-_sizeY,_sizeZ);
				glVertex3f(_sizeX,_sizeY,_sizeZ);

				glVertex3f(-_sizeX,-_sizeY,_sizeZ);
				glVertex3f(-_sizeX,_sizeY,_sizeZ);
			glEnd();
		}
		//else
		//{
		//	for(int i=1; i<100; ++i)
		//	{
		//		glColor4f(1.0f,0.0f,0.0f, 1.f);
		//		float _sizeX = (float)i, _sizeY = 0, _sizeZ = (float)i;
		//		glBegin(GL_LINES);
		//			glVertex3f(-_sizeX,-_sizeY,-_sizeZ);
		//			glVertex3f(_sizeX,-_sizeY,-_sizeZ);
		//			glVertex3f(_sizeX,-_sizeY,-_sizeZ);
		//			glVertex3f(_sizeX,-_sizeY,_sizeZ);
		//			glVertex3f(_sizeX,-_sizeY,_sizeZ);
		//			glVertex3f(-_sizeX,-_sizeY,_sizeZ);
		//			glVertex3f(-_sizeX,-_sizeY,_sizeZ);
		//			glVertex3f(-_sizeX,-_sizeY,-_sizeZ);
		//		glEnd();
		//	}
		//}





		glPopMatrix();
    }


	//if(_sizebuff > 0)
	//{
	//	glPushMatrix();
	//	glScalef(1, 0.5f, 1);

	//	for(int i=0; i<_sizebuff;)
	//	{
	//		int id1 = _bufferindiceroof[i++] * 3;
	//		float p1x =_buffervertexroof[id1];
	//		float p1y =_buffervertexroof[id1+1];
	//		float p1z =_buffervertexroof[id1+2];

	//		int id2 = _bufferindiceroof[i++] * 3;
	//		float p2x =_buffervertexroof[id2];
	//		float p2y =_buffervertexroof[id2+1];
	//		float p2z =_buffervertexroof[id2+2];

	//		int id3 = _bufferindiceroof[i++] * 3;
	//		float p3x =_buffervertexroof[id3];
	//		float p3y =_buffervertexroof[id3+1];
	//		float p3z =_buffervertexroof[id3+2];

	//		glColor4f(0.0f,0.0f,1.0f, 1.f);
	//		glBegin(GL_LINES);
	//			glVertex3f(p1x,p1y,p1z);
	//			glVertex3f(p2x,p2y,p2z);
	//			glVertex3f(p2x,p2y,p2z);
	//			glVertex3f(p3x,p3y,p3z);
	//			glVertex3f(p3x,p3y,p3z);
	//			glVertex3f(p1x,p1y,p1z);
	//		glEnd();	
	//	}

	//	glPopMatrix();
	//}



	//for(unsigned int i=0; i<gManager->getNbControllers(); ++i)
	//{
	//	NxController* ctrl = gManager->getController(i);
	//	NxExtendedVec3 vec = ctrl->getPosition();

	//	glPushMatrix();
	//	glScalef(1, 0.5f, 1);
	//	glTranslated(vec.x, vec.y, vec.z);
	//	glColor4f(1.0f,1.0f,0.0f, 1.f);

	//	float _sizeX = 0.4f, _sizeY = 2.5f, _sizeZ = 0.4f;
	//	glBegin(GL_LINES);
	//		glVertex3f(-_sizeX,-_sizeY,-_sizeZ);
	//		glVertex3f(_sizeX,-_sizeY,-_sizeZ);
	//		glVertex3f(_sizeX,-_sizeY,-_sizeZ);
	//		glVertex3f(_sizeX,-_sizeY,_sizeZ);
	//		glVertex3f(_sizeX,-_sizeY,_sizeZ);
	//		glVertex3f(-_sizeX,-_sizeY,_sizeZ);
	//		glVertex3f(-_sizeX,-_sizeY,_sizeZ);
	//		glVertex3f(-_sizeX,-_sizeY,-_sizeZ);

	//		glVertex3f(-_sizeX,_sizeY,-_sizeZ);
	//		glVertex3f(_sizeX,_sizeY,-_sizeZ);
	//		glVertex3f(_sizeX,_sizeY,-_sizeZ);
	//		glVertex3f(_sizeX,_sizeY,_sizeZ);
	//		glVertex3f(_sizeX,_sizeY,_sizeZ);
	//		glVertex3f(-_sizeX,_sizeY,_sizeZ);
	//		glVertex3f(-_sizeX,_sizeY,_sizeZ);
	//		glVertex3f(-_sizeX,_sizeY,-_sizeZ);

	//		glVertex3f(-_sizeX,-_sizeY,-_sizeZ);
	//		glVertex3f(-_sizeX,_sizeY,-_sizeZ);

	//		glVertex3f(_sizeX,-_sizeY,-_sizeZ);
	//		glVertex3f(_sizeX,_sizeY,-_sizeZ);

	//		glVertex3f(_sizeX,-_sizeY,_sizeZ);
	//		glVertex3f(_sizeX,_sizeY,_sizeZ);

	//		glVertex3f(-_sizeX,-_sizeY,_sizeZ);
	//		glVertex3f(-_sizeX,_sizeY,_sizeZ);
	//	glEnd();

	//	glPopMatrix();
	//}


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
}