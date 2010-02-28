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

		float sx, sz, ex, ez;
		file>>sx;
		file>>sz;
		file>>ex;
		file>>ez;
		np.Square._minX = MIN(sx, ex);
		np.Square._maxX = MAX(sx, ex);
		np.Square._minZ = MIN(sz, ez);
		np.Square._maxZ = MAX(sz, ez);

		file>>np.IsWater;
		_floors[np.Layer].push_back(np);
	}

	for(int i=0; i<sizewallX; ++i)
	{
		NormalPlane np;
		file>>np.Layer;

		float sx, sz, ex, ez;
		file>>sx;
		file>>sz;
		file>>ex;
		file>>ez;
		np.Square._minX = MIN(sx, ex);
		np.Square._maxX = MAX(sx, ex);
		np.Square._minZ = MIN(sz, ez);
		np.Square._maxZ = MAX(sz, ez);

		_wallsX[np.Layer].push_back(np);
	}

	for(int i=0; i<sizewallZ; ++i)
	{
		NormalPlane np;
		file>>np.Layer;

		float sx, sz, ex, ez;
		file>>sx;
		file>>sz;
		file>>ex;
		file>>ez;
		np.Square._minX = MIN(sx, ex);
		np.Square._maxX = MAX(sx, ex);
		np.Square._minZ = MIN(sz, ez);
		np.Square._maxZ = MAX(sz, ez);

		_wallsZ[np.Layer].push_back(np);
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
	res.TouchingGround = false;

	float ModifiedSpeedY;
	if(ColisionWithFloor(actorBB, res.NewSpeed, ModifiedSpeedY, res.TouchingWater))
	{
		res.TouchingGround = true;
		res.NewSpeed.y = ModifiedSpeedY;
	}

	float ModifiedSpeedX;
	if(ColisionWithWallX(actorBB, res.NewSpeed, ModifiedSpeedX))
	{
		res.NewSpeed.x = ModifiedSpeedX;
	}

	float ModifiedSpeedZ;
	if(ColisionWithWallZ(actorBB, res.NewSpeed, ModifiedSpeedZ))
	{
		res.NewSpeed.z = ModifiedSpeedZ;
	}


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

	std::map<int, std::vector<NormalPlane> >::iterator itfloor = _floors.begin();
	std::map<int, std::vector<NormalPlane> >::iterator endfloor = _floors.end();
	for(; itfloor != endfloor; ++itfloor)
	{
		for(size_t i=0; i<itfloor->second.size(); ++i)
		{
			NormalPlane pif = itfloor->second[i];
			glPushMatrix();

			glTranslated(0, pif.Layer/2. + 0.5, 0);
			if(pif.IsWater)
				glColor4f(0.0f,1.0f,0.0f, 1.f);
			else
				glColor4f(0.0f,0.0f,1.0f, 1.f);

			glBegin(GL_LINES);
				glVertex3f(pif.Square._minX,0,pif.Square._minZ);
				glVertex3f(pif.Square._maxX,0,pif.Square._minZ);
				glVertex3f(pif.Square._maxX,0,pif.Square._minZ);
				glVertex3f(pif.Square._maxX,0,pif.Square._maxZ);
				glVertex3f(pif.Square._maxX,0,pif.Square._maxZ);
				glVertex3f(pif.Square._minX,0,pif.Square._maxZ);
				glVertex3f(pif.Square._minX,0,pif.Square._maxZ);
				glVertex3f(pif.Square._minX,0,pif.Square._minZ);
			glEnd();

			glPopMatrix();
		}
	}

	std::map<int, std::vector<NormalPlane> >::iterator itwallsX = _wallsX.begin();
	std::map<int, std::vector<NormalPlane> >::iterator endwallsX = _wallsX.end();
	for(; itwallsX != endwallsX; ++itwallsX)
	{
		for(size_t i=0; i<itwallsX->second.size(); ++i)
		{
			NormalPlane pif = itwallsX->second[i];
			glPushMatrix();

			glTranslated(pif.Layer, 0, 0);
			glColor4f(1.0f,0.0f,0.0f, 1.f);
			glBegin(GL_LINES);
				glVertex3f(0,pif.Square._minX/2.f,pif.Square._minZ);
				glVertex3f(0,pif.Square._maxX/2.f,pif.Square._minZ);
				glVertex3f(0,pif.Square._maxX/2.f,pif.Square._minZ);
				glVertex3f(0,pif.Square._maxX/2.f,pif.Square._maxZ);
				glVertex3f(0,pif.Square._maxX/2.f,pif.Square._maxZ);
				glVertex3f(0,pif.Square._minX/2.f,pif.Square._maxZ);
				glVertex3f(0,pif.Square._minX/2.f,pif.Square._maxZ);
				glVertex3f(0,pif.Square._minX/2.f,pif.Square._minZ);
			glEnd();

			glPopMatrix();
		}
	}


	std::map<int, std::vector<NormalPlane> >::iterator itwallsZ = _wallsZ.begin();
	std::map<int, std::vector<NormalPlane> >::iterator endwallsZ = _wallsZ.end();
	for(; itwallsZ != endwallsZ; ++itwallsZ)
	{
		for(size_t i=0; i<itwallsZ->second.size(); ++i)
		{
			NormalPlane pif = itwallsZ->second[i];
			glPushMatrix();

			glTranslated(0, 0, pif.Layer);
			glColor4f(1.0f,0.0f,1.0f, 1.f);
			glBegin(GL_LINES);
				glVertex3f(pif.Square._minX,pif.Square._minZ/2.f,0);
				glVertex3f(pif.Square._maxX,pif.Square._minZ/2.f,0);
				glVertex3f(pif.Square._maxX,pif.Square._minZ/2.f,0);
				glVertex3f(pif.Square._maxX,pif.Square._maxZ/2.f,0);
				glVertex3f(pif.Square._maxX,pif.Square._maxZ/2.f,0);
				glVertex3f(pif.Square._minX,pif.Square._maxZ/2.f,0);
				glVertex3f(pif.Square._minX,pif.Square._maxZ/2.f,0);
				glVertex3f(pif.Square._minX,pif.Square._minZ/2.f,0);
			glEnd();

			glPopMatrix();
		}
	}



	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
}


