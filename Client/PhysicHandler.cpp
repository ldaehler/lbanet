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

#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <math.h>
#include <set>

#include "LocalActorsHandler.h"
#include "ExternalActorsHandler.h"
#include "lba_map_gl.h"

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

	short currstr = GetStructure(currX, currY+0.9, currZ);


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
				if(!jump && cY!=0 && (currstr > 1) && (currstr != 3) && (currstr != 4))
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
				if(cY!=0 && !jump && (currstr > 2) && (currstr != 5))
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




/*
--------------------------------------------------------------------------------------------------
look for floors  in the map
--------------------------------------------------------------------------------------------------
*/
void PhysicHandler::SearchWallX()
{
	SearchWallXNormal(_sizeY, _sizeX, _sizeZ);
	SearchWallXHidden(_sizeY, _sizeX, _sizeZ);
}

/*
--------------------------------------------------------------------------------------------------
look for floors  in the map
--------------------------------------------------------------------------------------------------
*/
void PhysicHandler::SearchWallXNormal(int sizeX, int sizeY, int sizeZ)
{
	_wallsX.clear();

	short * buffer = new short[sizeX*sizeY*sizeZ];
	for(int k=0; k<sizeY; ++k)
	{
		for(int i=0; i<sizeX; ++i)
		{
			for(int j=0; j<sizeZ; ++j)
			{
				buffer[(k*sizeX*sizeZ)+(i*sizeZ)+j] = _physicCube[(i*_sizeX*_sizeZ)+(k*_sizeZ)+j];
			}
		}
	}



	for(int k=0; k<sizeY; ++k)
	{
		for(int i=0; i<sizeX; ++i)
		{
			for(int j=0; j<sizeZ; ++j)
			{
				if((k+1)<sizeY)
				{
					if(buffer[((k+1)*sizeX*sizeZ)+(i*sizeZ)+j] != 0)
						buffer[(k*sizeX*sizeZ)+(i*sizeZ)+j] = 0;
				}
			}
		}
	}


	short * ptr = buffer;
	for(int i=0; i<sizeY; ++i)
	{
		SearchFloors(ptr, i, _wallsX, sizeX, sizeY, sizeZ);
		ptr += sizeX*sizeZ;
	}


	delete[] buffer;
}




/*
--------------------------------------------------------------------------------------------------
look for floors  in the map
--------------------------------------------------------------------------------------------------
*/
void PhysicHandler::SearchWallXHidden(int sizeX, int sizeY, int sizeZ)
{
	_wallsXhidden.clear();

	short * buffer = new short[sizeX*sizeY*sizeZ];
	for(int k=0; k<sizeY; ++k)
	{
		for(int i=0; i<sizeX; ++i)
		{
			for(int j=0; j<sizeZ; ++j)
			{
				buffer[(k*sizeX*sizeZ)+(i*sizeZ)+j] = _physicCube[(i*_sizeX*_sizeZ)+(k*_sizeZ)+j];
			}
		}
	}


	for(int k=sizeY-1; k>=0; --k)
	{
		for(int i=0; i<sizeX; ++i)
		{
			for(int j=0; j<sizeZ; ++j)
			{
				if((k-1)>=0)
				{
					if(buffer[((k-1)*sizeX*sizeZ)+(i*sizeZ)+j] != 0)
						buffer[(k*sizeX*sizeZ)+(i*sizeZ)+j] = 0;
				}
			}
		}
	}

	short * ptr = buffer;
	for(int i=0; i<sizeY; ++i)
	{
		SearchFloors(ptr, i, _wallsXhidden, sizeX, sizeY, sizeZ);
		ptr += sizeX*sizeZ;
	}


	delete[] buffer;
}





/*
--------------------------------------------------------------------------------------------------
look for floors  in the map
--------------------------------------------------------------------------------------------------
*/
void PhysicHandler::SearchWallZ()
{
	SearchWallZNormal(_sizeX, _sizeZ, _sizeY);
	SearchWallZHidden(_sizeX, _sizeZ, _sizeY);
}

