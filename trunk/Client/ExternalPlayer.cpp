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

#include "ExternalPlayer.h"
#include "ActorInfo.h"
#include "Player.h"

#include <IceUtil/Time.h>
#include "SynchronizedTimeHandler.h"

/***********************************************************
	Constructor
***********************************************************/
ExternalPlayer::ExternalPlayer(const LbaNet::ActorInfo & ainfo, float animationSpeed)
: _last_update(0), _renderer(NULL)
{
	_renderer = new Player(animationSpeed);

	Update(ainfo);
	_renderer->SetPosition(ainfo.X,  ainfo.Y, ainfo.Z);
	_renderer->SetRotation(ainfo.Rotation);

}

/***********************************************************
	Destructor
***********************************************************/
ExternalPlayer::~ExternalPlayer()
{

}


/***********************************************************
 update with external info
***********************************************************/
void ExternalPlayer::Update(const LbaNet::ActorInfo & ainfo)
{
	if(ainfo.Time > _last_update)
	{
		// update imediatly modifiable states
		_last_update = ainfo.Time;

		_renderer->DisplayName(ainfo.DisplayName);
		_renderer->SetName(ainfo.Name);

		if(ainfo.Visible)
			_renderer->Show();
		else
			_renderer->Hide();


		_renderer->changeAnimEntity(ainfo.Model, ainfo.Body);
		_renderer->setActorAnimation(ainfo.Animation);
		_renderer->SetBodyColor(ainfo.BodyColor);

		_velocityX = ainfo.vX;
		_velocityY = ainfo.vY;
		_velocityZ = ainfo.vZ;
		_velocityR = ainfo.vRotation;

		if(_velocityX == 0 && _velocityY == 0 && _velocityZ == 0)
			_renderer->SetPosition(ainfo.X,  ainfo.Y, ainfo.Z);

		if(_velocityR == 0)
			_renderer->SetRotation(ainfo.Rotation);

		// update dead reckon for the rest
		_dr.Set(SynchronizedTimeHandler::getInstance()->GetCurrentTimeDouble()/*ainfo.Time*/,
					ainfo.X,  ainfo.Y, ainfo.Z, ainfo.Rotation,
					ainfo.vX, ainfo.vY, ainfo.vZ, ainfo.vRotation);

		_renderer->SetNameColor(ainfo.NameR, ainfo.NameG, ainfo.NameB);
	}
}




/***********************************************************
 update with external info
***********************************************************/
void ExternalPlayer::UpdateLife(const LbaNet::ActorLifeInfo & ainfo)
{
	if(_renderer)
	{	
		_renderer->setCurrentLife(ainfo.CurrentLife);
		_renderer->setMaxLife(ainfo.MaxLife);
		_renderer->setCurrentMana(ainfo.CurrentMana);
		_renderer->setMaxMana(ainfo.MaxMana);
	}
}


/***********************************************************
do all check to be done when idle
***********************************************************/
int ExternalPlayer::Process(double tnow, float tdiff)
{

	// calculate prediction
	float predicted_posX = _renderer->GetPosX() + (_velocityX*tdiff);
	float predicted_posY = _renderer->GetPosY() + (_velocityY*tdiff);
	float predicted_posZ = _renderer->GetPosZ() + (_velocityZ*tdiff);
	float predicted_rotation = _renderer->GetRotation() + (_velocityR*tdiff);

	// calculate dead reckon
	_dr.Update(tnow);


	//// do interpolation X
	{
		float diffX = _dr._predicted_posX - predicted_posX;
		if(fabs(diffX) > 8)
			predicted_posX = _dr._predicted_posX;
		else
			predicted_posX += diffX / 40;
	}


	//// do interpolation Y
	{
		float diffY = _dr._predicted_posY - predicted_posY;
		if(fabs(diffY) > 8)
			predicted_posY = _dr._predicted_posY;
		else
			predicted_posY += diffY / 40;
	}


	//// do interpolation Z
	{
		float diffZ = _dr._predicted_posZ - predicted_posZ;
		if(fabs(diffZ) > 8)
			predicted_posZ = _dr._predicted_posZ;
		else
			predicted_posZ += diffZ / 40;
	}


	//// do interpolation rotation
	{
		float diffR = _dr._predicted_rotation - predicted_rotation;
		if(fabs(diffR) > 20)
			predicted_rotation = _dr._predicted_rotation;
		else
			predicted_rotation += diffR / 5;
	}

	if(predicted_posX != _renderer->GetPosX() || predicted_posY != _renderer->GetPosY()
			|| predicted_posZ != _renderer->GetPosZ())
	{
		_renderer->SetPosition(predicted_posX,  predicted_posY, predicted_posZ);
	}

	_renderer->SetRotation(predicted_rotation);


	return _renderer->Process(tnow, tdiff);
}


/***********************************************************
draw the object
***********************************************************/
void ExternalPlayer::draw(int RoomCut)
{
	_renderer->Render(RoomCut);
}






