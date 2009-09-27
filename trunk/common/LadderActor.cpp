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


#include "LadderActor.h"

#ifndef _LBANET_SERVER_SIDE_
	#include "ThreadSafeWorkpile.h"
	#include "GameEvents.h"
	#include <math.h>
	#include <windows.h>    // Header File For Windows
	#include <GL/gl.h>      // Header File For The OpenGL32 Library

	#ifndef M_PI
	#define M_PI    3.14159265358979323846f
	#endif
#endif

/***********************************************************
	Constructor
***********************************************************/
LadderActor::LadderActor(float activationdistance, float deltaX, float deltaY, float deltaZ, int direction)
: _deltaX(deltaX), _deltaY(deltaY), _deltaZ(deltaZ), _direction(direction), ActivableActor(activationdistance)
{

}


/***********************************************************
	Destructor
***********************************************************/
LadderActor::~LadderActor()
{

}

/***********************************************************
process activation
***********************************************************/
void LadderActor::ProcessActivation(float PlayerPosX, float PlayerPosY, float PlayerPosZ, float PlayerRotation)
{
	SendSignal(_outputsignal, _targets);

#ifndef _LBANET_SERVER_SIDE_
	float distX = _posX-PlayerPosX;
	float distY = _posY-PlayerPosY;
	float distZ = _posZ-PlayerPosZ;

	double distance = sqrt((distX * distX) + (distZ * distZ));

	float angle = (float)(180 * acos(distZ / distance) / M_PI);
	if(distX < 0)
		angle = 360-angle;



	float remX=0, remZ=0;
	float dX=0, dZ=0;
	switch(_direction)
	{
		case 0:
			remZ = -0.8f;
			dZ = 1.f;
		break;
		case 90:
			remX = -0.8f;
			dX = 1.f;
		break;
		case 180:
			remZ = 0.8f;
			dZ = -1.f;
		break;
		case 270:
			remX = 0.8f;
			dX = -1.f;
		break;
	}



	std::vector<PlayerScriptPart> script;

	// add first rotation toward ladder position
	{
	PlayerScriptPart rot1;
	rot1.Type = 0;
	rot1.Animation = (angle > PlayerRotation) ? 3 : 4;
	rot1.ValueA = angle;
	rot1.Speed = 0.08f;
	script.push_back(rot1);
	}

	// add translation to ladder
	{
	PlayerScriptPart trans1;
	trans1.Type = 1;
	trans1.Animation = 1;
	trans1.ValueA = _posX+remX;
	trans1.ValueB = _posY;
	trans1.ValueC = _posZ+remZ;
	trans1.Speed = 0.003f;
	script.push_back(trans1);
	}

	// add rotation before climbing
	{
	PlayerScriptPart rot2;
	rot2.Type = 0;
	rot2.Animation =  (_direction > angle) ? 3 : 4;
	rot2.ValueA = (float)_direction;
	rot2.Speed = 0.08f;
	script.push_back(rot2);
	}


	// play short climbing animation
	{
	PlayerScriptPart anim1;
	anim1.Type = 2;
	anim1.Animation = 21;
	anim1.ValueA = 3;
	script.push_back(anim1);
	}

	// add translation up the ladder
	{
	PlayerScriptPart trans2;
	trans2.Type = 1;
	trans2.Animation = 21;
	trans2.ValueA = _posX+remX+_deltaX;
	trans2.ValueB = _posY+_deltaY-5.f;
	trans2.ValueC = _posZ+remZ+_deltaZ;
	trans2.Speed = 0.0025f;
	if((_deltaY-5.f)  > 0)
		script.push_back(trans2);
	}

	// add translation up the ladder
	{
	PlayerScriptPart trans2b;
	trans2b.Type = 1;
	trans2b.Animation = 26;
	trans2b.ValueA = _posX+remX+_deltaX;
	trans2b.ValueB = _posY+_deltaY;
	trans2b.ValueC = _posZ+remZ+_deltaZ;
	trans2b.Speed = 0.0025f;
	script.push_back(trans2b);
	}


	// add translation to exit the ladder 2
	{
	PlayerScriptPart trans3;
	trans3.Type = 1;
	trans3.Animation = 26;
	trans3.ValueA = _posX+remX+_deltaX+dX;
	trans3.ValueB = _posY+_deltaY;
	trans3.ValueC = _posZ+remZ+_deltaZ+dZ;
	trans3.Speed = 0.003f;
	script.push_back(trans3);
	}

	ThreadSafeWorkpile::getInstance()->AddEvent(new MainPlayerScriptedEvent(script));
#endif
}


/***********************************************************
render editor part
***********************************************************/
void LadderActor::RenderEditor()
{
#ifndef _LBANET_SERVER_SIDE_
	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

	glLineWidth(2.0f);


	glPushMatrix();

	glTranslated(_posX, _posY/2. + 0.5, _posZ);
	glColor4f(0.0f,0.0f,1.0f, 1.f);
	glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(_deltaX,_deltaY/2,_deltaZ);
		glVertex3f(_deltaX-1,_deltaY/2,_deltaZ);
		glVertex3f(_deltaX+1,_deltaY/2,_deltaZ);
		glVertex3f(_deltaX,_deltaY/2,_deltaZ-1);
		glVertex3f(_deltaX,_deltaY/2,_deltaZ+1);

		glVertex3f(0,_deltaY/4,0);
		if(_direction == 0)
			glVertex3f(0,_deltaY/4,-2);
		if(_direction == 90)
			glVertex3f(-2,_deltaY/4,0);
		if(_direction == 180)
			glVertex3f(0,_deltaY/4,2);
		if(_direction == 270)
			glVertex3f(2,_deltaY/4,0);
	glEnd();

	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	Actor::RenderEditor();
#endif
}