/*
--------------------------------------------------------------------------------------------------
look for floors  in the map
--------------------------------------------------------------------------------------------------
*/
void PhysicHandler::SearchWallZNormal(int sizeX, int sizeY, int sizeZ)
{
	_wallsZ.clear();

	short * buffer = new short[sizeX*sizeY*sizeZ];
	for(int k=0; k<sizeY; ++k)
	{
		for(int i=0; i<sizeX; ++i)
		{
			for(int j=0; j<sizeZ; ++j)
			{
				buffer[(k*sizeX*sizeZ)+(i*sizeZ)+j] = _physicCube[(j*_sizeX*_sizeZ)+(i*_sizeZ)+k];
			}
		}
	}


	for(int k=0; k<sizeY; ++k)
	{
		for(int i=0; i<sizeX; ++i)
		{
			for(int j=0; j<sizeZ; ++j)
			{
				if((k+1)<sizeY)
				{
					if(buffer[((k+1)*sizeX*sizeZ)+(i*sizeZ)+j] != 0)
						buffer[(k*sizeX*sizeZ)+(i*sizeZ)+j] = 0;
				}
			}
		}
	}


	short * ptr = buffer;
	for(int i=0; i<sizeY; ++i)
	{
		SearchFloors(ptr, i, _wallsZ, sizeX, sizeY, sizeZ);
		ptr += sizeX*sizeZ;
	}


	delete[] buffer;
}




/*
--------------------------------------------------------------------------------------------------
look for floors  in the map
--------------------------------------------------------------------------------------------------
*/
void PhysicHandler::SearchWallZHidden(int sizeX, int sizeY, int sizeZ)
{
	_wallsZhidden.clear();

	short * buffer = new short[sizeX*sizeY*sizeZ];
	for(int k=0; k<sizeY; ++k)
	{
		for(int i=0; i<sizeX; ++i)
		{
			for(int j=0; j<sizeZ; ++j)
			{
				buffer[(k*sizeX*sizeZ)+(i*sizeZ)+j] = _physicCube[(j*_sizeX*_sizeZ)+(i*_sizeZ)+k];
			}
		}
	}


	for(int k=sizeY-1; k>=0; --k)
	{
		for(int i=0; i<sizeX; ++i)
		{
			for(int j=0; j<sizeZ; ++j)
			{
				if((k-1)>=0)
				{
					if(buffer[((k-1)*sizeX*sizeZ)+(i*sizeZ)+j] != 0)
						buffer[(k*sizeX*sizeZ)+(i*sizeZ)+j] = 0;
				}
			}
		}
	}

	short * ptr = buffer;
	for(int i=0; i<sizeY; ++i)
	{
		SearchFloors(ptr, i, _wallsZhidden, sizeX, sizeY, sizeZ);
		ptr += sizeX*sizeZ;
	}


	delete[] buffer;
}




/*
--------------------------------------------------------------------------------------------------
look for floors  in the map
--------------------------------------------------------------------------------------------------
*/
void PhysicHandler::SearchFloors()
{
	SearchFloorsNormal(_sizeX, _sizeY, _sizeZ);
	SearchFloorsSee(_sizeX, _sizeY, _sizeZ);
}





/*
--------------------------------------------------------------------------------------------------
look for floors  in the map
--------------------------------------------------------------------------------------------------
*/
void PhysicHandler::SearchFloorsNormal(int sizeX, int sizeY, int sizeZ)
{
	_planes.clear();

	short * buffer = new short[sizeX*sizeY*sizeZ];
	memcpy ( buffer, _physicCube, sizeX*sizeY*sizeZ*sizeof(short) );


	for(int k=1; k<(sizeY-1); ++k)
	{
		for(int i=0; i<sizeX; ++i)
		{
			for(int j=0; j<sizeZ; ++j)
			{
				if( _physicCube[((k+1)*sizeX*sizeZ)+(i*sizeZ)+j] != 0
					&& _physicCube[((k-1)*sizeX*sizeZ)+(i*sizeZ)+j] != 0 )
					buffer[(k*sizeX*sizeZ)+(i*sizeZ)+j] = 0;
			}
		}
	}


	short * ptr = buffer;
	for(int i=0; i<sizeY; ++i)
	{
		SearchFloors(ptr, i, _planes, sizeX, sizeY, sizeZ);
		ptr += sizeX*sizeZ;
	}


	delete[] buffer;
}



