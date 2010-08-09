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


#if !defined(__LbaNetModel_1_Actor_h)
#define __LbaNetModel_1_Actor_h

class MainPlayerHandler;
class D3ObjectRenderer;
#include <string>
#include <vector>
#include <sstream>
#include "SignalerBase.h"
#include "vector.h"
class ActorPositionHandler;

struct ActorStateInfo
{
	long ActorId;

	//used for switch
	bool On;

	//used for door
	bool Open;
	int Counter;
	bool SignalOn;

	//used for lift
	size_t CurrentScript;
	std::vector<long> CurrentSignals;
	float			X;
	float			Y;
	float			Z;
	float			Rotation;


	//used for NPC
	bool			Visible;
	std::vector<std::pair<long, long> >			Targets;
};



/***********************************************************************
 * Module:  Actor.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:53:50
 * Purpose: Declaration of the class Actor
 *********************************************************************/
class Actor
{
public:
	//! constructor
   Actor();

   //! destructor
   virtual ~Actor();

	//! render object
	virtual void Render(int RoomCut);

	//! do all check to be done when idle
	virtual int Process(double tnow, float tdiff);

   //! show object
   void Show(void);

   //! hide object
   void Hide(void);

	// set actor position in the scene
	virtual void SetPosition(float  posX, float  posY, float  posZ, bool refreshPhysic = true);

	// update actor position in the scene
	virtual void UpdatePosition(float deltaposX, float  deltaposY, float  deltaposZ, float tdiff);

	// update Y coordinate
	//void UpdateY(float  deltaY)
	//{
	//	_posY += deltaY;

	//	if(_posY < -10)
	//		_posY = -10;
	//}

	//// update Y coordinate
	//void SetY(float newY)
	//{
	//	_posY = newY;

	//	if(_posY < -10)
	//		_posY = -10;
	//}

	// set actor rotation
	void SetRotation(float  R)
	{
		_rotation = R;

		if(_rotation >= 360)
			_rotation = _rotation - 360;
		if(_rotation < 0)
			_rotation = 360 + _rotation;
	}

	//accessors
	float  GetPosX(){return _posX;} const
	float  GetPosY(){return _posY;} const
	float  GetPosZ(){return _posZ;} const
	float  GetRotation(){return _rotation;} const
	float  GetSizeX(){return _sizeX;} const
	float  GetSizeY(){return _sizeY;} const
	float  GetSizeZ(){return _sizeZ;} const
	bool  GetCollidable(){return _collidable;} const
	float  GetOffsetSizeY(){return _offsetsizeY;} const

	//! get actif
	bool GetActif() const
	{return _actif;}

	// set actor size
	void SetSize(float  sizeX, float  sizeY, float  sizeZ)
	{
		_sizeX = sizeX;
		_sizeY = sizeY;
		_sizeZ = sizeZ;
		RefreshBB();
	}

	// set offset apply to the BB on Y
	void SetOffsetSizeY(float  osizeY)
	{
		_offsetsizeY = osizeY;
	}

	//! set passable state
	void SetPassable(bool passable);

	//! set depth mask
	void SetDepthMask(bool depthmask);

	//! set movable
	void SetMovable(bool movable)
	{_movable = movable;}

	//! set collidable
	void SetCollidable(bool collidable)
	{_collidable = collidable;}

	//! set actif
	void SetActif(bool actif)
	{_actif = actif;}

	//! set actor id
	void SetId(long id)
	{
		_ID = id;
		std::stringstream str;
		str<<"A"<<_ID;
		_sid = str.str();
	}

	//! set actor id
	void SetType(long type)
	{_Type = type;}

	//! SetRenderer
	void SetRenderer(D3ObjectRenderer* renderer);

	//! get structure
	short GetStructure(int X, int Y, int Z);


	//! activate an actor
	virtual bool Activate(float PlayerPosX, float PlayerPosY, float PlayerPosZ, float PlayerRotation,
								int actionType, bool DirectActivation=true)
	{return false;}

	//! check zone activation
	virtual int ActivateZone(float PlayerPosX, float PlayerPosY, float PlayerPosZ, float PlayerRotation,
								MainPlayerHandler  * _mph, bool DirectActivation=true)
	{return false;}

	//! called to reload element when resizing screen
	virtual void Reload();

	//! cleanup
	virtual void CleanUp();

	//! called on signal
	virtual bool OnSignal(long SignalNumber){return false;}

	//! set output signal
	void SetOutputSignal(long outputsignal, std::vector<long> targets);

	//! process activation
	virtual void ProcessActivation(float PlayerPosX, float PlayerPosY, float PlayerPosZ,
										float PlayerRotation){}

	//! process desactivation
	virtual void ProcessDesactivation(float PlayerPosX, float PlayerPosY, float PlayerPosZ,
											float PlayerRotation){}

