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

#if !defined(__LbaNetModel_1_MainPlayerHandler_h)
#define __LbaNetModel_1_MainPlayerHandler_h

class Camera;
class Player;
class PhysicHandlerBase;

#include <math.h>
#include <vector>
#include "GameEvents.h"

#include "MagicBallHandler.h"



class DeadReckon
{
public:

	float			_posX;
	float			_posY;
	float			_posZ;
	float			_rotation;

	float			_sizeX;
	float			_sizeY;
	float			_sizeZ;

	float			_velocityX;
	float			_velocityY;
	float			_velocityZ;
	float			_velocityR;

	int				_Model;
	int				_Body;
	int				_Animation;
	short			_color;

	int				_nameR;
	int				_nameG;
	int				_nameB;

	bool			_visible;


	// set reckon value
	void Set( float posX, float posY, float posZ,
				float rotation, float velocityX, float velocityY,
				float velocityZ, float velocityR,
				int Model, int Body, int Animation, short color,
				int	nameR, int	nameG, int	nameB, bool Visible,
				float sizeX, float sizeY, float sizeZ)
	{
		_visible = Visible;

		_posX = posX;
		_posY = posY;
		_posZ = posZ;
		_rotation = rotation;

		_sizeX = sizeX;
		_sizeY = sizeY;
		_sizeZ = sizeZ;

		_velocityX = velocityX;
		_velocityY = velocityY;
		_velocityZ = velocityZ;
		_velocityR = velocityR;

		_Model = Model;
		_Body = Body;
		_Animation = Animation;
		_color = color;

		_nameR = nameR;
		_nameG = nameG;
		_nameB = nameB;
	}

	// update reackon on each tick
	void Update(double timediff)
	{
		_posX += _velocityX*timediff;
		_posY += _velocityY*timediff;
		_posZ += _velocityZ*timediff;

		_rotation += _velocityR*timediff;

		if(_rotation > 360)
			_rotation = _rotation - 360;
		if(_rotation < 0)
			_rotation = 360 + _rotation;
	}

	// check if reackon is still on track
	bool IsOntrack( float posX, float posY, float posZ,
					float rotation, float velocityX, float velocityY,
					float velocityZ, float velocityR,
					int Model, int Body, int Animation, short color,
					int	nameR, int	nameG, int	nameB, bool Visible,
					float sizeX, float sizeY, float sizeZ) const
	{
		if(_visible != Visible)
			return false;

		if(_Model != Model)
			return false;

		if(_Body != Body)
			return false;

		if(_Animation != Animation)
			return false;

		if(_color != color)
			return false;

		float diffpos = abs(posX - _posX) + abs(posY - _posY) +  abs(posZ - _posZ);
		if(diffpos > 5)
			return false;

		double diffrot = abs(rotation - _rotation);
		if(diffrot > 10)
			return false;

		if(abs(velocityX - _velocityX) > 0.00001f)
			return false;

		if(abs(velocityY - _velocityY) > 0.00001f)
			return false;

		if(abs(velocityZ - _velocityZ) > 0.00001f)
			return false;

		if(abs(velocityR - _velocityR) > 0.1f)
			return false;

		float diffsize = abs(sizeX - _sizeX) + abs(sizeY - _sizeY) +  abs(sizeZ - _sizeZ);
		if(diffsize > 0.01f)
			return false;

		if(_nameR != nameR)
			return false;

		if(_nameG != nameG)
			return false;

		if(_nameB != nameB)
			return false;

		return true;
	}
};



/***********************************************************************
 * Module:  MainPlayerHandler.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:54:27
 * Purpose: Declaration of the class MainPlayerHandler
 ***********************************************************************/
class MainPlayerHandler
{
public:
	//! constructor
	MainPlayerHandler(float speedNormal, float speedSport,
						float speedFight,float speedDiscrete,
						float speedHorse, float speedDino, float animationSpeed,
						float speedJump, float heightJump, float speedHurt);

	//! destructor
	~MainPlayerHandler();

	// set the actor name
	void SetName(const std::string & name);

	// set the actor status
	void SetStatus(const std::string & status);

	// set physic handler
	void SetPhysicHandler(PhysicHandlerBase * PH)
	{_RoomP = PH;}


	//! set actor position
	void SetPosition(float NewX, float NewY, float NewZ);
	void SetRotation(float  R);

	//! change the actor to display
	void SetDisplayedEntity(int entityNum, int bodyNum);
	void SetBody(int bodyNum);

	//! render object
	void Render();

	//! do all check to be done when idle
	int Process(double tnow, float tdiff);

	// player moves
	void PlayerStartMove(int moveDirection);
	void PlayerStopMove(int moveDirection);
	void PlayerStartMoveUpDown(bool Up);
	void PlayerStopMoveUpDown(bool Up);
	void PlayerChangeStance(int StanceNumber, bool forced = false);


	//return true if actor is hidden under roof
	int IsUnderRoof();

	// start drawning
	void Startdrowning();

	// stop state animation
	void Stopstate();

	// start drawning
	void Startdying();

	// return true if the player is in activation mode
	bool ActivationMode(bool ForcedNormalAction);

	// called when the space bar is pressed
	bool DoAction();

	// pause resume player
	void Pause() {_paused = true;}
	void Resume(bool interior, bool reinit);

