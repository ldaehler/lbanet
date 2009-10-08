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

#include "Player.h"
#include "MainPlayerHandler.h"
#include "SynchronizedTimeHandler.h"
#include "Camera.h"
#include "PhysicHandler.h"
#include "ThreadSafeWorkpile.h"
#include "MusicHandler.h"
#include "DataLoader.h"

#include <windows.h>    // Header File For Windows
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#include <GL/glu.h>     // Header File For The GLu32 Library
#include <math.h>
#include <IL/ilut.h>

#ifndef M_PI
#define M_PI    3.14159265358979323846f
#endif


#define M_SOUND_JUMP 58
#define M_SOUND_DISCRETE 106
#define M_SOUND_HIT_GROUND 32
#define M_SOUND_LAND_GROUND 76
#define M_SOUND_PROTO_PACK 157
#define M_SOUND_DROW_1 227
#define M_SOUND_DROW_2 228
#define M_SOUND_DROW_3 229




/***********************************************************
	helper function
***********************************************************/
void doCirclefill(double x, double y, double radius)
{

	double y1=y;
	double x1=x;
	glBegin(GL_TRIANGLES);
	for(int i=0;i<=360;i++)
	{
		double angle=(float)(((double)i)/57.29577957795135);
		double x2=x+(radius*(float)sin((double)angle));
		double y2=y+(radius*(float)cos((double)angle));
		glVertex2d(x,y);
		glVertex2d(x1,y1);
		glVertex2d(x2,y2);
		y1=y2;
		x1=x2;
	}
	glEnd();
}


/***********************************************************
	Constructor
***********************************************************/
MainPlayerHandler::MainPlayerHandler(float speedNormal, float speedSport,
										float speedFight,float speedDiscrete,
										float speedHorse, float speedDino,
										float animationSpeed,
										float speedJump, float heightJump, float speedHurt,
										PhysicHandler*	RoomP, Camera * cam)
: _isMovingForward(false), _isMovingRotation(false),
	_up_key_pressed(false),_down_key_pressed(false),
	_right_key_pressed(false),_left_key_pressed(false),
	_pageup_key_pressed(false), _pagedown_key_pressed(false),
	_state(Ac_Normal), _current_stance(1)
	, _velocityX(0), _velocityY(0), _velocityZ(0), _currMoveType(0)
	,_velocityR(0), _corrected_velocityX(0), _corrected_velocityY(0)
	, _corrected_velocityZ(0), _remembering(false), _paused(false),
	_speedNormal(speedNormal), _speedSport(speedSport),
	_speedFight(speedFight), _speedDiscrete(speedDiscrete),
	_speedHorse(speedHorse), _speedDino(speedDino),
	_speedJump(speedJump), _heightJump(heightJump), _speedHurt(speedHurt),
	_RoomP(RoomP), _currentstance(0), _camptr(cam),
	_isAttached(false), _isDiscrete(false), _needCheck(false)
{
	_player = new Player(animationSpeed, true);
	_player->DisplayName(true);
	_player->changeAnimEntity(0, 0);
	_player->setActorAnimation(0);
	_currentbody = 0;

	_sizeX = 0.4f;
	_sizeY = 5;
	_sizeZ = 0.4f;

	_player->SetSize(_sizeX, _sizeY, _sizeZ);
}


/***********************************************************
	Destructor
***********************************************************/
MainPlayerHandler::~MainPlayerHandler()
{
	delete _player;
}

/***********************************************************
set the actor name
***********************************************************/
void MainPlayerHandler::SetName(const std::string & name)
{
	_player->SetName(name);
}



/***********************************************************
set actor position
***********************************************************/
void MainPlayerHandler::SetPosition(float NewX, float NewY, float NewZ)
{
	_player->SetPosition(NewX, NewY, NewZ);

}

/***********************************************************
set actor position
***********************************************************/
void MainPlayerHandler::SetRotation(float  R)
{
	_player->SetRotation(R);
}


/***********************************************************
change the actor to display
***********************************************************/
void MainPlayerHandler::SetDisplayedEntity(int entityNum, int bodyNum)
{
	_player->changeAnimEntity(entityNum, bodyNum);
	_player->setActorAnimation(0);
	_currentbody = bodyNum;
}

/***********************************************************
change the actor to display
***********************************************************/
void MainPlayerHandler::SetBody(int bodyNum)
{
	_player->changeAnimEntity(_player->GetModel(), bodyNum);
	_player->setActorAnimation(0);
	_currentbody = bodyNum;
}


/***********************************************************
render object
***********************************************************/
void MainPlayerHandler::Render()
{
	// draw the shadow
    glEnable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);

	glPushMatrix();
	glColor4f(0.0f,0.0f,0.0f, 0.4f);
	glTranslated(_player->GetPosX(), _floorY/2. + 0.6, _player->GetPosZ());
	glRotatef( 90, 1.0, 0.0, 0.0 );
	doCirclefill(0, 0, GetShadowCircleSize());
	glPopMatrix();

    glEnable(GL_TEXTURE_2D);

	// draw the player
	_player->Render(-1);
}

