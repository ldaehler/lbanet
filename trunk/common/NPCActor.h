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


#if !defined(__LbaNetModel_1_NPCActor_h)
#define __LbaNetModel_1_NPCActor_h

#include "ScriptableActor.h"
#include "GameEvents.h"
#include <map>
#include "WorldInfo.h"

/***********************************************************************
 * Module:  LiftActor.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:53:50
 * Purpose: Declaration of the class Actor
 *********************************************************************/
class NPCActor : public ScriptableActor
{
public:
	//! constructor
	NPCActor(const std::vector<PlayerScriptPart> & scripts, bool IsLift, 
		int NPCType, float activationdistance, const std::string &Name, 
		const std::string &WelcomeSentence);

	//! destructor
	virtual ~NPCActor();

	//! do all check to be done when idle
	virtual int Process(double tnow, float tdiff);


	//! activate an actor
	virtual bool Activate(float PlayerPosX, float PlayerPosY, float PlayerPosZ, float PlayerRotation,
								int actionType, bool DirectActivation=true);


	//! check zone activation
	virtual int ActivateZone(float PlayerPosX, float PlayerPosY, float PlayerPosZ, float PlayerRotation,
								MainPlayerHandler  * _mph, bool DirectActivation=true);

	//!accessors
	float GetActivationDistance()
	{return _activationdistance;}
	void SetActivationDistance(float ad)
	{_activationdistance = ad;}
	void SetName(const std::string & name)
	{ _Name = name; }

	int GetNPCType()
	{ return _NPCType;}
	void SetNPCType()
	{ _NPCType = _NPCType;}
	std::string GetName()
	{return _Name;}

	//! update actor to target a player
	virtual void UpdateTargetedActor(long playerid, bool target);

	//! set trader item
	void SetItems(const std::map<long, TraderItem> &items)
	{ _items = items; }

	//! get trader items
	const std::map<long, TraderItem> &GetItems()
	{ return _items; }

protected:
	int		_NPCType;
	float	_activationdistance;

	bool	_activated;

	std::string _Name;

	std::string _WelcomeSentence;

	std::map<long, TraderItem>	_items;
};

#endif