	//! return true if the actor is currently moving
	bool IsMoving();

	//! return true if the actor is currently in scripted event
	bool IsScriptedEvent()
	{return (_state == Ac_scripted);}


	//! return player position
	float  GetPosX();
	float  GetPosY();
	float  GetPosZ();
	float  GetRotation();
	float  GetSizeX();
	float  GetSizeY();
	float  GetSizeZ();

	//update position of the floor
	void UpdateFloorY();

	// stance accessor
	int GetStance()
	{return _currentstance;}

	//! set body color
	void SetBodyColor(short newColor);

	//! get body color
	short GetBodyColor();

	// display a scripted event using the player character
	void DoPlayerScriptedEvent(const std::vector<PlayerScriptPart> &script);


	// set if the main actor is attached
	void SetAttached(bool attached)
	{
		_isAttached = attached;
	}

	// return true if the actor is attached
	bool IsAttached()
	{ return _isAttached;}


	//! accessor
	Player * GetPlayer()
	{return _player;}


	// player is hurt by an actor
	void PlayerHurt(long actorid);


	// return true if the map need to be checked for player position
	bool NeedCheck();


	// return true if actor is jumping
	bool IsJumping()
	{ return _state == Ac_Jumping;}


	//player life changed
	// return true if actor die
	void PlayerLifeChanged(float CurLife, float MaxLife, float CurMana, float MaxMana, bool Hurt);


	// render the main player at a fixed position for picture
	void RenderForPicture();


	// set player name color
	void SetNameColor(int R, int G, int B);


	// give signal to main player
	void SetSignal(int newsignal);


	// actor used for attach/detach
	long GetAttachActor()
	{ return _attachactor;}


	// get map player will pe teleported
	std::string GetNewMap()
	{ return _newmap;}


	// get map player will pe teleported
	std::string GetSpawning()
	{ return _spawning;}


	// show player
	void Show();

	// make actor use current weapon
	// return true if weapon can be used
	bool UseWeapon();

	//! clear the magic ball if launched (e.g we change map)
	void ClearMB();

protected:

	//get actor moving speed
	float GetMovingSpeed();

	// called when the actor must fall down
	void StartFallDown();

	// called when the actor start  jump
	void StartJump();

	// called when the actor finished jump
	void StopJump();

	// called when the actor finished hurt
	void StopHurt();

	// recalculate actor velocity
	// moveType: 0 - no move, 1 - move upward, -1 - move backward
	void CalculateVelocity(bool MoveForward, bool ManualSpeed=false, float speed=0.0f);

	// stop all move an reset all velocities
	void ResetMove();

	// get actor anim depending of it is moving or not
	int GetActoAnim();

	// change anim to fall down
	void ChangeAnimToFallDown();

	// change anim to hurt after fall
	bool ChangeAnimToHurt(bool StrongHurt);

	//get the size of the shadow circle depending of the actor
	float GetShadowCircleSize();

	// player script
	int PlayScript(double tnow, float tdiff);

	// finish process
	int FinishProcess(double tnow, float tdiff, int res);

	// return the animation number for the weapon use
	// depending of the stance
	int GetWeaponAnimation();

protected:
	enum ActorState { Ac_Normal=0, Ac_Drowning, Ac_Dying, Ac_Flying, Ac_Jumping, Ac_FallingDown, Ac_protopack, Ac_hurt_fall, Ac_scripted, Ac_hurt, Ac_useweapon };

	Player*			_player;
	PhysicHandlerBase*	_RoomP;

	bool			_isMovingForward;
	bool			_isMovingRotation;
	bool			_isDiscrete;

	int				_remembermodel;
	int				_rememberbody;
	ActorState		_rememberstate;
	bool			_remembering;
	bool			_touchedground;

	ActorState		_state;
	float			_keepYfall;

	float			_floorY;

	int				_currentstance;
	int				_currentbody;
	int				_current_stance;



	// remember if a key is pressed
	bool			_up_key_pressed;
	bool			_down_key_pressed;
	bool			_right_key_pressed;
	bool			_left_key_pressed;
	bool			_pageup_key_pressed;
	bool			_pagedown_key_pressed;


	float			_velocityX;
	float			_velocityY;
	float			_velocityZ;
	float			_velocityR;

	float			_corrected_velocityX;
	float			_corrected_velocityY;
	float			_corrected_velocityZ;


	DeadReckon		_dr;
	bool			_paused;


	float			_speedNormal;
	float			_speedSport;
	float			_speedFight;
	float			_speedDiscrete;
	float			_speedHorse;
	float			_speedDino;
	float			_speedJump;
	float			_heightJump;
	float			_speedHurt;

	bool			_jump_sound_started;
	bool			_drow_sound_started;

	unsigned long	_sound_proto;

	// used for scripted event
	std::vector<PlayerScriptPart>	_curr_script;
	int								_curr_script_position;

	// flag to tell if the main actor is attached to another actor or not
	bool			_isAttached;

	long			_hurtingactorId;
	bool			_needCheck;

	double			_lastdeathtime;

	// current signal set by external
	int				_currentsignal;

	// actor used for attach/detach
	long			_attachactor;

	MagicBallHandler	_magicballH;
	bool				_wlaunched;

	float				_GravityFalldown;


	std::string		_newmap;
	std::string		_spawning;

};

#endif