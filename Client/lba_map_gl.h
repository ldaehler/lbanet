/*
------------------------[ Lbanet Source ]-------------------------
Copyright (C) 2009
Author: Vivien Delage [Rincevent_123] on a previous work of David Glass
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

#ifndef _LBANET_LBA_MAP_GL_H_
#define _LBANET_LBA_MAP_GL_H_

#include <string>

#include "BrickInfoHandler.h"

class PhysicHandler;


struct LBA_SHARED_BRICK
{
    int id;
    int dx;
    int dz;
};


struct LBA_FACE
{
    double v[9];
    double vt[6];
    double vn[3];


    int dx;
    int dz;
};

class LBA_MAP_GL
{
public:
	// constructor
	LBA_MAP_GL(const std::string &filename, PhysicHandler * phH);

	// destructor
    ~LBA_MAP_GL();

	// perpare the room for display
	void RecompileRoom(int Cut);

	// return the brick index use at a specific map position
	int GetBrickIndex(int X, int Y, int Z);


	// class members
    unsigned int list_name;
    unsigned int texture_name;
    int brick_number;
    int LBA2;

protected:
	// load map into memory
	void LoadMap(const std::string &filename);




	// calculate a face
	void face(double X,double Y,double Z,double texture_x,double texture_y,
				double h,int a,int b,int c,bool hidden);

private:
	// buffer containing the brick info
	int *	_brickbuffer;
	PhysicHandler * _phH;

	// map size
	int		_sizeX;
	int		_sizeY;
	int		_sizeZ;

	//HZIP	_zipfile;
};



#endif
