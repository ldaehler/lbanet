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

#if !defined(__LbaNetModel_1_LbaNetEngine_h)
#define __LbaNetModel_1_LbaNetEngine_h


#include <string>

#include <boost/shared_ptr.hpp>
#include "LbaNetModel.h"
#include "CharacterController.h"
#include "GuiHandler.h"

class PhysXEngine;
class EventHandler;

/***********************************************************************
 * Module:  LbaNetEngine.h
 * Author:  vivien
 * Modified: mardi 14 juillet 2009 17:41:03
 * Purpose: Declaration of the class LbaNetEngine
 ***********************************************************************/
class LbaNetEngine
{
public:
	//!constructor
	LbaNetEngine();

	//!destructor
   ~LbaNetEngine();

	//! entry point of the engine
	void run(void);


	//! start a move from keyboard input
	void StartMove(int MoveType);

	//! stop a move from keyboard input
	void StopMove(int MoveType);

	//! do action from keyboard input
	void DoAction();


protected:
	//! process function
	void Process(void);

	//! initialize the class
	void Initialize(void);

private:
	GuiHandler						m_gui_handler;
	LbaNetModel						m_lbaNetModel;		// game model
	boost::shared_ptr<EventHandler>	m_eventHandler;		// handle input events
	boost::shared_ptr<PhysXEngine>	m_physic_engine;	//physic engine

	boost::shared_ptr<CharacterController>	m_controller;

	double							m_lasttime;


};

#endif