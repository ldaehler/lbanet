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

#include "lba_map_gl.h"
#include "PhysicHandler.h"
//#include "DataLoader.h"



#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>

#include <windows.h>    // Header File For Windows
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#include <GL/glu.h>     // Header File For The GLu32 Library




/***********************************************************
calculate a face
***********************************************************/
void LBA_MAP_GL::face(double X,double Y,double Z,double texture_x,double texture_y,
					  double h,int a,int b,int c,bool hidden)
{
    double vertex[20][3];
    double uv[24][2];
    for(int i=0;i<20;i++)
    vertex[i][0]=vertex[i][1]=vertex[i][2]=0.;

    vertex[ 0][0]=X+0.;vertex[ 0][1]=Y*h+h ,vertex[ 0][2]=Z+0.;
    vertex[ 1][0]=X+0.;vertex[ 1][1]=Y*h+h ,vertex[ 1][2]=Z+0.;
    vertex[ 2][0]=X+0.;vertex[ 2][1]=Y*h+h ,vertex[ 2][2]=Z+1.;
    vertex[ 3][0]=X+0.;vertex[ 3][1]=Y*h+h ,vertex[ 3][2]=Z+1.;
    vertex[ 4][0]=X+0.;vertex[ 4][1]=Y*h+0.,vertex[ 4][2]=Z+0.;
    vertex[ 5][0]=X+0.;vertex[ 5][1]=Y*h+0.,vertex[ 5][2]=Z+0.;
    vertex[ 6][0]=X+0.;vertex[ 6][1]=Y*h+0.,vertex[ 6][2]=Z+0.;
    vertex[ 7][0]=X+0.;vertex[ 7][1]=Y*h+0.,vertex[ 7][2]=Z+0.;
    vertex[ 8][0]=X+1.;vertex[ 8][1]=Y*h+h ,vertex[ 8][2]=Z+0.;
    vertex[ 9][0]=X+1.;vertex[ 9][1]=Y*h+h ,vertex[ 9][2]=Z+0.;
    vertex[10][0]=X+0.;vertex[10][1]=Y*h+0.,vertex[10][2]=Z+1.;
    vertex[11][0]=X+0.;vertex[11][1]=Y*h+0.,vertex[11][2]=Z+1.;
    vertex[12][0]=X+1.;vertex[12][1]=Y*h+h ,vertex[12][2]=Z+1.;
    vertex[13][0]=X+1.;vertex[13][1]=Y*h+h ,vertex[13][2]=Z+1.;
    vertex[14][0]=X+1.;vertex[14][1]=Y*h+h ,vertex[14][2]=Z+1.;
    vertex[15][0]=X+1.;vertex[15][1]=Y*h+h ,vertex[15][2]=Z+1.;
    vertex[16][0]=X+1.;vertex[16][1]=Y*h+0.,vertex[16][2]=Z+0.;
    vertex[17][0]=X+1.;vertex[17][1]=Y*h+0.,vertex[17][2]=Z+0.;
    vertex[18][0]=X+1.;vertex[18][1]=Y*h+0.,vertex[18][2]=Z+1.;
    vertex[19][0]=X+1.;vertex[19][1]=Y*h+0.,vertex[19][2]=Z+1.;

	//UV shared + OFFSET 0.5 NOT MIRRORED
    uv[0][0]=23.5;   uv[0][1]=0.5;
    uv[1][0]=23.5;   uv[1][1]=0.5;
    uv[2][0]=0.5;    uv[2][1]=11;
    uv[3][0]=0.5;   uv[3][1]=13.;
    uv[4][0]=23.5;  uv[4][1]=13.5;
    uv[5][0]=23.5;  uv[5][1]=13.5;
    uv[6][0]=23.5;  uv[6][1]=15.5;
    uv[7][0]=23.5;  uv[7][1]=15.5;
    uv[8][0]=46.5;  uv[8][1]=11;
    uv[9][0]=46.5;  uv[9][1]=13.;
    uv[10][0]=0.5;   uv[10][1]=24.5;
    uv[11][0]=0.5;   uv[11][1]=26;
    uv[12][0]=23.5; uv[12][1]=21.5;
    uv[13][0]=23.5;  uv[13][1]=21.5;
    uv[14][0]=22.5; uv[14][1]=24.;
    uv[15][0]=24.5; uv[15][1]=24.;
    uv[16][0]=46.5; uv[16][1]=24.5;
    uv[17][0]=46.5; uv[17][1]=26;
    uv[18][0]=22.5; uv[18][1]=36.5;
    uv[19][0]=24.5; uv[19][1]=36.5;

    double ab[3];
    ab[0]=vertex[b][0]-vertex[a][0];
    ab[1]=vertex[b][1]-vertex[a][1];
    ab[2]=vertex[b][2]-vertex[a][2];
    double ac[3];
    ac[0]=vertex[c][0]-vertex[a][0];
    ac[1]=vertex[c][1]-vertex[a][1];
    ac[2]=vertex[c][2]-vertex[a][2];
    double n[3];
    n[0]=ab[1]*ac[2]-ac[1]*ab[2];
    n[1]=ab[2]*ac[0]-ac[2]*ab[0];
    n[2]=ab[0]*ac[1]-ac[0]*ab[1];
    double l=sqrt(n[0]*n[0]+n[1]*n[1]+n[2]*n[2]);
    double g[3];
    g[0]=(vertex[a][0]+vertex[b][0]+vertex[c][0])/3.;
    g[1]=(vertex[a][1]+vertex[b][1]+vertex[c][1])/3.;
    g[2]=(vertex[a][2]+vertex[b][2]+vertex[c][2])/3.;
	double f=0.5;


	LBA_FACE face;
	face.v[0]=vertex[a][0];
	face.v[1]=vertex[a][1];
	face.v[2]=vertex[a][2];
	face.v[3]=vertex[b][0];
	face.v[4]=vertex[b][1];
	face.v[5]=vertex[b][2];
	face.v[6]=vertex[c][0];
	face.v[7]=vertex[c][1];
	face.v[8]=vertex[c][2];
	if(hidden)
	{
		face.vt[0]=0.;
		face.vt[1]=0.;
		face.vt[2]=0.;
		face.vt[3]=0.;
		face.vt[4]=0.;
		face.vt[5]=0.;
	}
	else
	{
		face.vt[0]=(texture_x+uv[a][0])/2048.;
		face.vt[1]=(texture_y+uv[a][1])/2048.;
		face.vt[2]=(texture_x+uv[b][0])/2048.;
		face.vt[3]=(texture_y+uv[b][1])/2048.;
		face.vt[4]=(texture_x+uv[c][0])/2048.;
		face.vt[5]=(texture_y+uv[c][1])/2048.;
	}

	face.vn[0]=n[0]/l;
	face.vn[1]=n[1]/l;
	face.vn[2]=n[2]/l;
	//lba_face.push_back(face);

	glNormal3d(face.vn[0],face.vn[1],face.vn[2]);
	glTexCoord2d(face.vt[0],face.vt[1]);
	glVertex3d(face.v[0],face.v[1],face.v[2]);

	glNormal3d(face.vn[0],face.vn[1],face.vn[2]);
	glTexCoord2d(face.vt[2],face.vt[3]);
	glVertex3d(face.v[3],face.v[4],face.v[5]);

	glNormal3d(face.vn[0],face.vn[1],face.vn[2]);
	glTexCoord2d(face.vt[4],face.vt[5]);
	glVertex3d(face.v[6],face.v[7],face.v[8]);
}




