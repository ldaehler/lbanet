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


#include "DoorActor.h"
#include <math.h>

#ifndef _LBANET_SERVER_SIDE_
#include "ThreadSafeWorkpile.h"
#include "GameEvents.h"
#include "DataLoader.h"
#include "MusicHandler.h"

#include <windows.h>    // Header File For Windows
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#endif

/***********************************************************
	Constructor
***********************************************************/
DoorActor::DoorActor(float zoneSizeX, float zoneSizeY, float zoneSizeZ, bool Locked, long KeyId,
						bool Hide, float OpenTransX, float OpenTransY, float OpenTransZ,
						float OpenTransSpeedX, float OpenTransSpeedY, float OpenTransSpeedZ, bool destroykey)
: ZoneActor(zoneSizeX, zoneSizeY, zoneSizeZ), _locked(Locked), _KeyId(KeyId), _state(CLOSED),
	_Hide(Hide),_OpenTransSpeedX(OpenTransSpeedX), _OpenTransSpeedY(OpenTransSpeedY),
		_OpenTransSpeedZ(OpenTransSpeedZ), _opencounter(0), _signalon(false),
		_OpenTransX(OpenTransX), _OpenTransY(OpenTransY), _OpenTransZ(OpenTransZ), _destroykey(destroykey)
{

}

/***********************************************************
	Destructor
***********************************************************/
DoorActor::~DoorActor()
{

}

/***********************************************************
do all check to be done when idle
***********************************************************/
int DoorActor::Process(double tnow, float tdiff)
{

	if(_Hide)
	{
		if(_state == OPENING)
		{
			Hide();
			_state = OPENED;
		}

		if(_state == CLOSING)
		{
			Show();
			_state = CLOSED;
		}
	}
	else
	{
		if(_state == OPENING)
		{
			double stepX = (_OpenTransSpeedX * tdiff);
			double stepY = (_OpenTransSpeedY * tdiff);
			double stepZ = (_OpenTransSpeedZ * tdiff);

			double diffX = _OpenedX - _posX;
			double diffY = _OpenedY - _posY;
			double diffZ = _OpenedZ - _posZ;

			if(fabs(stepX) > fabs(diffX))
				stepX = diffX;
			if(fabs(stepY) > fabs(diffY))
				stepY = diffY;
			if(fabs(stepZ) > fabs(diffZ))
				stepZ = diffZ;

			if(stepX == 0 && stepY == 0 && stepZ == 0)
				_state = OPENED;
			else
			{
				_posX += (float)stepX;
				_posY += (float)stepY;
				_posZ += (float)stepZ;
			}
		}

		if(_state == CLOSING)
		{
			double stepX = (-_OpenTransSpeedX * tdiff);
			double stepY = (-_OpenTransSpeedY * tdiff);
			double stepZ = (-_OpenTransSpeedZ * tdiff);

			double diffX = _ClosedX - _posX;
			double diffY = _ClosedY - _posY;
			double diffZ = _ClosedZ - _posZ;

			if(fabs(stepX) > fabs(diffX))
				stepX = diffX;
			if(fabs(stepY) > fabs(diffY))
				stepY = diffY;
			if(fabs(stepZ) > fabs(diffZ))
				stepZ = diffZ;

			if(stepX == 0 && stepY == 0 && stepZ == 0)
				_state = CLOSED;
			else
			{
				_posX += (float)stepX;
				_posY += (float)stepY;
				_posZ += (float)stepZ;
			}
		}
	}

	return Actor::Process(tnow, tdiff);
}


/***********************************************************
	process zone activation
***********************************************************/
void DoorActor::ProcessActivation(float PlayerPosX, float PlayerPosY, float PlayerPosZ,
									  float PlayerRotation)
{
	Open();
	SendSignal(1, _targets);
}

/***********************************************************
	process zone desactivation
***********************************************************/
void DoorActor::ProcessDesactivation(float PlayerPosX, float PlayerPosY, float PlayerPosZ,
										 float PlayerRotation)
{
	Close();
	SendSignal(2, _targets);
}


