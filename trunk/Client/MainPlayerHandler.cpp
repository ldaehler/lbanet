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
#include "PhysicHandlerBase.h"
#include "ThreadSafeWorkpile.h"
#include "MusicHandler.h"
#include "DataLoader.h"
#include "ConfigurationManager.h"

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
										float speedJump, float heightJump, float speedHurt)
: _isMovingForward(false), _isMovingRotation(false),
	_up_key_pressed(false),_down_key_pressed(false),
	_right_key_pressed(false),_left_key_pressed(false),
	_pageup_key_pressed(false), _pagedown_key_pressed(false),
	_state(Ac_Normal), _current_stance(1)
	, _velocityX(0), _velocityY(0), _velocityZ(0)
	,_velocityR(0), _corrected_velocityX(0), _corrected_velocityY(0)
	, _corrected_velocityZ(0), _remembering(false), _paused(false),
	_speedNormal(speedNormal), _speedSport(speedSport),
	_speedFight(speedFight), _speedDiscrete(speedDiscrete),
	_speedHorse(speedHorse), _speedDino(speedDino),
	_speedJump(speedJump), _heightJump(heightJump), _speedHurt(speedHurt),
	_RoomP(NULL), _currentstance(0), 
	_isDiscrete(false), _needCheck(false), _currentsignal(-1),
	_touchedground(true), _magicballH(true), _oldtdiff(1),
	_averageSpeedX(10), _averageSpeedY(10), _averageSpeedZ(10), _waittojump(false),
	_iscollisionup(false), _iscollisionx(false), _iscollisionz(false)
{
	_player = new Player(animationSpeed, true);
	_player->DisplayName(true);
	_player->changeAnimEntity(0, 0);
	_player->setActorAnimation(0);
	_currentbody = 0;

	_player->SetSize(0.4f, 5, 0.4f);

	_magicballH.SetOwner(_player);

	ConfigurationManager::GetInstance()->GetFloat("Physic.GravityFalldown", _GravityFalldown);
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
	UpdateFloorY();

	if(_RoomP)
		_RoomP->SetActorPos(-1, VECTOR(NewX, NewY, NewZ));
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
	if(!_player->Visible())
		return;

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
	glPushMatrix();
	//glTranslated(0, (-2.5 / 2), 0);
	_player->Render(-1);
	glPopMatrix();

	// render tha magic ball
	_magicballH.Render();
}


