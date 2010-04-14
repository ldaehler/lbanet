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


#ifndef __LBA_NET_PHYSICAL_MODIFICATION_H__
#define __LBA_NET_PHYSICAL_MODIFICATION_H__


#include "NxPhysics.h"
#include "PhysicalObjectHandlerBase.h"

#include <list>

/***********************************************************************
 * Module:  PhysXEngine.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:59:34
 * Purpose: base class for any Physical Modification
 ***********************************************************************/
class PhysicalModification
{
public:
	//! constructor
	PhysicalModification(unsigned int time)
		: _modtime(time){}

	//! destructor
	virtual ~PhysicalModification(){}


	//! comparison operator
	bool operator <(const PhysicalModification & tocompare)
	{
		return (_modtime < tocompare._modtime);
	}

	//! get time
	unsigned int GetTime()
	{return _modtime;}

	//! apply modification to physic engine
	virtual void Apply(float timeduration) = 0;

	//! check if same actor
	virtual bool SameActor(NxActor* actor) = 0;

	//! check if same character
	virtual bool SameCharacter(NxController* character) = 0;


protected:
	unsigned int _modtime;
};



/***********************************************************************
 * Module:  PhysXEngine.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:59:34
 * Purpose: class ActorPhysicalModification
 ***********************************************************************/
class ActorPhysicalModification : public PhysicalModification
{
public:
	//! constructor
	ActorPhysicalModification(NxActor* actor, unsigned int time)
		: PhysicalModification(time), _actor(actor){}

	//! destructor
	virtual ~ActorPhysicalModification(){}


	//! check if same actor
	virtual bool SameActor(NxActor* actor)
	{
		return (_actor == actor);
	}

	//! check if same character
	virtual bool SameCharacter(NxController* character)
	{
		return false;
	}


protected:
	NxActor* _actor;
};


/***********************************************************************
 * Module:  PhysXEngine.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:59:34
 * Purpose: class ActorPhysicalModification
 ***********************************************************************/
class CharacterPhysicalModification : public PhysicalModification
{
public:
	//! constructor
	CharacterPhysicalModification(NxController* controller, unsigned int time)
		: PhysicalModification(time), _controller(controller){}

	//! destructor
	virtual ~CharacterPhysicalModification(){}

	//! check if same actor
	virtual bool SameActor(NxActor* actor)
	{
		return false;
	}

	//! check if same character
	virtual bool SameCharacter(NxController* character)
	{
		return (_controller == character);
	}

	//! get user data pointer
	ActorUserData * GetuserData()
	{
		return (ActorUserData *) _controller->getActor()->userData;
	}

protected:
	NxController* _controller;
};



/***********************************************************************
 * Module:  PhysXEngine.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:59:34
 * Purpose: class MoveActorToModification
 ***********************************************************************/
class MoveActorToModification: public ActorPhysicalModification
{
public:
	//! constructor
	MoveActorToModification(unsigned int time, NxActor* act, const NxVec3 & targetPos)
		: ActorPhysicalModification(act, time), _targetPos(targetPos)
	{

	}

	//! destructor
	virtual ~MoveActorToModification(){}


	//! apply modification to physic engine
	virtual void Apply(float timeduration)
	{
		if(_actor)
			_actor->moveGlobalPosition(_targetPos*timeduration);
	}

protected:
	NxVec3 _targetPos;
};


/***********************************************************************
 * Module:  PhysXEngine.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:59:34
 * Purpose: class SetActorPosModification
 ***********************************************************************/
class SetActorPosModification: public ActorPhysicalModification
{
public:
	//! constructor
	SetActorPosModification(unsigned int time, NxActor* act, const NxVec3 & targetPos)
		: ActorPhysicalModification(act, time), _targetPos(targetPos)
	{

	}

	//! destructor
	virtual ~SetActorPosModification(){}


	//! apply modification to physic engine
	virtual void Apply(float timeduration)
	{
		if(_actor)
			_actor->setGlobalPosition(_targetPos);
	}

protected:
	NxVec3 _targetPos;
};

/***********************************************************************
 * Module:  PhysXEngine.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:59:34
 * Purpose: class SetActorRotationModification
 ***********************************************************************/
