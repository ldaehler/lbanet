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


#include "ZoneActivableActor.h"

#ifndef _LBANET_SERVER_SIDE_
#include <windows.h>    // Header File For Windows
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#endif

/***********************************************************
	Constructor
***********************************************************/
ZoneActivableActor::ZoneActivableActor(float ZoneSizeX, float ZoneSizeY, float ZoneSizeZ, int activationtype)
: _ZoneSizeX(ZoneSizeX), _ZoneSizeY(ZoneSizeY), _ZoneSizeZ(ZoneSizeZ), _activationtype(activationtype)
{

}



/***********************************************************
	Destructor
***********************************************************/
ZoneActivableActor::~ZoneActivableActor()
{

}

/***********************************************************
activate an actor
***********************************************************/
bool ZoneActivableActor::Activate(float PlayerPosX, float PlayerPosY, float PlayerPosZ, float PlayerRotation,
									int actionType, bool DirectActivation)
{
	float Xbl = _posX-_ZoneSizeX;
	float Ybl = _posY;
	float Zbl = _posZ-_ZoneSizeZ;

	float Xtr = _posX+_ZoneSizeX;
	float Ytr = _posY+_ZoneSizeY;
	float Ztr = _posZ+_ZoneSizeZ;

	if(_activationtype != actionType)
		return false;

	if (	(PlayerPosX >= Xbl && PlayerPosX <= Xtr) &&
			(PlayerPosY >= Ybl && PlayerPosY <= Ytr) &&
			(PlayerPosZ >= Zbl && PlayerPosZ <= Ztr)	)  // if the main actor is in zone
	{
		if(DirectActivation)
 			ProcessActivation(PlayerPosX, PlayerPosY, PlayerPosZ, PlayerRotation);

		return true;
	}

	return false;
}



/***********************************************************
render editor part
***********************************************************/
void ZoneActivableActor::RenderEditor()
{
#ifndef _LBANET_SERVER_SIDE_
	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

	glLineWidth(2.0f);


	glPushMatrix();

	glTranslated(_posX, _posY/2. + 0.5, _posZ);
	glColor4f(1.0f,1.0f,0.0f, 1.f);

	glBegin(GL_LINES);

		glVertex3f(-_ZoneSizeX,0,-_ZoneSizeZ);
		glVertex3f(_ZoneSizeX,0,-_ZoneSizeZ);
		glVertex3f(_ZoneSizeX,0,-_ZoneSizeZ);
		glVertex3f(_ZoneSizeX,0,_ZoneSizeZ);
		glVertex3f(_ZoneSizeX,0,_ZoneSizeZ);
		glVertex3f(-_ZoneSizeX,0,_ZoneSizeZ);
		glVertex3f(-_ZoneSizeX,0,_ZoneSizeZ);
		glVertex3f(-_ZoneSizeX,0,-_ZoneSizeZ);

		glVertex3f(-_ZoneSizeX,(_ZoneSizeY)/2,-_ZoneSizeZ);
		glVertex3f(_ZoneSizeX,(_ZoneSizeY)/2,-_ZoneSizeZ);
		glVertex3f(_ZoneSizeX,(_ZoneSizeY)/2,-_ZoneSizeZ);
		glVertex3f(_ZoneSizeX,(_ZoneSizeY)/2,_ZoneSizeZ);
		glVertex3f(_ZoneSizeX,(_ZoneSizeY)/2,_ZoneSizeZ);
		glVertex3f(-_ZoneSizeX,(_ZoneSizeY)/2,_ZoneSizeZ);
		glVertex3f(-_ZoneSizeX,(_ZoneSizeY)/2,_ZoneSizeZ);
		glVertex3f(-_ZoneSizeX,(_ZoneSizeY)/2,-_ZoneSizeZ);

		glVertex3f(-_ZoneSizeX,0,-_ZoneSizeZ);
		glVertex3f(-_ZoneSizeX,(_ZoneSizeY)/2,-_ZoneSizeZ);

		glVertex3f(_ZoneSizeX,0,-_ZoneSizeZ);
		glVertex3f(_ZoneSizeX,(_ZoneSizeY)/2,-_ZoneSizeZ);

		glVertex3f(_ZoneSizeX,0,_ZoneSizeZ);
		glVertex3f(_ZoneSizeX,(_ZoneSizeY)/2,_ZoneSizeZ);

		glVertex3f(-_ZoneSizeX,0,_ZoneSizeZ);
		glVertex3f(-_ZoneSizeX,(_ZoneSizeY)/2,_ZoneSizeZ);
	glEnd();

	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	Actor::RenderEditor();
#endif
}
