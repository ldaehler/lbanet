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

#include "PlanesPhysicHandler.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <math.h>
#include <set>
#include <algorithm>

#include "LocalActorsHandler.h"
#include "ExternalActorsHandler.h"

#include <windows.h>    // Header File For Windows
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#include <GL/glu.h>     // Header File For The GLu32 Library


/*
--------------------------------------------------------------------------------------------------
- constructor
--------------------------------------------------------------------------------------------------
*/
PlanesPhysicHandler::PlanesPhysicHandler(const std::string filename, 
										 LocalActorsHandler * LAH, ExternalActorsHandler * EAH)
	: _localAH(LAH), _externalAH(EAH)
{

	std::ifstream file(filename.c_str());
	if(!file.is_open())
		return;

	int sizePlanes, sizewallX, sizewallZ, sizeStairs, sizecornerStairs;
	file>>sizePlanes;
	file>>sizewallX;
	file>>sizewallZ;
	file>>sizeStairs;
	file>>sizecornerStairs;

	for(int i=0; i<sizePlanes; ++i)
	{
		NormalPlane np;
		file>>np.Layer;
		file>>np.StartX;
		file>>np.StartZ;
		file>>np.EndX;
		file>>np.EndZ;
		file>>np.IsWater;
		_floors.push_back(np);
	}

	for(int i=0; i<sizewallX; ++i)
	{
		NormalPlane np;
		file>>np.Layer;
		file>>np.StartX;
		file>>np.StartZ;
		file>>np.EndX;
		file>>np.EndZ;
		_wallsX.push_back(np);
	}

	for(int i=0; i<sizewallZ; ++i)
	{
		NormalPlane np;
		file>>np.Layer;
		file>>np.StartX;
		file>>np.StartZ;
		file>>np.EndX;
		file>>np.EndZ;
		_wallsZ.push_back(np);
	}

	for(int i=0; i<sizeStairs; ++i)
	{
		StairPlane sp;
		file>>sp.C1X;
		file>>sp.C1Y;
		file>>sp.C1Z;
		file>>sp.C2X;
		file>>sp.C2Y;
		file>>sp.C2Z;
		file>>sp.C3X;
		file>>sp.C3Y;
		file>>sp.C3Z;
		file>>sp.C4X;
		file>>sp.C4Y;
		file>>sp.C4Z;
		_stairs.push_back(sp);
	}

	for(int i=0; i<sizecornerStairs; ++i)
	{
		CornerStairPlane sp;
		file>>sp.C1X;
		file>>sp.C1Y;
		file>>sp.C1Z;
		file>>sp.C2X;
		file>>sp.C2Y;
		file>>sp.C2Z;
		file>>sp.C3X;
		file>>sp.C3Y;
		file>>sp.C3Z;
		_corner_stairs.push_back(sp);
	}
}

/*
--------------------------------------------------------------------------------------------------
- denstructor
--------------------------------------------------------------------------------------------------
*/
PlanesPhysicHandler::~PlanesPhysicHandler()
{

}

/*
--------------------------------------------------------------------------------------------------
// check if it is possible to move from one position to another
// the actor has a bounding box centered on currX, currZ
// and which goes up from currY
// actorSizeX and actorSizeZ are 1/2 the diameter from the center
// the physic engine will apply a gravity force to the move if not flying
// so that the actor might fall down if needed
--------------------------------------------------------------------------------------------------
*/
MoveOutput PlanesPhysicHandler::MoveActor(long ActorId, const AABB & actorBB,
											const VECTOR &Speed)
{
	MoveOutput res;
	res.NewSpeed = Speed;
	res.TouchingWater = false;
	res.TouchingGround = true;

	return res;
}

/*
--------------------------------------------------------------------------------------------------
- return int > 0 if there is a roof on top of the given position
- function used to know when to cut the room display in half
--------------------------------------------------------------------------------------------------
*/
int PlanesPhysicHandler::IsUnderRoof(const VECTOR & ActorPos)
{
	return -1;
}



/*
--------------------------------------------------------------------------------------------------
- get closest floor from the actor position
--------------------------------------------------------------------------------------------------
*/
float PlanesPhysicHandler::GetClosestFloor(const VECTOR & ActorPos)
{
	return 0;
}



/*
--------------------------------------------------------------------------------------------------
- return a positive number giving the speed of the gravity on the Y axis
--------------------------------------------------------------------------------------------------
*/
float PlanesPhysicHandler::GetGravitySpeed()
{
	return 0.03f;
}