class SetActorRotationModification: public ActorPhysicalModification
{
public:
	//! constructor
	SetActorRotationModification(unsigned int time, NxActor* act, const NxQuat & quat)
		: ActorPhysicalModification(act, time), _targetRot(quat)
	{

	}

	//! destructor
	virtual ~SetActorRotationModification(){}


	//! apply modification to physic engine
	virtual void Apply(float timeduration)
	{
		if(_actor)
			_actor->setGlobalOrientationQuat(_targetRot);
	}

protected:
	NxQuat _targetRot;
};

/***********************************************************************
 * Module:  PhysXEngine.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:59:34
 * Purpose: class SetActorRotationModification
 ***********************************************************************/
class RotateActoryAxisModification: public ActorPhysicalModification
{
public:
	//! constructor
	RotateActoryAxisModification(unsigned int time, NxActor* act, float Speed)
		: ActorPhysicalModification(act, time), _Speed(Speed)
	{

	}

	//! destructor
	virtual ~RotateActoryAxisModification(){}


	//! apply modification to physic engine
	virtual void Apply(float timeduration)
	{
		if(_actor)
		{
			NxQuat q(_Speed*timeduration, NxVec3(0, 1, 0));
			NxQuat targetRot = _actor->getGlobalOrientationQuat();
			targetRot = q * targetRot;
			_actor->moveGlobalOrientationQuat(targetRot);
		}		
	}

protected:
	float _Speed;
};



/***********************************************************************
 * Module:  PhysXEngine.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:59:34
 * Purpose: class SetCharacterPositionModification
 ***********************************************************************/
class MoveInDirectionActorModification : public ActorPhysicalModification
{
public:
	//! constructor
	MoveInDirectionActorModification(unsigned int time, NxActor* act,  
										float MoveSpeed, LbaVec3 Gravity)
		: ActorPhysicalModification(act, time), _MoveSpeed(MoveSpeed), _Gravity(Gravity)
	{

	}

	//! destructor
	virtual ~MoveInDirectionActorModification(){}


	//! apply modification to physic engine
	virtual void Apply(float timeduration)
	{
		if(_actor)
		{
			NxQuat quat = _actor->getGlobalOrientationQuat();
			NxVec3 current_direction(0, 0, 1);
			quat.rotate(current_direction);
			
			NxVec3 tmpvec(	current_direction.x+_Gravity.x, 
							current_direction.y+_Gravity.y, 
							current_direction.z+_Gravity.z);

			_actor->moveGlobalPosition(tmpvec*timeduration);
		}
	}

protected:
	float _MoveSpeed;
	LbaVec3 _Gravity;
};








/***********************************************************************
 * Module:  PhysXEngine.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:59:34
 * Purpose: class DeltaMoveCharacterModification
 ***********************************************************************/
class DeltaMoveCharacterModification : public CharacterPhysicalModification
{
public:
	//! constructor
	DeltaMoveCharacterModification(unsigned int time, NxController* act, const NxVec3 & deltamove, 
																					bool checkCollision)
		: CharacterPhysicalModification(act, time), _move(deltamove), _checkCollision(checkCollision)
	{

	}

	//! destructor
	virtual ~DeltaMoveCharacterModification(){}


	//! apply modification to physic engine
	virtual void Apply(float timeduration)
	{
		if(_controller)
		{
			unsigned int CollisionFlag = PhysXEngine::MoveCharacter(_controller, _move*timeduration, _checkCollision);
			ActorUserData * udata = GetuserData();
			udata->CollisionUpFlag = (CollisionFlag == NXCC_COLLISION_UP);
			udata->CollisionDownFlag = (CollisionFlag == NXCC_COLLISION_DOWN);
			udata->CollisionSideFlag = (CollisionFlag == NXCC_COLLISION_SIDES);
		}
	}

protected:
	NxVec3 _move;
	bool _checkCollision;
};

/***********************************************************************
 * Module:  PhysXEngine.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:59:34
 * Purpose: class SetCharacterPositionModification
 ***********************************************************************/
class SetCharacterPositionModification : public CharacterPhysicalModification
{
public:
	//! constructor
	SetCharacterPositionModification(unsigned int time, NxController* act, const NxExtendedVec3 & targetPos)
		: CharacterPhysicalModification(act, time), _targetPos(targetPos)
	{

	}