/*
--------------------------------------------------------------------------------------------------
look for floors  in the map
--------------------------------------------------------------------------------------------------
*/
void PhysicHandler::SearchFloorsSee(int sizeX, int sizeY, int sizeZ)
{
	_planessee.clear();

	short * buffer = new short[sizeX*sizeY*sizeZ];
	memcpy ( buffer, _physicCube, sizeX*sizeY*sizeZ*sizeof(short) );


	for(int k=0; k<sizeY; ++k)
	{
		for(int i=0; i<sizeX; ++i)
		{
			for(int j=0; j<sizeZ; ++j)
			{
				if((k+1)<sizeY)
				{
					if(buffer[((k+1)*sizeX*sizeZ)+(i*sizeZ)+j] != 0)
						buffer[(k*sizeX*sizeZ)+(i*sizeZ)+j] = 0;
				}
			}
		}
	}


	for(int k=0; k<sizeY; ++k)
	{
		for(int i=0; i<sizeX; ++i)
		{
			for(int j=0; j<sizeZ; ++j)
			{
				if(buffer[(k*sizeX*sizeZ)+(i*sizeZ)+j] < 15)
					buffer[(k*sizeX*sizeZ)+(i*sizeZ)+j] = 0;
				else
					buffer[(k*sizeX*sizeZ)+(i*sizeZ)+j] = 1;
			}
		}
	}


	short * ptr = buffer;
	for(int i=0; i<sizeY; ++i)
	{
		SearchFloors(ptr, i, _planessee, sizeX, sizeY, sizeZ);
		ptr += sizeX*sizeZ;
	}


	delete[] buffer;
}




/*
--------------------------------------------------------------------------------------------------
look for floors  in the map
--------------------------------------------------------------------------------------------------
*/
void PhysicHandler::SearchFloors(short * thisY, int Y, std::vector<PlaneInfo> &planes, 
									int sizeX, int sizeY, int sizeZ)
{
	bool found = true;

	while(found)
	{
		short * ptr = thisY;
		int maxsize = 0;
		int maxsX = 0;
		int maxsZ = 0;
		int maxeX = 0;
		int maxeZ = 0;
		found = false;

		for(int i=0; i<sizeX; ++i)
		{
			for(int j=0; j<sizeZ; ++j)
			{
				int startX, startZ, endX, endZ;
				int areasize = SearchMaxFloor(ptr, i, j, startX, startZ, endX, endZ, sizeX, sizeY, sizeZ);
				if(areasize > 0)
				{
					bool issquare = ((endX-startX) == (endZ-startZ));
					if(issquare)
					{
						if(areasize >= maxsize)
						{
							maxsize = areasize;
							maxsX = startX;
							maxsZ = startZ;
							maxeX = endX;
							maxeZ = endZ;
							found = true;
						}
					}
					else
					{
						if(areasize > maxsize)
						{
							maxsize = areasize;
							maxsX = startX;
							maxsZ = startZ;
							maxeX = endX;
							maxeZ = endZ;
							found = true;
						}
					}
				}

				++ptr;
			}
		}

		if(found)
		{
			PlaneInfo pif;
			pif.StartX = maxsX;
			pif.StartY = Y;
			pif.StartZ = maxsZ;

			pif.EndX = maxeX;
			pif.EndY = Y;
			pif.EndZ = maxeZ;

			planes.push_back(pif);

			for(int i=maxsX; i<maxeX; ++i)
			{
				for(int j=maxsZ; j<maxeZ; ++j)
				{
					thisY[i*sizeZ+j] = 0;
				}
			}
		}
	}

}