/***********************************************************
do all check to be done when idle
***********************************************************/
int MainPlayerHandler::Process(double tnow, float tdiff)
{
	if(_paused)
		return 0;


	_velocityR = 0;
	_corrected_velocityX = 0;
	_corrected_velocityY = 0;
	_corrected_velocityZ = 0;

	int res = -1;

	if(_state == Ac_scripted)
	{
		PlayerScriptPart ps = _curr_script[_curr_script_position];
		if(ps.Animation >= 0)
			_player->setActorAnimation(ps.Animation);

		switch(ps.Type)
		{
			case 0: // rotation
			{
				double expectedR = ps.ValueA;
				double currR = _player->GetRotation();
				double diff = expectedR - currR;
				if(expectedR == 0)
				{
					double diff2 = 360 - currR;
					if(abs(diff2) < abs(diff))
						diff = diff2;
				}

				float step = (float)(tdiff*ps.Speed * ((diff > 0) ? 1 : -1));
				if(fabs(step) > fabs(diff))
				{
					++_curr_script_position;
					step = (float)diff;
				}
				_velocityR = step / tdiff;
				_player->SetRotation(_player->GetRotation() + step);
			}
			break;
			case 1: // translation
			{
				double expectedX = ps.ValueA;
				double expectedY = ps.ValueB;
				double expectedZ = ps.ValueC;

				double currX = _player->GetPosX();
				double currY = _player->GetPosY();
				double currZ = _player->GetPosZ();

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

				_corrected_velocityX = (float)stepX;
				_corrected_velocityY = (float)stepY;
				_corrected_velocityZ = (float)stepZ;
				_player->SetPosition(_player->GetPosX() + (float)stepX, _player->GetPosY() + (float)stepY, _player->GetPosZ() + (float)stepZ);
				UpdateFloorY();
			}
			break;
			case 2: // animation
			{
				int animend = (int)ps.ValueA;
				if(animend >= 0)
				{
					if(_player->getKeyframe() >= animend)
						++_curr_script_position;
				}
				else
				{
					if(_player->Process(tnow, tdiff) == 1)
						++_curr_script_position;
				}
			}
			break;

			case 3: // inform event
			{
				if(ThreadSafeWorkpile::getInstance()->IsServeron())
				{
					LbaNet::ActorSignalInfo se;
					se.SignalId = (long)ps.ValueB;
					se.Targets.push_back((long)ps.ValueA);
					ThreadSafeWorkpile::getInstance()->AddSignalEvent(se);
				}
				else
				{
					std::vector<long> vectar;
					vectar.push_back((long)ps.ValueA);
					long targetsignal = (long)ps.ValueB;
					ThreadSafeWorkpile::getInstance()->AddEvent(new GameSignalvent(targetsignal, vectar));
				}



				++_curr_script_position;
			}
			break;
		}

		if(_curr_script_position >= (int)_curr_script.size())
			Stopstate();

		if(_camptr)
			_camptr->SetTarget(_player->GetPosX(), _player->GetPosY(), _player->GetPosZ());

		res = 0;
	}
	else if(_state == Ac_FallingDown)
	{
		float dtFall = 0.03f * tdiff;
		if(_nbYFall > dtFall)
		{
			_player->UpdateY(-dtFall);
			_nbYFall-= dtFall;
			_corrected_velocityY = -dtFall;

			if(_camptr)
				_camptr->SetTarget(_player->GetPosX(), _player->GetPosY(), _player->GetPosZ());
		}
		else
		{
			_player->SetY(_fallarrivalY);
			_nbYFall= 0;
			_state = Ac_hurt_fall;

			if(_camptr)
				_camptr->SetTarget(_player->GetPosX(), _player->GetPosY(), _player->GetPosZ());

			if(_RoomP && _RoomP->StepOnWater(_player->GetPosX(), _player->GetPosY(), _player->GetPosZ()))
				return 1;	// the actor should die in water

			if(_state != Ac_Flying && _player->GetPosY() == -1) // the actor should die by falling out of the map
				return 2;

			_needCheck = true;

			if(_keepYfall > 6)
				ThreadSafeWorkpile::getInstance()->AddPlayerHurtFall(_keepYfall-6);

			// playing sound only if not in water
			bool waitforanim = ChangeAnimToHurt(_keepYfall > 6);
			if(waitforanim)
			{
				return 4;
			}
			else
			{
				Stopstate();
				return 0;
			}
		}

	}
	else
	{
		if(_state == Ac_Drowning)
		{
			if(_player->getKeyframe() == 7)
			{
				if(!_drow_sound_started)
				{
					std::string soundp = DataLoader::getInstance()->GetSoundPath(M_SOUND_DROW_2);
					if(soundp != "")
						MusicHandler::getInstance()->PlaySample(soundp, 0);
					_drow_sound_started = true;
				}
			}
			else if(_player->getKeyframe() == 13)
			{
				if(!_drow_sound_started)
				{
					std::string soundp = DataLoader::getInstance()->GetSoundPath(M_SOUND_DROW_3);
					if(soundp != "")
						MusicHandler::getInstance()->PlaySample(soundp, 0);
					_drow_sound_started = true;
				}
			}
			else
				_drow_sound_started = false;
		}
		else if(_state != Ac_Dying && _RoomP)
		{
			float pY = _player->GetPosY();
			float tmpY;
			float cY = modf(pY, &tmpY);

			if((cY < 0.000001) && (_state != Ac_Flying) && ((_state != Ac_protopack) || !_up_key_pressed) && (_state != Ac_Jumping))
				if(_RoomP && _RoomP->StepOnWater(_player->GetPosX(), _player->GetPosY(), _player->GetPosZ()))
					return 1;	// the actor should die in water

			if(_state != Ac_Flying && _player->GetPosY() == -1) // the actor should die by falling out of the map
				return 2;


			if(_currentstance == 2)
			{
				if(_state == Ac_Jumping && _player->getKeyframe() > 2)
				{
					UpdateVelocity(1, true, -_speedJump);

					if(!_jump_sound_started)
					{
						std::string soundp = DataLoader::getInstance()->GetSoundPath(M_SOUND_JUMP);
						if(soundp != "")
							MusicHandler::getInstance()->PlaySample(soundp, 0);
						_jump_sound_started = true;
					}
				}
				if(_state == Ac_Jumping && _player->getKeyframe() > 2 && _player->getKeyframe() < 5)
				{
					float dy = _heightJump * tdiff;
					_RoomP->GoUpDown(_player->GetPosX(), _player->GetPosY(), _player->GetPosZ(), dy, _sizeY);
					if(dy > 0)
					{
						_player->UpdateY(dy);
						if(_camptr)
							_camptr->SetTarget(_player->GetPosX(), _player->GetPosY(), _player->GetPosZ());
					}
					_corrected_velocityY = dy;
				}
				if(_state == Ac_Jumping && _player->getKeyframe() > 4)
				{
					float dy = -_heightJump * tdiff;
					_RoomP->GoUpDown(_player->GetPosX(), _player->GetPosY(), _player->GetPosZ(), dy, _sizeY);
					if(dy < 0)
					{
						_player->UpdateY(dy);
						if(_camptr)
							_camptr->SetTarget(_player->GetPosX(), _player->GetPosY(), _player->GetPosZ());
					}
					_corrected_velocityY = dy;
				}
			}
			if(_currentstance == 5)
			{
				if(_state == Ac_Jumping)
					UpdateVelocity(1, true, -_speedHorse);
				if(_state == Ac_Jumping && _player->getKeyframe() < 3)
				{
					float dy = _heightJump * 2 * tdiff;
					_RoomP->GoUpDown(_player->GetPosX(), _player->GetPosY(), _player->GetPosZ(), dy, _sizeY);
					if(dy > 0)
					{
						_player->UpdateY(dy);
						if(_camptr)
							_camptr->SetTarget(_player->GetPosX(), _player->GetPosY(), _player->GetPosZ());
					}
					_corrected_velocityY = dy;
				}
				if(_state == Ac_Jumping && _player->getKeyframe() > 2)
				{
					float dy = -_heightJump * 2 * tdiff;
					_RoomP->GoUpDown(_player->GetPosX(), _player->GetPosY(), _player->GetPosZ(), dy, _sizeY);
					if(dy < 0)
					{
						_player->UpdateY(dy);
						if(_camptr)
							_camptr->SetTarget(_player->GetPosX(), _player->GetPosY(), _player->GetPosZ());
					}
					_corrected_velocityY = dy;
				}
			}

			if(_state == Ac_hurt)
			{
				UpdateVelocity(1, true, _speedHurt);
			}

			if(_right_key_pressed && _state != Ac_Drowning && _state != Ac_Dying &&
				_state != Ac_FallingDown && _state != Ac_hurt_fall && _state != Ac_Jumping && _state != Ac_hurt)
			{
				_velocityR = -0.15f;
				_player->SetRotation(_player->GetRotation() - tdiff*0.15f);
				UpdateVelocity(_currMoveType);
				res = 0;
			}

			if(_left_key_pressed && _state != Ac_Drowning && _state != Ac_Dying &&
				_state != Ac_FallingDown && _state != Ac_hurt_fall && _state != Ac_Jumping && _state != Ac_hurt)
			{
				_velocityR = 0.15f;
				_player->SetRotation(_player->GetRotation() + tdiff*0.15f);
				UpdateVelocity(_currMoveType);
				res = 0;
			}

			if(_up_key_pressed || _state == Ac_Jumping ||  _state == Ac_hurt)
			{
				if(MoveActor(true, tdiff))
					res = 0;
			}
			else if(_down_key_pressed)
			{
				if(MoveActor(false, tdiff))
					res = 0;
			}


			if(_pageup_key_pressed)
			{
				float dy = 0.02f * tdiff;
				_RoomP->GoUpDown(_player->GetPosX(), _player->GetPosY(), _player->GetPosZ(), dy, _sizeY);
				if(dy != 0)
				{
					_player->UpdateY(dy);
					if(_camptr)
						_camptr->SetTarget(_player->GetPosX(), _player->GetPosY(), _player->GetPosZ());

					res = 0;
				}
				_corrected_velocityY = dy;
			}

			if(_pagedown_key_pressed)
			{
				float dy = -0.02f * tdiff;
				_RoomP->GoUpDown(_player->GetPosX(), _player->GetPosY(), _player->GetPosZ(), dy, _sizeY);
				if(dy != 0)
				{
					_player->UpdateY(dy);
					if(_camptr)
						_camptr->SetTarget(_player->GetPosX(), _player->GetPosY(), _player->GetPosZ());

					res = 0;
				}
				_corrected_velocityY = dy;
			}
		}
	}


	int resA = _player->Process(tnow, tdiff);
	if(resA == 1 && res == -1)	// if actor animation is terminated and there is no other event
		res = 3;	// inform engine

	if(_state == Ac_Jumping && resA == 1)
		StopJump();

	if(_state == Ac_hurt && resA == 1)
		StopHurt();

	// normalize the velocity again
	_corrected_velocityX/=tdiff;
	_corrected_velocityY/=tdiff;
	_corrected_velocityZ/=tdiff;


	// if attached - correct velocity
	if(_isAttached)
	{
		_corrected_velocityX += _player->GetAddedvX();
		_corrected_velocityY += _player->GetAddedvY();
		_corrected_velocityZ += _player->GetAddedvZ();
		_player->SetAddedVelocity(0, 0, 0);

		if(_camptr)
			_camptr->SetTarget(_player->GetPosX(), _player->GetPosY(), _player->GetPosZ());
	}




	_dr.Update(tdiff);
	if(!_dr.IsOntrack(_player->GetPosX(), _player->GetPosY(), _player->GetPosZ(),
						_player->GetRotation(), _corrected_velocityX,
						_corrected_velocityY, _corrected_velocityZ, _velocityR,
						_player->GetModel(), _player->GetBody(), _player->GetAnimation(),
						_player->GetBodyColor() ))
	{
		_dr.Set(_player->GetPosX(), _player->GetPosY(), _player->GetPosZ(),
						_player->GetRotation(), _corrected_velocityX,
						_corrected_velocityY, _corrected_velocityZ, _velocityR,
						_player->GetModel(), _player->GetBody(), _player->GetAnimation(),
						_player->GetBodyColor());


		LbaNet::ActorInfo nai;
		nai.Time = tnow;
		nai.Name = _player->GetName();
		nai.DisplayName = _player->GetDisplayName();
		nai.X = _player->GetPosX();
		nai.Y = _player->GetPosY();
		nai.Z = _player->GetPosZ();
		nai.Rotation = _player->GetRotation();
		nai.Model = _player->GetModel();
		nai.Body = _player->GetBody();
		nai.Animation = _player->GetAnimation();
		nai.BodyColor = _player->GetBodyColor();
		nai.vX = _corrected_velocityX;
		nai.vY = _corrected_velocityY;
		nai.vZ = _corrected_velocityZ;
		nai.vRotation = _velocityR;
		ThreadSafeWorkpile::getInstance()->UpdateInfo(nai);
	}


	return res;
}