/***********************************************************
load map into memory
***********************************************************/
void LBA_MAP_GL::LoadMap(const std::string &filename)
{
	std::ifstream fileM(filename.c_str());
	std::vector<std::string> brick_indexes;

	// get map size
	fileM >> _sizeX;
	fileM >> _sizeY;
	fileM >> _sizeZ;
	_phH->Allocate(_sizeX, _sizeY, _sizeZ);


	// get the map info
	{
		int nbImgToLoad = 0;
		fileM >> nbImgToLoad;

		for(int i=0; i<nbImgToLoad; ++i)
		{
			std::string path;
			fileM >> path;
			brick_indexes.push_back(path);
		}
	}

	// load the map bricks info
	{
		_brickbuffer = new int[_sizeX*_sizeY*_sizeZ];

		int *	brickbufferPtr = _brickbuffer;
		short *	physicbufferPtr = _phH->GetBufferPtr();
		short *	materialbufferPtr = _phH->GetMaterialBufferPtr();

		for(int y=0; y<_sizeY; ++y)
		{
			for(int x=0; x<_sizeX; ++x)
			{
				for(int z=0; z<_sizeZ; ++z)
				{
					fileM >> *brickbufferPtr;
					++brickbufferPtr;
				}
			}

			for(int x=0; x<_sizeX; ++x)
			{
				for(int z=0; z<_sizeZ; ++z)
				{
					fileM >> *physicbufferPtr;
					++physicbufferPtr;
				}
			}

			for(int x=0; x<_sizeX; ++x)
			{
				for(int z=0; z<_sizeZ; ++z)
				{
					fileM >> *materialbufferPtr;
					++materialbufferPtr;
				}
			}
		}
	}



	// load the map texture
	{
		unsigned char *texture_map=new unsigned char[2048*2048*4];
		for(size_t i=0; i<brick_indexes.size(); ++i)
		{
			{
				//ILuint imn;
				//ilGenImages(1, &imn);
				//ilBindImage(imn);

				//ilLoadImage(("Data/"+brick_indexes[i]).c_str());
				//unsigned char * imgptr = ilGetData();

				//if(imgptr != NULL)
				//{
				//	for(int j=0;j<38;j++)
				//	{
				//		for(int k=0;k<48;k++)
				//		{
				//			int offset_x=(i%42);offset_x*=48;
				//			int offset_y=(i/42);offset_y*=38;
				//			texture_map[((offset_y+j)*2048+offset_x+k)*4+0]=*imgptr++;
				//			texture_map[((offset_y+j)*2048+offset_x+k)*4+1]=*imgptr++;
				//			texture_map[((offset_y+j)*2048+offset_x+k)*4+2]=*imgptr++;
				//			texture_map[((offset_y+j)*2048+offset_x+k)*4+3]=*imgptr++;
				//		}
				//	}
				//}

				//ilDeleteImages(1, &imn);
			}

		}



		glGenTextures(1, &texture_name);
		//glColorMask(GL_FALSE, GL_FALSE, GL_TRUE, GL_FALSE);
		glEnable(GL_TEXTURE_2D);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glColor3f(1,1,1);
		//glEnable (GL_BLEND); glBlendFunc (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D, texture_name);

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, /*GL_CLAMP*/ GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, /*GL_CLAMP*/ GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR/*GL_NEAREST*/);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR/*GL_NEAREST*/);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2048, 2048, 0, GL_RGBA, GL_UNSIGNED_BYTE,texture_map);


		delete texture_map;
	}
}