/*
--------------------------------------------------------------------------------------------------
- render physic shapes
--------------------------------------------------------------------------------------------------
*/
void PlanesPhysicHandler::Render()
{
	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

	glLineWidth(2.0f);


	for(size_t i=0; i<_corner_stairs.size(); ++i)
	{
		CornerStairPlane pif = _corner_stairs[i];
		glPushMatrix();

		glTranslated(0, 0.5, 0);
		glColor4f(0.0f, 0.0f, 1.0f, 1.f);
		glBegin(GL_LINES);
			glVertex3f(pif.C1X,pif.C1Y/2.0f,pif.C1Z);
			glVertex3f(pif.C2X,pif.C2Y/2.0f,pif.C2Z);
			glVertex3f(pif.C2X,pif.C2Y/2.0f,pif.C2Z);
			glVertex3f(pif.C3X,pif.C3Y/2.0f,pif.C3Z);
			glVertex3f(pif.C3X,pif.C3Y/2.0f,pif.C3Z);
			glVertex3f(pif.C1X,pif.C1Y/2.0f,pif.C1Z);
		glEnd();

		glPopMatrix();
	}


	for(size_t i=0; i<_stairs.size(); ++i)
	{
		StairPlane pif = _stairs[i];
		glPushMatrix();

		glTranslated(0, 0.5, 0);
		glColor4f(0.0f, 0.0f, 1.0f, 1.f);
		glBegin(GL_LINES);
			glVertex3f(pif.C1X,pif.C1Y/2.0f,pif.C1Z);
			glVertex3f(pif.C2X,pif.C2Y/2.0f,pif.C2Z);
			glVertex3f(pif.C2X,pif.C2Y/2.0f,pif.C2Z);
			glVertex3f(pif.C4X,pif.C4Y/2.0f,pif.C4Z);
			glVertex3f(pif.C4X,pif.C4Y/2.0f,pif.C4Z);
			glVertex3f(pif.C3X,pif.C3Y/2.0f,pif.C3Z);
			glVertex3f(pif.C3X,pif.C3Y/2.0f,pif.C3Z);
			glVertex3f(pif.C1X,pif.C1Y/2.0f,pif.C1Z);
		glEnd();

		glPopMatrix();
	}


	for(size_t i=0; i<_floors.size(); ++i)
	{
		NormalPlane pif = _floors[i];
		glPushMatrix();

		glTranslated(0, pif.Layer/2. + 0.5, 0);
		if(pif.IsWater)
			glColor4f(0.0f,1.0f,0.0f, 1.f);
		else
			glColor4f(0.0f,0.0f,1.0f, 1.f);

		glBegin(GL_LINES);
			glVertex3f(pif.StartX,0,pif.StartZ);
			glVertex3f(pif.EndX,0,pif.StartZ);
			glVertex3f(pif.EndX,0,pif.StartZ);
			glVertex3f(pif.EndX,0,pif.EndZ);
			glVertex3f(pif.EndX,0,pif.EndZ);
			glVertex3f(pif.StartX,0,pif.EndZ);
			glVertex3f(pif.StartX,0,pif.EndZ);
			glVertex3f(pif.StartX,0,pif.StartZ);
		glEnd();

		glPopMatrix();
	}


	for(size_t i=0; i<_wallsX.size(); ++i)
	{
		NormalPlane pif = _wallsX[i];
		glPushMatrix();

		glTranslated(pif.Layer, 0, 0);
		glColor4f(1.0f,0.0f,0.0f, 1.f);
		glBegin(GL_LINES);
			glVertex3f(0,pif.StartX/2.f,pif.StartZ);
			glVertex3f(0,pif.EndX/2.f,pif.StartZ);
			glVertex3f(0,pif.EndX/2.f,pif.StartZ);
			glVertex3f(0,pif.EndX/2.f,pif.EndZ);
			glVertex3f(0,pif.EndX/2.f,pif.EndZ);
			glVertex3f(0,pif.StartX/2.f,pif.EndZ);
			glVertex3f(0,pif.StartX/2.f,pif.EndZ);
			glVertex3f(0,pif.StartX/2.f,pif.StartZ);
		glEnd();

		glPopMatrix();
	}



	for(size_t i=0; i<_wallsZ.size(); ++i)
	{
		NormalPlane pif = _wallsZ[i];
		glPushMatrix();

		glTranslated(0, 0, pif.Layer);
		glColor4f(1.0f,0.0f,1.0f, 1.f);
		glBegin(GL_LINES);
			glVertex3f(pif.StartX,pif.StartZ/2.f,0);
			glVertex3f(pif.EndX,pif.StartZ/2.f,0);
			glVertex3f(pif.EndX,pif.StartZ/2.f,0);
			glVertex3f(pif.EndX,pif.EndZ/2.f,0);
			glVertex3f(pif.EndX,pif.EndZ/2.f,0);
			glVertex3f(pif.StartX,pif.EndZ/2.f,0);
			glVertex3f(pif.StartX,pif.EndZ/2.f,0);
			glVertex3f(pif.StartX,pif.StartZ/2.f,0);
		glEnd();

		glPopMatrix();
	}



	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
}



