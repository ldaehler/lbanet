/*
------------------------[ Lbanet Source ]-------------------------
Copyright (C) 2009
Author: Vivien Delage [Rincevent_123]
Email : vdelage@gmail.com

-------------------------------[ GNU License ]-------------------------------

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

-----------------------------------------------------------------------------
*/


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



/***********************************************************
render player directly with no check
***********************************************************/
void Player::BlankRender()
{
	if(_Renderer)
		static_cast<CharacterRenderer *>(_Renderer)->BlankRender();
}

