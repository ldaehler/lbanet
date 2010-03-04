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
#include "ODEPhysicHandler.h"
#include "SynchronizedTimeHandler.h"


ODEPhysicHandler* ODEPhysicHandler::_singletonInstance = NULL;


/***********************************************************
ODE collision callback
***********************************************************/
void nearCallback(void *data, dGeomID o0, dGeomID o1)
{
	reinterpret_cast<ODEPhysicHandler*>(data)->handleCollisionBetween(o0,o1);
} 


/***********************************************************
singleton pattern
***********************************************************/
ODEPhysicHandler * ODEPhysicHandler::getInstance()
{
    if(!_singletonInstance)
        _singletonInstance = new ODEPhysicHandler();

	return _singletonInstance;
}


/***********************************************************
	Constructor
***********************************************************/
ODEPhysicHandler::ODEPhysicHandler()
{

}


/***********************************************************
	Destructor
***********************************************************/
ODEPhysicHandler::~ODEPhysicHandler()
{

}



/***********************************************************
	init function
***********************************************************/
void ODEPhysicHandler::Init()
{
	dInitODE2(0);
	dAllocateODEDataForThread(dAllocateFlagCollisionData);

	_world = dWorldCreate();
	_space = dHashSpaceCreate(0);

	// Set up gravity force
	dWorldSetGravity(_world, 0, -9.81f, 0);

	// Create contact group
	_contactgroup = dJointGroupCreate(0);
}

/***********************************************************
	quit function
***********************************************************/
void ODEPhysicHandler::Quit()
{
	dSpaceDestroy(_space);
	dWorldDestroy(_world);
	dCloseODE();  
}



/***********************************************************
	Process function
***********************************************************/
void ODEPhysicHandler::Process()
{
	double currentime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeDouble();
	float tdiff = (float)(currentime - _lasttime);


	static float nbSecondsByStep = 0.001f;

	// Find the time elapsed between last time
	float nbSecsElapsed = tdiff/1000.0f;

	// Find the corresponding number of steps that must be taken
	int nbStepsToPerform = static_cast<int>(nbSecsElapsed/nbSecondsByStep);

	// Make these steps to advance world time
	for (int i=0;i<nbStepsToPerform;++i)
	{
		// Detect collision
		dSpaceCollide(_space, this, &nearCallback);

		// Step world
		dWorldQuickStep(_world, nbSecondsByStep);

		// Remove all temporary collision joints now that the world has been stepped
		dJointGroupEmpty(_contactgroup); 
	}

	_lasttime = currentime;
}


/***********************************************************
	hanlde collision
***********************************************************/
void ODEPhysicHandler::handleCollisionBetween(dGeomID o0, dGeomID o1)
{
	// Create an array of dContact objects to hold the contact joints
	static const int MAX_CONTACTS = 10;
	dContact contact[MAX_CONTACTS];

	for (int i = 0; i < MAX_CONTACTS; i++)
	{
		contact[i].surface.mode = dContactBounce | dContactSoftCFM;
		contact[i].surface.mu = dInfinity;
		contact[i].surface.mu2 = 0;
		contact[i].surface.bounce = 0.8;
		contact[i].surface.bounce_vel = 0.1;
		contact[i].surface.soft_cfm = 0.01;
	}
	if (int numc = dCollide(o0, o1, MAX_CONTACTS, &contact[0].geom, sizeof(dContact)))
	{
		// Get the dynamics body for each geom
		dBodyID b1 = dGeomGetBody(o0);
		dBodyID b2 = dGeomGetBody(o1);
		// To add each contact point found to our joint group we call dJointCreateContact which is just one of the many
		// different joint types available.  
		for (int i = 0; i < numc; i++)
		{
			// dJointCreateContact needs to know which world and joint group to work with as well as the dContact
			// object itself. It returns a new dJointID which we then use with dJointAttach to finally create the
			// temporary contact joint between the two geom bodies.
			dJointID c = dJointCreateContact(_world, _contactgroup, contact + i);
			dJointAttach(c, b1, b2);
		}
	}	
}