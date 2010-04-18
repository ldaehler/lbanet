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

#include "CharacterControllerLBA.h"
#include "PhysicalObjectHandlerBase.h"
#include "SynchronizedTimeHandler.h"
#include "PhysXEngine.h"

#include <iostream>

/***********************************************************
	Constructor
***********************************************************/
CharacterController::CharacterController()
: _isGhost(false), _lastinputime(0), _currentit(_storedinputs.end())
{

}


/***********************************************************
	Destructor
***********************************************************/
CharacterController::~CharacterController()
{

}


/***********************************************************
	Set character to control
***********************************************************/
void CharacterController::SetPhysicalCharacter(boost::shared_ptr<PhysicalObjectHandlerBase> charac, bool AsGhost)
{
	_character = charac;
	_isGhost = AsGhost;

	boost::shared_ptr<ActorUserData> udata = _character->GetUserData();
	if(udata)
		udata->Callback = this;
}


/***********************************************************
process the given input
***********************************************************/
void CharacterController::ProcessInput(unsigned int time, const Input & in)
{
	if(!_character)
		return;


	if(_isGhost)
	{
		float speedX = 0.0f;
		float speedY = 0.0f;
		float speedZ = 0.0f;

		//if right key pressed
		if(in.right)
			speedX = 10.0f;

		//if left key pressed
		if(in.left)
			speedX = -10.0f;

		//if up key pressed
		if(in.up)
			speedZ = -10.0f;

		//if down key pressed
		if(in.down)
			speedZ = 10.0f;

		_character->Move(time, speedX, speedY, speedZ, true);
	}
	else
	{
		//if right key pressed
		if(in.right)
			_character->RotateYAxis(time, -100.0f, true);

		//if left key pressed
		if(in.left)
			_character->RotateYAxis(time, 100.0f, true);

		//if up/down key
		float speed = 0.0f;
		if(in.up)
			speed = 5.0f;
		else if(in.down)
			speed = -5.0f;

		_character->MoveInDirection(time, speed, true, true);
	}
}


/************************************************************************/
/* called when we get new friend in list                                   
/************************************************************************/
void CharacterController::inputUpdated(unsigned int time, const Input & newinput)
{
	_storedinputs[time] = newinput;

	if(time > _lastinputime)
	{
		_lastinputime = time;
		_lastinput = newinput;
	}

	//delete old inputs
	unsigned int currentime = SynchronizedTimeHandler::getInstance()->GetCurrentTimeSync();


	std::map<unsigned int, Input>::iterator it = _storedinputs.begin();
	while(it != _storedinputs.end())
	{
		if((currentime - it->first) > MAX_HISTORY_TIME)
		{
			std::map<unsigned int, Input>::iterator ittmp = it;
			++ittmp;
			if(ittmp != _storedinputs.end())
			{
				if((currentime - ittmp->first) > MAX_HISTORY_TIME)
				{
					it = _storedinputs.erase(it);
					continue;
				}
			}
		}
		
		break;
	}

	_currentit = _storedinputs.begin();
}



/************************************************************************/
/* apply stored input for a specific time period                                
/************************************************************************/
void CharacterController::applyInput(unsigned int timeleftborder, unsigned int timerightborder)
{
	for(;_currentit != _storedinputs.end(); ++_currentit)
	{
		if(_currentit->first >= timeleftborder)
		{
			if(_currentit->first < timerightborder)
			{
				ProcessInput(timeleftborder+1, _currentit->second);
			}
			else
			{
				--_currentit;
				if(_currentit != _storedinputs.end())
				{
					ProcessInput(timeleftborder+1, _currentit->second);
				}
			}

			return;
		}	
	}

	//use the last input if exist
	std::map<unsigned int, Input>::reverse_iterator	ittmp = _storedinputs.rbegin();
	if(ittmp != _storedinputs.rend())
		ProcessInput(timeleftborder+1, ittmp->second);
	else
		ProcessInput(timeleftborder+1, Input());
}


/************************************************************************/
/* apply last character move                       
/************************************************************************/
void CharacterController::applyLastMove()
{
	ProcessInput(SynchronizedTimeHandler::getInstance()->GetCurrentTimeSync(), _lastinput);
}


/************************************************************************/
/* reset input iterator at each cycle                        
/************************************************************************/
void CharacterController::resetIterator()
{
	_currentit = _storedinputs.begin();
}
