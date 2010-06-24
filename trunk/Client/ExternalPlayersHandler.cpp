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

#include "ExternalPlayersHandler.h"
#include <cstdlib>
#include <climits>

#include "ActorInfo.h"
#include "ThreadSafeWorkpile.h"
#include "GameEvents.h"

/***********************************************************
constructor
***********************************************************/
ExternalPlayersHandler::ExternalPlayersHandler(float animationSpeed)
:  _animationSpeed(animationSpeed)
{

}


/***********************************************************
destructor
***********************************************************/
ExternalPlayersHandler::~ExternalPlayersHandler()
{
	ResetActors("");
}




/***********************************************************
draw function
***********************************************************/
void ExternalPlayersHandler::draw(int RoomCut)
{
	std::map<long, ExternalPlayer *>::iterator it = _actors.begin();
	std::map<long, ExternalPlayer *>::iterator end = _actors.end();

	for(;it!=end; ++it)
		it->second->draw(RoomCut);
}



/***********************************************************
animate actors
***********************************************************/
int ExternalPlayersHandler::Process(double tnow, float tdiff)
{
	// update from external info
	ThreadSafeWorkpile * wp = ThreadSafeWorkpile::getInstance();
	std::vector<LbaNet::ActorInfo> vecai;
	std::vector<long> vecq;
	std::vector<LbaNet::ActorLifeInfo> veclai;
	std::vector<std::pair<long, LbaNet::LaunchInfo> > mb_vec;

	wp->GetExtActorUpdate(vecai);
	wp->GetQuittedActors(vecq);
	wp->GetExtActorLifeUpdate(veclai);
	wp->GetMagicBallPlayed(mb_vec);

	for(size_t i=0; i<vecai.size(); ++i)
		UpdateActor(vecai[i]);
	for(size_t i=0; i<veclai.size(); ++i)
		UpdateLifeActor(veclai[i]);
	for(size_t i=0; i<vecq.size(); ++i)
		RemoveActor(vecq[i]);
	for(size_t i=0; i<mb_vec.size(); ++i)
		MagicBallPlayed(mb_vec[i].first, mb_vec[i].second);


	// update actors spped and animation
	std::map<long, ExternalPlayer *>::iterator it = _actors.begin();
	std::map<long, ExternalPlayer *>::iterator end = _actors.end();

	for(;it!=end; ++it)
		it->second->Process(tnow, tdiff);

	return 0;
}


/***********************************************************
if actor already ther - update information
else add actor to the list
**********************************************************/
void ExternalPlayersHandler::UpdateActor(const LbaNet::ActorInfo & ai)
{
	if(ai.MapName != _mapName)
		return;

	if(ai.ActorId == ThreadSafeWorkpile::getInstance()->GetPlayerId())
		return;

	std::map<long, ExternalPlayer *>::iterator it = _actors.find(ai.ActorId);
	if(it != _actors.end())
	{
		it->second->Update(ai);
	}
	else
	{
		ExternalPlayer * act = new ExternalPlayer(ai, _animationSpeed);
		_actors.insert(std::pair<long, ExternalPlayer *>(ai.ActorId,act));
	}
}



/***********************************************************
if actor already ther - update infroamtion
else add actor to the list
**********************************************************/
void ExternalPlayersHandler::UpdateLifeActor(const LbaNet::ActorLifeInfo & ai)
{
	if(ai.ActorId == ThreadSafeWorkpile::getInstance()->GetPlayerId())
	{
		ThreadSafeWorkpile::getInstance()->AddEvent(new PlayerLifeChangedEvent(ai.CurrentLife, 
															ai.MaxLife, ai.CurrentMana, ai.MaxMana,
															ai.ShouldHurt));
	}
	else
	{
		std::map<long, ExternalPlayer *>::iterator it = _actors.find(ai.ActorId);
		if(it != _actors.end())
			it->second->UpdateLife(ai);
	}
}


/***********************************************************
remove the actor from the list if present
**********************************************************/
void ExternalPlayersHandler::RemoveActor(long ActorId)
{
	if(ActorId == ThreadSafeWorkpile::getInstance()->GetPlayerId())
		return;

	std::map<long, ExternalPlayer *>::iterator it = _actors.find(ActorId);
	if(it != _actors.end())
	{
		delete it->second;
		_actors.erase(it);
	}
}



/***********************************************************
reset all actors - typically called when changing map
**********************************************************/
void ExternalPlayersHandler::ResetActors(const std::string &newmapName)
{
	std::map<long, ExternalPlayer *>::iterator it = _actors.begin();
	std::map<long, ExternalPlayer *>::iterator end = _actors.begin();

	for(;it!=end; ++it)
		delete it->second;

	_actors.clear();


	_mapName = newmapName;
}



/***********************************************************
get player from id
**********************************************************/
Player * ExternalPlayersHandler::GetPlayer(long playerid)
{
	std::map<long, ExternalPlayer *>::iterator it = _actors.find(playerid);
	if(it != _actors.end())
	{
		return it->second->GetPlayer();
	}

	return NULL;
}


/***********************************************************
Magic Ball Played
**********************************************************/
void ExternalPlayersHandler::MagicBallPlayed(long playerid, const LbaNet::LaunchInfo & linfo)
{
	std::map<long, ExternalPlayer *>::iterator it = _actors.find(playerid);
	if(it != _actors.end())
		it->second->MagicBallPlayed(linfo);
}

