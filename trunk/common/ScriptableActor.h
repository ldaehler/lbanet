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


#if !defined(__LbaNetModel_1_ScriptableActor_h)
#define __LbaNetModel_1_ScriptableActor_h

#include "Actor.h"
#include "GameEvents.h"

/***********************************************************************
 * Module:  LiftActor.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:53:50
 * Purpose: Declaration of the class Actor
 *********************************************************************/
class ScriptableActor : public Actor
{
public:
	//! constructor
	ScriptableActor(const std::vector<PlayerScriptPart> & scripts, bool IsLift);

	//! destructor
	virtual ~ScriptableActor();

	//! do all check to be done when idle
	virtual int Process(double tnow, float tdiff);

	//! called on signal
	virtual bool OnSignal(long SignalNumber);

	//! accessor
	std::vector<PlayerScriptPart> & GetScripts()
	{return _scripts;}

	//! get current actor state
	//! return false if the actor is stateless
	virtual bool Getstate(ActorStateInfo & currState);

	//! set the actor state
	virtual void Setstate(const ActorStateInfo & currState);

	//! check if the actor should be attached
	virtual bool CheckAttach(Actor * act);

	//! check if the actor should be dettached
	virtual bool CheckDettach(Actor * act);

	//! if the actor needs to be updated client side
	virtual bool NeedsUpdate();

protected:
	//! increase script position
	void IncreaseScriptPosition();

protected:
	std::vector<PlayerScriptPart>	_scripts;
	size_t							_curr_script_position;

	std::vector<long>				_receivedsignals;
	bool							_started_timer;
	double							_timer_start_time;

	int								_playedsound;
	unsigned long					_playingsound;

	bool							_IsLift;
	bool							_needs_update;
};

#endif