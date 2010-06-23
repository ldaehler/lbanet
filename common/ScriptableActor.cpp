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


#include "ScriptableActor.h"
#include <math.h>
#include <algorithm>

#ifndef _LBANET_SERVER_SIDE_
#include "ThreadSafeWorkpile.h"
#include "DataLoader.h"
#include "MusicHandler.h"
#include "CharacterRenderer.h"
#else
#include "ServerCharacterRenderer.h"
#endif

#ifndef M_PI
#define M_PI    3.14159265358979323846f
#endif


/***********************************************************
	Constructor
***********************************************************/
ScriptableActor::ScriptableActor(const std::vector<PlayerScriptPart> & scripts, bool IsLift)
: _scripts(scripts), _curr_script_position(0), _started_timer(false),
	_playedsound(-1), _playingsound(0), _IsLift(IsLift), _needs_update(false),
	_forceanim(false)
{
}

/***********************************************************
	Destructor
***********************************************************/
ScriptableActor::~ScriptableActor()
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
int ScriptableActor::Process(double tnow, float tdiff)
{
	if(_curr_script_position < _scripts.size())
	{
		PlayerScriptPart &ps = _scripts[_curr_script_position];

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
					_playingsound = MusicHandler::getInstance()->PlaySample(soundp, ps.SoundNum);
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

		if(ps.Type == 2 && _last_script_position != _curr_script_position)
			_forceanim = true;

		if(ps.Animation >= 0 && _Renderer && _Renderer->GetType() == 3)
			static_cast<CharacterRenderer *>(_Renderer)->setActorAnimation(ps.Animation, _forceanim);

		_forceanim = false;
		_last_script_position = _curr_script_position;
		#else
		if(ps.Animation >= 0 && _Renderer && _Renderer->GetType() == 3)
			static_cast<ServerCharacterRenderer *>(_Renderer)->setActorAnimation(ps.Animation);
		#endif

		switch(ps.Type)
		{
			case 0: // rotation
			{
				double expectedR = ps.ValueA;
				double currR = GetRotation();
				double diff, diff2;
				if(expectedR < currR)
					expectedR += 360;

				diff = expectedR - currR;
				diff2 = diff-360;

				if(fabs(diff2) < fabs(diff))
					diff = diff2;

				float step = (float)(tdiff*ps.Speed * ((diff > 0) ? 1 : -1));
				if(fabs(step) > fabs(diff))
				{
					IncreaseScriptPosition();
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
					IncreaseScriptPosition();

				UpdatePosition(stepX, stepY, stepZ, tdiff);
			}
			break;

			case 2: // animation
			{
				int animend = (int)ps.ValueA;
				#ifndef _LBANET_SERVER_SIDE_
				if(_Renderer)
				{
					if(animend >= 0)
					{
						if(_Renderer && _Renderer->GetType() == 3)						
							if(static_cast<CharacterRenderer *>(_Renderer)->getKeyframe() >= animend)
								IncreaseScriptPosition();
					}
					else
					{
						if(_Renderer && _Renderer->GetType() == 3)
							if(static_cast<CharacterRenderer *>(_Renderer)->IsAnimationFinished())
								IncreaseScriptPosition();
					}
				}

				if(!_started_timer)
				{
					_started_timer = true;
					_timer_start_time = tnow;
				}
				else
				{
					// put a timer in case the animation gets stuck
					if(tnow - _timer_start_time > 30000)
						IncreaseScriptPosition();
				}

				#else
				if(_Renderer)
				{
					if(animend >= 0)
					{
						if(_Renderer && _Renderer->GetType() == 3)					
							if(static_cast<ServerCharacterRenderer *>(_Renderer)->getKeyframe() >= animend)
								IncreaseScriptPosition();
					}
					else
					{
						if(_Renderer && _Renderer->GetType() == 3)			
							if(static_cast<ServerCharacterRenderer *>(_Renderer)->Process(tnow, tdiff) == 1)
								IncreaseScriptPosition();
					}
				}
				#endif
			}
			break;

			case 3: // inform event
			{
				std::vector<long> vectar;
				vectar.push_back((int)ps.ValueA);
				int targetsignal = (int)ps.ValueB;
				SendSignal(targetsignal, vectar);

				IncreaseScriptPosition();
			}
			break;

			case 4: // wait for signal event
			{
				long sig = (long)ps.ValueA;
				std::vector<long>::iterator it = std::find(_receivedsignals.begin(), _receivedsignals.end(), sig);
				if(it != _receivedsignals.end())
				{
					_receivedsignals.erase(it);
					IncreaseScriptPosition();
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
						IncreaseScriptPosition();
				}
			}
			break;

			case 6: // do a curve
			{
				// do rotation part
				double expectedR = ps.ValueA;
				double currR = GetRotation();
				double diff, diff2;
				if(expectedR < currR)
					expectedR += 360;

				diff = expectedR - currR;
				diff2 = diff-360;

				if(fabs(diff2) < fabs(diff))
					diff = diff2;

				float step = (float)(tdiff*ps.Speed * ((diff > 0) ? 1 : -1));
				if(fabs(step) > fabs(diff))
				{
					IncreaseScriptPosition();
					step = (float)diff;
				}

				SetRotation(GetRotation() + step);

				// do translation part
				{
					double translationSpeed = -ps.ValueB;


					int nbA = ((int)GetRotation()) / 90;
					int modA = ((int)GetRotation()) % 90;


					float radA =  M_PI * (modA) / 180.0f;
					double _velocityX, _velocityZ;

					if(nbA == 0)
					{
						_velocityX = sin(radA) * -translationSpeed;
						_velocityZ = cos(radA) * -translationSpeed;
					}
					if(nbA == 1)
					{
						_velocityX = cos((float)radA) * -translationSpeed;
						_velocityZ = sin((float)radA) * translationSpeed;
					}
					if(nbA == 2)
					{
						_velocityX = sin(radA) * translationSpeed;
						_velocityZ = cos(radA) * translationSpeed;
					}
					if(nbA == 3)
					{
						_velocityX = cos((float)radA) * translationSpeed;
						_velocityZ = sin((float)radA) * -translationSpeed;
					}

					double stepX = tdiff*_velocityX;
					double stepZ = tdiff*_velocityZ;
					UpdatePosition(stepX, 0, stepZ, tdiff);
				}
			}
			break;

	
			case 7: // hide
			{
				Hide();
				IncreaseScriptPosition();
			}
			break;		
	
			case 8: // show
			{
				Show();
				IncreaseScriptPosition();
			}
			break;	
	
			case 9: // change body
			{
				#ifndef _LBANET_SERVER_SIDE_
				if(_Renderer)
				{
					CharacterRenderer * rend = static_cast<CharacterRenderer *>(_Renderer);
					rend->changeAnimEntity(rend->GetModel(), (int)ps.ValueA);
					rend->setActorAnimation(((ps.Animation>= 0) ? ps.Animation : 0));
				}		
				#endif
				
				IncreaseScriptPosition();
			}
			break;	
		}

	}



	return Actor::Process(tnow, tdiff);
}


/***********************************************************
called on signal
***********************************************************/
bool ScriptableActor::OnSignal(long SignalNumber)
{
	_receivedsignals.push_back(SignalNumber);
	return true;
}

/***********************************************************
get current actor state
return false if the actor is stateless
***********************************************************/
bool ScriptableActor::Getstate(ActorStateInfo & currState)
{
	currState.ActorId = _ID;
	currState.CurrentScript = _curr_script_position;
	currState.CurrentSignals = _receivedsignals;
	currState.X = GetPosX();
	currState.Y = GetPosY();
	currState.Z = GetPosZ();
	currState.Rotation = GetRotation();
	currState.Visible = Visible();
	currState.Targets = _actiontargets;
	return true;
}


/***********************************************************
set the actor state
***********************************************************/
void ScriptableActor::Setstate(const ActorStateInfo & currState)
{
	_receivedsignals = currState.CurrentSignals;
	while(_curr_script_position != currState.CurrentScript)
	{
		if(_curr_script_position < _scripts.size())
		{
			PlayerScriptPart &ps = _scripts[_curr_script_position];
			if(_scripts[_curr_script_position].Type == 3)
			{
				std::vector<long> vectar;
				vectar.push_back((int)ps.ValueA);
				int targetsignal = (int)ps.ValueB;
				SendSignal(targetsignal, vectar);
			}
		}

		IncreaseScriptPosition(true);
	}


	SetRotation(currState.Rotation);
	SetPosition(currState.X, currState.Y, currState.Z);

	if(currState.Visible)
		Show();
	else
		Hide();

	_actiontargets = currState.Targets;
	_forceanim = true;
}



/***********************************************************
check if the actor should be attached
***********************************************************/
bool ScriptableActor::CheckAttach(Actor * act)
{
	if(!_IsLift)
		return false;

	float posX = GetPosX();
	float posY = GetPosY();
	float posZ = GetPosZ();


	if( (act->GetPosX() >= (posX-_sizeX) && act->GetPosX() < (posX+_sizeX)) &&
		(act->GetPosY() >= (posY)				&& act->GetPosY() < (posY+_sizeY+1)) &&
		(act->GetPosZ() >= (posZ-_sizeZ) && act->GetPosZ() < (posZ+_sizeZ)))
	{
		//act->SetPosition(act->GetPosX(), posY+_sizeY, act->GetPosZ());
		Attach(act);
		return true;
	}


	return false;
}


/***********************************************************
check if the actor should be dettached
***********************************************************/
bool ScriptableActor::CheckDettach(Actor * act)
{
	if(!_IsLift)
		return false;

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
increase script position
***********************************************************/
void ScriptableActor::IncreaseScriptPosition(bool forcedreset)
{
	++_curr_script_position;
	_started_timer = false;

	if(_curr_script_position >= _scripts.size())
	{
		#ifdef _LBANET_SERVER_SIDE_
		_curr_script_position = 0;
		_needs_update = true;
		#else
		if(forcedreset || !ThreadSafeWorkpile::getInstance()->IsServeron())
			_curr_script_position = 0;
		#endif
	}
}


/***********************************************************
if the actor needs to be updated client side
***********************************************************/
bool ScriptableActor::NeedsUpdate()
{
	bool res = _needs_update;
	_needs_update = false;
	return res;
}