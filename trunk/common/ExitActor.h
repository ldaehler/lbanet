#if !defined(__LbaNetModel_1_ExitActor_h)
#define __LbaNetModel_1_ExitActor_h

#include "ActivableActor.h"


/***********************************************************************
 * Module:  ExitActor.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:53:50
 * Purpose: Declaration of the class Actor
 *********************************************************************/
class ExitActor : public ActivableActor
{
public:
	//! constructor
	ExitActor(float activationdistance, float deltaX, float deltaY, float deltaZ, int direction);

	//! destructor
	virtual ~ExitActor();

	//! accessors
	float GetDX()
	{return _deltaX;}
	float GetDY()
	{return _deltaY;}
	float GetDZ()
	{return _deltaZ;}
	int GetDir()
	{return _direction;}

	void SetDX(float dx)
	{_deltaX = dx;}
	void SetDY(float dy)
	{_deltaY = dy;}
	void SetDZ(float dz)
	{_deltaZ = dz;}
	void SetDir(int dir)
	{_direction = dir;}

protected:
	//! process activation
	virtual void ProcessActivation(float PlayerPosX, float PlayerPosY, float PlayerPosZ, float PlayerRotation);

private:
	float _deltaX;
	float _deltaY;
	float _deltaZ;
	int _direction;
};

#endif