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

#include "PhysicHandler.h"

#include <fstream>
#include <sstream>
#include <assert.h>
#include <math.h>
#include "LocalActorsHandler.h"
#include "ExternalActorsHandler.h"

/*
--------------------------------------------------------------------------------------------------
- constructor
--------------------------------------------------------------------------------------------------
*/
PhysicHandler::PhysicHandler(LocalActorsHandler * LAH, ExternalActorsHandler * EAH)
: _physicCube(NULL), _localAH(LAH), _EAH(EAH), _materialCube(NULL)
{

}

/*
--------------------------------------------------------------------------------------------------
- destructor
--------------------------------------------------------------------------------------------------
*/
PhysicHandler::~PhysicHandler()
{
	ClearMemory();
}

/*
--------------------------------------------------------------------------------------------------
- load a map into memory
--------------------------------------------------------------------------------------------------
*/
void PhysicHandler::Allocate(int sizeX, int sizeY, int sizeZ)
{
	ClearMemory();
	AllocateMemory(sizeX, sizeY, sizeZ);
}

/*
--------------------------------------------------------------------------------------------------
- save the map physics to a file
--------------------------------------------------------------------------------------------------
*/
void PhysicHandler::SaveMap(const std::string Filename)
{
	if(!_physicCube)
		return;

	std::ofstream file(Filename.c_str());

	if(file.is_open())
	{
		short *	physicCubePtr = _physicCube;

		for(int y=0; y<_sizeY; ++y)
		{
			for(int x=0; x<_sizeX; ++x)
			{
				for(int z=0; z<_sizeZ; ++z)
				{
					file << std::hex << *physicCubePtr << " ";
					++physicCubePtr;
				}
				file << std::endl;
			}
			file << std::endl;
		}
	}
}


/*
--------------------------------------------------------------------------------------------------
- check if it is possible to move from one position to a certain X and Z offset
- change the offset if the move is not possible
- the position and offset are given with a factor 10
- the actor has a bounding box centered on currX, currZ
- and which goes up from currY
- // actorSizeX and actorSizeZ are 1/2 the diameter from the center
--------------------------------------------------------------------------------------------------
*/
void PhysicHandler::Move(float currX, float currY, float currZ,
							float actorSizeX, float actorSizeY, float actorSizeZ,
							float & MoveX, float &MoveZ, bool jump)
{
	// check if memory is allocated
	if(!_physicCube)
		return;

	//Display(currX, currY, currZ);
	float tmp;
	float cY = modf(currY, &tmp);


	// check if a move in the X side is possible
	if(MoveX != 0)
	{
		// calculate the end of a brick depending of it is a backward or a upward move
		float posX = ((MoveX > 0) ? (currX + actorSizeX) : (currX - actorSizeX));

		// variable representing the possible outcome position
		float resX = posX + MoveX;
		int currBlockX = (int)posX;
		int nextBlockX = (int)resX;

		// check if the move exit the map limit
		// in this case do not allow the move
		if(resX >= _sizeX)
			MoveX = _sizeX-posX;
		if(resX < 0)
			MoveX = -posX;

		// if we are in a middle of a brick the move is always allowed
		// so only check if we cross from one brick to another
		if((MoveX != 0) && (currBlockX != nextBlockX))
		{
			int ZBegin = (int)(currZ-actorSizeZ);
			int ZEnd = (int)(currZ+actorSizeZ);

			//check depending of the size of the actor if it try to cross a solid brick
			// within the actor bounding box
			for(; ZBegin<=ZEnd; ++ZBegin)
			{
				int yOffset = 0;
				if(cY!=0 && !jump)
					yOffset = 1;

				int YBegin = (int)currY + 1 + yOffset;
				int YEnd = (int)(currY + actorSizeY) + yOffset;

				// if the first layer is not a stair the move is not allowed
				if(IsSolid(nextBlockX, YBegin, ZBegin))
				{
					MoveX = 0;
					break;
				}
				++YBegin;

				for(; YBegin<=YEnd; ++YBegin)
				{
					// if the other layers are not empty the move is not allowed
					if(!IsEmpty(nextBlockX, YBegin, ZBegin))
					{
						MoveX = 0;
						break;
					}
				}
			}
		}

		// update X before calculating the move on the Y side
		currX += MoveX;
	}



	// check if a move in the Z side is possible
	if(MoveZ != 0)
	{
		// calculate the end of a brick depending of it is a backward or a upward move
		float posZ = ((MoveZ > 0) ? (currZ + actorSizeZ) : (currZ - actorSizeZ));


		// variable representing the possible outcome position
		float resZ = posZ + MoveZ;
		int currBlockZ = (int)posZ;
		int nextBlockZ = (int)resZ;

		// check if the move exit the map limit
		// in this case do not allow the move
		if(resZ >= _sizeZ)
			MoveZ = _sizeZ-posZ;
		if(resZ < 0)
			MoveZ = -posZ;


		// if we are in a middle of a brick the move is always allowed
		// so only check if we cross from one brick to another
		if((MoveZ != 0) && (currBlockZ != nextBlockZ))
		{
			int XBegin = (int)(currX-actorSizeX);
			int XEnd = (int)(currX+actorSizeX);

			//check depending of the size of the actor if it try to cross a solid brick
			// within the actor bounding box
			for(; XBegin<=XEnd; ++XBegin)
			{
				int yOffset = 0;
				if(cY!=0 && !jump)
					yOffset = 1;

				int YBegin = (int)currY + 1 + yOffset;
				int YEnd = (int)(currY+actorSizeY) + yOffset;

				// if the first layer is not a stair the move is not allowed
				if(IsSolid(XBegin, YBegin, nextBlockZ))
				{
					MoveZ = 0;
					break;
				}
				++YBegin;

				// if the other layers are not empty the move is not allowed
				for(; YBegin<=YEnd; ++YBegin)
				{
					if(!IsEmpty(XBegin, YBegin, nextBlockZ))
					{
						MoveZ = 0;
						break;
					}
				}
			}
		}
	}

}


