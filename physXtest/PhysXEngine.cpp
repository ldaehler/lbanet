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



#include <windows.h>    // Header File For Windows
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#include <GL/glu.h>     // Header File For The GLu32 Library


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
{

}


/***********************************************************
	Destructor
***********************************************************/
PhysXEngine::~PhysXEngine()
{

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
	CreatePlane(NxVec3(0, 0, 0), NxVec3(0, 1, 0));
	CreatePlane(NxVec3(10, 0, 0), NxVec3(1, 0, 0));

	CreateBox(NxVec3(0, 10, 0), 0.5f, 0.5f, 0.5f, 10);

	// init time
	_lasttime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeDouble();

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
}


/***********************************************************
	create plane actor
***********************************************************/
NxActor* PhysXEngine::CreatePlane(const NxVec3 & StartPosition, const NxVec3 & PlaneNormal)
{
    // Create a plane with default descriptor
    NxPlaneShapeDesc planeDesc;
	planeDesc.normal = PlaneNormal;

    NxActorDesc actorDesc;
    actorDesc.shapes.pushBack(&planeDesc);
	actorDesc.globalPose.t	= StartPosition;	

    return gScene->createActor(actorDesc);
}


/***********************************************************
	create box actor
***********************************************************/
NxActor* PhysXEngine::CreateBox(const NxVec3 & StartPosition, float dimX, float dimY, float dimZ, 
								float density)
{

	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;

	// The actor has one shape, a box, 1m on a side
	NxBoxShapeDesc boxDesc;
	boxDesc.dimensions.set(dimX, dimY, dimZ);
	boxDesc.localPose.t = NxVec3(0, 0, 0);
	actorDesc.shapes.pushBack(&boxDesc);

	actorDesc.body			= &bodyDesc;
	actorDesc.density		= density;
	actorDesc.globalPose.t	= StartPosition;	
	assert(actorDesc.isValid());
	NxActor *pActor = gScene->createActor(actorDesc);	
	assert(pActor);

	return pActor;
}

/***********************************************************
	create sphere actor
***********************************************************/
NxActor* PhysXEngine::CreateSphere(const NxVec3 & StartPosition, float radius, float density)
{
	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;

	// The actor has one shape, a sphere, 1m on radius
	NxSphereShapeDesc sphereDesc;
	sphereDesc.radius		= radius;
	sphereDesc.localPose.t	= NxVec3(0, 0, 0);

	actorDesc.shapes.pushBack(&sphereDesc);
	actorDesc.body			= &bodyDesc;
	actorDesc.density		= density;
	actorDesc.globalPose.t	= StartPosition;	
	return gScene->createActor(actorDesc);
}

/***********************************************************
	create capsule actor
***********************************************************/
NxActor* PhysXEngine::CreateCapsule(const NxVec3 & StartPosition, float radius, float height, float density)
{
	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;

	// The actor has one shape, a sphere, 1m on radius
	NxCapsuleShapeDesc capsuleDesc;
	capsuleDesc.radius		= radius;
	capsuleDesc.height		= height;
	capsuleDesc.localPose.t = NxVec3(0, 0, 0);
	
	//Rotate capsule shape
	//NxQuat quat(45, NxVec3(0, 0, 1));
	//NxMat33 m33(quat);
	//capsuleDesc.localPose.M = m33;

	actorDesc.shapes.pushBack(&capsuleDesc);
	actorDesc.body			= &bodyDesc;
	actorDesc.density		= density;
	actorDesc.globalPose.t	= StartPosition;

	return gScene->createActor(actorDesc);
}


/***********************************************************
	Render actors
***********************************************************/
void PhysXEngine::RenderActors()
{
    // Render all the actors in the scene
    NxU32 nbActors = gScene->getNbActors();
    NxActor** actors = gScene->getActors();
    while (nbActors--)
    {
        NxActor* actor = *actors++;

		glEnable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);

		glLineWidth(2.0f);


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


		//glBegin(GL_LINES);
		//	glVertex3f(-1,0,0);
		//	glVertex3f(1,0,0);
		//	glVertex3f(0,0,-1);
		//	glVertex3f(0,0,+1);
		//glEnd();

		
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
		else
		{
			for(int i=1; i<100; ++i)
			{
				glColor4f(1.0f,0.0f,0.0f, 1.f);
				float _sizeX = i, _sizeY = 0, _sizeZ = i;
				glBegin(GL_LINES);
					glVertex3f(-_sizeX,-_sizeY,-_sizeZ);
					glVertex3f(_sizeX,-_sizeY,-_sizeZ);
					glVertex3f(_sizeX,-_sizeY,-_sizeZ);
					glVertex3f(_sizeX,-_sizeY,_sizeZ);
					glVertex3f(_sizeX,-_sizeY,_sizeZ);
					glVertex3f(-_sizeX,-_sizeY,_sizeZ);
					glVertex3f(-_sizeX,-_sizeY,_sizeZ);
					glVertex3f(-_sizeX,-_sizeY,-_sizeZ);
				glEnd();
			}
		}


		glPopMatrix();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);

    }
}