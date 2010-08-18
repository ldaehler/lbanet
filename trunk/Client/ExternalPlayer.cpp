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
#include "ThreadSafeWorkpile.h"
#include "GameEvents.h"

#include "PhysXPhysicHandler.h"
#include "PhysXEngine.h"
#include "NxVec3.h"
#include "NxActor.h"
#include "Actor.h"
#include "DataLoader.h"
#include "SpriteRenderer.h"
#include "LogHandler.h"


/***********************************************************
	Constructor
***********************************************************/
ExternalPlayer::ExternalPlayer(const LbaNet::ActorInfo & ainfo, float animationSpeed)
: _last_update(0), _renderer(NULL), _magicballH(false), _physH(NULL), _usdata(NULL)
{
	_renderer = new Player(animationSpeed);

	Update(ainfo);
	_renderer->SetPosition(ainfo.X,  ainfo.Y, ainfo.Z);
	_renderer->SetRotation(ainfo.Rotation);

	float posx = _renderer->GetPosX();
	float posy = _renderer->GetPosY();
	float posz = _renderer->GetPosZ();
	float sizex = _renderer->GetSizeX();
	float sizey = _renderer->GetSizeY();
	float sizez = _renderer->GetSizeZ();

	if(sizex > 0)
	{
		sizey /= 2;
		posy += sizey;

		// add physique info
		_usdata = new ActorUserData(3, ainfo.ActorId, NULL);
		_physH = PhysXEngine::getInstance()->CreateBox(NxVec3(posx,  posy, posz), 
															sizex, sizey, sizez, 1.0, 2, _usdata, false);
	}

	_renderer->SetPhysController(new ActorPositionHandler(_physH, ainfo.X,  ainfo.Y, ainfo.Z));
	_magicballH.SetOwner(_renderer);

	_renderer->SetPosition(ainfo.X,  ainfo.Y, ainfo.Z);
}

/***********************************************************
	Destructor
***********************************************************/
ExternalPlayer::~ExternalPlayer()
{
	LogHandler::getInstance()->LogToFile("Destroying external player...");

	if(_physH)
		PhysXEngine::getInstance()->DestroyActor(_physH);

	if(_usdata)
		delete _usdata;

	{
		std::map<int, Actor *>::iterator itm = _ghosts.begin();
		std::map<int, Actor *>::iterator endm = _ghosts.end();
		for(; itm != endm; ++itm)
			delete itm->second;
	}

	if(_renderer)
		delete _renderer;

	LogHandler::getInstance()->LogToFile("External player destroyed.");
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

		_renderer->SetSize(ainfo.SizeX, ainfo.SizeY, ainfo.SizeZ); 

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
	_magicballH.Process();

	{
		std::map<int, Actor *>::iterator itm = _ghosts.begin();
		std::map<int, Actor *>::iterator endm = _ghosts.end();
		for(; itm != endm; ++itm)
			itm->second->Process(tnow, tdiff);
	}
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
	_magicballH.Render();

	{
		std::map<int, Actor *>::iterator itm = _ghosts.begin();
		std::map<int, Actor *>::iterator endm = _ghosts.end();
		for(; itm != endm; ++itm)
			itm->second->Render(RoomCut);
	}
}


/***********************************************************
magic ball played
***********************************************************/
void ExternalPlayer::MagicBallPlayed(const LbaNet::LaunchInfo & linfo)
{
	_magicballH.Launch(linfo.PosX, linfo.PosY, linfo.PosZ, linfo.DirX, linfo.DirZ, 
						linfo.Mode, linfo.Enoughmana, _physH);
}



/***********************************************************
magic ball played
***********************************************************/
void ExternalPlayer::MagicBallComeback()
{
	_magicballH.MagicBallComeback();
}







/***********************************************************
 update with external info
***********************************************************/
void ExternalPlayer::UpdateGhost(const LbaNet::GhostActorInfo & ainfo)
{
	std::map<int, Actor *>::iterator it = _ghosts.find(ainfo.GhostId);
	if(it == _ghosts.end())
	{
		//create new actor
		Actor * tmp = new Actor(0.6);
		tmp->SetRenderer(DataLoader ::getInstance()->CreateSpriteRenderer(ainfo.SpriteId));

		tmp->SetPosition(ainfo.X, ainfo.Y, ainfo.Z);
		tmp->SetRotation(ainfo.Rotation);

		// add missing attached
		for(size_t i=0; i<ainfo.AttachedToActors.size(); ++i)
			ThreadSafeWorkpile::getInstance()->AddEvent(new AttachActorToActorEvent(_renderer, ainfo.AttachedToActors[i]));

		if(ainfo.AttachedToPlayer)
			_renderer->Attach(tmp);

		_ghosts[ainfo.GhostId] = tmp;
	}
	else
	{
		//update existing actor
		Actor * tmp = it->second;
		tmp->SetPosition(ainfo.X, ainfo.Y, ainfo.Z);
		tmp->SetRotation(ainfo.Rotation);

		const std::vector<Actor *> & attachingacts = tmp->GetAttaching();

		// add missing attached
		for(size_t i=0; i<ainfo.AttachedToActors.size(); ++i)
		{
			bool found = false;
			for(size_t j=0; j<attachingacts.size(); ++j)
			{
				if(	attachingacts[j]->GetId() == ainfo.AttachedToActors[i])
				{
					found = true;
					break;
				}
			}	

			if(!found)
				ThreadSafeWorkpile::getInstance()->AddEvent(new AttachActorToActorEvent(tmp, ainfo.AttachedToActors[i]));
		}

		// remove old attached
		for(size_t j=0; j<attachingacts.size(); ++j)
		{
			bool found = false;
			for(size_t i=0; i<ainfo.AttachedToActors.size(); ++i)
			{
				if(	attachingacts[j]->GetId() == ainfo.AttachedToActors[i])
				{
					found = true;
					break;
				}
			}
			
			if(!found)
				tmp->RemoveAttaching(attachingacts[j]);
		}

		if(ainfo.AttachedToPlayer)
			_renderer->Attach(tmp);
		else
			_renderer->Dettach(tmp);
	}	
}   