/***********************************************************
	called on signal
***********************************************************/
bool DoorActor::OnSignal(long SignalNumber)
{
	if(SignalNumber == 1)	// open door signal
	{
		Open();
		return true;
	}

	if(SignalNumber == 2)	// close door signal
	{
		Close();
		return true;
	}

	if(SignalNumber == 4)	// switch open/close door signal
	{
		if(!_signalon)
		{
			_signalon = true;
			Open();
		}
		else
		{
			_signalon = false;
			Close();
		}
		return true;
	}

	return false;
}


/***********************************************************
open the door
***********************************************************/
bool DoorActor::Open()
{
	if(_opencounter < 0)
		_opencounter=0;

	++_opencounter;
	if(_state == CLOSED || _state == CLOSING)
	{
		#ifndef _LBANET_SERVER_SIDE_
		if(_attachedsound >= 0)
		{
			std::string soundp = DataLoader::getInstance()->GetSoundPath(_attachedsound);
			if(soundp != "")
				MusicHandler::getInstance()->PlaySample(soundp, 0);
		}
		#endif

		_state = OPENING;
		return true;
	}

	return false;
}


/***********************************************************
open the door
***********************************************************/
bool DoorActor::Close()
{
	if(_opencounter > 0)
		--_opencounter;

	if(_opencounter == 0)
		if(_state == OPENED || _state == OPENING)
		{
			#ifndef _LBANET_SERVER_SIDE_
			if(_attachedsound >= 0)
			{
				std::string soundp = DataLoader::getInstance()->GetSoundPath(_attachedsound);
				if(soundp != "")
					MusicHandler::getInstance()->PlaySample(soundp, 0);
			}
			#endif

			_state = CLOSING;
			return true;
		}

	return false;
}


/***********************************************************
set actor position in the scene
***********************************************************/
void DoorActor::SetPosition(float  posX, float  posY, float  posZ)
{
	Actor::SetPosition(posX, posY, posZ);

	UpdateCLoseOpen();
}



/***********************************************************
update info
***********************************************************/
void DoorActor::UpdateCLoseOpen()
{
	_OpenedX = _posX+_OpenTransX;
	_OpenedY = _posY+_OpenTransY;
	_OpenedZ = _posZ+_OpenTransZ;

	_ClosedX = _posX;
	_ClosedY = _posY;
	_ClosedZ = _posZ;
}


/***********************************************************
render editor part
***********************************************************/
void DoorActor::RenderEditor()
{
	#ifndef _LBANET_SERVER_SIDE_
	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

	glLineWidth(2.0f);


	glPushMatrix();

	glTranslated(GetZoneCenterX(), GetZoneCenterY()/2. + 0.5, GetZoneCenterZ());
	glColor4f(0.7f,0.7f,1.0f, 1.f);

	glBegin(GL_LINES);

		glVertex3f(0,0,0);
		glVertex3f(_OpenTransX,_OpenTransY,_OpenTransZ);
	glEnd();

	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	ZoneActor::RenderEditor();
	#endif
}


/***********************************************************
get current actor state
return false if the actor is stateless
***********************************************************/
bool DoorActor::Getstate(ActorStateInfo & currState)
{
	currState.Open = (_state == OPENING) || (_state == OPENED);
	currState.Counter = _opencounter;
	currState.SignalOn = _signalon;
	return true;
}

/***********************************************************
set the actor state
***********************************************************/
void DoorActor::Setstate(const ActorStateInfo & currState)
{
	_state = (currState.Open ? OPENED : CLOSED);
	_opencounter = currState.Counter;
	_signalon = currState.SignalOn;

	if(currState.Open)
	{
		if(_Hide)
		{
			Hide();
		}
		else
		{
			_posX = _OpenedX;
			_posY = _OpenedY;
			_posZ = _OpenedZ;
		}
	}
}