/*
--------------------------------------------------------------------------------------------------
look for floors  in the map
--------------------------------------------------------------------------------------------------
*/
int PhysicHandler::SearchMaxFloor(short * center, int idxX, int idxZ,
									int &startX, int &startZ,
									int &endX, int &endZ, int sizeX, int sizeY, int sizeZ)
{
	if(*center != 1)
		return 0;

	int squaresize = 1;

	// look for the biggest centered square
	while(true)
	{
		//check first corner
		--idxX;
		--idxZ;
		++squaresize;
		center -= (sizeZ+1);

		if((idxX >= 0) && (idxZ >= 0))
		{
			if(IsSolidHorLine(center, squaresize, sizeX, sizeY, sizeZ))
			{
				if(IsSolidVerLine(center, squaresize, sizeX, sizeY, sizeZ))
				{
					continue;
				}
			}
		}

		++idxX;
		++idxZ;
		--squaresize;
		center += (sizeZ+1);
		break;
	}


	int squaresizeX = squaresize;
	int squaresizeZ = squaresize;


	int maxX = idxX;
	int maxsizeX = squaresizeX;
	int maxZ = idxZ;
	int maxsizeZ = squaresizeZ;


	// try to build bigger rectangles on X
	{
		short * ptr = center;

		while(true)
		{
			ptr -= sizeZ;
			--maxX;
			++maxsizeX;
			if(maxX >= 0)
			{
				if(IsSolidHorLine(ptr, squaresizeZ, sizeX, sizeY, sizeZ))
					continue;
			}

			ptr += sizeZ;
			++maxX;
			--maxsizeX;
			break;
		}


		ptr += (maxsizeX-1)*sizeZ;
		while(true)
		{
			ptr += sizeZ;
			++maxsizeX;
			if((maxX+maxsizeX) <= sizeX)
			{
				if(IsSolidHorLine(ptr, squaresizeZ, sizeX, sizeY, sizeZ))
					continue;
			}

			--maxsizeX;
			break;
		}

	}


	// try to build bigger rectangles on Z
	{
		short * ptr = center;

		while(true)
		{
			--ptr;
			--maxZ;
			++maxsizeZ;
			if(maxZ >= 0)
			{
				if(IsSolidVerLine(ptr, squaresizeX, sizeX, sizeY, sizeZ))
					continue;
			}

			++ptr;
			++maxZ;
			--maxsizeZ;
			break;
		}



		ptr += (maxsizeZ-1);
		while(true)
		{
			++ptr;
			++maxsizeZ;
			if((maxZ+maxsizeZ) <= sizeZ)
			{
				if(IsSolidVerLine(ptr, squaresizeX, sizeX, sizeY, sizeZ))
					continue;
			}

			--maxsizeZ;
			break;
		}	
	}


	if(maxsizeX >= maxsizeZ)
	{
		idxX=maxX;
		squaresizeX=maxsizeX;
	}
	else
	{
		idxZ=maxZ;
		squaresizeZ=maxsizeZ;
	}



	startX=idxX;
	startZ=idxZ;
	endX=startX+squaresizeX;
	endZ=startZ+squaresizeZ;


	return (squaresizeX*squaresizeZ);
}



/*
--------------------------------------------------------------------------------------------------
IsSolidHorLine
--------------------------------------------------------------------------------------------------
*/
bool PhysicHandler::IsSolidHorLine(short * start, int size, int sizeX, int sizeY, int sizeZ)
{
	for(int i=0; i<size; ++i)
	{
		if(*start != 1)
			return false;

		++start;
	}
	
	return true;
}

/*
--------------------------------------------------------------------------------------------------
IsSolidVerLine
--------------------------------------------------------------------------------------------------
*/
bool PhysicHandler::IsSolidVerLine(short * start, int size, int sizeX, int sizeY, int sizeZ)
{
	for(int i=0; i<size; ++i)
	{
		if(*start != 1)
			return false;

		start+=sizeZ;
	}
	
	return true;
}



