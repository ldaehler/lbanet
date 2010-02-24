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

#include "ObjFileLoader.h"

#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#include <windows.h>    // Header File For Windows
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#include <GL/glu.h>     // Header File For The GLu32 Library




/***********************************************************
constructor
***********************************************************/
obj_face_t::obj_face_t(int numelems, bool hasTextures, bool hasNormals)
	: vert_indices(NULL), uvw_indices(NULL), norm_indices(NULL), num_elems(numelems)
{
	if (numelems == 3)
	{
		type = GL_TRIANGLES;
	}
	else if (numelems == 4)
	{
		type= GL_QUADS;
	}
	else
	{
		type = GL_POLYGON;
	}

	vert_indices = new int[numelems];

	if(hasTextures)
		uvw_indices = new int[numelems];

	if(hasNormals)
		norm_indices = new int[numelems];
}



/***********************************************************
destructor
***********************************************************/
obj_face_t::~obj_face_t()
{
	if(vert_indices)
		delete[] vert_indices;

	if(uvw_indices)
		delete[] uvw_indices;

	if(norm_indices)
		delete[] norm_indices;
}



/***********************************************************
constructor
***********************************************************/
obj_model_t::obj_model_t()
{
}

/***********************************************************
destructor
***********************************************************/
obj_model_t::~obj_model_t()
{
	for(size_t i=0; i<faces.size(); ++i)
		delete faces[i];

	Uncompile();
}

/***********************************************************
Uncompile
***********************************************************/
void obj_model_t::Uncompile()
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
void obj_model_t::Compile(int YLimit)
{
	Uncompile();
	glNewList(_list_name, GL_COMPILE);

	std::vector<obj_face_t *>::iterator itface = faces.begin();
	std::vector<obj_face_t *>::iterator endface = faces.end();
	for(;itface != endface; ++itface)
	{
		glBegin((*itface)->type);
		for(int j = 0; j < (*itface)->num_elems; ++j)
		{
			if(HasTextures())
				glTexCoord3fv(texCoords[(*itface)->uvw_indices[j]].uvw);

			if(HasNormals())
				glNormal3fv(normals[(*itface)->norm_indices[j]].ijk);

			glVertex4fv(vertices[(*itface)->vert_indices[j]].xyzw);
		}
		glEnd();
	}

	glEndList();
}


/***********************************************************
Render
***********************************************************/
void obj_model_t::Render()
{
	glCallList(_list_name);
}


/***********************************************************
SaveToFile
***********************************************************/
void obj_model_t::SaveToMapFile(const std::string & filename)
{
	std::ofstream file(filename.c_str(), std::fstream::binary);
	int size = faces.size();
	file.write((char*)&size, sizeof(int));
	//file<<faces.size()<<std::endl;

	std::vector<obj_face_t *>::iterator itface = faces.begin();
	std::vector<obj_face_t *>::iterator endface = faces.end();
	for(;itface != endface; ++itface)
	{
		for(int j = 0; j < (*itface)->num_elems; ++j)
		{
			if(HasTextures())
				file.write((char*)texCoords[(*itface)->uvw_indices[j]].uvw, 3*sizeof(float));
				//file<<texCoords[(*itface)->uvw_indices[j]].uvw[0]<<" "<<texCoords[(*itface)->uvw_indices[j]].uvw[1]<<" "<<texCoords[(*itface)->uvw_indices[j]].uvw[2]<<std::endl;

			if(HasNormals())
				file.write((char*)normals[(*itface)->norm_indices[j]].ijk, 3*sizeof(float));
				//file<<normals[(*itface)->norm_indices[j]].ijk[0]<<" "<<normals[(*itface)->norm_indices[j]].ijk[1]<<" "<<normals[(*itface)->norm_indices[j]].ijk[2]<<std::endl;

			file.write((char*)vertices[(*itface)->vert_indices[j]].xyzw, 4*sizeof(float));
			//file<<vertices[(*itface)->vert_indices[j]].xyzw[0]<<" "<<vertices[(*itface)->vert_indices[j]].xyzw[1]<<" "<<vertices[(*itface)->vert_indices[j]].xyzw[2]<<" "<<vertices[(*itface)->vert_indices[j]].xyzw[3]<<std::endl;
		}
	}
}


