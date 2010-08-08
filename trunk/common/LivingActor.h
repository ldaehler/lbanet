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


#if !defined(__LbaNetModel_1_LivingActor_h)
#define __LbaNetModel_1_LivingActor_h

#include "Actor.h"


/***********************************************************************
 * Module:  LivingActor.h
 * Author:  vivien
 * Modified: mercredi 15 juillet 2009 11:52:16
 * Purpose: Declaration of the class LivingActor
 ***********************************************************************/
class LivingActor : public Actor
{
public:
	//! constructor
	LivingActor(float animationSpeed);

	//! destructor
	virtual ~LivingActor();

	//! render object
	virtual void Render(int RoomCut);

	//! do all check to be done when idle
	virtual int Process(double tnow, float tdiff);

	// set the actor name
	void DisplayName(bool display)
	{ _DisplayName = display; }
	void SetName(const std::string & name)
	{ _Name = name; }

	//! change the actor to display
	void changeAnimEntity(int entityNum, int bodyNum);

	//! set the current animation to be played
	void setActorAnimation(int animNb);

	// accessors
	int GetModel();
	int GetBody();
	int GetAnimation();
	int getKeyframe();
	std::string GetName()
	{return _Name;}
	bool GetDisplayName()
	{return _DisplayName;}

	//! set the current keyframe
	void setKeyFrame(int keyframe);

	//! set body color
	void SetBodyColor(short newColor);

	//! get body color
	short GetBodyColor();

	
	//! life mana accessors
	float GetCurrentLife()
	{return _CurrentLife;}
	float GetMaxLife()
	{return _MaxLife;}

	float GetCurrentMana()
	{return _CurrentMana;}
	float GetMaxMana()
	{return _MaxMana;}

	void setCurrentLife(float x)
	{_CurrentLife = (x<0?0:x);}
	void setMaxLife(float x)
	{_MaxLife = x;}

	void setCurrentMana(float x)
	{_CurrentMana = (x<0?0:x);}
	void setMaxMana(float x)
	{_MaxMana = x;}


	void SetNameColor(int R, int G, int B)
	{
		_nameR = R;
		_nameG = G;
		_nameB = B;
	}

	int GetNameR()
	{return _nameR;}

	int GetNameG()
	{return _nameG;}

	int GetNameB()
	{return _nameB;}

	//! get speed associated with renderer
	double GetRendererSpeed();

	// return speed Yasociated with animation
	double GetRendererSpeedY();

	//! is animation finished
	bool IsAnimationFinished();


protected:
	std::string		_Name;
	bool			_DisplayName;
	float			_MaxLife;
	float			_MaxMana;
	float			_CurrentLife;
	float			_CurrentMana;

	int				_nameR;
	int				_nameG;
	int				_nameB;

	// get the place where to draw the text
	float GetTextHeight();
};

#endif