#if !defined(__LbaNetModel_1_ZoneActivableActor_h)
#define __LbaNetModel_1_ZoneActivableActor_h

#include "Actor.h"


/***********************************************************************
 * Module:  ZoneActivableActor.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:53:50
 * Purpose: Declaration of the class Actor
 *********************************************************************/
class ZoneActivableActor : public Actor
{
public:
	//! constructor
	ZoneActivableActor(float ZoneSizeX, float ZoneSizeY, float ZoneSizeZ);

	//! destructor
	virtual ~ZoneActivableActor();

	//! activate an actor
	virtual bool Activate(float PlayerPosX, float PlayerPosY, float PlayerPosZ, float PlayerRotation,
								bool DirectActivation=true);

	//! render editor
	void RenderEditor();

	//!accessors
	float GetZoneX()
	{return _ZoneSizeX;}
	float GetZoneY()
	{return _ZoneSizeY;}
	float GetZoneZ()
	{return _ZoneSizeZ;}

	void SetZoneX(float v)
	{_ZoneSizeX = v;}
	void SetZoneY(float v)
	{_ZoneSizeY = v;}
	void SetZoneZ(float v)
	{_ZoneSizeZ = v;}

private:
	float _ZoneSizeX;
	float _ZoneSizeY;
	float _ZoneSizeZ;
};

#endif