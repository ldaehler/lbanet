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

#if !defined(__LbaNetModel_1_MapFileLoader_h)
#define __LbaNetModel_1_MapFileLoader_h

#include <string>

/* vector */
typedef float lba_vec3[3];
typedef float lba_vec4[4];

/* polygon */
struct MapFace
{
	lba_vec3 p1Text;
	lba_vec3 p1Norm;
	lba_vec4 p1Vertex;

	lba_vec3 p2Text;
	lba_vec3 p2Norm;
	lba_vec4 p2Vertex;

	lba_vec3 p3Text;
	lba_vec3 p3Norm;
	lba_vec4 p3Vertex;
};



/* obj model structure */
class MapModel
{
public:
	//! constructor
	MapModel(int nbfaces, MapFace *	faces);

	//! destructor
	~MapModel();

	//! render
	void Render();

	//! compile map for fast display
	void Compile(int YLimit);

protected:
	void Uncompile();

private:
	MapFace *	_faces;
	int			_nbfaces;
	unsigned int _list_name;
	bool		_compiled;
};



/***********************************************************************
 * Module:  ObjFileLoader.h
 * Author:  Vivien
 * Modified: dimanche 12 juillet 2009 21:09:21
 * Purpose: Declaration of the class ObjFileLoader
 ***********************************************************************/
class MapFileLoader
{
public:
	static MapModel * LoadMapFile(const std::string &filename);
};

#endif