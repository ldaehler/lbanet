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


class ServerConnectionHandler;
struct SDL_Surface;

#include "EventHandler.h"
#include "GuiHandler.h"

#include <string>
#include <boost/shared_ptr.hpp>



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

	//! change the screen resolution
   void ChangeScreen(int screenX, int screenY, bool fullscreen);

   //! change from windowed to fullscreen and vice versa
   void ChangeFullScreen();

	// display a specific gui
	void DisplayGUI(int guinumber);



protected:
	//! process function
	bool Process(void);

	//! redraw the scene on screen
	void Redraw(void);

	//! calculate time left until next frame
	long TimeLeft(void);

	//! initialize the class
	void Initialize(void);

	//! create a new screen surface
	void ResetScreen();
	void ChangeScreenAndLinkedRessources();


private:
	boost::shared_ptr<EventHandler> 	m_eventHandler;				// event handler
	GuiHandler							m_guiHandler;				// gui handler

	SDL_Surface *						m_screen;			// pointer to the screen surface

	bool								m_fullscreen;		// flag recording if we are in fullscreen or windowed mode
	int									m_screen_size_X;	// display X size
	int									m_screen_size_Y;	// display Y size



};

#endif