/*
--------------------------------------------------------------------------------------------------
- check collision with floor
--------------------------------------------------------------------------------------------------
*/
bool PlanesPhysicHandler::ColisionWithFloor(const AABB & actorBB, const VECTOR &Speed, 
											float &ModifiedSpeedY, bool &water)
{
	float moveY = Speed.y;
	if(moveY == 0)
		return false;

	// calculate norm of speed
	VECTOR speedNorm = Speed.unit();
	std::vector<int> tovisit;
	float startY = 0;

	// if we go upward
	if(moveY > 0)
	{
		startY = actorBB.E.y;
		int stY = (int)(startY-0.0001f)+1;	// max actor Y
		int stopY = (int)(startY+moveY);
		for(;stY <= stopY; ++stY)
			tovisit.push_back(stY);
	}
	else // if we go backward
	{
		startY = actorBB.P.y;
		int stY = (int)startY;	// min actor Y
		int stopY = (int)(startY+moveY-0.0001f)+1;
		for(;stY >= stopY; --stY)
			tovisit.push_back(stY);
	}


	for(size_t i=0; i<tovisit.size(); ++i)
	{
		const std::vector<NormalPlane> & vecfloor = _floors[tovisit[i]];
		std::vector<NormalPlane>::const_iterator it = vecfloor.begin();
		std::vector<NormalPlane>::const_iterator end = vecfloor.end();

		// keep memory of the last checked layer
		float lastcheckedLayer = -1;
		Square2D memorysquare;

		// check until the planes beeing after the actor move
		for(; it != end; ++it)
		{
			if(it->Layer != lastcheckedLayer)
			{
				lastcheckedLayer = it->Layer;
				float distance = (lastcheckedLayer-startY) / speedNorm.y;

				float offsetx = speedNorm.x * distance;
				float offsetz = speedNorm.z * distance;

				memorysquare._minX = (actorBB.P.x + offsetx);
				memorysquare._maxX = (actorBB.E.x + offsetx);
				memorysquare._minZ = (actorBB.P.z + offsetz);
				memorysquare._maxZ = (actorBB.E.z + offsetz);
			}
			
			if(it->Square.Overlap(memorysquare))
			{
				ModifiedSpeedY = (lastcheckedLayer - startY);
				ModifiedSpeedY += (ModifiedSpeedY > 0) ? -0.0001 : 0.0001;
				water = it->IsWater;
				return true;
			}
		}
	}

	return false;
}


