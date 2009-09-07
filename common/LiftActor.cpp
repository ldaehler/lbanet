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


#include "LiftActor.h"
#include <math.h>
#include <algorithm>

#ifndef _LBANET_SERVER_SIDE_
#include "ThreadSafeWorkpile.h"
#include "DataLoader.h"
#include "MusicHandler.h"
#endif

/***********************************************************
	Constructor
***********************************************************/
LiftActor::LiftActor(const std::vector<PlayerScriptPart> & scripts)
: _scripts(scripts), _curr_script_position(0), _started_timer(false),
	_playedsound(-1), _playingsound(0)
{

}

/***********************************************************
	Destructor
***********************************************************/
LiftActor::~LiftActor()
{
#ifndef _LBANET_SERVER_SIDE_
	if(_playingsound > 0)
	{
		MusicHandler::getInstance()->StopSample(_playingsound);
		_playingsound = 0;
	}
#endif
}

/***********************************************************
do all check to be done when idle
***********************************************************/
int LiftActor::Process(double tnow, float tdiff)
{
	if(_curr_script_position < _scripts.size())
	{
		PlayerScriptPart ps = _scripts[_curr_script_position];

		#ifndef _LBANET_SERVER_SIDE_
		if(ps.Sound >= 0)
		{
			if(_playedsound != ps.Sound)
			{
				if(_playingsound > 0)
				{
					MusicHandler::getInstance()->StopSample(_playingsound);
					_playingsound = 0;
				}

				std::string soundp = DataLoader::getInstance()->GetSoundPath(ps.Sound);
				if(soundp != "")
					_playingsound = MusicHandler::getInstance()->PlaySample(soundp, -1);
			}
		}
		else
		{
			if(_playingsound > 0)
			{
				MusicHandler::getInstance()->StopSample(_playingsound);
				_playingsound = 0;
			}
		}
		_playedsound = ps.Sound;
		#endif

		switch(ps.Type)
		{
			case 0: // rotation
			{
				double expectedR = ps.ValueA;
				double currR = GetRotation();
				double diff = expectedR - currR;
				if(expectedR == 0)
				{
					double diff2 = 360 - currR;
					if(fabs(diff2) < fabs(diff))
						diff = diff2;
				}

				float step = (float)(tdiff*ps.Speed * ((diff > 0) ? 1 : -1));
				if(fabs(step) > fabs(diff))
				{
					++_curr_script_position;
					step = (float)diff;
				}

				SetRotation(GetRotation() + step);
			}
			break;
			case 1: // translation
			{
				double expectedX = ps.ValueA;
				double expectedY = ps.ValueB;
				double expectedZ = ps.ValueC;

				double currX = GetPosX();
				double currY = GetPosY();
				double currZ = GetPosZ();

				double diffX = expectedX - currX;
				double diffY = expectedY - currY;
				double diffZ = expectedZ - currZ;

				double stepX = tdiff*ps.Speed * ((diffX > 0) ? 1 : -1);
				double stepY = tdiff*ps.Speed * ((diffY > 0) ? 1 : -1);
				double stepZ = tdiff*ps.Speed * ((diffZ > 0) ? 1 : -1);

				if(fabs(stepX) > fabs(diffX))
					stepX = diffX;
				if(fabs(stepY) > fabs(diffY))
					stepY = diffY;
				if(fabs(stepZ) > fabs(diffZ))
					stepZ = diffZ;

				if(stepX == 0 && stepY == 0 && stepZ == 0)
					++_curr_script_position;

				SetPosition(GetPosX() + (float)stepX, GetPosY() + (float)stepY, GetPosZ() + (float)stepZ);

				std::vector<Actor *>::iterator itaa = _attachedActors.begin();
				std::vector<Actor *>::iterator endaa = _attachedActors.end();
				for(;itaa != endaa; ++itaa)
				{
					(*itaa)->SetPosition((*itaa)->GetPosX() + (float)stepX, (*itaa)->GetPosY() + (float)stepY, (*itaa)->GetPosZ() + (float)stepZ);
					(*itaa)->SetAddedVelocity((float)(stepX/tdiff), (float)(stepY/tdiff), (float)(stepZ/tdiff));
				}
			}
			break;

			case 3: // inform event
			{
				std::vector<long> vectar;
				vectar.push_back((int)ps.ValueA);
				int targetsignal = (int)ps.ValueB;
				SendSignal(targetsignal, vectar);

				++_curr_script_position;
			}
			break;

			case 4: // wait for signal event
			{
				long sig = (long)ps.ValueA;
				std::vector<long>::iterator it = std::find(_receivedsignals.begin(), _receivedsignals.end(), sig);
				if(it != _receivedsignals.end())
				{
					_receivedsignals.erase(it);
					++_curr_script_position;
				}
			}
			break;

			case 5: // wait for ms
			{
				if(!_started_timer)
				{
					_started_timer = true;
					_timer_start_time = tnow;
				}
				else
				{
					if(tnow - _timer_start_time > ps.ValueA)
					{
						_started_timer = false;
						++_curr_script_position;
					}
				}

			}
			break;
		}

		if(_curr_script_position >= _scripts.size())
			_curr_script_position = 0;
	}





	return Actor::Process(tnow, tdiff);
}


/***********************************************************
called on signal
***********************************************************/
bool LiftActor::OnSignal(long SignalNumber)
{
	_receivedsignals.push_back(SignalNumber);
	return true;
}


/***********************************************************
check if the actor should be attached
***********************************************************/
bool LiftActor::CheckAttach(Actor * act)
{
	float posX = GetPosX();
	float posY = GetPosY();
	float posZ = GetPosZ();


	if( (act->GetPosX() >= (posX-_sizeX) && act->GetPosX() < (posX+_sizeX)) &&
		(act->GetPosY() >= (posY)				&& act->GetPosY() < (posY+_sizeY+1)) &&
		(act->GetPosZ() >= (posZ-_sizeZ) && act->GetPosZ() < (posZ+_sizeZ)))
	{
		act->SetPosition(act->GetPosX(), posY+_sizeY, act->GetPosZ());
		Attach(act);
		return true;
	}


	return false;
}


/***********************************************************
check if the actor should be dettached
***********************************************************/
bool LiftActor::CheckDettach(Actor * act)
{
	float posX = GetPosX();
	float posY = GetPosY();
	float posZ = GetPosZ();

	if( (act->GetPosX() >= (posX-_sizeX) && act->GetPosX() < (posX+_sizeX)) &&
		(act->GetPosY() >= (posY)				&& act->GetPosY() < (posY+_sizeY+1)) &&
		(act->GetPosZ() >= (posZ-_sizeZ) && act->GetPosZ() < (posZ+_sizeZ)))
	{
		return false;
	}

	return Dettach(act);
}


/***********************************************************
get current actor state
return false if the actor is stateless
***********************************************************/
bool LiftActor::Getstate(ActorStateInfo & currState)
{
	currState.CurrentScript = _curr_script_position;
	currState.CurrentSignals = _receivedsignals;
	currState.X = GetPosX();
	currState.Y = GetPosY();
	currState.Z = GetPosZ();
	currState.Rotation = GetRotation();
	return true;
}


/***********************************************************
set the actor state
***********************************************************/
void LiftActor::Setstate(const ActorStateInfo & currState)
{
	_receivedsignals = currState.CurrentSignals;
	_curr_script_position = currState.CurrentScript;
	SetRotation(currState.Rotation);
	SetPosition(currState.X, currState.Y, currState.Z);
}