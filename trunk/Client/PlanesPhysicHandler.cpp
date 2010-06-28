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
		file>>sp.C1.x;
		file>>sp.C1.y;
		file>>sp.C1.z;
		file>>sp.C2.x;
		file>>sp.C2.y;
		file>>sp.C2.z;
		file>>sp.C3.x;
		file>>sp.C3.y;
		file>>sp.C3.z;
		file>>sp.C4.x;
		file>>sp.C4.y;
		file>>sp.C4.z;

		sp.Normal = ((sp.C2 - sp.C1).cross(sp.C3-sp.C1)).unit();
		sp.D = sp.Normal.dot(sp.C1);


		// calculate min max X
		{
			sp.minX = sp.C1.x;
			sp.maxX = sp.C1.x;

			if(sp.C2.x > sp.maxX)
				sp.maxX = sp.C2.x;

			if(sp.C2.x < sp.minX)
				sp.minX = sp.C2.x;

			if(sp.C3.x > sp.maxX)
				sp.maxX = sp.C3.x;

			if(sp.C3.x < sp.minX)
				sp.minX = sp.C3.x;

			if(sp.C4.x > sp.maxX)
				sp.maxX = sp.C4.x;

			if(sp.C4.x < sp.minX)
				sp.minX = sp.C4.x;
		}


		// calculate min max Z
		{
			sp.minZ = sp.C1.z;
			sp.maxZ = sp.C1.z;

			if(sp.C2.z > sp.maxZ)
				sp.maxZ = sp.C2.z;

			if(sp.C2.z < sp.minZ)
				sp.minZ = sp.C2.z;

			if(sp.C3.z > sp.maxZ)
				sp.maxZ = sp.C3.z;

			if(sp.C3.z < sp.minZ)
				sp.minZ = sp.C3.z;

			if(sp.C4.z > sp.maxZ)
				sp.maxZ = sp.C4.z;

			if(sp.C4.z < sp.minZ)
				sp.minZ = sp.C4.z;
		}

		_stairs.push_back(sp);
	}

	for(int i=0; i<sizecornerStairs; ++i)
	{
		CornerStairPlane sp;
		file>>sp.C1.x;
		file>>sp.C1.y;
		file>>sp.C1.z;
		file>>sp.C2.x;
		file>>sp.C2.y;
		file>>sp.C2.z;
		file>>sp.C3.x;
		file>>sp.C3.y;
		file>>sp.C3.z;

		sp.tr_wh_y = Triangle2D(Point2D(sp.C1.x, sp.C1.z), Point2D(sp.C2.x, sp.C2.z), Point2D(sp.C3.x, sp.C3.z));

		sp.Normal = ((sp.C2 - sp.C1).cross(sp.C3-sp.C1)).unit();
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

	bool collisioned = true;
	while(collisioned)
	{
		collisioned = false;

		// if already on stairs do a quick check of the actor center with the ground
		if(PointColisionWithFloor(actorBB, res.NewSpeed, ModifiedSpeedY, res.TouchingWater))
		{
			res.TouchingGround = true;
			if(res.NewSpeed.y != ModifiedSpeedY)
			{
				res.NewSpeed.y = ModifiedSpeedY;
				collisioned = true;
			}
		}

		// check if actor is on stairs
		if(ColisionWithStair(actorBB, res.NewSpeed, res.NewSpeed))
		{
			res.TouchingGround = true;
			collisioned = true;
		}

		// check if colision with corner stairs
		if(ColisionWithCornerStair(actorBB, res.NewSpeed, res.NewSpeed))
		{
			res.TouchingGround = true;
			collisioned = true;
		}
		
		if(!res.TouchingGround)
		//{
		//	// if already on stairs do a quick check of the actor center with the ground
		//	if(PointColisionWithFloor(actorBB, res.NewSpeed, ModifiedSpeedY, res.TouchingWater))
		//	{
		//		res.TouchingGround = true;
		//		res.NewSpeed.y = ModifiedSpeedY;
		//		collisioned = true;
		//	}
		//}
		//else
		{
			// if actor still not touching ground then do a complete check
			if(ColisionWithFloor(actorBB, res.NewSpeed, ModifiedSpeedY, res.TouchingWater))
			{
				if(res.NewSpeed.y != ModifiedSpeedY)
				{
					res.TouchingGround = true;
					res.NewSpeed.y = ModifiedSpeedY;
					collisioned = true;
				}
			}
		}

		// shifting check order every frame so that we do not get stuck were we should not
		if(_shiftcheck)
		{
			float ModifiedSpeedX, ModifiedSpeedY;
			if(ColisionWithWallX(actorBB, res.NewSpeed, ModifiedSpeedX, ModifiedSpeedY))
			{	
				if(res.NewSpeed.x != ModifiedSpeedX)
				{
					res.NewSpeed.x = ModifiedSpeedX;
					res.NewSpeed.y = ModifiedSpeedY;
					collisioned = true;
				}
			}

			float ModifiedSpeedZ;
			if(ColisionWithWallZ(actorBB, res.NewSpeed, ModifiedSpeedZ, ModifiedSpeedY))
			{
				if(res.NewSpeed.z != ModifiedSpeedZ)
				{
					res.NewSpeed.z = ModifiedSpeedZ;	
					res.NewSpeed.y = ModifiedSpeedY;
					collisioned = true;
				}
			}
		}
		else
		{
			float ModifiedSpeedZ, ModifiedSpeedY;
			if(ColisionWithWallZ(actorBB, res.NewSpeed, ModifiedSpeedZ, ModifiedSpeedY))
			{
				if(res.NewSpeed.z != ModifiedSpeedZ)
				{
					res.NewSpeed.z = ModifiedSpeedZ;
					res.NewSpeed.y = ModifiedSpeedY;
					collisioned = true;
				}
			}

			float ModifiedSpeedX;
			if(ColisionWithWallX(actorBB, res.NewSpeed, ModifiedSpeedX, ModifiedSpeedY))
			{
				if(res.NewSpeed.x != ModifiedSpeedX)
				{
					res.NewSpeed.x = ModifiedSpeedX;
					res.NewSpeed.y = ModifiedSpeedY;
					collisioned = true;
				}
			}
		}
	}


	_shiftcheck = !_shiftcheck;
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
int PlanesPhysicHandler::GetClosestFloor(const VECTOR & ActorPos)
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


	//for(size_t i=0; i<_corner_stairs.size(); ++i)
	//{
	//	CornerStairPlane pif = _corner_stairs[i];
	//	glPushMatrix();

	//	glTranslated(0, 0.5, 0);
	//	glColor4f(0.0f, 0.0f, 1.0f, 1.f);
	//	glBegin(GL_LINES);
	//		glVertex3f(pif.C1.x,pif.C1.y/2.0f,pif.C1.z);
	//		glVertex3f(pif.C2.x,pif.C2.y/2.0f,pif.C2.z);
	//		glVertex3f(pif.C2.x,pif.C2.y/2.0f,pif.C2.z);
	//		glVertex3f(pif.C3.x,pif.C3.y/2.0f,pif.C3.z);
	//		glVertex3f(pif.C3.x,pif.C3.y/2.0f,pif.C3.z);
	//		glVertex3f(pif.C1.x,pif.C1.y/2.0f,pif.C1.z);
	//	glEnd();

	//	glPopMatrix();
	//}


	//for(size_t i=0; i<_stairs.size(); ++i)
	//{
	//	StairPlane pif = _stairs[i];
	//	glPushMatrix();

	//	glTranslated(0, 0.5, 0);
	//	glColor4f(0.0f, 0.0f, 1.0f, 1.f);
	//	glBegin(GL_LINES);
	//		glVertex3f(pif.C1.x,pif.C1.y/2.0f,pif.C1.z);
	//		glVertex3f(pif.C2.x,pif.C2.y/2.0f,pif.C2.z);
	//		glVertex3f(pif.C2.x,pif.C2.y/2.0f,pif.C2.z);
	//		glVertex3f(pif.C4.x,pif.C4.y/2.0f,pif.C4.z);
	//		glVertex3f(pif.C4.x,pif.C4.y/2.0f,pif.C4.z);
	//		glVertex3f(pif.C3.x,pif.C3.y/2.0f,pif.C3.z);
	//		glVertex3f(pif.C3.x,pif.C3.y/2.0f,pif.C3.z);
	//		glVertex3f(pif.C1.x,pif.C1.y/2.0f,pif.C1.z);
	//	glEnd();

	//	glPopMatrix();
	//}

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

	//std::map<int, std::vector<NormalPlane> >::iterator itwallsX = _wallsX.begin();
	//std::map<int, std::vector<NormalPlane> >::iterator endwallsX = _wallsX.end();
	//for(; itwallsX != endwallsX; ++itwallsX)
	//{
	//	for(size_t i=0; i<itwallsX->second.size(); ++i)
	//	{
	//		NormalPlane pif = itwallsX->second[i];
	//		glPushMatrix();

	//		glTranslated(pif.Layer, 0.5f, 0);
	//		glColor4f(1.0f,0.0f,0.0f, 1.f);
	//		glBegin(GL_LINES);
	//			glVertex3f(0,pif.Square._minX/2.f,pif.Square._minZ);
	//			glVertex3f(0,pif.Square._maxX/2.f,pif.Square._minZ);
	//			glVertex3f(0,pif.Square._maxX/2.f,pif.Square._minZ);
	//			glVertex3f(0,pif.Square._maxX/2.f,pif.Square._maxZ);
	//			glVertex3f(0,pif.Square._maxX/2.f,pif.Square._maxZ);
	//			glVertex3f(0,pif.Square._minX/2.f,pif.Square._maxZ);
	//			glVertex3f(0,pif.Square._minX/2.f,pif.Square._maxZ);
	//			glVertex3f(0,pif.Square._minX/2.f,pif.Square._minZ);
	//		glEnd();

	//		glPopMatrix();
	//	}
	//}


	//std::map<int, std::vector<NormalPlane> >::iterator itwallsZ = _wallsZ.begin();
	//std::map<int, std::vector<NormalPlane> >::iterator endwallsZ = _wallsZ.end();
	//for(; itwallsZ != endwallsZ; ++itwallsZ)
	//{
	//	for(size_t i=0; i<itwallsZ->second.size(); ++i)
	//	{
	//		NormalPlane pif = itwallsZ->second[i];
	//		glPushMatrix();

	//		glTranslated(0, 0.5f, pif.Layer);
	//		glColor4f(1.0f,0.0f,1.0f, 1.f);
	//		glBegin(GL_LINES);
	//			glVertex3f(pif.Square._minX,pif.Square._minZ/2.f,0);
	//			glVertex3f(pif.Square._maxX,pif.Square._minZ/2.f,0);
	//			glVertex3f(pif.Square._maxX,pif.Square._minZ/2.f,0);
	//			glVertex3f(pif.Square._maxX,pif.Square._maxZ/2.f,0);
	//			glVertex3f(pif.Square._maxX,pif.Square._maxZ/2.f,0);
	//			glVertex3f(pif.Square._minX,pif.Square._maxZ/2.f,0);
	//			glVertex3f(pif.Square._minX,pif.Square._maxZ/2.f,0);
	//			glVertex3f(pif.Square._minX,pif.Square._minZ/2.f,0);
	//		glEnd();

	//		glPopMatrix();
	//	}
	//}



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

		float tmpY = startY+moveY;
		int stopY = (int)(tmpY-0.0001f)+1;
		if(tmpY < 0)
			stopY = 0;
		if(stopY < 0)
			stopY = 0;

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
				lastcheckedLayer = (float)it->Layer;
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
				ModifiedSpeedY += (ModifiedSpeedY > 0) ? -0.0001f : 0.0001f;
				water = it->IsWater;
				return true;
			}
		}
	}

	return false;
}




