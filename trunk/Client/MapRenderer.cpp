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
#include "TextWritter.h"
#include "ConfigurationManager.h"
#include "PhysicHandler.h"

#include <windows.h>    // Header File For Windows
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#include <GL/glu.h>     // Header File For The GLu32 Library


std::vector<PlaneInfo> testplanecarace;

/***********************************************************
	Constructor
***********************************************************/
MapRenderer::MapRenderer()
: _map_gl(NULL), _mapinfo(NULL), _phH(NULL)
{
	ConfigurationManager::GetInstance()->GetBool("Options.Video.DisplayExits", _display_exits);
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
bool MapRenderer::LoadMap(const std::string &filename, PhysicHandler * phH,
							const MapInfo * mapinfo)
{
	CleanUp();
	_phH = phH;

	_mapinfo = mapinfo;
	_map_gl = new LBA_MAP_GL(filename, phH);
	_current_cut = -1;
	//_phH->SearchFloors();
	//_phH->SearchWallX();
	//_phH->SearchWallZ();
	_phH->SearchStairs();
		
	//testplanecarace.clear();

	//std::vector<PlaneInfo> planes = _phH->GetPlanes();
	//for(int i=0; i<planes.size(); ++i)
	//{
	//	PlaneInfo pi = planes[i];
	//	std::vector<TexPlaneInfo> textareas;
	//	int sizeX= (pi.EndX-pi.StartX);
	//	int sizeY= (pi.EndZ-pi.StartZ);
	//	short * area = new short[sizeX*sizeY];
	//	short * tmppt = area;
	//	for(int i=0; i<sizeX; ++i)
	//	{
	//		for(int j=0; j<sizeY; ++j)
	//		{
	//			*tmppt = _map_gl->GetBrickIndex(pi.StartX+i, pi.StartY, pi.StartZ+j);
	//			++tmppt;
	//		}
	//	}

	//	_phH->SplitToTexture(area, sizeX, sizeY, textareas);
	//	delete[] area;


	//	for(size_t cc=0; cc<textareas.size(); ++cc)
	//	{
	//		PlaneInfo pfi;
	//		pfi.StartX=textareas[cc].StartX+pi.StartX;
	//		pfi.StartY=pi.StartY;
	//		pfi.StartZ=textareas[cc].StartY+pi.StartZ;

	//		pfi.EndX=textareas[cc].EndX+pi.StartX;
	//		pfi.EndY=pi.EndY;
	//		pfi.EndZ=textareas[cc].EndY+pi.StartZ;
	//		testplanecarace.push_back(pfi);
	//	}
	//}


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

		if(_display_exits)
			DisplayExitZones();
	}


	if(_phH)
	{
		std::vector<PlaneInfo> planes = _phH->GetPlanes();
		std::vector<PlaneInfo> planesh = _phH->GetPlanesHidden();
		std::vector<PlaneInfo> planess = _phH->GetPlanesSee();
		std::vector<PlaneInfo> WallX = _phH->GetWallsX();
		std::vector<PlaneInfo> WallXh = _phH->GetWallsXHidden();
		std::vector<PlaneInfo> WallZ = _phH->GetWallsZ();
		std::vector<PlaneInfo> WallZh = _phH->GetWallsZHidden();
		std::vector<StairInfo> stairs = _phH->GetStairs();

		glEnable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);

		glLineWidth(2.0f);

		//for(size_t i=0; i<testplanecarace.size(); ++i)
		//{
		//	PlaneInfo pif = testplanecarace[i];
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


		for(size_t i=0; i<planesh.size(); ++i)
		{
			PlaneInfo pif = planesh[i];
			glPushMatrix();

			glTranslated(0, pif.StartY/2. + 0.5, 0);
			glColor4f(1.0f,0.0f,0.0f, 1.f);
			glBegin(GL_LINES);
				glVertex3f(pif.StartX,0,pif.StartZ);
				glVertex3f(pif.EndX,0,pif.StartZ);
				glVertex3f(pif.EndX,0,pif.StartZ);
				glVertex3f(pif.EndX,0,pif.EndZ);
				glVertex3f(pif.EndX,0,pif.EndZ);
				glVertex3f(pif.StartX,0,pif.EndZ);
				glVertex3f(pif.StartX,0,pif.EndZ);
				glVertex3f(pif.StartX,0,pif.StartZ);
			glEnd();

			glPopMatrix();
		}


		for(size_t i=0; i<planes.size(); ++i)
		{
			PlaneInfo pif = planes[i];
			glPushMatrix();

			glTranslated(0, pif.StartY/2. + 0.5, 0);
			glColor4f(0.0f,0.0f,1.0f, 1.f);
			glBegin(GL_LINES);
				glVertex3f(pif.StartX,0,pif.StartZ);
				glVertex3f(pif.EndX,0,pif.StartZ);
				glVertex3f(pif.EndX,0,pif.StartZ);
				glVertex3f(pif.EndX,0,pif.EndZ);
				glVertex3f(pif.EndX,0,pif.EndZ);
				glVertex3f(pif.StartX,0,pif.EndZ);
				glVertex3f(pif.StartX,0,pif.EndZ);
				glVertex3f(pif.StartX,0,pif.StartZ);
			glEnd();

			glPopMatrix();
		}



		for(size_t i=0; i<planess.size(); ++i)
		{
			PlaneInfo pif = planess[i];
			glPushMatrix();

			glTranslated(0, pif.StartY/2. + 0.5, 0);
			glColor4f(0.0f,1.0f,0.0f, 1.f);
			glBegin(GL_LINES);
				glVertex3f(pif.StartX,0,pif.StartZ);
				glVertex3f(pif.EndX,0,pif.StartZ);
				glVertex3f(pif.EndX,0,pif.StartZ);
				glVertex3f(pif.EndX,0,pif.EndZ);
				glVertex3f(pif.EndX,0,pif.EndZ);
				glVertex3f(pif.StartX,0,pif.EndZ);
				glVertex3f(pif.StartX,0,pif.EndZ);
				glVertex3f(pif.StartX,0,pif.StartZ);
			glEnd();

			glPopMatrix();
		}


		for(size_t i=0; i<WallX.size(); ++i)
		{
			PlaneInfo pif = WallX[i];
			glPushMatrix();

			glTranslated(pif.StartY+1, 0, 0);
			glColor4f(0.0f,0.0f,1.0f, 1.f);
			glBegin(GL_LINES);
				glVertex3f(0,pif.StartX/2.f,pif.StartZ);
				glVertex3f(0,pif.EndX/2.f,pif.StartZ);
				glVertex3f(0,pif.EndX/2.f,pif.StartZ);
				glVertex3f(0,pif.EndX/2.f,pif.EndZ);
				glVertex3f(0,pif.EndX/2.f,pif.EndZ);
				glVertex3f(0,pif.StartX/2.f,pif.EndZ);
				glVertex3f(0,pif.StartX/2.f,pif.EndZ);
				glVertex3f(0,pif.StartX/2.f,pif.StartZ);
			glEnd();

			glPopMatrix();
		}

		for(size_t i=0; i<WallXh.size(); ++i)
		{
			PlaneInfo pif = WallXh[i];
			glPushMatrix();

			glTranslated(pif.StartY, 0, 0);
			glColor4f(1.0f,0.0f,0.0f, 1.f);
			glBegin(GL_LINES);
				glVertex3f(0,pif.StartX/2.f,pif.StartZ);
				glVertex3f(0,pif.EndX/2.f,pif.StartZ);
				glVertex3f(0,pif.EndX/2.f,pif.StartZ);
				glVertex3f(0,pif.EndX/2.f,pif.EndZ);
				glVertex3f(0,pif.EndX/2.f,pif.EndZ);
				glVertex3f(0,pif.StartX/2.f,pif.EndZ);
				glVertex3f(0,pif.StartX/2.f,pif.EndZ);
				glVertex3f(0,pif.StartX/2.f,pif.StartZ);
			glEnd();

			glPopMatrix();
		}


		for(size_t i=0; i<WallZ.size(); ++i)
		{
			PlaneInfo pif = WallZ[i];
			glPushMatrix();

			glTranslated(0, 0, pif.StartY+1);
			glColor4f(0.0f,0.0f,1.0f, 1.f);
			glBegin(GL_LINES);
				glVertex3f(pif.StartX,pif.StartZ/2.f,0);
				glVertex3f(pif.EndX,pif.StartZ/2.f,0);
				glVertex3f(pif.EndX,pif.StartZ/2.f,0);
				glVertex3f(pif.EndX,pif.EndZ/2.f,0);
				glVertex3f(pif.EndX,pif.EndZ/2.f,0);
				glVertex3f(pif.StartX,pif.EndZ/2.f,0);
				glVertex3f(pif.StartX,pif.EndZ/2.f,0);
				glVertex3f(pif.StartX,pif.StartZ/2.f,0);
			glEnd();

			glPopMatrix();
		}

		for(size_t i=0; i<WallZh.size(); ++i)
		{
			PlaneInfo pif = WallZh[i];
			glPushMatrix();

			glTranslated(0, 0, pif.StartY);
			glColor4f(1.0f,0.0f,0.0f, 1.f);
			glBegin(GL_LINES);
				glVertex3f(pif.StartX,pif.StartZ/2.f,0);
				glVertex3f(pif.EndX,pif.StartZ/2.f,0);
				glVertex3f(pif.EndX,pif.StartZ/2.f,0);
				glVertex3f(pif.EndX,pif.EndZ/2.f,0);
				glVertex3f(pif.EndX,pif.EndZ/2.f,0);
				glVertex3f(pif.StartX,pif.EndZ/2.f,0);
				glVertex3f(pif.StartX,pif.EndZ/2.f,0);
				glVertex3f(pif.StartX,pif.StartZ/2.f,0);
			glEnd();

			glPopMatrix();
		}


		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
	}






}


