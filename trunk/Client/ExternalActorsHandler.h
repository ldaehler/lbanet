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

#if !defined(__LbaNetModel_1_ExternalActorsHandler_h)
#define __LbaNetModel_1_ExternalActorsHandler_h

class Actor;
class PhysicHandler;
#include <vector>
#include <map>
struct ActorStateInfo;
class MainPlayerHandler;

/***********************************************************************
 * Module:  ExternalActorsHandler.h
 * Author:  vivien
 * Modified: Monday, July 27, 2009 2:53:58 PM
 * Purpose: Declaration of the class LocalActorsHandler
 ***********************************************************************/
class ExternalActorsHandler
{
public:
	//! constructor
	ExternalActorsHandler();

	//! destructor
	~ExternalActorsHandler();

	//! render object
	void Render(int RoomCut);

	//! do all check to be done when idle
	int Process(double tnow, float tdiff);

	//! cleanup allocated memory
	void Cleanup();

	//! set actors
	void SetActors(const std::map<long, Actor *> & actors);

	//! get structure
	short GetStructure(int X, int Y, int Z);

	//! move movable objects
	void Move(int X, int Y, int Z, float MoveX, float MoveZ);

	//! set physic handler
	void SetPhysic(PhysicHandler * pH)
	{_pH = pH;}

	//! try to activate actors
	bool Activate(float PlayerPosX, float PlayerPosY, float PlayerPosZ, float PlayerRotation, int actionType);

	//! check zone activation
	bool ActivateZone(float PlayerPosX, float PlayerPosY, float PlayerPosZ, float PlayerRotation, 
							MainPlayerHandler  * _mph);

	//! called to reload element when resizing screen
	void Reload();

	//! cleanup
	void CleanUp();

	// called when a signal has been generated
	void SignalEvent(long signal, const std::vector<long> &targets);

	// get a pointer to the actors
	std::map<long, Actor *> * GetActors(){return &_actors;}

	//! render editor part
	void RenderEditor();

	//! check if the actor should be attached
	bool CheckAttach(Actor * act);

	//! check if the actor should be dettached
	bool CheckDettach(Actor * act);

	//! update actor state
	void UpdateActorStates(const std::vector<ActorStateInfo> & newstate);

	// attach player to actor
	void ForcedAttach(Actor * act, long actorId);

	// dettach player from actor
	void ForcedDettach(Actor * act, long actorId);

protected:
	std::map<long, Actor *> _actors;
	PhysicHandler * _pH;

	bool _needupdate;

};

#endif