/*
--------------------------------------------------------------------------------------------------
split rectangle into part with same textures
--------------------------------------------------------------------------------------------------
*/
void PhysicHandler::SplitToTexture(short * area, int sizeX, int sizeY, std::vector<TexPlaneInfo> & res)
{
	//first check the number of different texture
	std::set<short> texs;

	short * tmpptr = area;
	for(int i=0; i<sizeX; ++i)
	{
		for(int j=0; j<sizeY; ++j)
		{
			texs.insert(*tmpptr);
			++tmpptr;
		}
	}

	//if only one texture then easy job!
	if(texs.size() == 1)
	{
		TexPlaneInfo pif;
		pif.StartX = 0;
		pif.StartY = 0;
		pif.EndX = sizeX;
		pif.EndY = sizeY;
		pif.textureid = *(texs.begin());
		res.push_back(pif);
		return;
	}

	std::vector<TextInfo *> textinfos;
	std::set<short>::iterator setit = texs.begin();
	std::set<short>::iterator setend = texs.end();
	for(; setit != setend; ++setit)
	{
		textinfos.push_back(new SingleTextInfo(*setit));

		//std::set<short>::iterator setit2 = texs.begin();
		//std::set<short>::iterator setend2 = texs.end();
		//for(; setit2 != setend2; ++setit2)
		//{
		//	textinfos.push_back(new DuoTextInfoX(*setit, *setit2));
		//	textinfos.push_back(new DuoTextInfoY(*setit, *setit2));

		//	//std::set<short>::iterator setit3 = texs.begin();
		//	//std::set<short>::iterator setend3 = texs.end();
		//	//for(; setit3 != setend3; ++setit3)
		//	//{
		//	//	std::set<short>::iterator setit4 = texs.begin();
		//	//	std::set<short>::iterator setend4 = texs.end();
		//	//	for(; setit4 != setend4; ++setit4)
		//	//	{
		//	//		textinfos.push_back(new QuadraTextInfo(*setit, *setit2, *setit3, *setit4));
		//	//	}
		//	//}
		//}
	}


	bool found = true;
	while(found)
	{
		short * ptr = area;
		int maxsize = 0;
		int maxsX = 0;
		int maxsY = 0;
		int maxeX = 0;
		int maxeY = 0;
		TextInfo * besttextid = NULL;
		found = false;

		for(int i=0; i<sizeX; ++i)
		{
			for(int j=0; j<sizeY; ++j)
			{
				for(size_t cc=0; cc<textinfos.size(); ++cc)
				{
					int startX, startY, endX, endY;
					int areasize = 	SearchMaxTexture(ptr, i, j,	startX, startY, endX, endY,
														sizeX, sizeY, textinfos[cc]);
					if(areasize > 0)
					{
						bool issquare = ((endX-startX) == (endY-startY));
						if(issquare)
						{
							if(areasize >= maxsize)
							{
								maxsize = areasize;
								maxsX = startX;
								maxsY = startY;
								maxeX = endX;
								maxeY = endY;
								found = true;
								besttextid = textinfos[cc];
							}
						}
						else
						{
							if(areasize > maxsize)
							{
								maxsize = areasize;
								maxsX = startX;
								maxsY = startY;
								maxeX = endX;
								maxeY = endY;
								found = true;
								besttextid = textinfos[cc];
							}
						}
					}
				}

				++ptr;
			}
		}

		if(found)
		{
			TexPlaneInfo pif;
			pif.StartX = maxsX;
			pif.StartY = maxsY;

			pif.EndX = maxeX;
			pif.EndY = maxeY;
			pif.textureid = besttextid->GetTexture(0, 0);
			if(pif.textureid != 0)
				res.push_back(pif);

			for(int i=maxsX; i<maxeX; ++i)
			{
				for(int j=maxsY; j<maxeY; ++j)
				{
					area[i*sizeY+j] = 0;
				}
			}
		}
	}

	for(size_t cc=0; cc<textinfos.size(); ++cc)
		delete textinfos[cc];

}





