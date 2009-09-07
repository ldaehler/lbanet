#if !defined(__LbaNetModel_1_SwitchActor_h)
#define __LbaNetModel_1_SwitchActor_h

#include "ActivableActor.h"


/***********************************************************************
 * Module:  TextActor.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:53:50
 * Purpose: Declaration of the class SwitchActor
 *********************************************************************/
class SwitchActor : public ActivableActor
{
public:
	//! constructor
	SwitchActor(float activationdistance);

	//! destructor
	virtual ~SwitchActor();

	//! called on signal
	virtual bool OnSignal(long SignalNumber);

	//! get current actor state
	//! return false if the actor is stateless
	virtual bool Getstate(ActorStateInfo & currState);

	//! set the actor state
	virtual void Setstate(const ActorStateInfo & currState);

protected:
	//! process activation
	virtual void ProcessActivation(float PlayerPosX, float PlayerPosY, float PlayerPosZ, float PlayerRotation);

	bool	_switchOn;

};

#endif