	//! get actor type
	long GetId(){return _ID;}
	long GetType(){return _Type;}
	bool IsPassable(){return _passable;}
	bool IsMovable(){return _movable;}
	bool IsDepthMask(){return _depthmask;}
	long GetSignal(){return _outputsignal;}
	std::vector<long> GetTargets(){return _targets;}

	D3ObjectRenderer* GetRenderer(){return _Renderer;}

	void SetRendererType(long renderertype, const std::vector<long> &renderertarget)
	{
		_renderertype = renderertype;
		_renderertarget = renderertarget;
	}

	long GetRendererType()
	{return _renderertype;}

	std::vector<long> GetRendererTarget()
	{return _renderertarget;}

	//! render editor part
	virtual void RenderEditor();

	//! get the attached sound
	long GetAttachedSound()
	{return _attachedsound;}

	//! set the attached sound
	void SetAttachedSound(long V)
	{_attachedsound = V;}

	//! attach another actor to self
	void Attach(Actor * act);

	//! dettach another actor from self
	bool Dettach(Actor * act);

	//! check if the actor should be attached
	virtual bool CheckAttach(Actor * act)
	{return false;}

	//! check if the actor should be dettached
	virtual bool CheckDettach(Actor * act)
	{return false;}


	// set actor added velocity - used by attached object
	void SetAddedVelocity(float vX, float vY, float vZ)
	{
		_AddedVelocityX = vX;
		_AddedVelocityY = vY;
		_AddedVelocityZ = vZ;
	}

	 // added velocity accessors
	float  GetAddedvX(){return _AddedVelocityX;}
	float  GetAddedvY(){return _AddedVelocityY;}
	float  GetAddedvZ(){return _AddedVelocityZ;}


	//! send signal to targets
	void SendSignal(long signal, const std::vector<long> &targets, bool broadcast = false);

	//! set object used as signaler
	void SetSignaler(SignalerBase * signaler)
	{_signaler = signaler;}

	//! check if the actor need desactivation
	virtual bool NeedDesactivation(){return false;}

	//! get current actor state
	//! return false if the actor is stateless
	virtual bool Getstate(ActorStateInfo & currState)
	{return false;}

	//! set the actor state
	virtual void Setstate(const ActorStateInfo & currState){}

	//! if the actor needs to be updated client side
	virtual bool NeedsUpdate()
	{return false;}

	//! visible
	bool Visible()
	{ return _visible;}

	//! update actor to target a player
	virtual void UpdateTargetedActor(long playerid, bool target){}


	//! inform aborted activation
	virtual void InformActivationAborted(){}

	//! give actor bounding box
	const AABB &GetBoundingBox()
	{return _boundingbox;}

	//! give actor position
	VECTOR GetPosition();

	void SetPhysController(ActorPositionHandler * actph)
	{_physposhandler = actph;}
	ActorPositionHandler * GetPhysController()
	{ return _physposhandler;}


	//! called when the magic ball hit an actor
	//! return true if actor should be activated
	virtual bool MagicBallHit(long LauncherId){return false;}

	//! check if actor is moving
	bool IsMoving()
	{ return _actormoving;}

	//! set if actor is moving
	void SetMoving(bool moving)
	{_actormoving = moving;}

	// set if the main actor is attached
	void SetAttached(bool attached);

	// return true if the actor is attached
	bool IsAttached()
	{ return _isAttached;}

	// actor avtivate other actors
	virtual void ActorActivateActor(Actor * act) {}

protected:
	//! refresh bounding box
	void RefreshBB();


protected:
   long		_ID;						// entity unique ID
   std::string	_sid;
   long		_Type;						// entity type
   float	_posX;						// position on the map
   float	_posY;
   float	_posZ;
   float	_rotation;					// rotation
   float	_sizeX;						// bounding box
   float	_sizeY;
   float	_sizeZ;

   float	_offsetsizeY;

   AABB		_boundingbox;

   float	_VelocityX;					// velocity
   float	_VelocityY;
   float	_VelocityZ;

   float	_AddedVelocityX;			// added velocity
   float	_AddedVelocityY;
   float	_AddedVelocityZ;


	bool	_actif;
	bool	_passable;
	bool	_movable;
	bool	_depthmask;
	bool	_visible;
	bool	_collidable;
	bool	_actormoving;
	bool	_isAttached;

	// used when actor activate actor
	long		_activatingactor;


	long	_outputsignal;
	std::vector<long> _targets;

	long _renderertype;
	std::vector<long> _renderertarget;

	long			_attachedsound;

   D3ObjectRenderer* _Renderer;


   std::vector<Actor *>	_attachedActors;

   SignalerBase * _signaler;

   std::vector<std::pair<long, long> >	_actiontargets;

   ActorPositionHandler *	_physposhandler;
};

#endif