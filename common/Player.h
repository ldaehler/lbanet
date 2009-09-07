#if !defined(__LbaNetModel_1_Player_h)
#define __LbaNetModel_1_Player_h

#include "LivingActor.h"


/***********************************************************************
 * Module:  Player.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:54:54
 * Purpose: Declaration of the class Player
 ***********************************************************************/
class Player : public LivingActor
{
public:
	//! constructor
	Player(float animationSpeed, bool MainPlayer = false);

	//! destructor
	~Player();

	//! render object
	virtual void Render(int RoomCut);

	//! do all check to be done when idle
	virtual int Process(double tnow, float tdiff); 

private:

};

#endif