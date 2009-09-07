#if !defined(__LbaNetModel_1_ActivableActor_h)
#define __LbaNetModel_1_ActivableActor_h

#include "Actor.h"


/***********************************************************************
 * Module:  ActivableActor.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:53:50
 * Purpose: Declaration of the class Actor
 *********************************************************************/
class ActivableActor : public Actor
{
public:
	//! constructor
	ActivableActor(float activationdistance);

	//! destructor
	virtual ~ActivableActor();

	//! activate an actor
	virtual bool Activate(float PlayerPosX, float PlayerPosY, float PlayerPosZ, float PlayerRotation,
								bool DirectActivation=true);

	//!accessors
	float GetActivationDistance()
	{return _activationdistance;}

	void SetActivationDistance(float ad)
	{_activationdistance = ad;}

private:
	float _activationdistance;
};

#endif