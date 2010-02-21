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
#include <vector>

class LocalActorsHandler;
class ExternalActorsHandler;


struct PlaneInfo
{
	int StartX;
	int StartY;
	int StartZ;

	int EndX;
	int EndY;
	int EndZ;
};

struct TexPlaneInfo
{
	int StartX;
	int StartY;

	int EndX;
	int EndY;

	int textureid;
};


class TextInfo
{
public: 
	//! constructor
	TextInfo(){}

	//! denstructor
	virtual ~TextInfo(){}

	virtual int GetTexture(int X, int Y) = 0;
};



class SingleTextInfo : public TextInfo
{
public: 
	//! constructor
	SingleTextInfo(int TexId)
	: _TexId(TexId){}

	//! denstructor
	virtual ~SingleTextInfo(){}

	virtual int GetTexture(int X, int Y)
	{ return _TexId;}

private:
	int _TexId;
};



class DuoTextInfoX : public TextInfo
{
public: 
	//! constructor
	DuoTextInfoX(int TexId, int TextId2)
	: _TexId(TexId), _TextId2(TextId2){}

	//! denstructor
	virtual ~DuoTextInfoX(){}

	virtual int GetTexture(int X, int Y)
	{ 
		if ( X & 1)
			return _TextId2;
		else
			return _TexId;
	}

private:
	int _TexId;
	int _TextId2;
};


class DuoTextInfoY : public TextInfo
{
public: 
	//! constructor
	DuoTextInfoY(int TexId, int TextId2)
	: _TexId(TexId), _TextId2(TextId2){}

	//! denstructor
	virtual ~DuoTextInfoY(){}

	virtual int GetTexture(int X, int Y)
	{ 
		if ( Y & 1)
			return _TextId2;
		else
			return _TexId;
	}

private:
	int _TexId;
	int _TextId2;
};


class QuadraTextInfo : public TextInfo
{
public: 
	//! constructor
	QuadraTextInfo(int TexId, int TextId2, int TexId3, int TextId4)
	: _TexId(TexId), _TextId2(TextId2), _TexId3(TexId3), _TextId4(TextId4)
	{}

	//! denstructor
	virtual ~QuadraTextInfo(){}

	virtual int GetTexture(int X, int Y)
	{ 
		if ( X & 1)
		{
			if ( Y & 1)
				return _TextId4;
			else
				return _TexId3;
		}
		else
		{
			if ( Y & 1)
				return _TextId2;
			else
				return _TexId;
		}
	}

private:
	int _TexId;
	int _TextId2;
	int _TexId3;
	int _TextId4;
};


class LBA_MAP_GL;

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


	//! look for floors  in the map
	void SearchFloors(LBA_MAP_GL * mapgl);
	void SearchFloorsNormal(LBA_MAP_GL * mapgl, int sizeX, int sizeY, int sizeZ);
	void SearchFloorsHidden(LBA_MAP_GL * mapgl, int sizeX, int sizeY, int sizeZ);
	void SearchFloorsSee(LBA_MAP_GL * mapgl, int sizeX, int sizeY, int sizeZ);

	void SearchWallX(LBA_MAP_GL * mapgl);
	void SearchWallXNormal(LBA_MAP_GL * mapgl, int sizeX, int sizeY, int sizeZ);
	void SearchWallXHidden(LBA_MAP_GL * mapgl, int sizeX, int sizeY, int sizeZ);

	void SearchWallZ(LBA_MAP_GL * mapgl);
	void SearchWallZNormal(LBA_MAP_GL * mapgl, int sizeX, int sizeY, int sizeZ);
	void SearchWallZHidden(LBA_MAP_GL * mapgl, int sizeX, int sizeY, int sizeZ);

	std::vector<PlaneInfo> GetPlanes()
	{ return _planes; }

	std::vector<PlaneInfo> GetPlanesHidden()
	{ return _planeshidden; }

	std::vector<PlaneInfo> GetPlanesSee()
	{ return _planessee; }

	std::vector<PlaneInfo> GetWallsX()
	{ return _wallsX; }

	std::vector<PlaneInfo> GetWallsXHidden()
	{ return _wallsXhidden; }

	std::vector<PlaneInfo> GetWallsZ()
	{ return _wallsZ; }

	std::vector<PlaneInfo> GetWallsZHidden()
	{ return _wallsZhidden; }


	//! split rectangle into part with same textures
	void SplitToTexture(short * area, int sizeX, int sizeY, std::vector<TexPlaneInfo> & res);

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


	//! look for floors  in the map
	void SearchFloors(short * thisY, int Y, std::vector<PlaneInfo> &planes, int sizeX, int sizeY, int sizeZ);


	//! look for floors  in the map
	int SearchMaxFloor(short * center, int idxX, int idxZ,
						int &startX, int &startZ,
						int &endX, int &endZ, int sizeX, int sizeY, int sizeZ);

	bool IsSolidHorLine(short * start, int size, int sizeX, int sizeY, int sizeZ);
	bool IsSolidVerLine(short * start, int size, int sizeX, int sizeY, int sizeZ);



	//! look for floors  in the map
	int SearchMaxTexture(short * center, int idxX, int idxY,
							int &startX, int &startY,
							int &endX, int &endY, int sizeX, int sizeY, TextInfo * txi);

	bool IsTexHorLine(short * start, int size, int sizeX, int sizeY, 
							int idxX, int idxY, TextInfo * txi);

	bool IsTexVerLine(short * start, int size, int sizeX, int sizeY, 
							int idxX, int idxY, TextInfo * txi);


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

	std::vector<PlaneInfo> _planes;
	std::vector<PlaneInfo> _planeshidden;
	std::vector<PlaneInfo> _planessee;

	std::vector<PlaneInfo> _wallsX;
	std::vector<PlaneInfo> _wallsXhidden;

	std::vector<PlaneInfo> _wallsZ;
	std::vector<PlaneInfo> _wallsZhidden;

};

#endif