/*
--------------------------------------------------------------------------------------------------
- check collision with wall X
--------------------------------------------------------------------------------------------------
*/
bool PlanesPhysicHandler::ColisionWithWallX(const AABB & actorBB, const VECTOR &Speed, float &ModifiedSpeedX)
{
	float moveX = Speed.x;
	if(moveX == 0)
		return false;

	// calculate norm of speed
	VECTOR speedNorm = Speed.unit();
	std::vector<int> tovisit;
	float startX = 0;

	// if we go upward
	if(moveX > 0)
	{
		startX = actorBB.E.x;
		int stX = (int)(startX-0.0001f)+1;	// max actor X
		int stopX = (int)(startX+moveX);
		for(;stX <= stopX; ++stX)
			tovisit.push_back(stX);
	}
	else // if we go backward
	{
		startX = actorBB.P.x;
		int stX = (int)startX;	// min actor X
		int stopX = (int)(startX+moveX-0.0001f)+1;
		for(;stX >= stopX; --stX)
			tovisit.push_back(stX);
	}

	for(size_t i=0; i<tovisit.size(); ++i)
	{
		const std::vector<NormalPlane> & vecfloor = _wallsX[tovisit[i]];
		std::vector<NormalPlane>::const_iterator it = vecfloor.begin();
		std::vector<NormalPlane>::const_iterator end = vecfloor.end();

		// keep memory of the last checked layer
		float lastcheckedLayer = -1;
		Square2D memorysquare;

		// check until the planes beeing after the actor move
		for(; it != end; ++it)
		{
			if(it->Layer != lastcheckedLayer)
			{
				lastcheckedLayer = it->Layer;
				float distance = (lastcheckedLayer-startX) / speedNorm.x;

				float offsety = speedNorm.y * distance;
				float offsetz = speedNorm.z * distance;

				memorysquare._minX = (actorBB.P.y + offsety);
				memorysquare._maxX = (actorBB.E.y + offsety);
				memorysquare._minZ = (actorBB.P.z + offsetz);
				memorysquare._maxZ = (actorBB.E.z + offsetz);
			}
			
			if(it->Square.Overlap(memorysquare))
			{
				ModifiedSpeedX = (lastcheckedLayer - startX);
				ModifiedSpeedX += (ModifiedSpeedX > 0) ? -0.0001 : 0.0001;
				return true;
			}
		}
	}

	return false;

}

/*
--------------------------------------------------------------------------------------------------
- check collision with wall Z
--------------------------------------------------------------------------------------------------
*/
bool PlanesPhysicHandler::ColisionWithWallZ(const AABB & actorBB, const VECTOR &Speed, float &ModifiedSpeedZ)
{
	float moveZ = Speed.z;
	if(moveZ == 0)
		return false;

	// calculate norm of speed
	VECTOR speedNorm = Speed.unit();
	std::vector<int> tovisit;
	float startZ = 0;

	// if we go upward
	if(moveZ > 0)
	{
		startZ = actorBB.E.z;
		int stZ = (int)(startZ-0.0001f)+1;	// max actor Z
		int stopZ = (int)(startZ+moveZ);
		for(;stZ <= stopZ; ++stZ)
			tovisit.push_back(stZ);
	}
	else // if we go backward
	{
		startZ = actorBB.P.z;
		int stZ = (int)startZ;	// min actor Z
		int stopZ = (int)(startZ+moveZ-0.0001f)+1;
		for(;stZ >= stopZ; --stZ)
			tovisit.push_back(stZ);
	}


	for(size_t i=0; i<tovisit.size(); ++i)
	{
		const std::vector<NormalPlane> & vecfloor = _wallsZ[tovisit[i]];
		std::vector<NormalPlane>::const_iterator it = vecfloor.begin();
		std::vector<NormalPlane>::const_iterator end = vecfloor.end();

		// keep memory of the last checked layer
		float lastcheckedLayer = -1;
		Square2D memorysquare;

		// check until the planes beeing after the actor move
		for(; it != end; ++it)
		{
			if(it->Layer != lastcheckedLayer)
			{
				lastcheckedLayer = it->Layer;
				float distance = (lastcheckedLayer-startZ) / speedNorm.z;

				float offsetx = speedNorm.x * distance;
				float offsety = speedNorm.y * distance;

				memorysquare._minX = (actorBB.P.x + offsetx);
				memorysquare._maxX = (actorBB.E.x + offsetx);
				memorysquare._minZ = (actorBB.P.y + offsety);
				memorysquare._maxZ = (actorBB.E.y + offsety);
			}
			
			if(it->Square.Overlap(memorysquare))
			{
				ModifiedSpeedZ = (lastcheckedLayer - startZ);
				ModifiedSpeedZ += (ModifiedSpeedZ > 0) ? -0.0001 : 0.0001;
				return true;
			}
		}
	}

	return false;
}

