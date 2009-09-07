#if !defined(__LbaNetModel_1_LiftActor_h)
#define __LbaNetModel_1_LiftActor_h

#include "Actor.h"
#include "GameEvents.h"

/***********************************************************************
 * Module:  LiftActor.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:53:50
 * Purpose: Declaration of the class Actor
 *********************************************************************/
class LiftActor : public Actor
{
public:
	//! constructor
	LiftActor(const std::vector<PlayerScriptPart> & scripts);

	//! destructor
	virtual ~LiftActor();

	//! do all check to be done when idle
	virtual int Process(double tnow, float tdiff); 

	//! called on signal
	virtual bool OnSignal(long SignalNumber);

	//! accessor
	std::vector<PlayerScriptPart> & GetScripts()
	{return _scripts;}


	//! check if the actor should be attached
	virtual bool CheckAttach(Actor * act);

	//! check if the actor should be dettached
	virtual bool CheckDettach(Actor * act);

	//! get current actor state
	//! return false if the actor is stateless
	virtual bool Getstate(ActorStateInfo & currState);

	//! set the actor state
	virtual void Setstate(const ActorStateInfo & currState);

protected:
	std::vector<PlayerScriptPart>	_scripts;
	size_t							_curr_script_position;

	std::vector<long>				_receivedsignals;
	bool							_started_timer;
	double							_timer_start_time;

	int								_playedsound;
	unsigned long					_playingsound;
};

#endif