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

#if !defined(__LbaNetModel_1_ODEPhysicHandler_h)
#define __LbaNetModel_1_ODEPhysicHandler_h

#include <ode/ode.h>


/***********************************************************************
 * Module:  ODEPhysicHandler.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:59:34
 * Purpose: Declaration of the class InventoryHandler
 ***********************************************************************/
class ODEPhysicHandler
{
public:

	// singleton pattern
	static ODEPhysicHandler * getInstance();

	//! destructor
	~ODEPhysicHandler();


	//! init function
	void Init();

	//! quit function
	void Quit();

	//! Process function
	void Process();

	//! handle collisions
	void handleCollisionBetween(dGeomID o0, dGeomID o1);

protected:
	//! constructor
	ODEPhysicHandler();


private:
	// singleton
	static ODEPhysicHandler *				_singletonInstance;


	dWorldID		_world;
	dSpaceID		_space;
	dJointGroupID	_contactgroup;

	double			_lasttime;
};

#endif