/***********************************************************
cut the room at a certain Y to display only bottom
***********************************************************/
void MapRenderer::CutRoom(int cut)
{
	if(_current_cut == cut)
		return;

	_current_cut = cut;

	if(_map_gl)
		_map_gl->RecompileRoom(cut);
}



/***********************************************************
load a room in memroy to be displayed
***********************************************************/
void MapRenderer::DisplayExitZones()
{
	std::map<std::string, ExitInfo>::const_iterator it = _mapinfo->Exits.begin();
	std::map<std::string, ExitInfo>::const_iterator end = _mapinfo->Exits.end();

	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	for(; it != end; ++it)
	{
		float Xbl = it->second.BottomLeftX;
		float Ybl = it->second.BottomLeftY/2;
		float Zbl = it->second.BottomLeftZ;
		float Xtr = it->second.TopRightX;
		float Ztr = it->second.TopRightZ;

		glPushMatrix();
		{
			glTranslated(0, 0.01, 0);

			glBegin(GL_QUADS);
				glColor4f(0.3f,0.0f,0.0f, 0.4f);
				glVertex3f(Xbl,Ybl,Zbl);
				glVertex3f(Xbl,Ybl,Ztr);
				glVertex3f(Xtr,Ybl,Ztr);
				glVertex3f(Xtr,Ybl,Zbl);
			glEnd();
		}
		glPopMatrix();
	}

    glEnable(GL_TEXTURE_2D);
}