/*
--------------------------------------------------------------------------------------------------
- get the correct Y associate with a position in space
- e.g. if the actor is currently in air - it should fall down
- if the actor is currently on stair - it should move up down
- the position and offset are given with a factor 10
--------------------------------------------------------------------------------------------------
*/
float PhysicHandler::GetNextY(float pX, float pY, float pZ,
								float actorSizeX, float actorSizeY, float actorSizeZ)
{
	if(!_physicCube)
		return pY;

	int currX = (int)(pX*10);
	int currY = (int)(pY*10);
	int currZ = (int)(pZ*10);

	//int actorSizeX = (int)(actorSX*10);
	//int actorSizeY = (int)(actorSY*10);
	//int actorSizeZ = (int)(actorSZ*10);

	int cY = currY/10;

	//check what kind of structure we step on and act accordingly
	short cstr = GetStructure(currX/10, cY+1, currZ/10);

	// if their is stairs - take it
	if((cstr > 1) && (cstr < 14))
		++cY;
	else
	{
		// if we are in the air, make the actor fall down
		// until we reach the ground
		cstr = GetStructure(currX/10, cY, currZ/10);
		while(cY > 0 && (cstr == 0) && EmptyUnderActor(pX, cY, pZ, actorSizeX, actorSizeY, actorSizeZ))
		{
			--cY;
			cstr = GetStructure(currX/10, cY, currZ/10);
		}
	}

	// please refer to shapes.gif for more explanation
	int max = 0;
	switch(cstr)
	{
		// we are on air
		// only possible if we fall all over up to the bottom of the map
		// means the actor should die
		case 0:
			if(EmptyUnderActor(pX, cY, pZ, actorSizeX, actorSizeY, actorSizeZ))
				return -1;
			else
				return cY;

		// we are on a solid floor
		case 1:
			return cY;

		// stair 2
		case 2:
			return (((cY-1)*10) + (currX%10)+1)/10.f;

		// stair 3
		case 3:
			return (((cY-1)*10) + (currZ%10)+1)/10.f;

		// stair 4
		 case 4:
			return (((cY-1)*10) + 10 - (currZ%10))/10.f;

		// stair 5
		 case 5:
			 return (((cY-1)*10) + 10 - (currX%10))/10.f;

		// stair 6
		 case 6:
			 max = std::max(currX%10, currZ%10);
			return (((cY-1)*10) + max+1)/10.f;

		// stair 7
		 case 7:
			 max = std::max(9-(currX%10), 9-(currZ%10));
			return (((cY-1)*10) + max+1)/10.f;

		// stair 8
		 case 8:
			 max = std::max(9-(currZ%10), currX%10);
			return (((cY-1)*10) + max+1)/10.f;

		// stair 9
		 case 9:
			 max = std::max(currZ%10, 9-(currX%10));
			return (((cY-1)*10) + max+1)/10.f;

		// stair 10
		 case 10:
			 max = std::min(currX%10, currZ%10);
			return (((cY-1)*10) + max+1)/10.f;

		// stair 11
		 case 11:
			 max = std::min(9-(currX%10), 9-(currZ%10));
			return (((cY-1)*10) + max+1)/10.f;

		// stair 12
		 case 12:
			 max = std::min(9-(currZ%10), currX%10);
			return (((cY-1)*10) + max+1)/10.f;

		// stair 13
		 case 13:
			 max = std::min(currZ%10, 9-(currX%10));
			return (((cY-1)*10) + max+1)/10.f;


		// we are on water
		case 15:
			return cY;
	}



	return cY;
}