/*
--------------------------------------------------------------------------------------------------
split rectangle into part with same textures
--------------------------------------------------------------------------------------------------
*/
int PhysicHandler::SearchMaxTexture(short * center, int idxX, int idxY,
										int &startX, int &startY,
										int &endX, int &endY, int sizeX, int sizeY, TextInfo * txi)
{
	if(*center != txi->GetTexture(idxX, idxY))
		return 0;

	int squaresize = 1;

	// look for the biggest centered square
	while(true)
	{
		//check first corner
		--idxX;
		--idxY;
		++squaresize;
		center -= (sizeY+1);

		if((idxX >= 0) && (idxY >= 0))
		{
			if(IsTexHorLine(center, squaresize, sizeX, sizeY, idxX, idxY, txi))
			{
				if(IsTexVerLine(center, squaresize, sizeX, sizeY, idxX, idxY, txi))
				{
					continue;
				}
			}
		}

		++idxX;
		++idxY;
		--squaresize;
		center += (sizeY+1);
		break;
	}


	int squaresizeX = squaresize;
	int squaresizeY = squaresize;


	int maxX = idxX;
	int maxsizeX = squaresizeX;
	int maxY = idxY;
	int maxsizeY = squaresizeY;


	// try to build bigger rectangles on X
	{
		short * ptr = center;

		while(true)
		{
			ptr -= sizeY;
			--maxX;
			++maxsizeX;
			if(maxX >= 0)
			{
				if(IsTexHorLine(ptr, squaresizeY, sizeX, sizeY, maxX, idxY, txi))
					continue;
			}

			ptr += sizeY;
			++maxX;
			--maxsizeX;
			break;
		}


		ptr += (maxsizeX-1)*sizeY;
		while(true)
		{
			ptr += sizeY;
			++maxsizeX;
			if((maxX+maxsizeX) <= sizeX)
			{
				if(IsTexHorLine(ptr, squaresizeY, sizeX, sizeY, maxX, idxY, txi))
					continue;
			}

			--maxsizeX;
			break;
		}

	}


	// try to build bigger rectangles on Y
	{
		short * ptr = center;

		while(true)
		{
			--ptr;
			--maxY;
			++maxsizeY;
			if(maxY >= 0)
			{
				if(IsTexVerLine(ptr, squaresizeX, sizeX, sizeY, idxX, maxY, txi))
					continue;
			}

			++ptr;
			++maxY;
			--maxsizeY;
			break;
		}



		ptr += (maxsizeY-1);
		while(true)
		{
			++ptr;
			++maxsizeY;
			if((maxY+maxsizeY) <= sizeY)
			{
				if(IsTexVerLine(ptr, squaresizeX, sizeX, sizeY, idxX, maxY, txi))
					continue;
			}

			--maxsizeY;
			break;
		}	
	}


	if(maxsizeX >= maxsizeY)
	{
		idxX=maxX;
		squaresizeX=maxsizeX;
	}
	else
	{
		idxY=maxY;
		squaresizeY=maxsizeY;
	}



	startX=idxX;
	startY=idxY;
	endX=startX+squaresizeX;
	endY=startY+squaresizeY;


	return (squaresizeX*squaresizeY);
}



/*
--------------------------------------------------------------------------------------------------
split rectangle into part with same textures
--------------------------------------------------------------------------------------------------
*/
bool PhysicHandler::IsTexHorLine(short * start, int size, int sizeX, int sizeY, 
								 int idxX, int idxY, TextInfo * txi)
{
	for(int i=0; i<size; ++i)
	{
		if(*start != txi->GetTexture(idxX, idxY))
			return false;

		++start;
		++idxY;
	}
	
	return true;
}



/*
--------------------------------------------------------------------------------------------------
split rectangle into part with same textures
--------------------------------------------------------------------------------------------------
*/
bool PhysicHandler::IsTexVerLine(short * start, int size, int sizeX, int sizeY, 
								 int idxX, int idxY, TextInfo * txi)
{
	for(int i=0; i<size; ++i)
	{
		if(*start != txi->GetTexture(idxX, idxY))
			return false;

		start+=sizeY;
		++idxX;
	}
	
	return true;
}



/*
--------------------------------------------------------------------------------------------------
split rectangle into part with same textures
--------------------------------------------------------------------------------------------------
*/
void PhysicHandler::SearchStairs()
{
	_stairs.clear();
	short * buffer = new short[_sizeX*_sizeY*_sizeZ];
	memcpy ( buffer, _physicCube, _sizeX*_sizeY*_sizeZ*sizeof(short) );
	SearchStairs(buffer, _sizeX, _sizeY, _sizeZ, _stairs);
	delete[] buffer;
}