/***********************************************************
check if the main actor is exiting the room
return the number of the new room if applicable
***********************************************************/
bool MapRenderer::CheckIfExitRoom(float currX, float currY, float currZ,
									std::string & NewRoomName,
									std::string & NewSpawningPoint,
									float &Xoffset, float &Yoffset, float &Zoffset)
{
	if(!_mapinfo)
		return false;


	float X = currX;
	float Y = currY + 1.f;
	float Z = currZ;


	std::map<std::string, ExitInfo>::const_iterator it = _mapinfo->Exits.begin();
	std::map<std::string, ExitInfo>::const_iterator end = _mapinfo->Exits.end();

	for(; it != end; ++it)
	{
		float Xbl = it->second.BottomLeftX;
		float Ybl = it->second.BottomLeftY;
		float Zbl = it->second.BottomLeftZ;

		float Xtr = it->second.TopRightX;
		float Ytr = it->second.TopRightY;
		float Ztr = it->second.TopRightZ;

		if (	(X >= Xbl && X <= Xtr) &&
				(Y >= Ybl && Y <= Ytr) &&
				(Z >= Zbl && Z <= Ztr)	)  // if the main actor is in zone
		{
			NewRoomName = it->second.NewMap; // change the room
			NewSpawningPoint = it->second.Spawning;

			Xoffset = (X - Xbl);
			Yoffset = (int)(Y - Ybl);
			Zoffset = (Z - Zbl);

			return true;
		}
	}

	return false;
}



