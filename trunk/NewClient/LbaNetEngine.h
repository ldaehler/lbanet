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
#include "GuiHandler.h"

class PhysXEngine;
class EventHandler;
class ChatClient;

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
	LbaNetEngine(ChatClient* Chatcl);

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

	// display a specific gui
	void DisplayGUI(int guinumber);

	//connection callback
	void ConnectionCallback(int SuccessFlag, const std::string & reason);

	// focus the chatbox
	void FocusChatbox(bool focus);


protected:
	//! process function
	void Process(void);

	//! initialize the class
	void Initialize(void);

	//! called to check for game events and handle them
	void HandleGameEvents();

	//! try to login to the server
	void TryLogin(const std::string &Name, const std::string &Passwordl);

	//! switch gui helpers
	void SwitchGuiToLogin();
	void SwitchGuiToChooseWorld();
	void SwitchGuiToGame();
	void SwitchGuiToMenu();
	void SwitchGuiToOption();

	// exit current active gui
	void ExitGui();

	// change the world
	void ChangeWorld(const std::string & NewWorld);

	// called to play the assigned music when menu
	void PlayMenuMusic();


private:
	GuiHandler						m_gui_handler;		// pointer on gui class
	LbaNetModel						m_lbaNetModel;		// game model
	boost::shared_ptr<EventHandler>	m_eventHandler;		// handle input events
	boost::shared_ptr<PhysXEngine>	m_physic_engine;	//physic engine
	ChatClient*						m_Chatcl;			//chat client


	// game states
	enum EngineState {ELogin=0, EChoosingWorld, EGaming, EMenu, EOption };
	EngineState							m_currentstate;
	EngineState							m_oldstate;

	// last music played
	std::string							m_lastmusic;
};

#endif