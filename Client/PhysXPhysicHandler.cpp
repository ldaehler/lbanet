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

#include "PhysXPhysicHandler.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <math.h>
#include <set>
#include <algorithm>

#include "LocalActorsHandler.h"
#include "ExternalActorsHandler.h"
#include "PhysXEngine.h"
#include "NxVec3.h"
#include "NxController.h"

#include <windows.h>    // Header File For Windows
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#include <GL/glu.h>     // Header File For The GLu32 Library




/*
--------------------------------------------------------------------------------------------------
- constructor
--------------------------------------------------------------------------------------------------
*/
PhysXPhysicHandler::PhysXPhysicHandler(const std::string filename, 
										 LocalActorsHandler * LAH, ExternalActorsHandler * EAH,
										float posX, float posY, float posZ)
	: _localAH(LAH), _externalAH(EAH)
{
	std::ifstream file(filename.c_str(), std::ifstream::binary);
	unsigned int sizevertex;
	unsigned int sizeindices;
	file.read((char*)&sizevertex, sizeof(unsigned int));
	file.read((char*)&sizeindices, sizeof(unsigned int));

	float *buffervertex = new float[sizevertex];
	unsigned int *bufferindices = new unsigned int[sizeindices];
	file.read((char*)buffervertex, sizevertex*sizeof(float));
	file.read((char*)bufferindices, sizeindices*sizeof(unsigned int));


	//std::ifstream file(filename.c_str());
	//if(!file.is_open())
	//	return;


	//std::map<VECTOR, NxU32> vertexmap;
	//std::vector<float> vertexes;
	//std::vector<NxU32> indices;
	//NxU32 indiceidx = 0;

	//int sizePlanes, sizewallX, sizewallZ, sizeStairs, sizecornerStairs;
	//file>>sizePlanes;
	//file>>sizewallX;
	//file>>sizewallZ;
	//file>>sizeStairs;
	//file>>sizecornerStairs;

	//for(int i=0; i<sizePlanes; ++i)
	//{
	//	NormalPlane np;
	//	file>>np.Layer;

	//	float sx, sz, ex, ez;
	//	file>>sx;
	//	file>>sz;
	//	file>>ex;
	//	file>>ez;
	//	np.Square._minX = MIN(sx, ex);
	//	np.Square._maxX = MAX(sx, ex);
	//	np.Square._minZ = MIN(sz, ez);
	//	np.Square._maxZ = MAX(sz, ez);

	//	VECTOR p1(np.Square._minX, np.Layer, np.Square._minZ);
	//	VECTOR p2(np.Square._minX, np.Layer, np.Square._maxZ);
	//	VECTOR p3(np.Square._maxX, np.Layer, np.Square._maxZ);
	//	VECTOR p4(np.Square._maxX, np.Layer, np.Square._minZ);

	//	//VECTOR norm1 = ((p2 - p1).cross(p3-p1)).unit();
	//	//VECTOR norm2 = ((p1 - p4).cross(p3-p4)).unit();

	//	indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, p1));
	//	indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, p2));
	//	indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, p3));
	//	indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, p4));
	//	indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, p1));
	//	indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, p3));

	//	//if(norm1.y < 0.5)
	//	//	std::cout<<"oups"<<std::endl;
	//	//if(norm2.y < 0.5)
	//	//	std::cout<<"oups"<<std::endl;

	//	file>>np.IsWater;
	//	_floors[np.Layer].push_back(np);
	//}

	//for(int i=0; i<sizewallX; ++i)
	//{
	//	NormalPlane np;
	//	file>>np.Layer;

	//	float sx, sz, ex, ez;
	//	file>>sx;
	//	file>>sz;
	//	file>>ex;
	//	file>>ez;
	//	np.Square._minX = MIN(sx, ex);
	//	np.Square._maxX = MAX(sx, ex);
	//	np.Square._minZ = MIN(sz, ez);
	//	np.Square._maxZ = MAX(sz, ez);


	//	VECTOR p1(np.Layer, np.Square._minX, np.Square._minZ);
	//	VECTOR p2(np.Layer, np.Square._minX, np.Square._maxZ);
	//	VECTOR p3(np.Layer, np.Square._maxX, np.Square._maxZ);
	//	VECTOR p4(np.Layer, np.Square._maxX, np.Square._minZ);

	//	//VECTOR norm1 = ((p3-p1).cross(p2 - p1)).unit();
	//	//VECTOR norm2 = ((p3-p4).cross(p1 - p4)).unit();

	//	indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, p1));
	//	indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, p3));
	//	indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, p2));
	//	indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, p4));
	//	indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, p3));
	//	indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, p1));


	//	//if(norm1.x < 0.5)
	//	//	std::cout<<"oups"<<std::endl;
	//	//if(norm2.x < 0.5)
	//	//	std::cout<<"oups"<<std::endl;


	//	_wallsX[np.Layer].push_back(np);
	//}

	//for(int i=0; i<sizewallZ; ++i)
	//{
	//	NormalPlane np;
	//	file>>np.Layer;

	//	float sx, sz, ex, ez;
	//	file>>sx;
	//	file>>sz;
	//	file>>ex;
	//	file>>ez;
	//	np.Square._minX = MIN(sx, ex);
	//	np.Square._maxX = MAX(sx, ex);
	//	np.Square._minZ = MIN(sz, ez);
	//	np.Square._maxZ = MAX(sz, ez);


	//	VECTOR p1(np.Square._minX, np.Square._minZ, np.Layer);
	//	VECTOR p2(np.Square._minX, np.Square._maxZ, np.Layer);
	//	VECTOR p3(np.Square._maxX, np.Square._maxZ, np.Layer);
	//	VECTOR p4(np.Square._maxX, np.Square._minZ, np.Layer);

	//	//VECTOR norm1 = ((p3-p1).cross(p2 - p1)).unit();
	//	//VECTOR norm2 = ((p3-p4).cross(p1 - p4)).unit();

	//	indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, p1));
	//	indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, p3));
	//	indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, p2));
	//	indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, p4));
	//	indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, p3));
	//	indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, p1));


	//	//if(norm1.z < 0.5)
	//	//	std::cout<<"oups"<<std::endl;
	//	//if(norm2.z < 0.5)
	//	//	std::cout<<"oups"<<std::endl;


	//	_wallsZ[np.Layer].push_back(np);
	//}

	//for(int i=0; i<sizeStairs; ++i)
	//{
	//	StairPlane sp;
	//	file>>sp.C1.x;
	//	file>>sp.C1.y;
	//	file>>sp.C1.z;
	//	file>>sp.C2.x;
	//	file>>sp.C2.y;
	//	file>>sp.C2.z;
	//	file>>sp.C3.x;
	//	file>>sp.C3.y;
	//	file>>sp.C3.z;
	//	file>>sp.C4.x;
	//	file>>sp.C4.y;
	//	file>>sp.C4.z;


	//	VECTOR norm1 = ((sp.C3 - sp.C1).cross(sp.C2-sp.C1)).unit();
	//	VECTOR norm2 = ((sp.C2 - sp.C4).cross(sp.C3-sp.C4)).unit();

	//	if(norm1.y < 0.5)
	//	{
	//		indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, sp.C1));
	//		indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, sp.C2));
	//		indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, sp.C3));
	//	}
	//	else
	//	{
	//		indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, sp.C1));
	//		indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, sp.C3));
	//		indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, sp.C2));
	//	}

	//	if(norm2.y < 0.5)
	//	{
	//		indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, sp.C4));
	//		indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, sp.C3));
	//		indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, sp.C2));
	//	}
	//	else
	//	{
	//		indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, sp.C4));
	//		indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, sp.C2));
	//		indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, sp.C3));
	//	}



	//	sp.Normal = ((sp.C2 - sp.C1).cross(sp.C3-sp.C1)).unit();
	//
	//	_stairs.push_back(sp);
	//}

	//for(int i=0; i<sizecornerStairs; ++i)
	//{
	//	CornerStairPlane sp;
	//	file>>sp.C1.x;
	//	file>>sp.C1.y;
	//	file>>sp.C1.z;
	//	file>>sp.C2.x;
	//	file>>sp.C2.y;
	//	file>>sp.C2.z;
	//	file>>sp.C3.x;
	//	file>>sp.C3.y;
	//	file>>sp.C3.z;

	//	VECTOR norm1 = ((sp.C3 - sp.C1).cross(sp.C2-sp.C1)).unit();
	//	if(norm1.y < 0.5)
	//	{
	//		indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, sp.C1));
	//		indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, sp.C2));
	//		indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, sp.C3));
	//	}
	//	else
	//	{
	//		indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, sp.C1));
	//		indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, sp.C3));
	//		indices.push_back(findvertexinmap(vertexmap, indiceidx, vertexes, sp.C2));
	//	}


	//	sp.Normal = ((sp.C2 - sp.C1).cross(sp.C3-sp.C1)).unit();
	//	_corner_stairs.push_back(sp);
	//}

	_lastposX = posX;
	_lastposY = posY;
	_lastposZ = posZ;

	_controller = PhysXEngine::getInstance()->CreateCharacter(NxVec3(_lastposX, _lastposY, _lastposZ), 0.4, 3.8);

	_map = PhysXEngine::getInstance()->CreateTriangleMesh(NxVec3(0,0,0), buffervertex, 
															sizevertex, bufferindices, sizeindices);

	delete buffervertex;
	delete bufferindices;
}