/***********************************************************
player moves
***********************************************************/
void MainPlayerHandler::PlayerStartMove(int moveDirection)
{
	_isDiscrete = false;

	if(moveDirection == 1 || moveDirection == 2)
		if(_up_key_pressed || _down_key_pressed)
			return;

	if(moveDirection == 3 && _left_key_pressed)
		return;

	if(moveDirection == 4 && _right_key_pressed)
		return;

	if(_state == Ac_Jumping && moveDirection == 2)
	{
		StopJump();
		return;
	}

	if(_state == Ac_protopack && moveDirection == 2)
		return;
	if(_state == Ac_Flying && moveDirection == 2)
		return;

	if(_state == Ac_Drowning || _state == Ac_Dying || _state == Ac_FallingDown || _state == Ac_hurt_fall || _state == Ac_Jumping || _state == Ac_scripted ||  _state == Ac_hurt)
		return;

	switch(moveDirection)
	{
		case 1:
			_up_key_pressed = true;
			_currMoveType = 1;
			UpdateVelocity(_currMoveType);

			if(_state == Ac_protopack)
			{
				std::string soundp = DataLoader::getInstance()->GetSoundPath(M_SOUND_PROTO_PACK);
				if(soundp != "")
					_sound_proto = MusicHandler::getInstance()->PlaySample(soundp, -1);
			}

		break;
		case 2:
			_down_key_pressed = true;
			_currMoveType = -1;
			UpdateVelocity(_currMoveType);
		break;
		case 3:
			_left_key_pressed = true;
			_right_key_pressed = false;
		break;
		case 4:
			_left_key_pressed = false;
			 _right_key_pressed = true;
		break;
	}

	// moving forward animation is more important than rotation animation
	if((moveDirection == 1) || (moveDirection == 2))
	{
		if(!_isMovingForward)
		{
			_isMovingForward = true;
			_player->setActorAnimation(moveDirection);
	}
	}
	else
	{
		if(!_isMovingForward)
		{
			if(!_isMovingRotation)
			{
				_isMovingRotation = true;
				_player->setActorAnimation(moveDirection);
			}
		}
		else
			_isMovingRotation = true;
	}
}


