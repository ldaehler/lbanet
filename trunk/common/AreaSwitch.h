#if !defined(__LbaNetModel_1_AreaSwitch_h)
#define __LbaNetModel_1_AreaSwitch_h

#include "ZoneActor.h"

/***********************************************************************
 * Module:  DoorActor.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:53:50
 * Purpose: Declaration of the class Actor
 *********************************************************************/
class AreaSwitch : public ZoneActor
{
public:
	//! constructor
	AreaSwitch(float zoneSizeX, float zoneSizeY, float zoneSizeZ);

	//! destructor
	virtual ~AreaSwitch();

protected:
	//! process zone activation
	virtual void ProcessActivation(float PlayerPosX, float PlayerPosY, float PlayerPosZ, 
		float PlayerRotation);
	
	//! process zone desactivation
	virtual void ProcessDesactivation(float PlayerPosX, float PlayerPosY, float PlayerPosZ, 
		float PlayerRotation);


};

#endif