	//! destructor
	virtual ~SetCharacterPositionModification(){}


	//! apply modification to physic engine
	virtual void Apply(float timeduration)
	{
		if(_controller)
			_controller->setPosition(_targetPos);
	}

protected:
	NxExtendedVec3 _targetPos;
};



/***********************************************************************
 * Module:  PhysXEngine.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:59:34
 * Purpose: class SetCharacterPositionModification
 ***********************************************************************/
class RotateCharacteryAxisModification : public CharacterPhysicalModification
{
public:
	//! constructor
	RotateCharacteryAxisModification(unsigned int time, NxController* act, float Speed)
		: CharacterPhysicalModification(act, time), _Speed(Speed)
	{

	}

	//! destructor
	virtual ~RotateCharacteryAxisModification(){}


	//! apply modification to physic engine
	virtual void Apply(float timeduration)
	{
		if(_controller)
		{
			ActorUserData * udata = GetuserData();
			if(udata && udata->RotH)
				udata->RotH->RotateYAxis(timeduration*_Speed);
		}
	}

protected:
	float _Speed;
};




/***********************************************************************
 * Module:  PhysXEngine.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:59:34
 * Purpose: class SetCharacterRotationModification
 ***********************************************************************/
class SetCharacterRotationModification : public CharacterPhysicalModification
{
public:
	//! constructor
	SetCharacterRotationModification(unsigned int time, NxController* act, const LbaQuaternion& Q)
		: CharacterPhysicalModification(act, time), _Q(Q)
	{

	}

	//! destructor
	virtual ~SetCharacterRotationModification(){}


	//! apply modification to physic engine
	virtual void Apply(float timeduration)
	{
		if(_controller)
		{
			ActorUserData * udata = GetuserData();
			if(udata && udata->RotH)
				udata->RotH->SetRotation(_Q);
		}
	}

protected:
	LbaQuaternion _Q;
};


/***********************************************************************
 * Module:  PhysXEngine.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:59:34
 * Purpose: class SetCharacterPositionModification
 ***********************************************************************/
class MoveInDirectionCharacterModification : public CharacterPhysicalModification
{
public:
	//! constructor
	MoveInDirectionCharacterModification(unsigned int time, NxController* act, 
											float MoveSpeed,	LbaVec3 Gravity)
		: CharacterPhysicalModification(act, time),  _MoveSpeed(MoveSpeed), _Gravity(Gravity)
	{

	}

	//! destructor
	virtual ~MoveInDirectionCharacterModification(){}


	//! apply modification to physic engine
	virtual void Apply(float timeduration)
	{
		if(_controller)
		{
			ActorUserData * udata = GetuserData();
			if(udata && udata->RotH)
			{
				LbaVec3 current_direction(0, 0, 0);
				current_direction = udata->RotH->GetDirection(LbaVec3(0, 0, 1));

				NxVec3 tmpvec(	current_direction.x+_Gravity.x, 
								current_direction.y+_Gravity.y, 
								current_direction.z+_Gravity.z);

				unsigned int CollisionFlag = PhysXEngine::MoveCharacter(_controller, tmpvec*timeduration, true);
				udata->CollisionUpFlag = (CollisionFlag == NXCC_COLLISION_UP);
				udata->CollisionDownFlag = (CollisionFlag == NXCC_COLLISION_DOWN);
				udata->CollisionSideFlag = (CollisionFlag == NXCC_COLLISION_SIDES);
			}
		}
	}

protected:
	float _MoveSpeed;
	LbaVec3 _Gravity;
};






/***********************************************************************
 * Module:  PhysXEngine.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:59:34
 * Purpose: store the saved state
 ***********************************************************************/
class SavedState
{
public:
	//! constructor
	SavedState()
	{}

	//! destructor
	~SavedState()
	{
	}

	//! add actor to save
	void AddActor(NxActor* actor)
	{
		//only save dynamic actors
		if(actor->isDynamic())
		{
			SavedActor sav;
			sav.Pose = actor->getGlobalPose();
			_savedActors[actor] = sav;
		}
	}

