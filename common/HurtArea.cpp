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


#include "HurtArea.h"

#ifndef _LBANET_SERVER_SIDE_
#include "ThreadSafeWorkpile.h"
#include "GameEvents.h"
#include <windows.h>    // Header File For Windows
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#include "MainPlayerHandler.h"
#endif

/***********************************************************
	Constructor
***********************************************************/
HurtArea::HurtArea(float zoneSizeX, float zoneSizeY, float zoneSizeZ, int LifeTaken)
: _zoneSizeX(zoneSizeX), _zoneSizeY(zoneSizeY), _zoneSizeZ(zoneSizeZ), 
	_LifeTaken(LifeTaken), _activated(false), _timer(false)
{

}

/***********************************************************
	Destructor
***********************************************************/
HurtArea::~HurtArea()
{

}

/***********************************************************
check zone activation
***********************************************************/
int HurtArea::ActivateZone(float PlayerPosX, float PlayerPosY, float PlayerPosZ, float PlayerRotation,
								MainPlayerHandler  * _mph, bool DirectActivation)
{
	#ifndef _LBANET_SERVER_SIDE_
	if(_mph->IsJumping())
		return 0;

	float posX = _posX;
	float posY = _posY;
	float posZ = _posZ;


	if( (PlayerPosX >= (posX-_zoneSizeX-_mph->GetSizeX()) && PlayerPosX < (posX+_zoneSizeX+_mph->GetSizeX())) &&
		(PlayerPosY >= (posY)				&& PlayerPosY <= (posY+_zoneSizeY)) &&
		(PlayerPosZ >= (posZ-_zoneSizeZ-_mph->GetSizeZ()) && PlayerPosZ < (posZ+_zoneSizeZ+_mph->GetSizeZ())))
	{
		if(!_activated)
		{
			ThreadSafeWorkpile::getInstance()->AddEvent(new PlayerHurtEvent(_ID)); 
			_activated = true;
			return 1;
		}
	}
	else
	{
		_activated = false;
	}
	#endif


	return 0;
}


/***********************************************************
render editor part
***********************************************************/
void HurtArea::RenderEditor()
{
#ifndef _LBANET_SERVER_SIDE_
	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

	glLineWidth(2.0f);


	glPushMatrix();

	glTranslated(_posX, _posY/2. + 0.5, _posZ);
	glColor4f(0.9f,0.2f,1.0f, 1.f);

	glBegin(GL_LINES);

		glVertex3f(-_zoneSizeX,0,-_zoneSizeZ);
		glVertex3f(_zoneSizeX,0,-_zoneSizeZ);
		glVertex3f(_zoneSizeX,0,-_zoneSizeZ);
		glVertex3f(_zoneSizeX,0,_zoneSizeZ);
		glVertex3f(_zoneSizeX,0,_zoneSizeZ);
		glVertex3f(-_zoneSizeX,0,_zoneSizeZ);
		glVertex3f(-_zoneSizeX,0,_zoneSizeZ);
		glVertex3f(-_zoneSizeX,0,-_zoneSizeZ);

		glVertex3f(-_zoneSizeX,(_zoneSizeY)/2,-_zoneSizeZ);
		glVertex3f(_zoneSizeX,(_zoneSizeY)/2,-_zoneSizeZ);
		glVertex3f(_zoneSizeX,(_zoneSizeY)/2,-_zoneSizeZ);
		glVertex3f(_zoneSizeX,(_zoneSizeY)/2,_zoneSizeZ);
		glVertex3f(_zoneSizeX,(_zoneSizeY)/2,_zoneSizeZ);
		glVertex3f(-_zoneSizeX,(_zoneSizeY)/2,_zoneSizeZ);
		glVertex3f(-_zoneSizeX,(_zoneSizeY)/2,_zoneSizeZ);
		glVertex3f(-_zoneSizeX,(_zoneSizeY)/2,-_zoneSizeZ);

		glVertex3f(-_zoneSizeX,0,-_zoneSizeZ);
		glVertex3f(-_zoneSizeX,(_zoneSizeY)/2,-_zoneSizeZ);

		glVertex3f(_zoneSizeX,0,-_zoneSizeZ);
		glVertex3f(_zoneSizeX,(_zoneSizeY)/2,-_zoneSizeZ);

		glVertex3f(_zoneSizeX,0,_zoneSizeZ);
		glVertex3f(_zoneSizeX,(_zoneSizeY)/2,_zoneSizeZ);

		glVertex3f(-_zoneSizeX,0,_zoneSizeZ);
		glVertex3f(-_zoneSizeX,(_zoneSizeY)/2,_zoneSizeZ);
	glEnd();

	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	Actor::RenderEditor();
#endif
}

/***********************************************************
called on signal
***********************************************************/
bool HurtArea::OnSignal(long SignalNumber)
{
	if(SignalNumber == 3)
	{
		_timer = true;
		_cumutime = 0;
		return true;
	}

	return false;
}


/***********************************************************
do all check to be done when idle
***********************************************************/
int HurtArea::Process(double tnow, float tdiff)
{
#ifndef _LBANET_SERVER_SIDE_
	if(_timer)
	{
		_cumutime += tdiff;
		if(_cumutime > 400)
		{
			_timer = false;
			_activated = false;
			ThreadSafeWorkpile::getInstance()->AddEvent(new DoFullCheckEvent()); 
		}
	}
#endif

	return Actor::Process(tnow, tdiff);
}
