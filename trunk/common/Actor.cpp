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


#include "Actor.h"

#ifndef _LBANET_SERVER_SIDE_
#include "TextWritter.h"
#include "3DObjectRenderer.h"
#include <windows.h>    // Header File For Windows
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#endif

#include <algorithm>

/***********************************************************
	Constructor
***********************************************************/
Actor::Actor()
: _posX(0),_posY(0), _posZ(0), _rotation(0),
	_passable(true), _depthmask(true), _Renderer(NULL),
	_VelocityX(0), _VelocityY(0), _VelocityZ(0), _movable(false),
	_sizeX(0),_sizeY(0), _sizeZ(0), _visible(true), _outputsignal(-1),
	_attachedsound(-1), _renderertype(0),
	_AddedVelocityX(0), _AddedVelocityY(0), _AddedVelocityZ(0), _signaler(NULL)
{

}

/***********************************************************
	Destructor
***********************************************************/
Actor::~Actor()
{
#ifndef _LBANET_SERVER_SIDE_
	if(_Renderer)
		delete _Renderer;
#endif
}

/***********************************************************
	render object
***********************************************************/
void Actor::Render(int RoomCut)
{
#ifndef _LBANET_SERVER_SIDE_
	if(_Renderer && ((RoomCut < 0) || (_posY < RoomCut)))
	{
		if(!_depthmask)
			glDepthMask( GL_FALSE );

		// draw
		glPushMatrix();
		{
			glTranslated(_posX, _posY/2. + 0.5, _posZ);
			glRotatef( (float)_rotation, 0.0, 1.0, 0.0 );

			_Renderer->Render();
		}
		glPopMatrix();


		if(!_depthmask)
			glDepthMask( GL_TRUE );
	}
#endif
}

/***********************************************************
	do all check to be done when idle
***********************************************************/
int Actor::Process(double tnow, float tdiff)
{
#ifndef _LBANET_SERVER_SIDE_
	if(_Renderer)
		return _Renderer->Process(tnow, tdiff);
#endif

	return 0;
}


/***********************************************************
show object
***********************************************************/
void Actor::Show(void)
{
	_visible = true;

#ifndef _LBANET_SERVER_SIDE_
	if(_Renderer)
		return _Renderer->Show();
#endif
}

/***********************************************************
hide object
***********************************************************/
void Actor::Hide(void)
{
	_visible = false;

#ifndef _LBANET_SERVER_SIDE_
	if(_Renderer)
		return _Renderer->Hide();
#endif
}


/***********************************************************
set passable state
***********************************************************/
void Actor::SetPassable(bool passable)
{
	_passable = passable;
}


/***********************************************************
set depth mask
***********************************************************/
void Actor::SetDepthMask(bool depthmask)
{
	_depthmask = depthmask;
}

/***********************************************************
SetRenderer()
***********************************************************/
void Actor::SetRenderer(D3ObjectRenderer* renderer)
{
#ifndef _LBANET_SERVER_SIDE_
	if(_Renderer)
		delete _Renderer;

	_Renderer = renderer;
#endif
}


/***********************************************************
get structure
***********************************************************/
short Actor::GetStructure(int X, int Y, int Z)
{
	if(_passable || !_visible)
		return 0;

	if(	(X >= (_posX-_sizeX) && X < (_posX+_sizeX)) &&
		(Y >= (_posY)		&& Y < (_posY+_sizeY)) &&
		(Z >= (_posZ-_sizeZ) && Z < (_posZ+_sizeZ)))
		return 1;

	return 0;
}



/***********************************************************
called to reload element when resizing screen
***********************************************************/
void Actor::Reload()
{
#ifndef _LBANET_SERVER_SIDE_
	if(_Renderer)
		_Renderer->Reload();
#endif
}


/***********************************************************
set output signal
***********************************************************/
void Actor::SetOutputSignal(long outputsignal, std::vector<long> targets)
{
	_outputsignal = outputsignal;
	_targets = targets;
}


/***********************************************************
render editor part
***********************************************************/
void Actor::RenderEditor()
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
		glVertex3f(-1,0,0);
		glVertex3f(1,0,0);
		glVertex3f(0,0,-1);
		glVertex3f(0,0,+1);
	glEnd();

	glBegin(GL_LINES);

		glVertex3f(-_sizeX,0,-_sizeZ);
		glVertex3f(_sizeX,0,-_sizeZ);
		glVertex3f(_sizeX,0,-_sizeZ);
		glVertex3f(_sizeX,0,_sizeZ);
		glVertex3f(_sizeX,0,_sizeZ);
		glVertex3f(-_sizeX,0,_sizeZ);
		glVertex3f(-_sizeX,0,_sizeZ);
		glVertex3f(-_sizeX,0,-_sizeZ);

		glVertex3f(-_sizeX,(_sizeY)/2,-_sizeZ);
		glVertex3f(_sizeX,(_sizeY)/2,-_sizeZ);
		glVertex3f(_sizeX,(_sizeY)/2,-_sizeZ);
		glVertex3f(_sizeX,(_sizeY)/2,_sizeZ);
		glVertex3f(_sizeX,(_sizeY)/2,_sizeZ);
		glVertex3f(-_sizeX,(_sizeY)/2,_sizeZ);
		glVertex3f(-_sizeX,(_sizeY)/2,_sizeZ);
		glVertex3f(-_sizeX,(_sizeY)/2,-_sizeZ);

		glVertex3f(-_sizeX,0,-_sizeZ);
		glVertex3f(-_sizeX,(_sizeY)/2,-_sizeZ);

		glVertex3f(_sizeX,0,-_sizeZ);
		glVertex3f(_sizeX,(_sizeY)/2,-_sizeZ);

		glVertex3f(_sizeX,0,_sizeZ);
		glVertex3f(_sizeX,(_sizeY)/2,_sizeZ);

		glVertex3f(-_sizeX,0,_sizeZ);
		glVertex3f(-_sizeX,(_sizeY)/2,_sizeZ);
	glEnd();



	glTranslated(0,1,0);
	glRotatef( 40, 0.0, 1.0, 0.0 );
	glScalef(0.04f, 0.04f, 0.04f);
	TextWritter::getInstance()->glPrintText(_sid, 0, false);
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
#endif
}



/***********************************************************
attach another actor to self
***********************************************************/
void Actor::Attach(Actor * act)
{
	_attachedActors.push_back(act);
}

/***********************************************************
dettach another actor from self
***********************************************************/
bool Actor::Dettach(Actor * act)
{
	std::vector<Actor *>::iterator it = std::find(_attachedActors.begin(), _attachedActors.end(), act);
	if(it != _attachedActors.end())
	{
		_attachedActors.erase(it);
		return true;
	}

	return false;
}


/***********************************************************
send stored signal to targets
***********************************************************/
void Actor::SendSignal(long signal, const std::vector<long> &targets)
{
	if(targets.size() > 0 && _signaler)
		_signaler->SendSignal(signal, targets);
}