/***********************************************************
player moves
***********************************************************/
void MainPlayerHandler::PlayerStopMove(int moveDirection)
{
	switch(moveDirection)
	{
		case 1:
			_up_key_pressed = false;
		break;
		case 2:
			_down_key_pressed = false;
		break;
		case 3:
			_left_key_pressed = false;
		break;
		case 4:
			 _right_key_pressed = false;
		break;
	}




	if((moveDirection == 1) || (moveDirection == 2))
	{
		if(_isMovingForward && !_up_key_pressed && !_down_key_pressed)
		{
			_isMovingForward = false;
			_currMoveType = 0;

			if(_state != Ac_Jumping)
				UpdateVelocity(_currMoveType);

			if(_state == Ac_Normal || _state == Ac_Flying || _state == Ac_protopack)
			{
				if(_isMovingRotation)
					_player->setActorAnimation(3);
				else
					_player->setActorAnimation(0);
			}
		}
	}
	else
	{
		if(!_isMovingForward)
		{
			if(_isMovingRotation && !_left_key_pressed && !_right_key_pressed)
			{
				_isMovingRotation = false;

				if(_state == Ac_Normal || _state == Ac_Flying || _state == Ac_protopack)
					_player->setActorAnimation(0);
			}
		}
		else
			_isMovingRotation = false;
	}

	if(!_isMovingForward && _state == Ac_protopack)
		MusicHandler::getInstance()->StopSample(_sound_proto);
}