/*
--------------------------------------------------------------------------------------------------
- clean the memory
--------------------------------------------------------------------------------------------------
*/
void PhysicHandler::ClearMemory()
{
	if(_physicCube != NULL)
		delete _physicCube;

	if(_materialCube != NULL)
		delete _materialCube;

	_physicCube = NULL;
	_materialCube =NULL;
}


/*
--------------------------------------------------------------------------------------------------
- allocate a memory buffer of given size
--------------------------------------------------------------------------------------------------
*/
void PhysicHandler::AllocateMemory(int sizeX, int sizeY, int sizeZ)
{
	_physicCube = new short [sizeY*sizeX*sizeZ];
	_materialCube = new short [sizeY*sizeX*sizeZ];

	_sizeX = sizeX;
	_sizeY = sizeY;
	_sizeZ = sizeZ;
}



/*
--------------------------------------------------------------------------------------------------
- return true if the brick asked contain a solid
- structure - return false otherwise
--------------------------------------------------------------------------------------------------
*/
bool PhysicHandler::IsSolid(int X, int Y, int Z)
{
	short brickS = GetStructure(X, Y, Z);
	return (brickS == 1 || brickS > 13);
}

/*
--------------------------------------------------------------------------------------------------
- return true if the brick asked contain a empty
- structure - return false otherwise
--------------------------------------------------------------------------------------------------
*/
bool PhysicHandler::IsEmpty(int X, int Y, int Z)
{
	short brickS = GetStructure(X, Y, Z);
	return (brickS == 0);
}

/*
--------------------------------------------------------------------------------------------------
- return true if the actor is stepping on empty place
--------------------------------------------------------------------------------------------------
*/
bool PhysicHandler::EmptyUnderActor(double X, double Y, double Z,
						double actorSizeX, double actorSizeY, double actorSizeZ)
{
	int XBegin = (int)(X-actorSizeX);
	int XEnd = (int)(X+actorSizeX);
	int ZBegin = (int)(Z-actorSizeZ);
	int ZEnd = (int)(Z+actorSizeZ);

	for(; XBegin<=XEnd; ++XBegin)
		for(; ZBegin<=ZEnd; ++ZBegin)
			if(!IsEmpty(XBegin, (int)Y, ZBegin))
				return false;

	return true;
}



/*
--------------------------------------------------------------------------------------------------
- return the structure of a specific brick
--------------------------------------------------------------------------------------------------
*/
short PhysicHandler::GetStructure(int X, int Y, int Z)
{
	if(X < 0 || Y < 0 || Z < 0 || X >= _sizeX || Y >= _sizeY || Z >= _sizeZ)
		return 0;

	if(!_physicCube)
		return 0;

	short res = _physicCube[Y*_sizeX*_sizeZ + X*_sizeZ + Z];
	if(res == 0)
		res = _localAH->GetStructure(X, Y, Z);
	if(res == 0)
		res = _EAH->GetStructure(X, Y, Z);

	return res;
}


/*
--------------------------------------------------------------------------------------------------
- return true if the position is on water - the actor should then drow
--------------------------------------------------------------------------------------------------
*/
bool PhysicHandler::StepOnWater(int X, int Y, int Z)
{
	short stru = GetStructure(X, Y, Z);
	return ( stru == 15 || stru == 16 || stru == 17);
}

