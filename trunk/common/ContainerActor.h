#if !defined(__LbaNetModel_1_ContainerActor_h)
#define __LbaNetModel_1_ContainerActor_h

#include "ZoneActivableActor.h"


/***********************************************************************
 * Module:  ActivableActor.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:53:50
 * Purpose: Declaration of the class Actor
 *********************************************************************/
class ContainerActor : public ZoneActivableActor
{
public:
	//! constructor
	ContainerActor(float ZoneSizeX, float ZoneSizeY, float ZoneSizeZ);

	//! destructor
	virtual ~ContainerActor();


	//! process zone activation
	virtual void ProcessActivation(float PlayerPosX, float PlayerPosY, float PlayerPosZ, 
		float PlayerRotation);
};

#endif