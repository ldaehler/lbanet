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
#include "UserAllocator.h"


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
						NxF32 coeff = actor.getMass() * hit.length * 10.0f;
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
: _current_controller_idx(0), gAllocator(NULL)
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
void PhysXEngine::Init()
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
    sceneDesc.gravity               = NxVec3(0,-9.8f,0);
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


	// add a cube and a plane
	//CreatePlane(NxVec3(0, 0, 0), NxVec3(0, 1, 0));

	//CreateBox(NxVec3(0, 4, 0), 3.0f, 3.0f, 3.0f, 10, false);
	//CreateBox(NxVec3(7, 10, 7), 2.0f, 3.0f, 2.0f, 10, true);
	CreateBox(NxVec3(20, -1, 20), 50.0f, 1.0f, 50.0f, 10, false);

	// init time
	_lasttime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeDouble();


	// init character controllers
	gManager = NxCreateControllerManager(gAllocator);


	//float TimeStep = 1.0f / 60.0f;
	gScene->setTiming();


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
		// Make sure to fetchResults() before shutting down
		GetPhysicsResults();  
		gPhysicsSDK->releaseScene(*gScene);
	}

	if (gPhysicsSDK)  
		gPhysicsSDK->release();

	// clean up character controllers
	gManager->purgeControllers();
	_current_controller_idx = 0;
	NxReleaseControllerManager(gManager);
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
								float density, bool Pushable)
{

	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;


	// The actor has one shape, a box, 1m on a side
	NxBoxShapeDesc boxDesc;
	boxDesc.dimensions.set(dimX, dimY, dimZ);
	boxDesc.localPose.t = NxVec3(0, 0, 0);

	if(Pushable)
	{
		NxBodyDesc bodyDesc;
		actorDesc.body	= &bodyDesc;
		actorDesc.density = density;
		boxDesc.group = GROUP_COLLIDABLE_PUSHABLE;
	}
	else
		boxDesc.group = GROUP_COLLIDABLE_NON_PUSHABLE;

	actorDesc.shapes.pushBack(&boxDesc);


	actorDesc.globalPose.t	= StartPosition;	
	assert(actorDesc.isValid());
	NxActor *pActor = gScene->createActor(actorDesc);	
	assert(pActor);

	return pActor;
}

/***********************************************************
	create sphere actor
***********************************************************/
NxActor* PhysXEngine::CreateSphere(const NxVec3 & StartPosition, float radius, float density, bool Pushable)
{
	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;

	// The actor has one shape, a sphere, 1m on radius
	NxSphereShapeDesc sphereDesc;
	sphereDesc.radius		= radius;
	sphereDesc.localPose.t	= NxVec3(0, 0, 0);

	if(Pushable)
	{
		NxBodyDesc bodyDesc;
		actorDesc.body	= &bodyDesc;
		actorDesc.density		= density;
		sphereDesc.group = GROUP_COLLIDABLE_PUSHABLE;
	}
	else
		sphereDesc.group = GROUP_COLLIDABLE_NON_PUSHABLE;

	actorDesc.shapes.pushBack(&sphereDesc);
	actorDesc.globalPose.t	= StartPosition;	
	return gScene->createActor(actorDesc);
}

/***********************************************************
	create capsule actor
***********************************************************/
NxActor* PhysXEngine::CreateCapsule(const NxVec3 & StartPosition, float radius, float height, float density, 
									bool Pushable)
{
	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;

	// The actor has one shape, a sphere, 1m on radius
	NxCapsuleShapeDesc capsuleDesc;
	capsuleDesc.radius		= radius;
	capsuleDesc.height		= height;
	capsuleDesc.localPose.t = NxVec3(0, 0, 0);

	if(Pushable)
	{
		NxBodyDesc bodyDesc;
		actorDesc.body	= &bodyDesc;
		actorDesc.density = density;
		capsuleDesc.group = GROUP_COLLIDABLE_PUSHABLE;
	}
	else
		capsuleDesc.group = GROUP_COLLIDABLE_NON_PUSHABLE;

	actorDesc.shapes.pushBack(&capsuleDesc);
	actorDesc.globalPose.t	= StartPosition;

	return gScene->createActor(actorDesc);
}



/***********************************************************
	create character controller
***********************************************************/
unsigned int PhysXEngine::CreateCharacter(const NxVec3 & StartPosition, float radius, float height)
{
	NxCapsuleControllerDesc desc;
	desc.position.x		= StartPosition.x;
	desc.position.y		= StartPosition.y;
	desc.position.z		= StartPosition.z;
	desc.radius			= radius;
	desc.height			= height;
	desc.upDirection	= NX_Y;
	desc.slopeLimit		= 0;
	desc.skinWidth		= SKINWIDTH;
	desc.stepOffset		= radius * 0.5f;
	desc.callback		= &gControllerHitReport;
	gManager->createController(gScene, desc);

	return _current_controller_idx++;
}



/***********************************************************
 move character
 returned collision flags, collection of NxControllerFlag
***********************************************************/
unsigned int PhysXEngine::MoveCharacter(unsigned int characterIndex, const NxVec3& moveVector)
{
	NxU32 collisionFlags;

	NxController* ctrl = gManager->getController(characterIndex);
	if(ctrl)
		ctrl->move(moveVector, COLLIDABLE_MASK, 0.000001f, collisionFlags);

	return collisionFlags;
}



/***********************************************************
get gravity
***********************************************************/
void PhysXEngine::GetGravity(NxVec3 & Gravity)
{
	return gScene->getGravity(Gravity);
}



void PhysXEngine::GetCharacterPosition(unsigned int characterIndex, float &posX, float &posY, float &posZ)
{
	NxExtendedVec3 vec = gManager->getController(characterIndex)->getPosition();
	posX = (float)vec.x;
	posY = (float)vec.y;
	posZ = (float)vec.z;
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

	for(unsigned int i=0; i<gManager->getNbControllers(); ++i)
	{
		NxController* ctrl = gManager->getController(i);
		NxExtendedVec3 vec = ctrl->getPosition();

		glPushMatrix();
		glTranslated(vec.x, vec.y, vec.z);
		glColor4f(1.0f,1.0f,0.0f, 1.f);

		float _sizeX = 0.4f, _sizeY = 2.5f, _sizeZ = 0.4f;
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

		glPopMatrix();
	}


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
}