/***********************************************************
player moves
***********************************************************/
void MainPlayerHandler::PlayerStartMoveUpDown(bool Up)
{
	// if we are flying
	if(_state == Ac_Flying)
	{
		if(Up)
		{
			_pagedown_key_pressed = false;
			_pageup_key_pressed = true;
		}
		else
		{
			_pageup_key_pressed = false;
			_pagedown_key_pressed = true;
		}
	}
}


/***********************************************************
player moves
***********************************************************/
void MainPlayerHandler::PlayerStopMoveUpDown(bool Up)
{
	if(Up)
		_pageup_key_pressed = false;
	else
		_pagedown_key_pressed = false;
}


/***********************************************************
player moves
***********************************************************/
void MainPlayerHandler::PlayerChangeStance(int StanceNumber, bool forced)
{
	if(!forced)
	{
		if(_state == Ac_Drowning || _state == Ac_Dying || _state == Ac_FallingDown || _state == Ac_hurt_fall || _state == Ac_Jumping  || _state == Ac_scripted ||  _state == Ac_hurt)
			return;

		if(StanceNumber == _currentstance)
			return;
	}

	_isDiscrete = false;

	if(_state == Ac_protopack)
		MusicHandler::getInstance()->StopSample(_sound_proto);

	_currentstance = StanceNumber;

	switch(StanceNumber)
	{
		case 1:
			_player->changeAnimEntity(0, _currentbody);
			if(_state == Ac_Flying || _state == Ac_protopack)
				_state = Ac_Normal;
			_pageup_key_pressed = false;
			_pagedown_key_pressed = false;
			UpdateVelocity(_currMoveType);
			_player->setActorAnimation(GetActoAnim());
			CheckY();
		break;

		case 2:
			_player->changeAnimEntity(1, _currentbody);
			if(_state == Ac_Flying || _state == Ac_protopack)
				_state = Ac_Normal;
			_pageup_key_pressed = false;
			_pagedown_key_pressed = false;
			UpdateVelocity(_currMoveType);
			_player->setActorAnimation(GetActoAnim());
			CheckY();
		break;

		case 3:
			_player->changeAnimEntity(2, _currentbody);
			if(_state == Ac_Flying || _state == Ac_protopack)
				_state = Ac_Normal;
			_pageup_key_pressed = false;
			_pagedown_key_pressed = false;
			UpdateVelocity(_currMoveType);
			_player->setActorAnimation(GetActoAnim());
			CheckY();
		break;

		case 4:
			_player->changeAnimEntity(3, _currentbody);
			if(_state == Ac_Flying || _state == Ac_protopack)
				_state = Ac_Normal;
			_pageup_key_pressed = false;
			_pagedown_key_pressed = false;
			UpdateVelocity(_currMoveType);
			_player->setActorAnimation(GetActoAnim());
			CheckY();
		break;

		case 5:
			_player->changeAnimEntity(47, 0);
			if(_state == Ac_Flying || _state == Ac_protopack)
				_state = Ac_Normal;
			_pageup_key_pressed = false;
			_pagedown_key_pressed = false;
			UpdateVelocity(_currMoveType);
			_player->setActorAnimation(GetActoAnim());
			CheckY();
		break;

		case 6:
			_player->changeAnimEntity(64, 0);
			_state = Ac_Flying;
			UpdateVelocity(_currMoveType);
			_player->setActorAnimation(GetActoAnim());
		break;

		case 7:
			_player->changeAnimEntity(4, 0);
			_state = Ac_protopack;
			UpdateVelocity(_currMoveType);
			_player->setActorAnimation(GetActoAnim());
			CheckY();

			if(_currMoveType == 1)
			{
				std::string soundp = DataLoader::getInstance()->GetSoundPath(M_SOUND_PROTO_PACK);
				if(soundp != "")
					_sound_proto = MusicHandler::getInstance()->PlaySample(soundp, -1);
			}

			break;
	}

	_current_stance = StanceNumber;
}





/***********************************************************
check if actor goes up stairs or falldown
***********************************************************/
void MainPlayerHandler::CheckY()
{
	if(_isAttached)
		return;

	float newY = _RoomP->GetNextY(_player->GetPosX(), _player->GetPosY(), _player->GetPosZ(), _sizeX, _sizeY, _sizeZ);
	float deltaY = _player->GetPosY() - newY;


	if(deltaY <= 2 && _state != Ac_Jumping && (_pageup_key_pressed == false || (_state != Ac_Flying)) || deltaY < 0)
	{
		_player->SetPosition(_player->GetPosX(), newY, _player->GetPosZ());
		_corrected_velocityY -= deltaY;

		if(_camptr)
			_camptr->SetTarget(_player->GetPosX(), _player->GetPosY(), _player->GetPosZ());
	}
	else
	{
		if(_state != Ac_Flying && _state != Ac_Jumping)
			StartFallDown(deltaY, newY);
	}
}


/***********************************************************
return true if actor is hidden under roof
***********************************************************/
int MainPlayerHandler::IsUnderRoof()
{
	return _RoomP->IsUnderRoof(_player->GetPosX(), _player->GetPosY(), _player->GetPosZ());
}


