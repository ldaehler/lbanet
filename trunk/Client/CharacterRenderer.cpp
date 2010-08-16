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

#include "CharacterRenderer.h"
#include "LBA1ModelClass.h"
#include "DataFileHandler.h"
#include "DataLoader.h"
#include "LogHandler.h"

#include <windows.h>    // Header File For Windows
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#include <GL/glu.h>     // Header File For The GLu32 Library

#include <math.h>




/***********************************************************
	Constructor
***********************************************************/
CharacterRenderer::CharacterRenderer(float animationSpeed)
: _modelRenderer(NULL),  _translatedAnimation(-1),
	_currAnimation(-1), _currModel(-1), _currBody(-1), _displayHidden(false),
	_animationSpeed(animationSpeed), _bodyColor(-1), _animationfinished(false)
{
	_type = 3;
}

/***********************************************************
	Destructor
***********************************************************/
CharacterRenderer::~CharacterRenderer()
{
	delete _modelRenderer;
}


/***********************************************************
clear from memory
***********************************************************/
void CharacterRenderer::Clear()
{
	if(_modelRenderer)
		delete _modelRenderer;

	_modelRenderer = NULL;
}


/***********************************************************
draw the object
***********************************************************/
void CharacterRenderer::Render(float alpha)
{
	if(!_modelRenderer || !Visible)
		return;

	if(_currModel < 0 || _currAnimation < 0)
		return;


    glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_TEXTURE_2D);

	glPushMatrix();
	{
		glScalef(32, 32, 32);

		if(_displayHidden)
		{
			glDisable(GL_DEPTH_TEST);
			_modelRenderer->RenderizeModel(50);
		}

		glEnable(GL_DEPTH_TEST);
		_modelRenderer->RenderizeModel(255);
	}
	glPopMatrix();
}



/***********************************************************
render player directly with no check
***********************************************************/
void CharacterRenderer::BlankRender()
{
	if(!_modelRenderer)
		return;

    glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_TEXTURE_2D);

	glPushMatrix();
	{
		glScalef(40, 40, 40);
		glDisable(GL_DEPTH_TEST);
		_modelRenderer->RenderizeModel(255);
	}
	glPopMatrix();
}



/***********************************************************
	void Actor::changeAnimEntity(int entityNum)
***********************************************************/
void CharacterRenderer::changeAnimEntity(int entityNum, int bodyNum, bool forced)
{
	if(!forced && (_currModel == entityNum) && (_currBody == bodyNum))
		return;

	_currModel = entityNum;
	_currBody = bodyNum;
	_currAnimation = -1;
	_translatedAnimation = -1;

	Clear();


	std::stringstream strs;
	strs<<"Loading new lba1 model with entity "<<entityNum<<" and body "<<bodyNum<<std::endl;
	LogHandler::getInstance()->LogToFile(strs.str());


	entitiesTableStruct*  estruct = DataLoader::getInstance()->GetEntitiesInfo();
	_modelRenderer = new LBA1ModelClass(estruct, DataFileHandler::GetPath("BODY"),
										DataFileHandler::GetPath("ANIM"), entityNum,
										estruct->entitiesTable[_currModel].bodyList[_currBody].body);

	if(_bodyColor >= 0)
	{
		short oldc = 66;
		if(_currBody == 1)
			oldc = 176;
		if(_currBody == 2)
			oldc = 243;
		_modelRenderer->changeAllPolygonColors(oldc, _bodyColor);
	}

	setActorAnimation(0);

	LogHandler::getInstance()->LogToFile("Model loaded correctly");
}


/***********************************************************
set the current animation to be played
***********************************************************/
void CharacterRenderer::setActorAnimation(int animNb, bool forced)
{
	if(!_modelRenderer)
		return;

	if(_currModel < 0)
		return;

	int tanim = GetAnimNumberOfEntity(animNb);
	if(!forced && _translatedAnimation == tanim)
		return;


	entitiesTableStruct*  estruct = DataLoader::getInstance()->GetEntitiesInfo();
	if(tanim < 0 || tanim >= estruct->entitiesTable[_currModel].numOfAnims)
		tanim = 0;

	_currAnimation = animNb;
	_translatedAnimation = tanim;
	_animationfinished = false;


	_modelRenderer->LoadAnim(estruct,
		estruct->entitiesTable[_currModel].animList[_translatedAnimation].index);


	for(int k = 1; k<=_modelRenderer->getNumKeyFrames(); ++k)
		_modelRenderer->setKeyframeLength(k, (short)(_modelRenderer->getKeyframeLength(k)*_animationSpeed));
}


/***********************************************************
do all check to be done when idle
***********************************************************/
int CharacterRenderer::Process(double tnow, float tdiff)
{
	if(!_modelRenderer)
		return 0;

	if(_currModel < 0 || _currAnimation < 0)
		return 0;

	_animationfinished = _modelRenderer->AnimateModel();

	if(_animationfinished)
		return 1;	// return 1 if animation is terminated
	else
		return 0;	// return 0 else
}


/***********************************************************
translate anim number into correct animation depending of the entity
***********************************************************/
int CharacterRenderer::GetAnimNumberOfEntity(int anim)
{
	if(_currModel == 64)
		return 1;

	if(_currModel == 47)
	{
		if(anim == 1)
			return 6;

		if(anim == 3)
			return 4;

		if(anim == 4)
			return 5;
	}


	if(_currModel == 4)
	{
		if(anim == 2)
			return 1;

		if(anim == 3)
			return 2;

		if(anim == 4)
			return 3;
	}

	return anim;
}


/***********************************************************
get current key frame
***********************************************************/
int CharacterRenderer::getKeyframe()
{
	if(_modelRenderer)
		return _modelRenderer->getKeyframe();

	return 0;
}

/***********************************************************
set the current keyframe
***********************************************************/
void CharacterRenderer::setKeyFrame(int keyframe)
{
	if(_modelRenderer)
		_modelRenderer->setAtKeyFrame(keyframe, false);
}



/***********************************************************
set body color
***********************************************************/
void CharacterRenderer::SetBodyColor(short newColor)
{
	if(_bodyColor != newColor)
	{
		_bodyColor = newColor;
		int tmpa = _currAnimation;
		changeAnimEntity(_currModel, _currBody, true);
		setActorAnimation(tmpa);
	}
}

/***********************************************************
get body color
***********************************************************/
short CharacterRenderer::GetBodyColor()
{
	return _bodyColor;
}



/***********************************************************
return speed asociated with animation
***********************************************************/
double CharacterRenderer::GetCurrentSpeed()
{
	if(!_modelRenderer)
		return 0;

	return _modelRenderer->GetCurrentSpeed();
}

/***********************************************************
return speed asociated with animation
***********************************************************/
double CharacterRenderer::GetCurrentSpeedY()
{
	if(!_modelRenderer)
		return 0;

	return _modelRenderer->GetCurrentSpeedY();
}
