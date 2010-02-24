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

#if !defined(__LbaNetModel_1_ObjFileLoader_h)
#define __LbaNetModel_1_ObjFileLoader_h

#include <string>
#include <vector>

/* vector */
typedef float vec3_t[3];
typedef float vec4_t[4];


/* vertex */
struct obj_vertex_t
{
	vec4_t xyzw;
};


/* texture coordinates */
struct obj_texCoord_t
{
	vec3_t uvw;
};


/* normal vector */
struct obj_normal_t
{
	vec3_t ijk;
};


/* polygon */
class obj_face_t
{
public:
	//! constructor
	obj_face_t(int numelems, bool hasTextures, bool hasNormals);

	//! destructor
	~obj_face_t();


	int type;			/* primitive type */
	int num_elems;      /* number of vertices */

	int *vert_indices;  /* vertex indices */
	int *uvw_indices;   /* texture coordinate indices */
	int *norm_indices;  /* normal vector indices */
};



/* obj model structure */
class obj_model_t
{
public:
	//! constructor
	obj_model_t();

	//! destructor
	~obj_model_t();

	//! compile map for fast display
	void Compile(int YLimit);

	//! render
	void Render();

	//! accessors
	bool HasTextures(){return (texCoords.size()>0);}
	bool HasNormals(){return (normals.size()>0);}

	std::vector<obj_vertex_t> &GetVertices(){return vertices;}
	std::vector<obj_texCoord_t> &GetTexCoords(){return texCoords;}
	std::vector<obj_normal_t> &GetNormals(){return normals;}
	std::vector<obj_face_t*> &GetFaces(){return faces;}

	void SaveToMapFile(const std::string & filename);


protected:
	void Uncompile();

private:
	std::vector<obj_vertex_t> vertices;			/* vertex list */
	std::vector<obj_texCoord_t> texCoords;		/* tex. coord. list */
	std::vector<obj_normal_t> normals;			/* normal vector list */
	std::vector<obj_face_t *> faces;			/* model's polygons */

	unsigned int _list_name;
	bool		_compiled;
};



/***********************************************************************
 * Module:  ObjFileLoader.h
 * Author:  Vivien
 * Modified: dimanche 12 juillet 2009 21:09:21
 * Purpose: Declaration of the class ObjFileLoader
 ***********************************************************************/
class ObjFileLoader
{
public:
	static obj_model_t * LoadObjFile(const std::string &filename);

private:
	static void SecondPass (FILE *fp, obj_model_t *mdl);
};

#endif