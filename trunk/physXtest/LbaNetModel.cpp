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

#include "LbaNetModel.h"
#include "Camera.h"
#include "PhysXEngine.h"
#include "MapMapRenderer.h"

#include <windows.h>    // Header File For Windows
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#include <GL/glu.h>     // Header File For The GLu32 Library



/***********************************************************
	Constructor
***********************************************************/
LbaNetModel::LbaNetModel()
{
    _camera= new Camera();

}


/***********************************************************
	Destructor
***********************************************************/
LbaNetModel::~LbaNetModel()
{
	delete _camera;

}

/***********************************************************
init function
***********************************************************/
void LbaNetModel::Initialize()
{
    glEnable(GL_TEXTURE_2D);
    glAlphaFunc(GL_GREATER,0.5);
    glEnable(GL_ALPHA_TEST);
    glColor3f(1,1,1);
    glColorMask(1,1,1,1);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);


	_map = new MapMapRenderer("Twinsenshouse.map", "Twinsenshouse.png");
}



/***********************************************************
set screen size
***********************************************************/
void LbaNetModel::SetScreenSize(int screenX, int screenY)
{
	_windowWidth = screenX;
	_windowHeight = screenY;
}


/***********************************************************
draw the current state on the screen
***********************************************************/
void LbaNetModel::Draw()
{
	// set opengl default view
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER,0.09f);
    glDisable(GL_ALPHA_TEST);
    glMatrixMode(GL_PROJECTION);
	glPushMatrix();
    glLoadIdentity();

	float posX, posY, posZ;
	PhysXEngine::getInstance()->GetCharacterPosition(0, posX, posY, posZ);
	_camera->SetTarget(	posX, posY, posZ);


	if(_camera->IsPerspective())
	{
		gluPerspective(_camera->GetFOV(),_windowWidth/(double)_windowHeight,0.01,2000);
		glTranslated(0,0,-_camera->GetDistance());
		glRotated(_camera->GetZenit(),1,0,0);
	}
	else
	{
		glOrtho(-0.12*_camera->GetDistance(), 0.12*_camera->GetDistance(), -0.12*_camera->GetDistance(), 0.12*_camera->GetDistance(), -2000, 2000);
		glTranslated(0,0,-1000);
		glRotated(30,1,0,0);
	}


    glRotated(-_camera->GetAzimut()-45,0,1,0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glAlphaFunc(GL_GREATER,0);
    glEnable(GL_ALPHA_TEST);
    glColor3f(1,1,1);
    glPushMatrix();
    glTranslated(-_camera->GetTargetX(),-_camera->GetTargetY()/2.0,-_camera->GetTargetZ());


	_map->Render();


	// render physic actors
	PhysXEngine::getInstance()->RenderActors();

	// reset opengl
    glPopMatrix();
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

/***********************************************************
create a new screen surface
***********************************************************/
void LbaNetModel::DeltaUpdateZoom(bool ZoomIn)
{
	if(ZoomIn)
		_camera->DeltaUpdateDistance(5);
	else
		_camera->DeltaUpdateDistance(-5);
}

/***********************************************************
create a new screen surface
***********************************************************/
void LbaNetModel::DeltaUpdateZenit(double zenit)
{
	_camera->DeltaUpdateZenit(zenit);
}
