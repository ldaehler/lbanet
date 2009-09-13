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

#ifndef __LBA_NET_PHYSIC_HANDLER_H__
#define __LBA_NET_PHYSIC_HANDLER_H__

#include <string>
class LocalActorsHandler;
class ExternalActorsHandler;

//*************************************************************************************************
//*                                      class PhysicHandler
//*************************************************************************************************

class PhysicHandler
{
public:

	//! constructor
	PhysicHandler(LocalActorsHandler * LAH, ExternalActorsHandler * EAH);

	//! destructor
	~PhysicHandler();

	// load a map into memory
	void Allocate(int sizeX, int sizeY, int sizeZ);

	// save the map physics to a file
	void SaveMap(const std::string Filename);


	// return pointer to the physic buffer in order to fill it
	short * GetBufferPtr()
	{ return _physicCube; }

	short * GetMaterialBufferPtr()
	{ return _materialCube; }


	// check if it is possible to move from one position to a certain X and Z offset
	// change the offset if the move is not possible
	// the actor has a bounding box centered on currX, currZ
	// and which goes up from currY
	// actorSizeX and actorSizeZ are 1/2 the diameter from the center
	void Move(float currX, float currY, float currZ,
				float actorSizeX, float actorSizeY, float actorSizeZ,
				float & MoveX, float &MoveZ, bool jump);

	// used when flying to check if we can go down
	void GoUpDown(float currX, float currY, float currZ, float & deltaY, float actorSizeY);


	// get the correct Y associate with a position in space
	// e.g. if the actor is currently in air - it should fall down
	// if the actor is currently on stair - it should move up down
	float GetNextY(float pX, float pY, float pZ,
						float actorSizeX, float actorSizeY, float actorSizeZ);


	// get the correct Y velocity associated with a position in space
	// correct pY if necessary
	//double GetYVelocity(double pX, double &pY, double pZ, double vX, double vZ,
	//					double actorSizeX, double actorSizeY, double actorSizeZ);

	// return true if the position is on water - the actor should then drow
	bool StepOnWater(int X, int Y, int Z);

	// return int > 0 if there is a roof on top of the given position
	// function used to know when to cut the room in half
	int IsUnderRoof(int X, int Y, int Z);


	// used for debugging to display where a character is on the map
	void Display(int currX, int currY, int currZ);

	// used for debugging to display where a character is on the map
	std::string DisplayS(int currX, int currY, int currZ);

	// return the structure of a specific brick
	short GetStructure(int X, int Y, int Z);

	// return the Y position of the floor
	int GetFloorY(int X, int Y, int Z);

	// clean the memory
	void ClearMemory();

	// return the sound of a specific brick
	short GetSound(int X, int Y, int Z);

protected:

	// allocate a memory buffer of given size
	void AllocateMemory(int sizeX, int sizeY, int sizeZ);

	// return true if the brick asked contain a solid
	// structure - return false otherwise
	bool IsSolid(int X, int Y, int Z);

	// return true if the brick asked contain an empty
	// structure - return false otherwise
	bool IsEmpty(int X, int Y, int Z);


	// return true if the actor is stepping on empty place
	bool EmptyUnderActor(double X, double Y, double Z,
							double actorSizeX, double actorSizeY, double actorSizeZ);

private:

	// cube representing the map physic
	short *	_physicCube;
	short *	_materialCube;
	LocalActorsHandler*	_localAH;
	ExternalActorsHandler * _EAH;

	// size of the cube
	int			_sizeX;
	int			_sizeY;
	int			_sizeZ;
};

#endif
