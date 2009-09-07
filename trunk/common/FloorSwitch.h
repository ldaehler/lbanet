#if !defined(__LbaNetModel_1_FloorSwitch_h)
#define __LbaNetModel_1_FloorSwitch_h

#include "ZoneActivableActor.h"


/***********************************************************************
 * Module:  ActivableActor.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:53:50
 * Purpose: Declaration of the class Actor
 *********************************************************************/
class FloorSwitch : public ZoneActivableActor
{
public:
	//! constructor
	FloorSwitch(float ZoneSizeX, float ZoneSizeY, float ZoneSizeZ);

	//! destructor
	virtual ~FloorSwitch();


	//! process zone activation
	virtual void ProcessActivation(float PlayerPosX, float PlayerPosY, float PlayerPosZ, 
		float PlayerRotation);

	//! called on signal
	virtual bool OnSignal(long SignalNumber);

};

#endif