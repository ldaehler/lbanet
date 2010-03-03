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

#include "LbaNetModel.h"
#include "averages.h"
#include "EventHandler.h"


#include <string>



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

	// update camera functions
	void DeltaUpdateZoom(bool ZoomIn);
	void DeltaUpdateZenit(double zenit);

	// player moves
	void PlayerStartMove(int moveDirection);
	void PlayerStopMove(int moveDirection);
	void PlayerDoAction(bool ForcedNormalAction);

protected:


	//! redraw the scene on screen
	void Redraw(void);

	//! calculate time left until next frame
	long TimeLeft(void);

	//! initialize the class
	void Initialize(void);

	void ResetScreen();

private:

	LbaNetModel							m_lbaNetModel;				// game model
	EventHandler						m_eventHandler;

	SDL_Surface *						m_screen;			// pointer to the screen surface

	bool								m_fullscreen;		// flag recording if we are in fullscreen or windowed mode
	int									m_screen_size_X;	// display X size
	int									m_screen_size_Y;	// display Y size

	// used to calculate FPS
	EMA									m_currframetime;
	double								m_lasttime;
	bool								m_DisplayFPS;
};

#endif