/***********************************************************
display spawning on the screen
***********************************************************/
void MapRenderer::DisplaySpawnings()
{
	std::map<std::string, SpawningInfo>::const_iterator it = _mapinfo->Spawnings.begin();
	std::map<std::string, SpawningInfo>::const_iterator end = _mapinfo->Spawnings.end();



	for(; it != end; ++it)
	{
		float Xbl = it->second.PosX;
		float Ybl = it->second.PosY/2.0f;
		float Zbl = it->second.PosZ;

		glEnable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);

		glPushMatrix();
		{
			glLineWidth(2.0f);

			glTranslated(0, 0.5, 0);

			glBegin(GL_LINES);
				glColor4f(0.0f,1.0f,0.0f, 1.f);
				glVertex3f(Xbl,Ybl,Zbl);
				glVertex3f(Xbl,Ybl+2,Zbl);
				glVertex3f(Xbl-1,Ybl,Zbl);
				glVertex3f(Xbl+1,Ybl,Zbl);
				glVertex3f(Xbl,Ybl,Zbl-1);
				glVertex3f(Xbl,Ybl,Zbl+1);
			glEnd();

			glPushMatrix();
			glTranslated(Xbl-1,Ybl+2.1,Zbl+1);
			glRotatef( 40, 0.0, 1.0, 0.0 );
			glScalef(0.04f, 0.04f, 0.04f);
			TextWritter::getInstance()->glPrintText(it->first, 0, false);
			glPopMatrix();
		}
		glPopMatrix();
	}
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
}


/***********************************************************
load a room in memroy to be displayed
***********************************************************/
void MapRenderer::DisplayDetailsExitZones()
{
	std::map<std::string, ExitInfo>::const_iterator it = _mapinfo->Exits.begin();
	std::map<std::string, ExitInfo>::const_iterator end = _mapinfo->Exits.end();


	for(; it != end; ++it)
	{
		float Xbl = it->second.BottomLeftX;
		float Ybl = it->second.BottomLeftY/2;
		float Zbl = it->second.BottomLeftZ;
		float Xtr = it->second.TopRightX;
		float Ytr = it->second.TopRightY/2;
		float Ztr = it->second.TopRightZ;

		glEnable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);

		glPushMatrix();
		{
			glLineWidth(2.0f);

			glTranslated(0, 0.01, 0);

			glBegin(GL_LINES);
				glColor4f(1.0f,0.0f,0.0f, 1.f);
				glVertex3f(Xbl,Ybl,Zbl);
				glVertex3f(Xbl,Ybl,Ztr);
				glVertex3f(Xbl,Ybl,Ztr);
				glVertex3f(Xtr,Ybl,Ztr);
				glVertex3f(Xtr,Ybl,Ztr);
				glVertex3f(Xtr,Ybl,Zbl);
				glVertex3f(Xtr,Ybl,Zbl);
				glVertex3f(Xbl,Ybl,Zbl);

				glVertex3f(Xbl,Ytr,Zbl);
				glVertex3f(Xbl,Ytr,Ztr);
				glVertex3f(Xbl,Ytr,Ztr);
				glVertex3f(Xtr,Ytr,Ztr);
				glVertex3f(Xtr,Ytr,Ztr);
				glVertex3f(Xtr,Ytr,Zbl);
				glVertex3f(Xtr,Ytr,Zbl);
				glVertex3f(Xbl,Ytr,Zbl);

				glVertex3f(Xbl,Ybl,Zbl);
				glVertex3f(Xbl,Ytr,Zbl);

				glVertex3f(Xbl,Ybl,Ztr);
				glVertex3f(Xbl,Ytr,Ztr);

				glVertex3f(Xtr,Ybl,Ztr);
				glVertex3f(Xtr,Ytr,Ztr);

				glVertex3f(Xtr,Ybl,Zbl);
				glVertex3f(Xtr,Ytr,Zbl);
			glEnd();

			glPushMatrix();
			glTranslated(Xtr-1,Ytr+0.1,Ztr+1);
			glRotatef( 40, 0.0, 1.0, 0.0 );
			glScalef(0.04f, 0.04f, 0.04f);
			TextWritter::getInstance()->glPrintText(it->first, 0, false);
			glPopMatrix();
		}
		glPopMatrix();
	}

	glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
}



/***********************************************************
render editor part
***********************************************************/
void MapRenderer::RenderEditor()
{
	if(_mapinfo)
	{
		DisplayDetailsExitZones();
		DisplaySpawnings();
	}
}


/***********************************************************
display map extis
***********************************************************/
void MapRenderer::DisplayExits(bool display)
{
	_display_exits = display;
}

