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

#include "MapRenderer.h"
#include "lba_map_gl.h"
#include "PhysicHandler.h"

#include <windows.h>    // Header File For Windows
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#include <GL/glu.h>     // Header File For The GLu32 Library


/***********************************************************
	Constructor
***********************************************************/
MapRenderer::MapRenderer(const std::string &filename, PhysicHandler * phH)
: _map_gl(NULL), _phH(phH)
{
	LoadMap(filename);
}

/***********************************************************
	destructor
***********************************************************/
MapRenderer::~MapRenderer()
{
	CleanUp();
}

/***********************************************************
	cleanup
***********************************************************/
void MapRenderer::CleanUp()
{
	if(_map_gl)
	{
		delete _map_gl;
		_map_gl = NULL;
	}
}

/***********************************************************
	load new map
***********************************************************/
bool MapRenderer::LoadMap(const std::string &filename)
{
	CleanUp();

	_currentmap_file = filename;
	_map_gl = new LBA_MAP_GL(filename, _phH);
	_current_cut = -1;
	_phH->SearchFloors();
	_phH->SearchWallX();
	_phH->SearchWallZ();
	_phH->SearchStairs();
	_phH->SavePlanes("map3.phy");
	
	return true;
}

/***********************************************************
	render
***********************************************************/
void MapRenderer::Render()
{
	if(_map_gl)
	{
		glColor4d(1,1,1,1);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_COLOR_MATERIAL);
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _map_gl->texture_name);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);

		//glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
		//glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glCallList(_map_gl->list_name);
	}


	if(_phH)
	{
		std::vector<PlaneInfo> planes = _phH->GetPlanes();
		std::vector<PlaneInfo> planess = _phH->GetPlanesSee();
		std::vector<PlaneInfo> WallX = _phH->GetWallsX();
		std::vector<PlaneInfo> WallXh = _phH->GetWallsXHidden();
		std::vector<PlaneInfo> WallZ = _phH->GetWallsZ();
		std::vector<PlaneInfo> WallZh = _phH->GetWallsZHidden();
		std::vector<StairInfo> stairs = _phH->GetStairs();
		std::vector<CornerStairInfo> cstairs = _phH->GetCornerStairs();

		glEnable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);

		glLineWidth(2.0f);


		for(size_t i=0; i<cstairs.size(); ++i)
		{
			CornerStairInfo pif = cstairs[i];
			glPushMatrix();

			glTranslated(0, 0.5, 0);
			glColor4f(0.0f,0.0f,1.0f, 1.f);
			glBegin(GL_LINES);
				glVertex3f(pif.C1X,pif.C1Y/2.0f,pif.C1Z);
				glVertex3f(pif.C2X,pif.C2Y/2.0f,pif.C2Z);
				glVertex3f(pif.C2X,pif.C2Y/2.0f,pif.C2Z);
				glVertex3f(pif.C3X,pif.C3Y/2.0f,pif.C3Z);
				glVertex3f(pif.C3X,pif.C3Y/2.0f,pif.C3Z);
				glVertex3f(pif.C1X,pif.C1Y/2.0f,pif.C1Z);
			glEnd();

			glPopMatrix();
		}


		for(size_t i=0; i<stairs.size(); ++i)
		{
			StairInfo pif = stairs[i];
			glPushMatrix();

			glTranslated(0, 0.5, 0);
			glColor4f(0.0f,0.0f,1.0f, 1.f);
			glBegin(GL_LINES);
				glVertex3f(pif.C1X,pif.C1Y/2.0f,pif.C1Z);
				glVertex3f(pif.C2X,pif.C2Y/2.0f,pif.C2Z);
				glVertex3f(pif.C2X,pif.C2Y/2.0f,pif.C2Z);
				glVertex3f(pif.C4X,pif.C4Y/2.0f,pif.C4Z);
				glVertex3f(pif.C4X,pif.C4Y/2.0f,pif.C4Z);
				glVertex3f(pif.C3X,pif.C3Y/2.0f,pif.C3Z);
				glVertex3f(pif.C3X,pif.C3Y/2.0f,pif.C3Z);
				glVertex3f(pif.C1X,pif.C1Y/2.0f,pif.C1Z);
			glEnd();

			glPopMatrix();
		}




		//for(size_t i=0; i<planes.size(); ++i)
		//{
		//	PlaneInfo pif = planes[i];
		//	glPushMatrix();

		//	glTranslated(0, pif.StartY/2. + 0.5, 0);
		//	glColor4f(0.0f,0.0f,1.0f, 1.f);
		//	glBegin(GL_LINES);
		//		glVertex3f(pif.StartX,0,pif.StartZ);
		//		glVertex3f(pif.EndX,0,pif.StartZ);
		//		glVertex3f(pif.EndX,0,pif.StartZ);
		//		glVertex3f(pif.EndX,0,pif.EndZ);
		//		glVertex3f(pif.EndX,0,pif.EndZ);
		//		glVertex3f(pif.StartX,0,pif.EndZ);
		//		glVertex3f(pif.StartX,0,pif.EndZ);
		//		glVertex3f(pif.StartX,0,pif.StartZ);
		//	glEnd();

		//	glPopMatrix();
		//}



		//for(size_t i=0; i<planess.size(); ++i)
		//{
		//	PlaneInfo pif = planess[i];
		//	glPushMatrix();

		//	glTranslated(0, pif.StartY/2. + 0.5, 0);
		//	glColor4f(0.0f,1.0f,0.0f, 1.f);
		//	glBegin(GL_LINES);
		//		glVertex3f(pif.StartX,0,pif.StartZ);
		//		glVertex3f(pif.EndX,0,pif.StartZ);
		//		glVertex3f(pif.EndX,0,pif.StartZ);
		//		glVertex3f(pif.EndX,0,pif.EndZ);
		//		glVertex3f(pif.EndX,0,pif.EndZ);
		//		glVertex3f(pif.StartX,0,pif.EndZ);
		//		glVertex3f(pif.StartX,0,pif.EndZ);
		//		glVertex3f(pif.StartX,0,pif.StartZ);
		//	glEnd();

		//	glPopMatrix();
		//}


		//for(size_t i=0; i<WallX.size(); ++i)
		//{
		//	PlaneInfo pif = WallX[i];
		//	glPushMatrix();

		//	glTranslated(pif.StartY/*+1*/, 0.5f, 0);
		//	glColor4f(0.0f,0.0f,1.0f, 1.f);
		//	glBegin(GL_LINES);
		//		glVertex3f(0,pif.StartX/2.f,pif.StartZ);
		//		glVertex3f(0,pif.EndX/2.f,pif.StartZ);
		//		glVertex3f(0,pif.EndX/2.f,pif.StartZ);
		//		glVertex3f(0,pif.EndX/2.f,pif.EndZ);
		//		glVertex3f(0,pif.EndX/2.f,pif.EndZ);
		//		glVertex3f(0,pif.StartX/2.f,pif.EndZ);
		//		glVertex3f(0,pif.StartX/2.f,pif.EndZ);
		//		glVertex3f(0,pif.StartX/2.f,pif.StartZ);
		//	glEnd();

		//	glPopMatrix();
		//}

		//for(size_t i=0; i<WallXh.size(); ++i)
		//{
		//	PlaneInfo pif = WallXh[i];
		//	glPushMatrix();

		//	glTranslated(pif.StartY, 0.5f, 0);
		//	glColor4f(1.0f,0.0f,0.0f, 1.f);
		//	glBegin(GL_LINES);
		//		glVertex3f(0,pif.StartX/2.f,pif.StartZ);
		//		glVertex3f(0,pif.EndX/2.f,pif.StartZ);
		//		glVertex3f(0,pif.EndX/2.f,pif.StartZ);
		//		glVertex3f(0,pif.EndX/2.f,pif.EndZ);
		//		glVertex3f(0,pif.EndX/2.f,pif.EndZ);
		//		glVertex3f(0,pif.StartX/2.f,pif.EndZ);
		//		glVertex3f(0,pif.StartX/2.f,pif.EndZ);
		//		glVertex3f(0,pif.StartX/2.f,pif.StartZ);
		//	glEnd();

		//	glPopMatrix();
		//}


		//for(size_t i=0; i<WallZ.size(); ++i)
		//{
		//	PlaneInfo pif = WallZ[i];
		//	glPushMatrix();

		//	glTranslated(0, 0.5f, pif.StartY/*+1*/);
		//	glColor4f(0.0f,0.0f,1.0f, 1.f);
		//	glBegin(GL_LINES);
		//		glVertex3f(pif.StartX,pif.StartZ/2.f,0);
		//		glVertex3f(pif.EndX,pif.StartZ/2.f,0);
		//		glVertex3f(pif.EndX,pif.StartZ/2.f,0);
		//		glVertex3f(pif.EndX,pif.EndZ/2.f,0);
		//		glVertex3f(pif.EndX,pif.EndZ/2.f,0);
		//		glVertex3f(pif.StartX,pif.EndZ/2.f,0);
		//		glVertex3f(pif.StartX,pif.EndZ/2.f,0);
		//		glVertex3f(pif.StartX,pif.StartZ/2.f,0);
		//	glEnd();

		//	glPopMatrix();
		//}

		//for(size_t i=0; i<WallZh.size(); ++i)
		//{
		//	PlaneInfo pif = WallZh[i];
		//	glPushMatrix();

		//	glTranslated(0, 0.5f, pif.StartY);
		//	glColor4f(1.0f,0.0f,0.0f, 1.f);
		//	glBegin(GL_LINES);
		//		glVertex3f(pif.StartX,pif.StartZ/2.f,0);
		//		glVertex3f(pif.EndX,pif.StartZ/2.f,0);
		//		glVertex3f(pif.EndX,pif.StartZ/2.f,0);
		//		glVertex3f(pif.EndX,pif.EndZ/2.f,0);
		//		glVertex3f(pif.EndX,pif.EndZ/2.f,0);
		//		glVertex3f(pif.StartX,pif.EndZ/2.f,0);
		//		glVertex3f(pif.StartX,pif.EndZ/2.f,0);
		//		glVertex3f(pif.StartX,pif.StartZ/2.f,0);
		//	glEnd();

		//	glPopMatrix();
		//}


		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
	}
}


/***********************************************************
cut the room at a certain Y to display only bottom
***********************************************************/
void MapRenderer::SetMapYLimit(int YLimit)
{
	if(_current_cut == YLimit)
		return;

	_current_cut = YLimit;

	if(_map_gl)
		_map_gl->RecompileRoom(_current_cut);
}



/***********************************************************
flush the current map texture
***********************************************************/
void MapRenderer::FlushTexture()
{
	CleanUp();
}

/***********************************************************
reload the current map texture
***********************************************************/
void MapRenderer::ReloadTexture()
{
	_map_gl = new LBA_MAP_GL(_currentmap_file, _phH);
	_map_gl->RecompileRoom(_current_cut);
}
