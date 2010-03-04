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

#include "MapFileLoader.h"


#include <fstream>

#include <windows.h>    // Header File For Windows
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#include <GL/glu.h>     // Header File For The GLu32 Library




/***********************************************************
constructor
***********************************************************/
MapModel::MapModel(int nbfaces, MapFace *	faces)
: _nbfaces(nbfaces), _faces(faces), _compiled(false)
{
	Compile(-1);
}



/***********************************************************
destructor
***********************************************************/
MapModel::~MapModel()
{
	Uncompile();

	if(_faces)
		delete[] _faces;
}


/***********************************************************
Uncompile
***********************************************************/
void MapModel::Uncompile()
{
	if(_compiled)
	{
		glDeleteLists(_list_name,1);
		_compiled = false;
	}
}


/***********************************************************
compile map for fast display
***********************************************************/
void MapModel::Compile(int YLimit)
{
	Uncompile();

	MapFace *	ptr = _faces;
	glNewList(_list_name, GL_COMPILE);

	glBegin(GL_TRIANGLES);
	for(int i=0; i<_nbfaces; ++i, ++ptr)
	{
		glTexCoord3fv(ptr->p1Text);
		glNormal3fv(ptr->p1Norm);
		glVertex4fv(ptr->p1Vertex);

		glTexCoord3fv(ptr->p2Text);
		glNormal3fv(ptr->p2Norm);
		glVertex4fv(ptr->p2Vertex);

		glTexCoord3fv(ptr->p3Text);
		glNormal3fv(ptr->p3Norm);
		glVertex4fv(ptr->p3Vertex);
	}
	glEnd();

	glEndList();
}



/***********************************************************
Render
***********************************************************/
void MapModel::Render()
{
	glCallList(_list_name);
}



/***********************************************************
reload the current map texture
***********************************************************/
MapModel *MapFileLoader::LoadMapFile(const std::string &filename)
{
	std::ifstream file(filename.c_str(), std::ifstream::binary);
	int nbfaces =0;
	file.read((char*)&nbfaces, sizeof(int));

	MapFace * faces = new MapFace[nbfaces];
	MapFace * ptr = faces;
	for(int i=0; i<nbfaces; ++i, ++ptr)
	{
		file.read((char*)&ptr->p1Text, 3*sizeof(float));
		file.read((char*)&ptr->p1Norm, 3*sizeof(float));
		file.read((char*)&ptr->p1Vertex, 4*sizeof(float));

		file.read((char*)&ptr->p2Text, 3*sizeof(float));
		file.read((char*)&ptr->p2Norm, 3*sizeof(float));
		file.read((char*)&ptr->p2Vertex, 4*sizeof(float));

		file.read((char*)&ptr->p3Text, 3*sizeof(float));
		file.read((char*)&ptr->p3Norm, 3*sizeof(float));
		file.read((char*)&ptr->p3Vertex, 4*sizeof(float));
	}

	return new MapModel(nbfaces, faces);
}