/***********************************************************
reload the current map texture
***********************************************************/
obj_model_t *ObjFileLoader::LoadObjFile(const std::string &filename)
{
	obj_model_t *mdl = NULL; 

	FILE *fp;
	fp = fopen (filename.c_str(), "r");
	if (!fp)
	{
		fprintf (stderr, "error: couldn't open \"%s\"!", filename.c_str());
		return mdl;
	}

	/* first pass: read model info */
	mdl = new obj_model_t();


	/* second pass: read model data */
	SecondPass (fp, mdl);

	mdl->SaveToMapFile("Test.map");
	mdl->Compile(-1);

	fclose (fp);
	return mdl;
}



/***********************************************************
SecondPass
***********************************************************/
void ObjFileLoader::SecondPass (FILE *fp, obj_model_t *mdl)
{
	std::vector<obj_vertex_t> &pverts = mdl->GetVertices();
	std::vector<obj_texCoord_t> &puvws = mdl->GetTexCoords();
	std::vector<obj_normal_t> &pnorms = mdl->GetNormals();
	std::vector<obj_face_t *> &pfaces = mdl->GetFaces();

	char buf[128], *pbuf;

	while (!feof (fp))
	{
		/* read whole line */
		fgets (buf, sizeof (buf), fp);

		switch (buf[0])
		{
			case 'v':
			{
				if (buf[1] == ' ')
				{
					obj_vertex_t pvert;

					/* vertex */
					if (sscanf (buf + 2, "%f %f %f %f",
							&pvert.xyzw[0], &pvert.xyzw[1],
							&pvert.xyzw[2], &pvert.xyzw[3]) != 4)
					{
						if (sscanf (buf + 2, "%f %f %f", &pvert.xyzw[0],
								&pvert.xyzw[1], &pvert.xyzw[2] ) != 3)
						{
							fprintf (stderr, "error reading vertex data!\n");
							return;
						}
						else
						{
							pvert.xyzw[3] = 1.0f;
						}
					}

					pverts.push_back(pvert);
				}
				else if (buf[1] == 't')
				{
					obj_texCoord_t puvw;

					/* texture coords. */
					if (sscanf (buf + 2, "%f %f %f", &puvw.uvw[0],
						&puvw.uvw[1], &puvw.uvw[2]) != 3)
					{
						if (sscanf (buf + 2, "%f %f", &puvw.uvw[0],
							&puvw.uvw[1]) != 2)
						{
							if (sscanf (buf + 2, "%f", &puvw.uvw[0]) != 1)
							{
								fprintf (stderr, "error reading texture coordinates!\n");
								return;
							}
							else
							{
								puvw.uvw[1] = 0.0;
								puvw.uvw[2] = 0.0;
							}
						}
						else
						{
							puvw.uvw[2] = 0.0;
						}
					}

					puvws.push_back(puvw);
				}
				else if (buf[1] == 'n')
				{
					obj_normal_t pnorm;

					/* normal vector */
					if (sscanf (buf + 2, "%f %f %f", &pnorm.ijk[0],
						&pnorm.ijk[1], &pnorm.ijk[2]) != 3)
					{
						fprintf (stderr, "error reading normal vectors!\n");
						return;
					}

					pnorms.push_back(pnorm);
				}

				break;
			}

			case 'f':
			{
				pbuf = buf;
				int face_num_elems = 0;

				/* count number of vertices for this face */
				while (*pbuf)
				{
					if (*pbuf == ' ')
						++face_num_elems;

					pbuf++;
				}

				/* memory allocation for vertices */
				obj_face_t *tmpface = new obj_face_t(face_num_elems, mdl->HasTextures(), mdl->HasNormals());


				/* read face data */
				pbuf = buf;
				for (int i = 0; i < face_num_elems; ++i)
				{
					pbuf = strchr (pbuf, ' ');
					pbuf++; /* skip space */

					/* try reading vertices */
					if(sscanf (pbuf, "%d/%d/%d",
						&tmpface->vert_indices[i],
						&tmpface->uvw_indices[i],
						&tmpface->norm_indices[i]) != 3)
					{
						if(sscanf (pbuf, "%d//%d", &tmpface->vert_indices[i],
							&tmpface->norm_indices[i]) != 2)
						{
							if(sscanf (pbuf, "%d/%d", &tmpface->vert_indices[i],
								&tmpface->uvw_indices[i]) != 2)
							{
								sscanf (pbuf, "%d", &tmpface->vert_indices[i]);
							}
						}
					}

					/* indices must start at 0 */
					tmpface->vert_indices[i]--;

					if (mdl->HasTextures())
						tmpface->uvw_indices[i]--;

					if (mdl->HasNormals())
						tmpface->norm_indices[i]--;
				}


				pfaces.push_back(tmpface);
				break;
			}
		}
	}
}