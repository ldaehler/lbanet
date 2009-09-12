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

#if !defined(__LbaNetModel_1_EventHandler_h)
#define __LbaNetModel_1_EventHandler_h

class LbaNetEngine;
union SDL_Event;


class EventHandler
{
public:
	//! constructor
	EventHandler(LbaNetEngine* engine);

	//! destructor
	~EventHandler();


	// handle function
	bool Handle(SDL_Event flevent);


private:
	LbaNetEngine*		_lbaNetEngine;

	// remember the mouse position
	int			_current_mouse_x;
	int			_current_mouse_y;

	// remember if a mouse button is pressed
	bool		_button_1_pressed;
	bool		_button_2_pressed;
};

#endif