/*
--------------------------------------------------------------------------------------------------
- check collision with floor
--------------------------------------------------------------------------------------------------
*/
bool PlanesPhysicHandler::PointColisionWithFloor(const AABB & actorBB, const VECTOR &Speed, 
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

		float tmpY = startY+moveY;
		int stopY = (int)(tmpY-0.0001f)+1;
		if(tmpY < 0)
			stopY = 0;
		if(stopY < 0)
			stopY = 0;

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
		float ptx, ptz;

		// check until the planes beeing after the actor move
		for(; it != end; ++it)
		{
			if(it->Layer != lastcheckedLayer)
			{
				lastcheckedLayer = (float)it->Layer;
				float distance = (lastcheckedLayer-startY) / speedNorm.y;

				ptx = (actorBB.P.x + actorBB.E.x)/2.0f + (speedNorm.x * distance);
				ptz = (actorBB.P.z + actorBB.E.z)/2.0f + (speedNorm.z * distance);
			}
			
			if(it->Square.Overlap(ptx, ptz))
			{
				ModifiedSpeedY = (lastcheckedLayer - startY);
				ModifiedSpeedY += (ModifiedSpeedY > 0) ? -0.0001f : 0.0001f;
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
bool PlanesPhysicHandler::ColisionWithWallX(const AABB & actorBB, const VECTOR &Speed, 
											float &ModifiedSpeedX, float &ModifiedSpeedY)
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

		float tmpX = startX+moveX;
		int stopX = (int)(tmpX-0.0001f)+1;
		if(tmpX < 0)
			stopX = 0;

		if(stopX < 0)
			stopX = 0;

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
		float distance;

		// check until the planes beeing after the actor move
		for(; it != end; ++it)
		{
			if(it->Layer != lastcheckedLayer)
			{
				lastcheckedLayer = (float)it->Layer;
				distance = (lastcheckedLayer-startX) / speedNorm.x;

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
				ModifiedSpeedX += (ModifiedSpeedX > 0) ? -0.0001f : 0.0001f;
				ModifiedSpeedY *= distance;
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
bool PlanesPhysicHandler::ColisionWithWallZ(const AABB & actorBB, const VECTOR &Speed, 
											float &ModifiedSpeedZ, float &ModifiedSpeedY)
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

		float tmpZ = startZ+moveZ;
		int stopZ = (int)(tmpZ-0.0001f)+1;
		if(tmpZ < 0)
			stopZ = 0;

		if(stopZ < 0)
			stopZ = 0;

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
		float distance;

		// check until the planes beeing after the actor move
		for(; it != end; ++it)
		{
			if(it->Layer != lastcheckedLayer)
			{
				lastcheckedLayer = (float)it->Layer;
				distance = (lastcheckedLayer-startZ) / speedNorm.z;

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
				ModifiedSpeedZ += (ModifiedSpeedZ > 0) ? -0.0001f : 0.0001f;
				ModifiedSpeedY *= distance;
				return true;
			}
		}
	}

	return false;
}




/*
--------------------------------------------------------------------------------------------------
- check collision with stairs
--------------------------------------------------------------------------------------------------
*/
bool PlanesPhysicHandler::ColisionWithStair(const AABB & actorBB, const VECTOR &Speed, VECTOR &ModifiedSpeed)
{
	float moveX = Speed.x;
	float moveZ = Speed.z;

	// calculate norm of speed
	VECTOR speedNorm = Speed.unit();

	float startX = (actorBB.P.x+actorBB.E.x)/2.0f;
	float startZ = (actorBB.P.z+actorBB.E.z)/2.0f;

	std::vector<StairPlane>::const_iterator it = _stairs.begin();
	std::vector<StairPlane>::const_iterator end = _stairs.end();

	// for each stairs
	for(int i=0; it != end; ++it, ++i)
	{
		// project point to plane and check if we cross it
		float DotProduct=speedNorm.dot(it->Normal);

		// Determine If Ray Parallel To Plane
		if (abs(DotProduct) > 0.000001f)
		{
			// Find Distance To Collision Point
			float l2=(it->Normal.dot(it->C1-VECTOR(startX, actorBB.P.y, startZ)))/DotProduct;	

			// Test If Collision Behind Start or after end
			if (l2 > 0 && l2 < Speed.length())							
			{
				float collionsX = startX + (speedNorm.x * l2);
				float collionsZ = startZ + (speedNorm.z * l2);

				if((collionsX >= it->minX) && (collionsX <= it->maxX))
				{
					if((collionsZ >= it->minZ) && (collionsZ <= it->maxZ))
					{
						VECTOR spmY(Speed.x, 0, Speed.z);
						VECTOR Vt(it->Normal.dot(spmY)*it->Normal);
						VECTOR Vn(spmY - Vt);
						ModifiedSpeed = Vn;
						return true;
					}
				}
			}
		}
	}

	return false;
}



/*
--------------------------------------------------------------------------------------------------
- check collision with stairs
--------------------------------------------------------------------------------------------------
*/
bool PlanesPhysicHandler::ColisionWithCornerStair(const AABB & actorBB, const VECTOR &Speed, VECTOR &ModifiedSpeed)
{
	float moveX = Speed.x;
	float moveZ = Speed.z;

	// calculate norm of speed
	VECTOR speedNorm = Speed.unit();

	float startX = (actorBB.P.x+actorBB.E.x)/2.0f;
	float startZ = (actorBB.P.z+actorBB.E.z)/2.0f;

	std::vector<CornerStairPlane>::const_iterator it = _corner_stairs.begin();
	std::vector<CornerStairPlane>::const_iterator end = _corner_stairs.end();

	// for each stairs
	for(int i=0; it != end; ++it, ++i)
	{
		// project point to plane and check if we cross it
		float DotProduct=speedNorm.dot(it->Normal);

		// Determine If Ray Parallel To Plane
		if (abs(DotProduct) > 0.000001f)
		{
			// Find Distance To Collision Point
			float l2=(it->Normal.dot(it->C1-VECTOR(startX, actorBB.P.y, startZ)))/DotProduct;	

			// Test If Collision Behind Start or after end
			if (l2 > 0 && l2 < Speed.length())							
			{
				float collionsX = startX + (speedNorm.x * l2);
				float collionsZ = startZ + (speedNorm.z * l2);

				if(it->tr_wh_y.contains(Point2D(collionsX, collionsZ)))
				{
					VECTOR spmY(Speed.x, 0, Speed.z);
					VECTOR Vt(it->Normal.dot(spmY)*it->Normal);
					VECTOR Vn(spmY - Vt);
					ModifiedSpeed = Vn;
					return true;
				}
			}
		}
	}

	return false;
}