/*
--------------------------------------------------------------------------------------------------
split rectangle into part with same textures
--------------------------------------------------------------------------------------------------
*/
void PhysicHandler::SearchStairs(short * start, int sizeX, int sizeY, int sizeZ,
									std::vector<StairInfo> & stairs)
{
	for(int Y=0; Y< sizeY; ++Y)
	{
		for(int X=0; X< sizeX; ++X)
		{
			for(int Z=0; Z< sizeZ; ++Z)
			{
				StairInfo stri = PhysicHandler::FindStairUp(start, X, Y, Z, sizeX, sizeY, sizeZ, true);
				if(stri.length > 0)
				{
					if(stri.type == 2 || stri.type == 5)
					{
						for(int lZ=Z+1; lZ< sizeZ; ++lZ)
						{
							StairInfo stri2 = PhysicHandler::FindStairUp(start, X, Y, lZ, sizeX, sizeY, sizeZ, false);
							if(stri2.length == stri.length && stri2.type == stri.type)
							{
								stri.C2X = stri2.C2X;
								stri.C2Y = stri2.C2Y;
								stri.C2Z = stri2.C2Z;

								stri.C4X = stri2.C4X;
								stri.C4Y = stri2.C4Y;
								stri.C4Z = stri2.C4Z;

								 PhysicHandler::FindStairUp(start, X, Y, lZ, sizeX, sizeY, sizeZ, true);
							}
							else
								break;
						}
					}

					if(stri.type == 3 || stri.type == 4)
					{
						for(int lX=X+1; lX< sizeX; ++lX)
						{
							StairInfo stri2 = PhysicHandler::FindStairUp(start, lX, Y, Z, sizeX, sizeY, sizeZ, false);
							if(stri2.length == stri.length && stri2.type == stri.type)
							{
								stri.C2X = stri2.C2X;
								stri.C2Y = stri2.C2Y;
								stri.C2Z = stri2.C2Z;

								stri.C4X = stri2.C4X;
								stri.C4Y = stri2.C4Y;
								stri.C4Z = stri2.C4Z;

								 PhysicHandler::FindStairUp(start, lX, Y, Z, sizeX, sizeY, sizeZ, true);
							}
							else
								break;
						}
					}
					stairs.push_back(stri);
				}
			}
		}
	}


}


/*
--------------------------------------------------------------------------------------------------
split rectangle into part with same textures
--------------------------------------------------------------------------------------------------
*/
StairInfo PhysicHandler::FindStairUp(short * start, int idX, int idY, int idZ,  
								int sizeX, int sizeY, int sizeZ, bool overwrite)
{
	StairInfo res;
	res.length = 0;

	int proto = start[idY*sizeX*sizeZ + idX*sizeZ + idZ];
	if(proto == 2 || proto == 3 || proto == 4 || proto == 5)
	{
		++res.length;
		res.type = proto;

		if(overwrite)
			start[idY*sizeX*sizeZ + idX*sizeZ + idZ] = 0;

		int offsetX = 0;
		int offsetZ = 0;
		int minoffsetX = 0;
		int minoffsetZ = 0;


		if(res.type == 2)
			++offsetZ;
		if(res.type == 3)
			++offsetX;
		if(res.type == 4)
		{
			++offsetX;
			++minoffsetZ;
		}
		if(res.type == 5)
		{
			++offsetZ;
			++minoffsetX;
		}


		res.C1X = idX+minoffsetX;
		res.C1Y = idY-1;
		res.C1Z = idZ+minoffsetZ;

		res.C2X = idX+offsetX+minoffsetX;
		res.C2Y = idY-1;
		res.C2Z = idZ+offsetZ+minoffsetZ;

		res.C3X = 0+minoffsetX;
		res.C3Y = -1;
		res.C3Z = 0+minoffsetZ;

		res.C4X = offsetX+minoffsetX;
		res.C4Y = -1;
		res.C4Z = offsetZ+minoffsetZ;


		while(true)
		{
			++idY;

			if(res.type == 2)
				++idX;
			if(res.type == 3)
				++idZ;
			if(res.type == 4)
				--idZ;
			if(res.type == 5)
				--idX;

			if((idY) < sizeY && (idY) >= 0)
			{
				if((idX) < sizeX && (idX) >= 0)
				{
					if((idZ) < sizeZ && (idZ) >= 0)
					{
						if(start[idY*sizeX*sizeZ + idX*sizeZ + idZ] == res.type)
						{
							if(overwrite)
								start[idY*sizeX*sizeZ + idX*sizeZ + idZ] = 0;
							++res.length;
							continue;
						}
					}
				}
			}
			break;
		}


		res.C3X += idX;
		res.C3Y += idY;
		res.C3Z += idZ;	

		res.C4X += idX;
		res.C4Y += idY;
		res.C4Z += idZ;
	}

	return res;
}