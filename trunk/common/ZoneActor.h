#if !defined(__LbaNetModel_1_ZoneActor_h)
#define __LbaNetModel_1_ZoneActor_h

#include "Actor.h"

/***********************************************************************
 * Module:  ZoneActor.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:53:50
 * Purpose: Declaration of the class Actor
 *********************************************************************/
class ZoneActor : public Actor
{
public:
	//! constructor
	ZoneActor(float zoneSizeX, float zoneSizeY, float zoneSizeZ);

	//! destructor
	virtual ~ZoneActor();

	//! check zone activation
	virtual int ActivateZone(float PlayerPosX, float PlayerPosY, float PlayerPosZ, float PlayerRotation,
								bool DirectActivation=true);

	//! render editor part
	virtual void RenderEditor();

	//! accessors
	float GetZoneX()
	{return _zoneSizeX;}
	float GetZoneY()
	{return _zoneSizeY;}
	float GetZoneZ()
	{return _zoneSizeZ;}

	void SetZoneX(float zx)
	{_zoneSizeX = zx;}
	void SetZoneY(float zy)
	{_zoneSizeY = zy;}
	void SetZoneZ(float zz)
	{_zoneSizeZ = zz;}

	//! check if the actor need desactivation
	virtual bool NeedDesactivation(){return true;}
	
protected:
	// used to get the zone center - depend of the actor type
	virtual float GetZoneCenterX(){return _posX;}
	virtual float GetZoneCenterY(){return _posY;}
	virtual float GetZoneCenterZ(){return _posZ;}

private:
	float	_zoneSizeX;
	float	_zoneSizeY;
	float	_zoneSizeZ;

	bool	_activated;
};

#endif