/***********************************************************
return the brick index use at a specific map position
***********************************************************/
int LBA_MAP_GL::GetBrickIndex(int X, int Y, int Z)
{
	return _brickbuffer[Y*_sizeX*_sizeZ + X*_sizeZ + Z];
}


/***********************************************************
constructor
***********************************************************/
LBA_MAP_GL::LBA_MAP_GL(const std::string &filename, PhysicHandler * phH)
: _phH(phH)
{
	LoadMap(filename);

	list_name=glGenLists(1);
	RecompileRoom(-1);




}

/***********************************************************
perpare the room for display
***********************************************************/
void LBA_MAP_GL::RecompileRoom(int Cut)
{
	int texture_x,texture_y;
	int X,Y,Z;
	int cmpt=0;
	brick_number=cmpt;
	double h=0.5;

	int Ymax = _sizeY;

	if(Cut > 0)
		if(Cut < 18 && Cut > 13)
			Ymax = 13;
		else
			Ymax = Cut;

	glDeleteLists(list_name,1);
	glNewList(list_name, GL_COMPILE);
	glBegin(GL_QUADS);

	for(Z=0;Z<_sizeZ;Z++)
	for(X=0;X<_sizeX;X++)
	for(Y=0;Y<Ymax;Y++)
	{
		int idx = GetBrickIndex(X, Y, Z);
		if(idx != 0)
		{
			cmpt++;
			texture_x=(idx-1)%42;
			texture_x*=48;
			texture_y=(idx-1)/42;
			texture_y*=38;

			short shp = _phH->GetStructure(X, Y, Z);

			if(shp == 0 || shp == 1 || shp > 13)
			{
				glNormal3d(0,0,1);glTexCoord2d((texture_x+0.) /2048.,(texture_y+25.)/2048.);  glVertex3d( X+0.,Y/2.+0. , Z+1.);
				glNormal3d(0,0,1);glTexCoord2d((texture_x+23.)/2048.,(texture_y+36.)/2048.);  glVertex3d( X+1.,Y/2.+0. , Z+1.);
				glNormal3d(0,0,1);glTexCoord2d((texture_x+23.)/2048.,(texture_y+23.)/2048.);  glVertex3d( X+1.,Y/2.+0.5, Z+1.);
				glNormal3d(0,0,1);glTexCoord2d((texture_x+0. )/2048.,(texture_y+13.)/2048.);  glVertex3d( X+0.,Y/2.+0.5, Z+1.);

				glNormal3d(1,0,0);glTexCoord2d((texture_x+24.)/2048.,(texture_y+36.)/2048.);  glVertex3d( X+1.,Y/2.+0. , Z+1.);
				glNormal3d(1,0,0);glTexCoord2d((texture_x+47.)/2048.,(texture_y+25.)/2048.);  glVertex3d( X+1.,Y/2.+0. , Z+0.);
				glNormal3d(1,0,0);glTexCoord2d((texture_x+47.)/2048.,(texture_y+12.)/2048.);  glVertex3d( X+1.,Y/2.+0.5, Z+0.);
				glNormal3d(1,0,0);glTexCoord2d((texture_x+24.)/2048.,(texture_y+24.)/2048.);  glVertex3d( X+1.,Y/2.+0.5, Z+1.);

				glNormal3d(0,1,0);glTexCoord2d((texture_x+2.) /2048.,(texture_y+11.)/2048.);  glVertex3d( X+0.,Y/2.+0.5, Z+1.);
				glNormal3d(0,1,0);glTexCoord2d((texture_x+24.)/2048.,(texture_y+22.)/2048.);  glVertex3d( X+1.,Y/2.+0.5, Z+1.);
				glNormal3d(0,1,0);glTexCoord2d((texture_x+45.)/2048.,(texture_y+11.)/2048.);  glVertex3d( X+1.,Y/2.+0.5, Z+0.);
				glNormal3d(0,1,0);glTexCoord2d((texture_x+24.)/2048.,(texture_y+0. )/2048.);  glVertex3d( X+0.,Y/2.+0.5, Z+0.);
			}
		}
	}

	glEnd();


	glBegin(GL_TRIANGLES);

	for(Z=0;Z<_sizeZ;Z++)
	for(X=0;X<_sizeX;X++)
	for(Y=0;Y<Ymax;Y++)
	{
		int idx = GetBrickIndex(X, Y, Z);
		if(idx != 0)
		{
			cmpt++;
			texture_x=(idx-1)%42;
			texture_x*=48;
			texture_y=(idx-1)/42;
			texture_y*=38;


			switch(_phH->GetStructure(X, Y, Z))
			{
				//case 0: case 1: case 15:
				//	face(X,Y,Z,texture_x,texture_y,h,3,11,18,false);
				//	face(X,Y,Z,texture_x,texture_y,h,18,14,3,false);
				//	face(X,Y,Z,texture_x,texture_y,h,15,19,17,false);
				//	face(X,Y,Z,texture_x,texture_y,h,17,9,15,false);
				//	face(X,Y,Z,texture_x,texture_y,h,0,2,12,false);
				//	face(X,Y,Z,texture_x,texture_y,h,13,8,1,false);
				//	break;
				case 2:
					face(X,Y,Z,texture_x,texture_y,h,11,18,14,false);
					face(X,Y,Z,texture_x,texture_y,h,11,13,8,false);
					face(X,Y,Z,texture_x,texture_y,h,8,6,11,false);
					face(X,Y,Z,texture_x,texture_y,h,19,17,9,false);
					face(X,Y,Z,texture_x,texture_y,h,9,15,19,false);
					face(X,Y,Z,texture_x,texture_y,h,9,16,5,false);
					break;
				case 3:
					face(X,Y,Z,texture_x,texture_y,h,15,19,17,false);
					face(X,Y,Z,texture_x,texture_y,h,2,12,17,false);
					face(X,Y,Z,texture_x,texture_y,h,17,7,2,false);
					face(X,Y,Z,texture_x,texture_y,h,11,18,14,false);
					face(X,Y,Z,texture_x,texture_y,h,14,3,10,false);
					face(X,Y,Z,texture_x,texture_y,h,2,4,10,false);
					break;
				case 4:
					face(X,Y,Z,texture_x,texture_y,h,11,18,8,false);
					face(X,Y,Z,texture_x,texture_y,h,8,1,11,false);
					face(X,Y,Z,texture_x,texture_y,h,19,17,9,false);
					face(X,Y,Z,texture_x,texture_y,h,1,8,16,false);
					face(X,Y,Z,texture_x,texture_y,h,16,5,1,false);
					face(X,Y,Z,texture_x,texture_y,h,0,4,10,false);
					break;
				case 5:
					face(X,Y,Z,texture_x,texture_y,h,19,17,0,false);
					face(X,Y,Z,texture_x,texture_y,h,0,2,19,false);
					face(X,Y,Z,texture_x,texture_y,h,3,11,18,false);
					face(X,Y,Z,texture_x,texture_y,h,1,16,5,false);
					face(X,Y,Z,texture_x,texture_y,h,2,0,4,false);
					face(X,Y,Z,texture_x,texture_y,h,4,10,3,false);
					break;
				case 6:
					face(X,Y,Z,texture_x,texture_y,h,4,2,12,false);
					face(X,Y,Z,texture_x,texture_y,h,13,8,5,false);
					face(X,Y,Z,texture_x,texture_y,h,3,11,18,false);
					face(X,Y,Z,texture_x,texture_y,h,18,14,3,false);
					face(X,Y,Z,texture_x,texture_y,h,19,17,9,false);
					face(X,Y,Z,texture_x,texture_y,h,9,15,19,false);
					face(X,Y,Z,texture_x,texture_y,h,5,9,16,false);
					face(X,Y,Z,texture_x,texture_y,h,4,3,10,false);
					break;
				case 7:
					face(X,Y,Z,texture_x,texture_y,h,19,8,1,false);
					face(X,Y,Z,texture_x,texture_y,h,0,2,18,false);
					face(X,Y,Z,texture_x,texture_y,h,3,11,18,false);
					face(X,Y,Z,texture_x,texture_y,h,19,17,9,false);
					face(X,Y,Z,texture_x,texture_y,h,1,8,16,false);
					face(X,Y,Z,texture_x,texture_y,h,16,5,1,false);
					face(X,Y,Z,texture_x,texture_y,h,10,2,0,false);
					face(X,Y,Z,texture_x,texture_y,h,0,4,10,false);
					break;
				case 8:
					face(X,Y,Z,texture_x,texture_y,h,0,4,10,false);
					face(X,Y,Z,texture_x,texture_y,h,10,13,8,false);
					face(X,Y,Z,texture_x,texture_y,h,8,1,10,false);
					face(X,Y,Z,texture_x,texture_y,h,11,18,14,false);
					face(X,Y,Z,texture_x,texture_y,h,15,19,17,false);
					face(X,Y,Z,texture_x,texture_y,h,17,9,15,false);
					face(X,Y,Z,texture_x,texture_y,h,1,8,16,false);
					face(X,Y,Z,texture_x,texture_y,h,16,5,1,false);
					break;
				case 9:
					face(X,Y,Z,texture_x,texture_y,h,16,0,2,false);
					face(X,Y,Z,texture_x,texture_y,h,2,12,16,false);
					face(X,Y,Z,texture_x,texture_y,h,3,11,18,false);
					face(X,Y,Z,texture_x,texture_y,h,18,14,3,false);
					face(X,Y,Z,texture_x,texture_y,h,15,19,17,false);
					face(X,Y,Z,texture_x,texture_y,h,16,5,1,false);
					face(X,Y,Z,texture_x,texture_y,h,10,2,0,false);
					face(X,Y,Z,texture_x,texture_y,h,0,4,10,false);
					break;
				case 10:
					face(X,Y,Z,texture_x,texture_y,h,6,11,12,false);
					face(X,Y,Z,texture_x,texture_y,h,13,17,7,false);
					face(X,Y,Z,texture_x,texture_y,h,11,18,14,false);
					face(X,Y,Z,texture_x,texture_y,h,15,19,17,false);
					break;
				case 11:
					face(X,Y,Z,texture_x,texture_y,h,0,11,18,false);
					face(X,Y,Z,texture_x,texture_y,h,19,17,1,false);
					face(X,Y,Z,texture_x,texture_y,h,0,4,10,false);
					face(X,Y,Z,texture_x,texture_y,h,1,5,16,false);
					break;
				case 12:
					face(X,Y,Z,texture_x,texture_y,h,11,18,9,false);
					face(X,Y,Z,texture_x,texture_y,h,8,4,10,false);
					face(X,Y,Z,texture_x,texture_y,h,19,17,9,false);
					face(X,Y,Z,texture_x,texture_y,h,8,16,5,false);
					break;
				case 13:
					face(X,Y,Z,texture_x,texture_y,h,3,19,17,false);
					face(X,Y,Z,texture_x,texture_y,h,16,5,2,false);
					face(X,Y,Z,texture_x,texture_y,h,2,4,10,false);
					face(X,Y,Z,texture_x,texture_y,h,3,18,11,false);
					break;
			}
		}
	}

	glEnd();


	glEndList();
}


/***********************************************************
destructor
***********************************************************/
LBA_MAP_GL::~LBA_MAP_GL()
{
	delete _brickbuffer;

    glDeleteTextures(1,&texture_name);
    glDeleteLists(list_name,1);
}

