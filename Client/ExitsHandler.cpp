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

#include "ExitsHandler.h"
#include "ConfigurationManager.h"
#include "TextWritter.h"


#include <windows.h>    // Header File For Windows
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#include <GL/glu.h>     // Header File For The GLu32 Library



/***********************************************************
	Constructor
***********************************************************/
ExitsHandler::ExitsHandler()
: _mapinfo(NULL)
{
	ConfigurationManager::GetInstance()->GetBool("Options.Video.DisplayExits", _display_exits);
}

/***********************************************************
	destructor
***********************************************************/
ExitsHandler::~ExitsHandler()
{
}


/***********************************************************
	load new map
***********************************************************/
void ExitsHandler::LoadMap(const MapInfo * mapinfo)
{
	_mapinfo = mapinfo;
}

/***********************************************************
	render
***********************************************************/
void ExitsHandler::Render()
{
	if(_display_exits)
		DisplayExitZones();
}


/***********************************************************
load a room in memroy to be displayed
***********************************************************/
void ExitsHandler::DisplayExitZones()
{
	if(!_mapinfo)
		return;

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
bool ExitsHandler::CheckIfExitRoom(float currX, float currY, float currZ,
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
void ExitsHandler::DisplaySpawnings()
{
	if(!_mapinfo)
		return;

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
void ExitsHandler::DisplayDetailsExitZones()
{
	if(!_mapinfo)
		return;

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
void ExitsHandler::RenderEditor()
{
	DisplayDetailsExitZones();
	DisplaySpawnings();
}


/***********************************************************
display map extis
***********************************************************/
void ExitsHandler::DisplayExits(bool display)
{
	_display_exits = display;
}

