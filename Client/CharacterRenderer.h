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


#if !defined(__LbaNetModel_1_CharacterRenderer_h)
#define __LbaNetModel_1_CharacterRenderer_h

#include "3DObjectRenderer.h"
class LBA1ModelClass;

/***********************************************************************
 * Module:  CharacterRenderer.h
 * Author:  Vivien
 * Modified: vendredi 31 juillet 2009 19:51:32
 * Purpose: Declaration of the class CharacterRenderer
 ***********************************************************************/
class CharacterRenderer : public D3ObjectRenderer
{
public:
	//! constructor
	CharacterRenderer(float animationSpeed);

	//! destructor
	~CharacterRenderer();

	//! clear from memory
	void Clear();

	//! render object
	virtual void Render();

	//! render player directly with no check
	void BlankRender();

	//! do all check to be done when idle
	virtual int Process(double tnow, float tdiff);


	//! change the actor to display
	void changeAnimEntity(int entityNum, int bodyNum=0, bool forced=false);

	//! set the current animation to be played
	void setActorAnimation(int animNb);


	// set true if the actor should still display on the screen
	// even if he is hidden by walls
	void SetHiddenDisplay(bool Display)
	{_displayHidden = Display;}

	// accessors
	int GetModel()
	{ return _currModel;}
	int GetBody()
	{ return _currBody;}
	int GetAnimation()
	{ return _currAnimation;}
	int getKeyframe();

	//! set body color
	void SetBodyColor(short newColor);

	//! get body color
	short GetBodyColor();
protected:
	// translate anim number into correct animation depending of the entity
	int GetAnimNumberOfEntity(int anim);

private:
	int					_currModel;
	int					_currBody;
	int					_currAnimation;
	int					_translatedAnimation;
	bool				_displayHidden;
	float				_animationSpeed;

	short				_bodyColor;

	LBA1ModelClass *	_modelRenderer;
};

#endif