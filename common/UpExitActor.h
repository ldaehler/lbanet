#if !defined(__LbaNetModel_1_UpExitActor_h)
#define __LbaNetModel_1_UpExitActor_h

#include "ActivableActor.h"


/***********************************************************************
 * Module:  ExitActor.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:53:50
 * Purpose: Declaration of the class Actor
 *********************************************************************/
class UpExitActor : public ActivableActor
{
public:
	//! constructor
	UpExitActor(float activationdistance, int direction);

	//! destructor
	virtual ~UpExitActor();

	//! accessors
	int GetDir()
	{return _direction;}

	void SetDir(int dir)
	{_direction = dir;}

protected:
	//! process activation
	virtual void ProcessActivation(float PlayerPosX, float PlayerPosY, float PlayerPosZ, float PlayerRotation);

private:
	int _direction;
};

#endif