/***********************************************************
start drawning
***********************************************************/
void MainPlayerHandler::Startdrowning()
{
	if(_state != Ac_Drowning)
	{
		StopJump();
		ResetMove();


		if(!_remembering)
		{
			_remembermodel = _player->GetModel();
			_rememberbody = _player->GetBody();
			_rememberstate = _state;
			_remembering = true;
		}

		_player->changeAnimEntity(0, _currentbody);
		_player->setActorAnimation(29);

		_state = Ac_Drowning;

		std::string soundp = DataLoader::getInstance()->GetSoundPath(M_SOUND_DROW_1);
		if(soundp != "")
			MusicHandler::getInstance()->PlaySample(soundp, 0);

	}
}


/***********************************************************
start dying
***********************************************************/
void MainPlayerHandler::Startdying()
{
	if(_state != Ac_Dying)
	{
		StopJump();
		Stopstate();
		ResetMove();

		if(!_remembering)
		{
			_remembermodel = _player->GetModel();
			_rememberbody = _player->GetBody();
			_rememberstate = _state;
			_remembering = true;
		}

		_player->changeAnimEntity(0, _currentbody);
		_player->setActorAnimation(19);
		_state = Ac_Dying;
	}
}

/***********************************************************
Stopstate
***********************************************************/
void MainPlayerHandler::Stopstate()
{
	if(_remembering)
	{
		if(_state == Ac_Dying || _state == Ac_Drowning)
			ThreadSafeWorkpile::getInstance()->AddRaisedEvent();

		_state = _rememberstate;
		_player->changeAnimEntity(_remembermodel, _rememberbody);
		_player->setActorAnimation(GetActoAnim());
		_remembering = false;
	}
}




/***********************************************************
function called to move the attached actor upward or backward
***********************************************************/
bool MainPlayerHandler::MoveActor(bool Upward, float timediff)
{
	bool res = false;


	if(_state == Ac_Drowning || _state == Ac_Dying || _state == Ac_FallingDown || _state == Ac_hurt_fall)
		return res;

	if(_RoomP)
	{
		_corrected_velocityX = _velocityX*timediff;
		_corrected_velocityZ = _velocityZ*timediff;
		_RoomP->Move(_player->GetPosX(), _player->GetPosY(), _player->GetPosZ(), _sizeX, _sizeY, _sizeZ,
						_corrected_velocityX, _corrected_velocityZ, _state == Ac_Jumping);
	}


	float vXabs = abs(_corrected_velocityX);
	float vZabs = abs(_corrected_velocityZ);

	for(float x=0; x<vXabs; x+=0.1f)
	{
		float diff = vXabs - x;
		float halfM = (diff >= 0.1) ? 0.1f : diff;

		if(_corrected_velocityX<0)
			halfM *= -1;

		SetPosition(_player->GetPosX()+halfM, _player->GetPosY(), _player->GetPosZ());
		CheckY();

	}

	for(float z=0; z<vZabs; z+=0.1f)
	{
		float diff = vZabs - z;
		float halfM = (diff >= 0.1f) ? 0.1f : diff;

		if(_corrected_velocityZ<0)
			halfM *= -1;

		SetPosition(_player->GetPosX(), _player->GetPosY(), _player->GetPosZ()+halfM);
		CheckY();
	}


	if(_camptr)
		_camptr->SetTarget(_player->GetPosX(), _player->GetPosY(), _player->GetPosZ());


	UpdateFloorY();
	return true;
}


/***********************************************************
get actor moving speed
***********************************************************/
float MainPlayerHandler::GetMovingSpeed()
{
	switch(_player->GetModel())
	{
		case 0:
			return _speedNormal;
		case 1:
			return _speedSport;
		case 2:
			return _speedFight;
		case 3:
			return _speedDiscrete;
		case 47:
			return _speedHorse;
		case 64:
			return _speedDino;
	}

	return _speedNormal;
}



/***********************************************************
called when the actor must fall down
***********************************************************/
void MainPlayerHandler::StartFallDown(float nbY, float fallarrivalY)
{
	if(_state != Ac_FallingDown)
	{
		if(_state == Ac_protopack)
			MusicHandler::getInstance()->StopSample(_sound_proto);

		//ResetMove();

		_nbYFall = nbY;
		_keepYfall = nbY;
		_fallarrivalY = fallarrivalY;

		if(!_remembering)
		{
			_remembermodel = _player->GetModel();
			_rememberbody = _player->GetBody();
			_rememberstate = _state;
			_remembering = true;
		}

		ChangeAnimToFallDown();

		_state = Ac_FallingDown;

	}
}


/***********************************************************
called when the space bar is pressed
***********************************************************/
void MainPlayerHandler::DoAction()
{
	//setActorAnimation(_currAnimation+1);

	if(_player->GetModel() == 1 || _player->GetModel() == 47)
		StartJump();

	// go discrete
	if(_player->GetModel() == 3 && !_isDiscrete && !_isMovingForward && !_isMovingRotation)
	{
		_isDiscrete = true;
		_player->setActorAnimation(5);

		std::string soundp = DataLoader::getInstance()->GetSoundPath(M_SOUND_DISCRETE);
		if(soundp != "")
			MusicHandler::getInstance()->PlaySample(soundp, 0);
	}
}


/***********************************************************
called when the actor start  jump
***********************************************************/
void MainPlayerHandler::StartJump()
{
	if(_state != Ac_Jumping)
	{
		_jump_sound_started = false;
		UpdateVelocity(0);
		_state = Ac_Jumping;

		if(_player->GetModel() == 1)
			_player->setActorAnimation(5);

		if(_player->GetModel() == 47)
			_player->setActorAnimation(9);
	}
}