/*
--------------------------------------------------------------------------------------------------
- denstructor
--------------------------------------------------------------------------------------------------
*/
PhysXPhysicHandler::~PhysXPhysicHandler()
{
	PhysXEngine::getInstance()->DestroyActor(_map);
	PhysXEngine::getInstance()->DestroyCharacter(_controller);
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
MoveOutput PhysXPhysicHandler::MoveActor(long ActorId, const AABB & actorBB,
											const VECTOR &Speed)
{
	MoveOutput res;
	res.NewSpeed = Speed;
	res.TouchingWater = false;
	res.TouchingGround = false;


	unsigned int flags = PhysXEngine::getInstance()->MoveCharacter(_controller, 
													NxVec3(Speed.x, Speed.y, Speed.z));

	float posX, posY, posZ;
	PhysXEngine::getInstance()->GetCharacterPosition(_controller, posX, posY, posZ);

	res.NewSpeed.x = posX - _lastposX;
	res.NewSpeed.y = posY - _lastposY;
	res.NewSpeed.z = posZ - _lastposZ;

	res.TouchingGround = flags & NXCC_COLLISION_DOWN;

	_lastposX = posX;
	_lastposY = posY;
	_lastposZ = posZ;

	return res;
}


/*
--------------------------------------------------------------------------------------------------
// set actor position
--------------------------------------------------------------------------------------------------
*/
void PhysXPhysicHandler::SetActorPos(long ActorId, const VECTOR &NewPos)
{
	_lastposX = NewPos.x;
	_lastposY = NewPos.y;
	_lastposZ = NewPos.z;
	PhysXEngine::getInstance()->SetCharacterPos(_controller, NxVec3(NewPos.x, NewPos.y, NewPos.z));
}


/*
--------------------------------------------------------------------------------------------------
- return int > 0 if there is a roof on top of the given position
- function used to know when to cut the room display in half
--------------------------------------------------------------------------------------------------
*/
int PhysXPhysicHandler::IsUnderRoof(const VECTOR & ActorPos)
{
	return -1;
}



/*
--------------------------------------------------------------------------------------------------
- get closest floor from the actor position
--------------------------------------------------------------------------------------------------
*/
float PhysXPhysicHandler::GetClosestFloor(const VECTOR & ActorPos)
{
	return 0;
}



/*
--------------------------------------------------------------------------------------------------
- return a positive number giving the speed of the gravity on the Y axis
--------------------------------------------------------------------------------------------------
*/
float PhysXPhysicHandler::GetGravitySpeed()
{
	NxVec3 vec;
	PhysXEngine::getInstance()->GetGravity(vec);
	return vec.y;
}


/*
--------------------------------------------------------------------------------------------------
- render physic shapes
--------------------------------------------------------------------------------------------------
*/
void PhysXPhysicHandler::Render()
{
	PhysXEngine::getInstance()->RenderActors();
}