	//! add actor to save
	void AddCharacter(NxController* character)
	{
		SavedCharacter sav;
		sav.Position = character->getPosition();

		ActorUserData * udata = (ActorUserData *) character->getActor()->userData;
		if(udata && udata->RotH)
			udata->RotH->GetRotation(sav.Rotation);

		_savedChars[character] = sav;
	}

	//! check if same actor
	void RemoveActor(NxActor* actor)
	{
		std::map<NxActor*, SavedActor>::iterator it = _savedActors.find(actor);
		if(it != _savedActors.end())
			_savedActors.erase(it);
	}

	//! check if same character
	void RemoveCharacter(NxController* character)
	{
		std::map<NxController*, SavedCharacter>::iterator it = _savedChars.find(character);
		if(it != _savedChars.end())
			_savedChars.erase(it);
	}


	//! load back the saved state
	void Load()
	{
		//load actors
		{
			std::map<NxActor*, SavedActor>::iterator it = _savedActors.begin();
			std::map<NxActor*, SavedActor>::iterator end = _savedActors.end();
			for(; it != end; ++it)
			{
				//TODO only reset if different
				it->first->setGlobalPose(it->second.Pose);
			}
		}

		//load characters
		{
			std::map<NxController*, SavedCharacter>::iterator it = _savedChars.begin();
			std::map<NxController*, SavedCharacter>::iterator end = _savedChars.end();
			for(; it != end; ++it)
			{
				//TODO only reset if different
				it->first->setPosition(it->second.Position);

				ActorUserData * udata = (ActorUserData *) it->first->getActor()->userData;
				if(udata && udata->RotH)
					udata->RotH->SetRotation(it->second.Rotation);		
			}
		}
	}



	struct SavedActor
	{
		NxMat34 Pose;
	};

	struct SavedCharacter
	{
		NxExtendedVec3	Position;
		LbaQuaternion	Rotation;
	};

protected:
	std::map<NxActor*, SavedActor>			_savedActors;
	std::map<NxController*, SavedCharacter>	_savedChars;
};


/***********************************************************************
 * Module:  PhysXEngine.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:59:34
 * Purpose: store the physical state at a time t
 ***********************************************************************/
class PhysicalState
{
public:
	//! constructor
	PhysicalState(unsigned int time, float simulationduration)
		: _time(time), _simulationduration(simulationduration), _savedstate(NULL)
	{}

	//! destructor
	~PhysicalState()
	{
		ReleaseState();
	}

	//! get time
	unsigned int GetTime()
	{return _time;}

	//! get sim duration
	float GetSimDuration()
	{return _simulationduration;}


	//! add a modification to this state
	void AddModification(boost::shared_ptr<PhysicalModification> mod)
	{
		_modification.push_back(mod);
	}

	//! check if same actor
	void RemoveActor(NxActor* actor)
	{
		std::list<boost::shared_ptr<PhysicalModification> >::iterator it = _modification.begin();
		while(it != _modification.end())
		{
			if((*it)->SameActor(actor))
				it = _modification.erase(it);
			else
				++it;
		}
	}

	//! check if same character
	void RemoveCharacter(NxController* character)
	{
		std::list<boost::shared_ptr<PhysicalModification> >::iterator it = _modification.begin();
		while(it != _modification.end())
		{
			if((*it)->SameCharacter(character))
				it = _modification.erase(it);
			else
				++it;
		}
	}

	//! release saved state
	void ReleaseState()
	{
		if(_savedstate)
			 delete _savedstate;
	}


	//! set saved state
	void SetSavedState(SavedState * state)
	{
		ReleaseState();
		_savedstate = state;
	}

	//! get saved state
	SavedState * GetSavedState()
	{
		return _savedstate;
	}

	//! comparaison operator
	bool operator< (const PhysicalState& tocompare) const
	{
		return (_time < tocompare._time);
	}

	//! Apply the modification
	void ApplyModification(float timeduration)
	{
		std::list<boost::shared_ptr<PhysicalModification> >::iterator it =	_modification.begin();
		std::list<boost::shared_ptr<PhysicalModification> >::iterator end =	_modification.end();
		for(; it != end; ++it)
			(*it)->Apply(timeduration);
	}

protected:
	unsigned int _time;
	float _simulationduration;

	std::list<boost::shared_ptr<PhysicalModification> >	_modification;

	SavedState * _savedstate;
};


#endif