/***********************************************************
called when the actor finished jump
***********************************************************/
void MainPlayerHandler::StopJump()
{
	if(_state == Ac_Jumping)
	{
		 _state = Ac_Normal;
		_player->setActorAnimation(0);
		UpdateVelocity(_currMoveType);
		_player->setActorAnimation(GetActoAnim());
		CheckY();

		_needCheck = true;
	}
}





/***********************************************************
recalculate actor velocity
***********************************************************/
void MainPlayerHandler::UpdateVelocity(int MoveType, bool ManualSpeed, float speed)
{
	float halfM = GetMovingSpeed() * MoveType * ((MoveType == 1) ? 1.0f : 0.5f) * -1;
	if(ManualSpeed)
		halfM = speed;


	int nbA = ((int)_player->GetRotation()) / 90;
	int modA = ((int)_player->GetRotation()) % 90;


	float radA =  M_PI * (modA) / 180.0f;


	if(nbA == 0)
	{
		_velocityX = sin(radA) * -halfM;
		_velocityZ = cos(radA) * -halfM;
	}
	if(nbA == 1)
	{
		_velocityX = cos((float)radA) * -halfM;
		_velocityZ = sin((float)radA) * halfM;
	}
	if(nbA == 2)
	{
		_velocityX = sin(radA) * halfM;
		_velocityZ = cos(radA) * halfM;
	}
	if(nbA == 3)
	{
		_velocityX = cos((float)radA) * halfM;
		_velocityZ = sin((float)radA) * -halfM;
	}
}


/***********************************************************
stop all move an reset all velocities
***********************************************************/
void MainPlayerHandler::ResetMove()
{
	_up_key_pressed = false;
	_down_key_pressed = false;
	_right_key_pressed = false;
	_left_key_pressed = false;
	_pageup_key_pressed = false;
	_pagedown_key_pressed = false;

	_currMoveType = 0;
	_velocityX = 0;
	_velocityY = 0;
	_velocityZ = 0;
	_velocityR = 0;

	_corrected_velocityX = 0;
	_corrected_velocityY = 0;
	_corrected_velocityZ = 0;

	_isMovingForward = false;
	_isMovingRotation = false;

	_player->setActorAnimation(0);
}


/***********************************************************
Resume
***********************************************************/
void MainPlayerHandler::Resume(bool interior, bool reinit)
{
	_paused = false;

	if(reinit)
	{
		if(_state != Ac_Normal && _state != Ac_protopack && _state != Ac_Flying)
			Stopstate();


		// remove horse or dino inside rooms
		if(interior)
		{
			if(GetStance() == 5 || GetStance() == 6)
				PlayerChangeStance(1, true);
		}

		StopJump();
		CheckY();
	}
}


/***********************************************************
get actor anim depending of it is moving or not
***********************************************************/
int MainPlayerHandler::GetActoAnim()
{
	if(_up_key_pressed)
		return 1;

	if(_down_key_pressed)
		return 2;

	if(_right_key_pressed)
		return 4;

	if(_left_key_pressed)
		return 3;

	return 0;
}



/***********************************************************
change anim to fall down
***********************************************************/
void MainPlayerHandler::ChangeAnimToFallDown()
{
	if(_player->GetModel() == 47)
	{
		_player->setActorAnimation(0);
		return;
	}

	if(_player->GetModel() == 4)
	{
		_player->setActorAnimation(11);
		return;
	}

	_player->changeAnimEntity(0, _currentbody);
	_player->setActorAnimation(9);
}


/***********************************************************
change anim to hurt after fall
***********************************************************/
bool MainPlayerHandler::ChangeAnimToHurt(bool StrongHurt)
{
	if(_player->GetModel() == 47)
		return false;

	if(_player->GetModel() == 4)
	{
		_player->setActorAnimation(12);

		if(_currMoveType == 1)
		{
			MusicHandler::getInstance()->StopSample(_sound_proto);
			std::string soundp = DataLoader::getInstance()->GetSoundPath(M_SOUND_PROTO_PACK);
			if(soundp != "")
				_sound_proto = MusicHandler::getInstance()->PlaySample(soundp, -1);
		}

		return true;
	}


	if(StrongHurt)
	{
		_player->setActorAnimation(15);

		std::string soundp = DataLoader::getInstance()->GetSoundPath(M_SOUND_HIT_GROUND);
		if(soundp != "")
			MusicHandler::getInstance()->PlaySample(soundp, 0);
	}
	else
	{
		_player->setActorAnimation(14);

		std::string soundp = DataLoader::getInstance()->GetSoundPath(M_SOUND_LAND_GROUND);
		if(soundp != "")
			MusicHandler::getInstance()->PlaySample(soundp, 0);
	}

	return true;
}



/***********************************************************
return true if the actor is currently moving
***********************************************************/
bool MainPlayerHandler::IsMoving()
{
	if(_paused)
		return false;

	return (_isMovingForward || (_state == Ac_Jumping) || (_state == Ac_FallingDown) || (_state == Ac_hurt));
}



/***********************************************************
return player position
***********************************************************/
float  MainPlayerHandler::GetPosX()
{
	return _player->GetPosX();
}

/***********************************************************
return player position
***********************************************************/
float  MainPlayerHandler::GetPosY()
{
	return _player->GetPosY();
}

/***********************************************************
return player position
***********************************************************/
float  MainPlayerHandler::GetPosZ()
{
	return _player->GetPosZ();
}

