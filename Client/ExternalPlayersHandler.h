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

#ifndef _LBANET_EXTERNAL_ACT_HANDLER_H_
#define _LBANET_EXTERNAL_ACT_HANDLER_H_

#include "ExternalPlayer.h"

#include <map>
#include <string>

namespace LbaNet
{
	struct ActorInfo;
}




//*************************************************************************************************
//*                               class ExternalPlayersHandler
//*************************************************************************************************
/**
* @brief Class taking care of displaying actors from external source
*
*/
class ExternalPlayersHandler
{
public:

	// constructor
	ExternalPlayersHandler(float animationSpeed);

	// destructor
	~ExternalPlayersHandler();

	// draw the object
    virtual void draw(int RoomCut);

	// animate actors
	virtual int Process(double tnow, float tdiff);

	// if actor already ther - update infroamtion
	// else add actor to the list
	void UpdateActor(const LbaNet::ActorInfo & ai);

	// remove the actor from the list if present
	void RemoveActor(long ActorId);

	// reset all actors - typically called when changing map
	void ResetActors(const std::string &newmapName);

	// if actor already ther - update infroamtion
	// else add actor to the list
	void UpdateLifeActor(const LbaNet::ActorLifeInfo & ai);

	//! get player from id
	Player * GetPlayer(long playerid);

protected:
	void MagicBallPlayed(long playerid, const LbaNet::LaunchInfo & linfo);

private:
	std::map<long, ExternalPlayer *>	_actors;
	std::string							_mapName;
	float								_animationSpeed;
};


#endif