/***********************************************************
play script
***********************************************************/
int MainPlayerHandler::PlayScript(double tnow, float tdiff)
{
	if(_curr_script_position >= (int)_curr_script.size())
	{
		if(_RoomP)
			_RoomP->SetActorPos(-1, VECTOR(_player->GetPosX(), _player->GetPosY(), _player->GetPosZ()));

		Stopstate();
		return 0;
	}

	PlayerScriptPart ps = _curr_script[_curr_script_position];
	if(ps.Animation >= 0)
		_player->setActorAnimation(ps.Animation);

	switch(ps.Type)
	{
		case 0: // rotation
		{
			double expectedR = ps.ValueA;
			double currR = _player->GetRotation();
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

			CalculateVelocity();
			double stepX = _velocityX;
			double stepY = _player->GetRendererSpeedY();
			double stepZ = _velocityZ;

			if(ps.AutoMoveY)
			{
				if(abs(diffY) > 0.0000001)
					stepY = tdiff*0.0025f * ((diffY > 0) ? 1 : -1);
				else
					stepY = diffY;
			}

			//double stepX = tdiff*ps.Speed * ((diffX > 0) ? 1 : -1);
			//double stepY = tdiff*ps.Speed * ((diffY > 0) ? 1 : -1);
			//double stepZ = tdiff*ps.Speed * ((diffZ > 0) ? 1 : -1);

			bool finishedx = false;
			bool finishedy = false;
			bool finishedz = false;

			if(fabs(stepX) >= fabs(diffX) || (stepX > 0 && diffX < 0) || (stepX < 0 && diffX > 0) || (abs(diffX) < 0.01))
			{
				finishedx = true;
				stepX = diffX;
			}
			if(fabs(stepY) >= fabs(diffY) || (stepY > 0 && diffY < 0) || (stepY < 0 && diffY > 0) || (abs(diffY) < 0.01))
			{
				finishedy = true;
				stepY = diffY;
			}
			if(fabs(stepZ) >= fabs(diffZ) || (stepZ > 0 && diffZ < 0) || (stepZ < 0 && diffZ > 0) || (abs(diffZ) < 0.01))
			{
				finishedz = true;
				stepZ = diffZ;
			}

			_corrected_velocityX = (float)stepX;
			_corrected_velocityY = (float)stepY;
			_corrected_velocityZ = (float)stepZ;
			_player->SetPosition(	_player->GetPosX() + _corrected_velocityX, 
									_player->GetPosY() + _corrected_velocityY, 
									_player->GetPosZ() + _corrected_velocityZ);


			if(finishedx && finishedy && finishedz)
				++_curr_script_position;

			UpdateFloorY();
		}
		break;
		case 2: // animation
		{
			//do the move from the animation
			CalculateVelocity();
			double stepX = _velocityX;
			double stepY = _player->GetRendererSpeedY();
			double stepZ = _velocityZ;

			//if need to boost Y
			//if(ps.BoostY)
			//{
			//	if(abs(stepY) > 0.00001)
			//		stepY += tdiff*ps.Speed * ((stepY > 0) ? 1 : -1);
			//}

			_corrected_velocityX = (float)stepX;
			_corrected_velocityY = (float)stepY;
			_corrected_velocityZ = (float)stepZ;
			_player->SetPosition(	_player->GetPosX() + _corrected_velocityX, 
									_player->GetPosY() + _corrected_velocityY, 
									_player->GetPosZ() + _corrected_velocityZ);

			int animend = (int)ps.ValueA;
			if(animend >= 0)
			{
				if(_player->getKeyframe() >= animend)
					++_curr_script_position;
			}
			else
			{
				if(_player->IsAnimationFinished())
					++_curr_script_position;
			}
		}
		break;

		case 3: // inform event
		{
			if(ps.Flag && ThreadSafeWorkpile::getInstance()->IsServeron())
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

		case 4: // wait for signal event
		{
			long sig = (long)ps.ValueA;
			if(sig == _currentsignal)
				++_curr_script_position;
		}
		break;

		case 7: // hide
		{
			_player->Hide();
			++_curr_script_position;
		}
		break;		

		case 8: // show
		{
			_player->Show();
			++_curr_script_position;
		}
		break;	

		case 9: // change stance
		{
			PlayerChangeStance((int)ps.ValueA, true);
			++_curr_script_position;
		}
		break;

		case 10: // attach player to another actor
		{
			++_curr_script_position;
			_attachactor = (long)ps.ValueA;
			return 5;
		}
		break;

		case 11: // dettach player from another actor
		{
			++_curr_script_position;
			_attachactor = (long)ps.ValueA;
			return 6;
		}
		break;

		case 12: // teleport player to another map
		{
			++_curr_script_position;
			_newmap = ps.NewMap;
			_spawning = ps.Spawning;
			return 7;
		}
		break;

	}

	if(_curr_script_position >= (int)_curr_script.size())
	{
		if(_RoomP)
			_RoomP->SetActorPos(-1, VECTOR(_player->GetPosX(), _player->GetPosY(), _player->GetPosZ()));

		Stopstate();
	}

	_currentsignal = -1;
	return -1;
}


/***********************************************************
do all check to be done when idle
***********************************************************/
int MainPlayerHandler::Process(double tnow, float tdiff)
{
	// process magic ball
	_magicballH.Process();


	if(_paused)
		return 0;

	_iscollisionup = false; 
	_iscollisionx = false; 
	_iscollisionz = false; 


	_velocityR = 0;
	_corrected_velocityX = 0;
	_corrected_velocityY = 0;
	_corrected_velocityZ = 0;

	// in case of scripted event - do not take the usual process path
	if(_state == Ac_scripted)
	{
		return FinishProcess(tnow, tdiff, PlayScript(tnow, tdiff));
	}

	// in the case we are drowning
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

		return FinishProcess(tnow, tdiff, -1);
	}


	// in the case we are dying
	if(_state == Ac_Dying)
	{
		return FinishProcess(tnow, tdiff, -1);
	}


	// in case we are using a weapon
	if(_state == Ac_useweapon)
	{
		if(!_wlaunched && _player->getKeyframe() > 5)
		{
			_wlaunched = true;

			int nbA = ((int)_player->GetRotation()) / 90;
			int modA = ((int)_player->GetRotation()) % 90;
			float radA =  M_PI * (modA) / 180.0f;
			float dirX, dirZ;

			if(nbA == 0)
			{
				dirX = sin(radA);
				dirZ = cos(radA);
			}
			if(nbA == 1)
			{
				dirX = cos((float)radA);
				dirZ = -sin((float)radA);
			}
			if(nbA == 2)
			{
				dirX = -sin(radA);
				dirZ = -cos(radA);
			}
			if(nbA == 3)
			{
				dirX = -cos((float)radA);
				dirZ = sin((float)radA);
			}


			int mode = _currentstance;
			if(mode > 4)
				mode = 1;
			_magicballH.Launch(GetPosX(), GetPosY(), GetPosZ(), dirX, dirZ, mode, 
									(_player->GetCurrentMana() > 0), NULL);
		}

		return FinishProcess(tnow, tdiff, -1);
	}



	// in case we are jumping
	if(_state == Ac_Jumping)
	{
		// in case we are in sport mode
		if(_currentstance == 2)
		{
			if(_player->getKeyframe() > 2)
			{
				//CalculateVelocity(true, true, -_speedJump);

				if(!_jump_sound_started)
				{
					std::string soundp = DataLoader::getInstance()->GetSoundPath(M_SOUND_JUMP);
					if(soundp != "")
						MusicHandler::getInstance()->PlaySample(soundp, 0);
					_jump_sound_started = true;
				}
			}
			//else
			//{
			//	CalculateVelocity(true, true, 0);
			//}

			//if(_player->getKeyframe() > 2 && _player->getKeyframe() < 5)
			//	_corrected_velocityY = _heightJump * tdiff;

			//if(_player->getKeyframe() > 4)
			//	_corrected_velocityY = -_heightJump * tdiff;
		}

		// in case we are in horse mode
		//if(_currentstance == 5)
		//{
		//	CalculateVelocity(true, true, -_speedHorse);

		//	if( _player->getKeyframe() < 3)
		//		_corrected_velocityY = _heightJump * 2 * tdiff;

		//	if(_player->getKeyframe() > 2)
		//		_corrected_velocityY = -_heightJump * 2 * tdiff;
		//}

		CalculateVelocity();
		_corrected_velocityX = _velocityX/* * tdiff*/;
		_corrected_velocityZ = _velocityZ/* * tdiff*/;
		_corrected_velocityY = _player->GetRendererSpeedY();

		_cumujumpY += _corrected_velocityY;
		if(_cumujumpY > 2.2)
			_corrected_velocityY = 0;

		//if(_corrected_velocityY > 0)
		//	_corrected_velocityY += 0.08;
		//if(_corrected_velocityY < 0)
		//	_corrected_velocityY -= 0.08;
	}


	// in case player has been touch and is hurt - make him move backward
	if(_state == Ac_hurt)
	{
		CalculateVelocity(/*false, true, _speedHurt*/);
		_corrected_velocityX = _velocityX/* * tdiff*/;
		_corrected_velocityZ = _velocityZ/* * tdiff*/;
	}


	// if we are not jumping neither flying we need to add the gravity
	if(_state != Ac_Jumping && _state != Ac_Flying && !_player->IsAttached())
	{
		_corrected_velocityY = _GravityFalldown * tdiff;
		if(_state == Ac_FallingDown)
			_corrected_velocityY = _player->GetRendererSpeedY();
	}



	// normal case - check all player inputs and calculate new speeds
	if(_state != Ac_Drowning && _state != Ac_Dying &&
		_state != Ac_FallingDown && _state != Ac_hurt_fall &&
		_state != Ac_Jumping && _state != Ac_hurt)
	{
		//if right key pressed
		if(_right_key_pressed && _state != Ac_movingobjects)
		{
			_velocityR = -0.15f;
			_player->SetRotation(_player->GetRotation() - tdiff*0.15f);
		}

		//if left key pressed
		if(_left_key_pressed && _state != Ac_movingobjects)
		{
			_velocityR = 0.15f;
			_player->SetRotation(_player->GetRotation() + tdiff*0.15f);
		}

		// if we are flying - check up/down input
		if(_state == Ac_Flying)
		{
			if(_pageup_key_pressed)
				_corrected_velocityY = 0.02f * tdiff;

			if(_pagedown_key_pressed)
				_corrected_velocityY = -0.02f * tdiff;
		}

		// if up/down key
		if(_up_key_pressed || _down_key_pressed)
		{
			if(_state != Ac_movingobjects || _up_key_pressed)
			{
				CalculateVelocity(/*_up_key_pressed*/);
				_corrected_velocityX = _velocityX;// * tdiff;
				_corrected_velocityZ = _velocityZ;// * tdiff;
			}
		}
	}


	// now check the physic handler 
	int res = -1;
	if(_RoomP)
	{
		MoveOutput moveO = _RoomP->MoveActor(-1, _player->GetBoundingBox(),
										VECTOR(_corrected_velocityX, _corrected_velocityY, _corrected_velocityZ));

		_iscollisionup = moveO.CollisionUp; 
		_iscollisionx = moveO.Collisionx; 
		_iscollisionz = moveO.Collisionz; 

		// update actor speed
		_corrected_velocityX = moveO.NewSpeed.x;		
		_corrected_velocityY = moveO.NewSpeed.y;		
		_corrected_velocityZ = moveO.NewSpeed.z;	

		// update actor position
		_player->SetPosition(	_player->GetPosX()+_corrected_velocityX, 
								_player->GetPosY()+_corrected_velocityY, 
								_player->GetPosZ()+_corrected_velocityZ);

		if(moveO.MovingObject)
		{
			SetCharMovingObject(moveO.MovingDirection);
		}


		//if not flying or jumping make a few tests
		if(_state != Ac_Jumping && _state != Ac_Flying)
		{
			_waittojump = false;

			if(_player->GetPosY() < -0.5) // the actor should die by falling out of the map
				return 2;

			// if we are on water
			if(moveO.TouchingWater)
			{
				//if we do not use the protopack then we drown
				if((_state != Ac_protopack) || !_up_key_pressed)
				{
					if((tnow - _lastdeathtime) > 3000) // leave 10sec time to recover between 2 drowning
						return 1;	// the actor should die in water
				}
			}


			// if the actor does not touch the ground it means he is falling down
			if(!moveO.TouchingGround && !_player->IsAttached())
			{
				if(!_chefkiffall)
				{
					_chefkiffall = true;
					_ycheckiffall = _player->GetPosY();
				}
				else
				{
					float fallsize = _ycheckiffall - _player->GetPosY();
					if(fallsize > 0.1)
					{
						StartFallDown();
						_touchedground = false;
					}
				}
			}
			else
			{
				_chefkiffall = false;

				//if we were falling down then player will be hurt by touching the ground
				if(!_touchedground &&  (_state == Ac_FallingDown))
				{
					_touchedground = true;
					_needCheck = true;
					float fallsize = _keepYfall - _player->GetPosY();

					if(fallsize > 1.5)
					{
						if(_keepYfall > 6)
							ThreadSafeWorkpile::getInstance()->AddPlayerHurtFall(fallsize-6);

						// playing sound
						bool waitforanim = ChangeAnimToHurt(fallsize > 6);
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
					else
					{
						Stopstate();
						return 0;
					}
				}
			}
		}

		// return the correct value if actor has moved
		if(abs(_corrected_velocityX) > 0.0001f || abs(_corrected_velocityZ) > 0.0001f)
		{
			res = 0;
			UpdateFloorY();
		}
	}


	return FinishProcess(tnow, tdiff, res);
}




/***********************************************************
FinishProcess
***********************************************************/
int MainPlayerHandler::FinishProcess(double tnow, float tdiff, int res)
{
	int resA = _player->Process(tnow, tdiff);
	if(resA == 1 && res == -1) // if actor animation is terminated and there is no other event
		res = 3;	// inform engine



	if(_state == Ac_Jumping && resA == 1)
		StopJump();

	if(_state == Ac_hurt && resA == 1)
		StopHurt();


	// normalize the velocity again
	_corrected_velocityX/=_oldtdiff;
	_corrected_velocityY/=_oldtdiff;
	_corrected_velocityZ/=_oldtdiff;

	_oldtdiff = tdiff;


	// if attached - correct velocity
	if(_player->IsAttached() || (_state == Ac_scripted))
	{
		_corrected_velocityX += _player->GetAddedvX();
		_corrected_velocityY += _player->GetAddedvY();
		_corrected_velocityZ += _player->GetAddedvZ();

		_RoomP->MoveActor(-1, _player->GetBoundingBox(),
										VECTOR	(_player->GetAddedvX(), 
												_player->GetAddedvY(), 
												_player->GetAddedvZ()),
										false);

		_player->SetAddedVelocity(0, 0, 0);
	}


	double avspx = _averageSpeedX.Update(_corrected_velocityX);
	double avspy = _averageSpeedY.Update(_corrected_velocityY);
	double avspz = _averageSpeedZ.Update(_corrected_velocityZ);

	bool fwdR = _isMovingForward || (_state == Ac_Jumping) || (_state == Ac_hurt) 
				|| (_state == Ac_scripted) || (_state == Ac_FallingDown);


	_dr.Update(tdiff);
	if(!_dr.IsOntrack(_player->GetPosX(), _player->GetPosY(), _player->GetPosZ(),
						_player->GetRotation()/*, avspx, avspy, avspz*/, _velocityR,
						_player->GetModel(), _player->GetBody(), _player->GetAnimation(),
						_player->GetBodyColor(), _player->GetNameR(), 
						_player->GetNameG(), _player->GetNameB(), _player->Visible(),
						_player->GetSizeX(), _player->GetSizeY(), _player->GetSizeZ(), 
						fwdR, _iscollisionx, _iscollisionz))
	{
		_dr.Set(_player->GetPosX(), _player->GetPosY(), _player->GetPosZ(),
						_player->GetRotation()/*, avspx, avspy, avspz*/, _velocityR,
						_player->GetModel(), _player->GetBody(), _player->GetAnimation(),
						_player->GetBodyColor(), _player->GetNameR(), 
						_player->GetNameG(), _player->GetNameB(), _player->Visible(),
						_player->GetSizeX(), _player->GetSizeY(), _player->GetSizeZ(), 
						fwdR, _iscollisionx, _iscollisionz
						/*,chexkvx, chexkvy, chexkvz, tdiff*/);


		LbaNet::ActorInfo nai;
		nai.Time = tnow;
		nai.Name = _player->GetName();
		nai.DisplayName = _player->GetDisplayName();
		nai.X = _player->GetPosX();
		nai.Y = _player->GetPosY();
		nai.Z = _player->GetPosZ();
		nai.Rotation = _player->GetRotation();
		nai.SizeX = _player->GetSizeX();
		nai.SizeY = _player->GetSizeY();
		nai.SizeZ = _player->GetSizeZ();
		nai.Model = _player->GetModel();
		nai.Body = _player->GetBody();
		nai.Animation = _player->GetAnimation();
		nai.BodyColor = _player->GetBodyColor();
		nai.vX = (int)fwdR;//_corrected_velocityX;
		nai.vY = (int)_iscollisionx; // _corrected_velocityY;
		nai.vZ = (int)_iscollisionz; //_corrected_velocityZ;
		nai.vRotation = _velocityR;
		nai.NameR = _player->GetNameR();
		nai.NameG = _player->GetNameG();
		nai.NameB = _player->GetNameB();
		nai.Visible = _player->Visible();
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

	if(_state == Ac_Drowning || _state == Ac_Dying || _state == Ac_FallingDown || _state == Ac_hurt_fall || 
		_state == Ac_Jumping || _state == Ac_scripted ||  _state == Ac_hurt || _state == Ac_useweapon)
		return;

	switch(moveDirection)
	{
		case 1:
			_up_key_pressed = true;

			if(_state == Ac_protopack)
			{
				std::string soundp = DataLoader::getInstance()->GetSoundPath(M_SOUND_PROTO_PACK);
				if(soundp != "")
					_sound_proto = MusicHandler::getInstance()->PlaySample(soundp, -1);
			}

		break;
		case 2:
			_down_key_pressed = true;
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
			_player->SetMoving(true);
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
			EndCharMovingObject();
		break;
		case 2:
			_down_key_pressed = false;
			EndCharMovingObject();
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
			_player->SetMoving(false);

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

			_player->setActorAnimation(GetActoAnim());
		break;

		case 2:
			_player->changeAnimEntity(1, _currentbody);
			if(_state == Ac_Flying || _state == Ac_protopack)
				_state = Ac_Normal;

			_player->setActorAnimation(GetActoAnim());
		break;

		case 3:
			_player->changeAnimEntity(2, _currentbody);
			if(_state == Ac_Flying || _state == Ac_protopack)
				_state = Ac_Normal;

			_player->setActorAnimation(GetActoAnim());
		break;

		case 4:
			_player->changeAnimEntity(3, _currentbody);
			if(_state == Ac_Flying || _state == Ac_protopack)
				_state = Ac_Normal;
	
			_player->setActorAnimation(GetActoAnim());
		break;

		case 5:
			_player->changeAnimEntity(47, 0);
			if(_state == Ac_Flying || _state == Ac_protopack)
				_state = Ac_Normal;

			_player->setActorAnimation(GetActoAnim());
		break;

		case 6:
			_player->changeAnimEntity(64, 0);
			_state = Ac_Flying;
			_player->setActorAnimation(GetActoAnim());
		break;

		case 7:
			_player->changeAnimEntity(4, 0);
			_state = Ac_protopack;
			_player->setActorAnimation(GetActoAnim());

			if(_up_key_pressed)
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
return true if actor is hidden under roof
***********************************************************/
int MainPlayerHandler::IsUnderRoof()
{
	if(_RoomP)
		return _RoomP->IsUnderRoof(_player->GetPosition());
	else
		return -1;
}



/***********************************************************
start drawning
***********************************************************/
void MainPlayerHandler::Startdrowning()
{
	ThreadSafeWorkpile::getInstance()->InformDrown();

	if(_state != Ac_Drowning)
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
		{
			ThreadSafeWorkpile::getInstance()->AddRaisedEvent();
			_lastdeathtime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeDouble();
		}

		_state = _rememberstate;
		_player->changeAnimEntity(_remembermodel, _rememberbody);
		_player->setActorAnimation(GetActoAnim());
		_remembering = false;
	}
}





/***********************************************************
get actor moving speed
***********************************************************/
float MainPlayerHandler::GetMovingSpeed()
{
	if(_state == Ac_movingobjects)
		return _speedNormal/2;

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
void MainPlayerHandler::StartFallDown()
{
	if(_state != Ac_FallingDown)
	{
		if(_state == Ac_protopack)
			MusicHandler::getInstance()->StopSample(_sound_proto);


		_keepYfall = _player->GetPosY();

		
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
bool MainPlayerHandler::DoAction()
{
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

		return true;
	}

	return false;
}


/***********************************************************
called when the actor start  jump
***********************************************************/
void MainPlayerHandler::StartJump()
{
	if(_state == Ac_Normal && !_waittojump && !_chefkiffall)
	{
		_cumujumpY = 0;
		_jump_sound_started = false;
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
		_player->setActorAnimation(GetActoAnim());

		_needCheck = true;
		_waittojump = true;
	}
}





/***********************************************************
recalculate actor velocity
***********************************************************/
void MainPlayerHandler::CalculateVelocity(/*bool MoveForward, bool ManualSpeed, float speed*/)
{
	//std::cout<<_player->GetRendererSpeed()<<std::endl;

	//float halfM = GetMovingSpeed() * (MoveForward ? -1.0f : 0.5f);
	float halfM = -_player->GetRendererSpeed();// / -2.0;
	
	//speed up if dino
	if(_currentstance == 6)
		halfM *= 3;

	//if(ManualSpeed)
	//	halfM = speed;




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

		if(_up_key_pressed)
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

float  MainPlayerHandler::GetSizeX(){return _player->GetSizeX();}
float  MainPlayerHandler::GetSizeY(){return _player->GetSizeY();}
float  MainPlayerHandler::GetSizeZ(){return _player->GetSizeZ();}


/***********************************************************
update position of the floor
***********************************************************/
void MainPlayerHandler::UpdateFloorY()
{
	_floorY = GetPosY();
	if(_RoomP)
	{
		_floorY = _RoomP->GetClosestFloor(_player->GetPosition());
		//_floorY = std::min(_RoomP->GetFloorY(posX+1, posY, posZ) , _floorY);
		//_floorY = std::min(_RoomP->GetFloorY(posX-1, posY, posZ) , _floorY);
		//_floorY = std::min(_RoomP->GetFloorY(posX, posY, posZ+1) , _floorY);
		//_floorY = std::min(_RoomP->GetFloorY(posX, posY, posZ-1) , _floorY);
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
	_player->setActorAnimation(0);

	_state = Ac_scripted;
	_curr_script = script;
	_curr_script_position = 0;
}


/***********************************************************
player is hurt by an actor
***********************************************************/
void MainPlayerHandler::PlayerHurt(long actorid)
{
	// if already hurting then do nothing
	if(_state == Ac_hurt || _state == Ac_Drowning || _state == Ac_Dying || _state == Ac_scripted)
	{
		if(actorid >= 0)
		{
			// inform back actor that the hurting is finished
			std::vector<long> vectar;
			vectar.push_back(_hurtingactorId);
			long targetsignal = 3;
			ThreadSafeWorkpile::getInstance()->AddEvent(new GameSignalvent(targetsignal, vectar));
		}

		return;
	}

	Stopstate();

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
		_player->setActorAnimation(GetActoAnim());

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
void MainPlayerHandler::PlayerLifeChanged(float CurLife, float MaxLife, float CurMana, float MaxMana, bool Hurt)
{
	_player->setCurrentLife(CurLife);
	_player->setMaxLife(MaxLife);
	_player->setCurrentMana(CurMana);
	_player->setMaxMana(MaxMana);

	if(Hurt)
		PlayerHurt(-1);
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


/***********************************************************
set player name color
***********************************************************/
void MainPlayerHandler::SetNameColor(int R, int G, int B)
{
	_player->SetNameColor(R, G, B);
}



/***********************************************************
give signal to main player
***********************************************************/
void MainPlayerHandler::SetSignal(int newsignal)
{
	_currentsignal = newsignal;
}


/***********************************************************
show player
***********************************************************/
void MainPlayerHandler::Show()
{
	_player->Show();
}


/***********************************************************
make actor use current weapon
return true if weapon can be used
***********************************************************/
bool MainPlayerHandler::UseWeapon()
{
	if(_state != Ac_Normal && _state != Ac_protopack)
		return false;

	if(_currentstance > 4 && _state != Ac_protopack)
		return false;

	if(_magicballH.Launched())
		return false;

	_wlaunched = false;
	_remembering = true;
	_rememberstate = _state;
	_remembermodel = _player->GetModel();
	_rememberbody = _player->GetBody();
	_player->setActorAnimation(GetWeaponAnimation());
	_state = Ac_useweapon;
	return true;
}



/***********************************************************
called when player start moving objects
***********************************************************/
void MainPlayerHandler::SetCharMovingObject(int MovingDirection)
{
	if(_state != Ac_Normal && _state != Ac_protopack)
		return;

	_RoomP->SetAllowedMoving(true);

	_remembering = true;
	_rememberstate = _state;
	_remembermodel = _player->GetModel();
	_rememberbody = _player->GetBody();

	if(_up_key_pressed)
	{
		_player->changeAnimEntity(0, _currentbody);
		_player->setActorAnimation(18);

		switch(MovingDirection)
		{
			case 1:
				_player->SetRotation(90);
			break;
			case 2:
				_player->SetRotation(270);
			break;
			case 3:
				_player->SetRotation(0);
			break;
			case 4:
				_player->SetRotation(180);
			break;
		}
	}

	_state = Ac_movingobjects;
}

/***********************************************************
called when player should stop moving objects
***********************************************************/
void MainPlayerHandler::EndCharMovingObject()
{
	if(_state != Ac_movingobjects)
		return;

	_RoomP->SetAllowedMoving(false);
	Stopstate();
}


/***********************************************************
return the animation number for the weapon use
depending of the stance
**************************************************-********/
int MainPlayerHandler::GetWeaponAnimation()
{
	switch(_currentstance)
	{
		case 1:
			return 10;

		case 2:
			return 6;

		case 3:
			return 14;

		case 4:
			return 12;

		case 7:
			return 8;
	}

	return 0;
}


/***********************************************************
clear the magic ball if launched (e.g we change map)
**************************************************-********/
void MainPlayerHandler::ClearMB()
{
	_magicballH.Clear();
}



/***********************************************************
set if the main actor is attached
**************************************************-********/
void MainPlayerHandler::SetAttached(bool attached)
{
	_player->SetAttached(attached);
}

/***********************************************************
return true if the actor is attached
**************************************************-********/
bool MainPlayerHandler::IsAttached()
{ 
	return _player->IsAttached();
}