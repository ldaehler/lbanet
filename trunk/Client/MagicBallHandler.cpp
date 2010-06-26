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

#include "PhysXPhysicHandler.h"

#include "DataLoader.h"
#include "MusicHandler.h"
#include "ConfigurationManager.h"

#define _size_ball_	0.25f
#define _offset_y_	5.0f

#define M_SOUND_MB_ 24
#define M_SOUND_BOUNCE_ 3080//58


	//materialDesc.restitution = 0.9f;    
	//materialDesc.staticFriction = 0.5f;    
	//materialDesc.dynamicFriction = 0.5f; 

/*
--------------------------------------------------------------------------------------------------
- constructor
--------------------------------------------------------------------------------------------------
*/
MagicBallHandler::MagicBallHandler(bool MainPlayer)
	: _launched(false), _comeback(false), _owner(NULL),
		_MainPlayer(MainPlayer)
{
	ConfigurationManager::GetInstance()->GetFloat("Physic.MagicBallBounciness", _MagicBallBounciness);
	ConfigurationManager::GetInstance()->GetFloat("Physic.MagicBallStaticFriction", _MagicBallStaticFriction); 
	ConfigurationManager::GetInstance()->GetFloat("Physic.MagicBallDynamicFriction", _MagicBallDynamicFriction); 
	ConfigurationManager::GetInstance()->GetFloat("Physic.NormalMBForce", _NormalMBForce);
	ConfigurationManager::GetInstance()->GetFloat("Physic.NormalMBForceUp", _NormalMBForceUp); 
	ConfigurationManager::GetInstance()->GetFloat("Physic.SportMBForce", _SportMBForce);
	ConfigurationManager::GetInstance()->GetFloat("Physic.SportMBForceUp", _SportMBForceUp);  
	ConfigurationManager::GetInstance()->GetFloat("Physic.AggresiveMBForce", _AggresiveMBForce);
	ConfigurationManager::GetInstance()->GetFloat("Physic.AggresiveMBForceUp", _AggresiveMBForceUp);    
	ConfigurationManager::GetInstance()->GetFloat("Physic.DiscreteMBForce", _DiscreteMBForce);
	ConfigurationManager::GetInstance()->GetFloat("Physic.DiscreteMBForceUp", _DiscreteMBForceUp); 

	ConfigurationManager::GetInstance()->GetFloat("Physic.NormalMBForceUpOnImpact", _NormalMBForceUpOnImpact); 
	ConfigurationManager::GetInstance()->GetFloat("Physic.SportMBForceUpOnImpact", _SportMBForceUpOnImpact);  
	ConfigurationManager::GetInstance()->GetFloat("Physic.AggresiveMBForceUpOnImpact", _AggresiveMBForceUpOnImpact);    
	ConfigurationManager::GetInstance()->GetFloat("Physic.DiscreteMBForceUpOnImpact", _DiscreteMBForceUpOnImpact);  
}


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

		glColor4f(0.9f, 0.788f, 0.376f, 0.6f);
	}
	else
	{
		NxVec3 vec = _physH->getGlobalPosition();
		drawX = vec.x;
		drawY = vec.y+1.0f+_size_ball_;
		drawZ = vec.z;
		glColor4f(0.9f, 0.788f, 0.376f, 1.0f);
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
void MagicBallHandler::Launch(float PosX, float PosY, float PosZ, float dirX, float dirZ, int mode, 
								bool enoughmana, NxActor* ownerphys)
{
	// do nothing if ball is already launched
	if(_MainPlayer && _launched)
		return;

	_currmode = mode;


	// clean in case we still have an MB running
	cleanPhys();

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

	//play sound
	std::string soundp = DataLoader::getInstance()->GetSoundPath(M_SOUND_MB_);
	if(soundp != "")
		MusicHandler::getInstance()->PlaySample(soundp, 0);

	_lastlaunchtime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeDouble();
	_launched = true;
	_comeback = false;
	_touch_counter = 0;
	_enoughmana = enoughmana;

	_physdata = new ActorUserData(4, -1, this);
	_physH =  PhysXEngine::getInstance()->CreateSphere(NxVec3(PosX, PosY+_offset_y_, PosZ), _size_ball_, 1.0, 
															3, _physdata, 
															_MagicBallBounciness,
															_MagicBallStaticFriction,
															_MagicBallDynamicFriction);

	_physH->setContactReportFlags(NX_NOTIFY_ON_START_TOUCH);

	if(ownerphys)
		PhysXEngine::getInstance()->IgnoreActorContact(_physH, ownerphys);

	float coeffforce = 0.0f;
	float coeffforceup = 0.0f;

	switch(_currmode)
	{
		case 1:
			coeffforce = _NormalMBForce;
			coeffforceup = _NormalMBForceUp; 
		break;

		case 2:
			coeffforce = _SportMBForce;
			coeffforceup = _SportMBForceUp; 
		break;

		case 3:
			coeffforce = _AggresiveMBForce;
			coeffforceup = _AggresiveMBForceUp; 
		break;

		case 4:
			coeffforce = _DiscreteMBForce;
			coeffforceup = _DiscreteMBForceUp; 
		break;
	}

	_physH->addForce(NxVec3(dirX*coeffforce, coeffforceup, dirZ*coeffforce));
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
	else
	{
		//check if time is up and magic ball should come back
		double ctime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeDouble();
		if((ctime - _lastlaunchtime) > 5000)
		{
			BallComeBack();
		}
	}

}


/*
--------------------------------------------------------------------------------------------------
- clear the magic ball if launched (e.g we change map)
--------------------------------------------------------------------------------------------------
*/
void MagicBallHandler::Clear()
{
	if(!_launched)
		return;

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
	//play sound
	//play sound
	std::string soundp = DataLoader::getInstance()->GetSoundPath(M_SOUND_BOUNCE_);
	if(soundp != "")
		MusicHandler::getInstance()->PlaySample(soundp, 0);


	if(TouchedActorType == 1)
	{
		if(_MainPlayer)
		{
			ThreadSafeWorkpile::getInstance()->MbHittedActor(TouchedActorIdx);
		}

		BallComeBack();
		return;
	}

	if(TouchedActorType == 3)
	{
		if(_MainPlayer)
			ThreadSafeWorkpile::getInstance()->MbHittedPlayer(TouchedActorIdx);

		BallComeBack();
		return;
	}

	++_touch_counter;
	if(_touch_counter > 3 || !_enoughmana)
	{
		if(_MainPlayer)
			ThreadSafeWorkpile::getInstance()->MbHittedActor(-1);

		BallComeBack();
		return;
	}


	float coeffforceup = 0.0f;
	switch(_currmode)
	{
		case 1:
			coeffforceup = _NormalMBForceUpOnImpact; 
		break;

		case 2:
			coeffforceup = _SportMBForceUpOnImpact; 
		break;

		case 3:
			coeffforceup = _AggresiveMBForceUpOnImpact; 
		break;

		case 4:
			coeffforceup = _DiscreteMBForceUpOnImpact; 
		break;
	}
	_physH->addForce(NxVec3(0, coeffforceup, 0));
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