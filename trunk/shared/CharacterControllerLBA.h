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

#if !defined(__LbaNetModel_1_CharacterController_h)
#define __LbaNetModel_1_CharacterController_h

#include <boost/shared_ptr.hpp>
#include "CommonTypes.h"
#include "PlayerCallbackBase.h"

#include <map>

class PhysicalObjectHandlerBase;


/***********************************************************************
 * Module:  CharacterController.h
 * Author:  vivien
 * Modified: mardi 14 juillet 2009 17:41:03
 * Purpose: Declaration of the class CharacterController
 ***********************************************************************/
class CharacterController : public PlayerCallbackBase
{
public:
	//! constructor
	CharacterController();

	//! destructor
	~CharacterController();

	// called when we get new friend in list
	virtual void inputUpdated(unsigned int time, const Input & newinput);

	// capply stored input for a specific time period
	virtual void applyInput(unsigned int timeleftborder, unsigned int timerightborder);

	// set physcial character
	virtual void SetPhysicalCharacter(boost::shared_ptr<PhysicalObjectHandlerBase> charac, bool AsGhost=false);

	// reset input iterator at each cycle
	virtual void resetIterator();

	// apply last character move
	virtual void applyLastMove();

protected:
	//! process the given input
	void ProcessInput(unsigned int time, const Input & in);


private:
	boost::shared_ptr<PhysicalObjectHandlerBase> _character;

	bool			_isGhost;

	std::map<unsigned int, Input>				_storedinputs;
	std::map<unsigned int, Input>::iterator		_currentit;

	Input										_lastinput;
	unsigned int								_lastinputime;
};

#endif