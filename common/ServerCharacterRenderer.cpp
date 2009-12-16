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

#include "ServerCharacterRenderer.h"
#include "ServerLBA1ModelClass.h"
#include "DataFileHandler.h"


#include <math.h>


entitiesTableStruct* ServerCharacterRenderer::_estruct = NULL;

/***********************************************************
	get entity info
***********************************************************/
entitiesTableStruct* ServerCharacterRenderer::GetEntitiesInfo()
{
	if(_estruct == NULL)
		_estruct = parseEntities(DataFileHandler::GetPath("FILE3D"));

	return _estruct;
}

/***********************************************************
	Constructor
***********************************************************/
ServerCharacterRenderer::ServerCharacterRenderer(float animationSpeed)
: _modelRenderer(NULL),  _translatedAnimation(-1),
	_currAnimation(-1), _currModel(-1), _currBody(-1), _displayHidden(false),
	_animationSpeed(animationSpeed), _bodyColor(-1)
{
	_type = 3;
}

/***********************************************************
	Destructor
***********************************************************/
ServerCharacterRenderer::~ServerCharacterRenderer()
{
	delete _modelRenderer;
}


/***********************************************************
clear from memory
***********************************************************/
void ServerCharacterRenderer::Clear()
{
	if(_modelRenderer)
		delete _modelRenderer;

	_modelRenderer = NULL;
}


/***********************************************************
draw the object
***********************************************************/
void ServerCharacterRenderer::Render()
{

}



/***********************************************************
render player directly with no check
***********************************************************/
void ServerCharacterRenderer::BlankRender()
{

}



/***********************************************************
	void Actor::changeAnimEntity(int entityNum)
***********************************************************/
void ServerCharacterRenderer::changeAnimEntity(int entityNum, int bodyNum, bool forced)
{
	if(!forced && (_currModel == entityNum) && (_currBody == bodyNum))
		return;

	_currModel = entityNum;
	_currBody = bodyNum;
	_currAnimation = -1;
	_translatedAnimation = -1;

	Clear();

	entitiesTableStruct*  estruct = GetEntitiesInfo();
	_modelRenderer = new ServerLBA1ModelClass(estruct, DataFileHandler::GetPath("BODY"),
										DataFileHandler::GetPath("ANIM"), entityNum,
										estruct->entitiesTable[_currModel].bodyList[_currBody].body);

}


/***********************************************************
set the current animation to be played
***********************************************************/
void ServerCharacterRenderer::setActorAnimation(int animNb)
{
	if(!_modelRenderer)
		return;

	int tanim = GetAnimNumberOfEntity(animNb);
	if(_translatedAnimation == tanim)
		return;

	_currAnimation = animNb;
	_translatedAnimation = tanim;

	entitiesTableStruct*  estruct = GetEntitiesInfo();
	_modelRenderer->LoadAnim(estruct,
		estruct->entitiesTable[_currModel].animList[_translatedAnimation].index);


	for(int k = 1; k<=_modelRenderer->getNumKeyFrames(); ++k)
		_modelRenderer->setKeyframeLength(k, (short)(_modelRenderer->getKeyframeLength(k)*_animationSpeed));
}


/***********************************************************
do all check to be done when idle
***********************************************************/
int ServerCharacterRenderer::Process(double tnow, float tdiff)
{
	if(!_modelRenderer)
		return 0;

	bool animTerminated = _modelRenderer->AnimateModel();

	if(animTerminated)
		return 1;	// return 1 if animation is terminated
	else
		return 0;	// return 0 else
}


/***********************************************************
translate anim number into correct animation depending of the entity
***********************************************************/
int ServerCharacterRenderer::GetAnimNumberOfEntity(int anim)
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
int ServerCharacterRenderer::getKeyframe()
{
	if(_modelRenderer)
		return _modelRenderer->getKeyframe();

	return 0;
}



/***********************************************************
set body color
***********************************************************/
void ServerCharacterRenderer::SetBodyColor(short newColor)
{

}

/***********************************************************
get body color
***********************************************************/
short ServerCharacterRenderer::GetBodyColor()
{
	return 0;
}

