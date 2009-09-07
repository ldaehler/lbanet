#include "Player.h"
#include "CharacterRenderer.h"

/***********************************************************
	Constructor
***********************************************************/
Player::Player(float animationSpeed, bool MainPlayer)
: LivingActor(animationSpeed)
{
	if(MainPlayer && _Renderer)
		static_cast<CharacterRenderer *>(_Renderer)->SetHiddenDisplay(true);
}


/***********************************************************
	Destructor
***********************************************************/
Player::~Player()
{

}


/***********************************************************
	render object
***********************************************************/
void Player::Render(int RoomCut)
{
	LivingActor::Render(RoomCut);
}


/***********************************************************
do all check to be done when idle
***********************************************************/
int Player::Process(double tnow, float tdiff)
{
	return LivingActor::Process(tnow, tdiff);
}

