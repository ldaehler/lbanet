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


class PhysXEngine;
class PhysicalObjectHandlerBase;

/***********************************************************************
 * Module:  CharacterController.h
 * Author:  vivien
 * Modified: mardi 14 juillet 2009 17:41:03
 * Purpose: Declaration of the class CharacterController
 ***********************************************************************/
class CharacterController
{
public:
	//! constructor
	CharacterController(boost::shared_ptr<PhysXEngine> pEngine);

	//! destructor
	~CharacterController();

	//! set character to control
	void SetCharacter(boost::shared_ptr<PhysicalObjectHandlerBase> charac, bool AsGhost=false);


	//! process function
	void Process(unsigned int tnow, float tdiff);

	//! apply inputs
	void ApplyInputs(const Input & in);

private:
	boost::shared_ptr<PhysicalObjectHandlerBase> _character;
	boost::shared_ptr<PhysXEngine>	_pEngine;

	bool			_isGhost;

	Input			_lastInputs;

	LbaVec3			_current_direction;
};

#endif