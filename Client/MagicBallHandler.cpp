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
#include <windows.h>    // Header File For Windows
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#include <GL/glu.h>     // Header File For The GLu32 Library

#include "MagicBallHandler.h"
#include "SynchronizedTimeHandler.h"
#include "PhysXEngine.h"
#include "NxVec3.h"
#include "NxActor.h"
#include "NxUserContactReport.h"

#include "Actor.h"
#include "ThreadSafeWorkpile.h"

#define _size_ball_	0.25f
#define _offset_y_	5.0f


/*
--------------------------------------------------------------------------------------------------
- constructor
--------------------------------------------------------------------------------------------------
*/
MagicBallHandler::MagicBallHandler(bool MainPlayer)
	: _launched(false), _comeback(false), _owner(NULL),
		_MainPlayer(MainPlayer)
{}


/*
--------------------------------------------------------------------------------------------------
- render
--------------------------------------------------------------------------------------------------
*/
void MagicBallHandler::Render()
{
	if(!_launched)
		return;

	float drawX = 0;
	float drawY = 0;
	float drawZ = 0;

	if(_comeback)
	{
		drawX = _currX;
		drawY = _currY;
		drawZ = _currZ;

		glColor4f(0.3f, 0.6f, 0.4f, 0.8f);
	}
	else
	{
		NxVec3 vec = _physH->getGlobalPosition();
		drawX = vec.x;
		drawY = vec.y+1.0f+_size_ball_;
		drawZ = vec.z;
		glColor4f(0.3f, 0.6f, 0.4f, 1.0f);
	}

    glEnable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);


	GLUquadric* quadric=gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);

	glPushMatrix();
	glTranslatef(drawX, drawY/2.0f, drawZ);
	gluSphere(quadric, _size_ball_, 20, 20);
	glPopMatrix();

	gluDeleteQuadric(quadric);

    glEnable(GL_TEXTURE_2D);
}


/*
--------------------------------------------------------------------------------------------------
- launch the magic ball
--------------------------------------------------------------------------------------------------
*/
void MagicBallHandler::Launch(float PosX, float PosY, float PosZ, float dirX, float dirZ, int mode, bool enoughmana)
{
	// do nothing if ball is already launched
	if(_launched)
		return;

	if(_MainPlayer)
	{
		LbaNet::LaunchInfo linfo;
		linfo.PosX = PosX;
		linfo.PosY = PosY;
		linfo.PosZ = PosZ;
		linfo.DirX = dirX;
		linfo.DirZ = dirZ;
		linfo.Mode = mode;
		linfo.Enoughmana = enoughmana;
		ThreadSafeWorkpile::getInstance()->ThrowMagicBall(linfo);
	}

	_lastlaunchtime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeDouble();
	_launched = true;
	_comeback = false;
	_touch_counter = 0;
	_enoughmana = enoughmana;

	_physdata = new ActorUserData(4, -1, this);
	_physH =  PhysXEngine::getInstance()->CreateSphere(NxVec3(PosX, PosY+_offset_y_, PosZ), _size_ball_, 1.0, 
															3, _physdata);
	_physH->setContactReportFlags(NX_NOTIFY_ON_START_TOUCH);

	float coeffforce = 2.0f;

	_physH->addForce(NxVec3(dirX*coeffforce, 0.0, dirZ*coeffforce));
}


/*
--------------------------------------------------------------------------------------------------
- proccess magic ball
--------------------------------------------------------------------------------------------------
*/
void MagicBallHandler::Process()
{
	if(!_launched)
		return;

	if(_comeback)
	{
		double ctime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeDouble();
		float diff = (float)(ctime - _lasttime) / 60;
		_lasttime = ctime;


		NxVec3 launchedp(_owner->GetPosX(), _owner->GetPosY()+_offset_y_, _owner->GetPosZ());
		NxVec3 vec(_currX, _currY, _currZ);
		NxVec3 diffv(launchedp - vec);

		float leftx = diffv.x;
		float lefty = diffv.y;
		float leftz = diffv.z;

		diffv.normalize();

		float movex = diffv.x * diff;
		float movey = diffv.y * diff;
		float movez = diffv.z * diff;	


		bool finishedx=false, finishedy=false, finishedz=false;

		if(abs(movex) >= abs(leftx))
		{
			movex = leftx;
			finishedx = true;
		}

		if(abs(movey) >= abs(lefty))
		{
			movey = lefty;
			finishedy = true;
		}

		if(abs(movez) >= abs(leftz))
		{
			movez = leftz;
			finishedz = true;
		}

		_currX += movex;
		_currY += movey;
		_currZ += movez;	

		if(finishedx && finishedy && finishedz)
		{
			Clear();
		}
	}
	//else
	//{
	//	//check if time is up and magic ball should come back
	//	double ctime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeDouble();
	//	if((ctime - _lastlaunchtime) > 5000)
	//	{
	//		BallComeBack();
	//	}
	//}

}


/*
--------------------------------------------------------------------------------------------------
- clear the magic ball if launched (e.g we change map)
--------------------------------------------------------------------------------------------------
*/
void MagicBallHandler::Clear()
{
	if(_MainPlayer)
		ThreadSafeWorkpile::getInstance()->MagicBallEnd();

	cleanPhys();

	_launched = false;
	_comeback = false;
}


/*
--------------------------------------------------------------------------------------------------
- clean physycall shape if needed
--------------------------------------------------------------------------------------------------
*/
void MagicBallHandler::cleanPhys()
{
	if(!_launched || _comeback)
		return;

	if(!_physdata->released)
		PhysXEngine::getInstance()->DestroyActor(_physH);
	delete _physdata;
}


/*
--------------------------------------------------------------------------------------------------
- callback function
--------------------------------------------------------------------------------------------------
*/
void MagicBallHandler::CallbackOnContact(int TouchedActorType, long TouchedActorIdx)
{
	++_touch_counter;
	if(_touch_counter > 3 || !_enoughmana)
	{
		BallComeBack();
	}

	
	if(TouchedActorType == 1)
	{
		if(_MainPlayer)
			ThreadSafeWorkpile::getInstance()->MbHittedActor(TouchedActorIdx);

		BallComeBack();
	}

	if(TouchedActorType == 3)
	{
		if(_MainPlayer)
			ThreadSafeWorkpile::getInstance()->MbHittedPlayer(TouchedActorIdx);

		BallComeBack();
	}


}



/*
--------------------------------------------------------------------------------------------------
- make ball come back
--------------------------------------------------------------------------------------------------
*/
void MagicBallHandler::BallComeBack()
{
	if(_launched && !_comeback)
	{
		NxVec3 vec = _physH->getGlobalPosition();
		_currX = vec.x;
		_currY = vec.y+1.0f+_size_ball_;
		_currZ = vec.z;	

		cleanPhys();

		_comeback = true;
		_lasttime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeDouble();
	}
}