/***********************************************************
return player position
***********************************************************/
float  MainPlayerHandler::GetRotation()
{
	return _player->GetRotation();
}


/***********************************************************
update position of the floor
***********************************************************/
void MainPlayerHandler::UpdateFloorY()
{
	int posX = (int)GetPosX();
	int posY = (int)GetPosY();
	int posZ = (int)GetPosZ();

	_floorY = posY;
	if(_RoomP)
	{
		_floorY = _RoomP->GetFloorY(posX, posY, posZ);
		_floorY = std::min(_RoomP->GetFloorY(posX+1, posY, posZ) , _floorY);
		_floorY = std::min(_RoomP->GetFloorY(posX-1, posY, posZ) , _floorY);
		_floorY = std::min(_RoomP->GetFloorY(posX, posY, posZ+1) , _floorY);
		_floorY = std::min(_RoomP->GetFloorY(posX, posY, posZ-1) , _floorY);
	}
}

/***********************************************************
get the size of the shadow circle depending of the actor
***********************************************************/
float MainPlayerHandler::GetShadowCircleSize()
{
	switch(_player->GetModel())
	{
		case 47:
			return 1.2f;
		case 64:
			return 1.5f;
	}

	return 0.8f;
}



/***********************************************************
set body color
***********************************************************/
void MainPlayerHandler::SetBodyColor(short newColor)
{
	_player->SetBodyColor(newColor);
}

/***********************************************************
get body color
***********************************************************/
short MainPlayerHandler::GetBodyColor()
{
	return _player->GetBodyColor();
}



/***********************************************************
return true if the player is in activation mode
***********************************************************/
bool MainPlayerHandler::ActivationMode(bool ForcedNormalAction)
{
	if(_state == Ac_Drowning || _state == Ac_Dying || _state == Ac_FallingDown || _state == Ac_hurt_fall || _state == Ac_Jumping  || _state == Ac_scripted ||  _state == Ac_hurt)
		return false;

	if(_currentstance==1 || _currentstance==2 || _currentstance==3 || _currentstance==4)
		return (_currentstance==1 || ForcedNormalAction);

	return false;
}

/***********************************************************
display a scripted event using the player character
***********************************************************/
void MainPlayerHandler::DoPlayerScriptedEvent(const std::vector<PlayerScriptPart> &script)
{
	if(!_remembering)
	{
		_remembermodel = _player->GetModel();
		_rememberbody = _player->GetBody();
		_rememberstate = _state;
		_remembering = true;
	}
	_player->changeAnimEntity(0, _currentbody);

	_state = Ac_scripted;
	_curr_script = script;
	_curr_script_position = 0;
}


/***********************************************************
player is hurt by an actor
***********************************************************/
void MainPlayerHandler::PlayerHurt(long actorid)
{
	Stopstate();

	ThreadSafeWorkpile::getInstance()->AddPlayerHurt(actorid);

	std::string soundp = DataLoader::getInstance()->GetSoundPath(31);
	if(soundp != "")
		MusicHandler::getInstance()->PlaySample(soundp, 0);

	_hurtingactorId = actorid;
	_remembering = true;
	_rememberstate = _state;
	_remembermodel = _player->GetModel();
	_rememberbody = _player->GetBody();
	_player->changeAnimEntity(0, _currentbody);
	_player->setActorAnimation(12);
	_state = Ac_hurt;
}


/***********************************************************
called when the actor finished hurt
***********************************************************/
void MainPlayerHandler::StopHurt()
{
	if(_state == Ac_hurt)
	{
		 _state = Ac_Normal;
		_player->changeAnimEntity(_remembermodel, _currentbody);
		_player->setActorAnimation(0);
		UpdateVelocity(_currMoveType);
		_player->setActorAnimation(GetActoAnim());
		CheckY();

		// inform actor that the hurting animation is finished
		std::vector<long> vectar;
		vectar.push_back(_hurtingactorId);
		long targetsignal = 3;
		ThreadSafeWorkpile::getInstance()->AddEvent(new GameSignalvent(targetsignal, vectar));
		_needCheck = true;
	}
}


/***********************************************************
return true if the map need to be checked for player position
***********************************************************/
bool MainPlayerHandler::NeedCheck()
{
	if(_needCheck)
	{
		_needCheck = false;
		return true;
	}

	return false;
}


/***********************************************************
player life changed
***********************************************************/
bool MainPlayerHandler::PlayerLifeChanged(float CurLife, float MaxLife, float CurMana, float MaxMana)
{
	_player->setCurrentLife(CurLife);
	_player->setMaxLife(MaxLife);
	_player->setCurrentMana(CurMana);
	_player->setMaxMana(MaxMana);

	if(CurLife <= 0)
	{
		Startdying();
		return true;
	}

	return false;
}


/***********************************************************
render the main player at a fixed position for picture
***********************************************************/
void MainPlayerHandler::RenderForPicture()
{
	_remembermodel = _player->GetModel();
	_rememberbody = _player->GetBody();
	int anim = _player->GetAnimation();

	_player->changeAnimEntity(0, _currentbody);
	_player->setActorAnimation(1);
	_player->setActorAnimation(0);

	float rot = _player->GetRotation();
	_player->SetRotation(60);

	// draw the player
	_player->Render(-1);


	_player->SetRotation(rot);
	_player->changeAnimEntity(_remembermodel, _currentbody);
	_player->setActorAnimation(anim);
}