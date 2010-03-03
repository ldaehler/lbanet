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

#include "LbaNetEngine.h"
#include "EventHandler.h"


#ifdef _WIN32
	#include "SDL.h"
#else
	#include <SDL/SDL.h>
#endif





/***********************************************************
	Constructor
***********************************************************/
EventHandler::EventHandler(LbaNetEngine* engine)
: _lbaNetEngine(engine), _button_1_pressed(false), _button_2_pressed(false)
{

}


/***********************************************************
	Destructor
***********************************************************/
EventHandler::~EventHandler()
{

}



/***********************************************************
handle function
***********************************************************/
bool EventHandler::Handle(SDL_Event flevent)
{
	switch( flevent.type )
	{
		//***** mouse events
        case SDL_MOUSEBUTTONDOWN:	// a mouse button has been pushed


			if(flevent.button.button == 4)	// mouse wheel up
				_lbaNetEngine->DeltaUpdateZoom(false);

			if(flevent.button.button == 5)	// mouse wheel down
				_lbaNetEngine->DeltaUpdateZoom(true);

			_button_2_pressed = (flevent.button.button == 3);
			if(_button_2_pressed)
				_current_mouse_y = flevent.button.y;
		break;

		case SDL_MOUSEBUTTONUP:	// a mouse button has been pushed

			if(flevent.button.button == 3)
				_button_2_pressed = false;
		break;

		case SDL_MOUSEMOTION:	// the user move the mouse with a button activated

			if(_button_2_pressed)
			{
				_lbaNetEngine->DeltaUpdateZenit(flevent.motion.y-_current_mouse_y);
				_current_mouse_y = flevent.motion.y;
			}
		break;


		//***** keyboard events
		case SDL_KEYDOWN:	// in case of keyboard key down

			switch (flevent.key.keysym.sym)
			{
				case SDLK_UP:
					_lbaNetEngine->PlayerStartMove(1);
				break;

				case SDLK_DOWN:
					_lbaNetEngine->PlayerStartMove(2);
				break;

				case SDLK_LEFT:
					_lbaNetEngine->PlayerStartMove(3);
				break;

				case SDLK_RIGHT:
					_lbaNetEngine->PlayerStartMove(4);
				break;


				case SDLK_ESCAPE:
					return true;
				break;


				case SDLK_SPACE:
					_lbaNetEngine->PlayerDoAction(false);
					break;

			}
		break;


		case SDL_KEYUP:	// in case of keyboard key down

			switch (flevent.key.keysym.sym)
			{
				case SDLK_UP:
					_lbaNetEngine->PlayerStopMove(1);
				break;

				case SDLK_DOWN:
					_lbaNetEngine->PlayerStopMove(2);
				break;

				case SDLK_LEFT:
					_lbaNetEngine->PlayerStopMove(3);
				break;

				case SDLK_RIGHT:
					_lbaNetEngine->PlayerStopMove(4);
				break;
			}
		break;


		// SDL_QUIT event (window close)
		case SDL_QUIT:
		{
			return true;
		}
	}

	return false;
}