/*
--------------------------------------------------------------------------------------------------
- return true if there is a roof on top of the given position
- function used to know when to cut the room in half
--------------------------------------------------------------------------------------------------
*/
int PhysicHandler::IsUnderRoof(int X, int Y, int Z)
{
	//for(int i=Y+1; i<_sizeY; ++i)
	//{
	//	if(!IsEmpty(X, i, Z))
	//		if(i+1 < _sizeY && !IsEmpty(X, i+1, Z))
	//			if(i+2 < _sizeY && !IsEmpty(X, i+2, Z))
	//				return i;
	//}

	//return -1;

	for(int i=Y+3; i<(_sizeY-1); ++i)
	{
		if(!IsEmpty(X, i, Z))
		{
			if((X-1 >= 0) && (Z-1 >= 0) && (X+1 < _sizeX) && (Z+1 < _sizeZ))
				if(!IsEmpty(X-1, i, Z-1) || !IsEmpty(X-1, i+1, Z-1) || !IsEmpty(X-1, i-1, Z-1))
					if(!IsEmpty(X+1, i, Z+1) || !IsEmpty(X+1, i+1, Z+1) || !IsEmpty(X+1, i-1, Z+1))
						if(!IsEmpty(X-1, i, Z+1) || !IsEmpty(X-1, i+1, Z+1) || !IsEmpty(X-1, i-1, Z+1))
							if(!IsEmpty(X+1, i, Z-1) || !IsEmpty(X+1, i+1, Z-1) || !IsEmpty(X+1, i-1, Z-1))
								return i;
		}
	}

	return -1;
}


/*
--------------------------------------------------------------------------------------------------
- used for debugging to display where a character is on the map
--------------------------------------------------------------------------------------------------
*/
void PhysicHandler::Display(int currX, int currY, int currZ)
{
	if(!_physicCube)
		return;

	std::ofstream file("check_char.txt", std::ios::app);
	file<<std::endl<<std::endl<<std::endl<<"---------------------------------------------------"<<std::endl;
	file<<currX<<" "<<currY<<" "<<currZ<<std::endl<<std::endl;

	if(file.is_open())
	{
		int y = currY/10 + 1;
		//for(int y=0; y<_sizeY; ++y)
		//{
			for(int x=0; x<_sizeX/**10*/; ++x)
			{
				for(int z=0; z<_sizeZ/**10*/; ++z)
				{
					if(currX/10 == x && currZ/10 == z)
						file << "X";
					else
						file << std::hex << GetStructure(x/*/10*/, y, z/*/10*/);
				}
				file << std::endl;
			}
			file << std::endl;
		//}
	}
}



/*
--------------------------------------------------------------------------------------------------
- used for debugging to display where a character is on the map
--------------------------------------------------------------------------------------------------
*/
std::string PhysicHandler::DisplayS(int currX, int currY, int currZ)
{
	if(!_physicCube)
		return "";

	std::stringstream strs;

	int y = currY/10;
	for(int x=0; x<_sizeX*10; ++x)
	{
		for(int z=0; z<_sizeZ*10; ++z)
		{
			if(currX == x && currZ == z)
				strs << "X";
			else
				strs << std::hex  << GetStructure(x/10, y, z/10);
		}
		strs << "\n";
	}



	return strs.str();
}


/*
--------------------------------------------------------------------------------------------------
- used when flying to check if we can go down
--------------------------------------------------------------------------------------------------
*/
void PhysicHandler::GoUpDown(float currX, float currY, float currZ, float & deltaY, float actorSizeY)
{
	float resY = currY + deltaY;
	if(resY < 0)
	{
		deltaY = 0;
		return;
	}

	if((resY) >= (_sizeY-1))
	{
		deltaY = (_sizeY-1-currY);
		return;
	}

	if(deltaY < 0)
	{
		if(!IsEmpty(currX, currY, currZ))
		{
			float tmp;
			float cY = modf(currY, &tmp);
			if(-deltaY >= cY)
				deltaY = -cY;
		}
	}
}

/*
--------------------------------------------------------------------------------------------------
return the Y position of the floor
--------------------------------------------------------------------------------------------------
*/
int PhysicHandler::GetFloorY(int X, int Y, int Z)
{
	int res = Y+3;
	for(int cc=0;IsEmpty(X, res, Z) && res > 0 && cc < 100; ++cc)
		--res;

	return res;

}


/*
--------------------------------------------------------------------------------------------------
return the sound of a specific brick
--------------------------------------------------------------------------------------------------
*/
short PhysicHandler::GetSound(int X, int Y, int Z)
{
	if(X < 0 || Y < 0 || Z < 0 || X >= _sizeX || Y >= _sizeY || Z >= _sizeZ)
		return 0;

	if(!_materialCube)
		return 0;

	short res = _materialCube[Y*_sizeX*_sizeZ + X*_sizeZ + Z];
	return res;
}

