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

#include "ThreadSafeWorkpile.h"
#include "LbaNetEngine.h"
#include "EventHandler.h"
#include "ThreadSafeWorkpile.h"

#ifdef _WIN32
	#include "SDL.h"
#else
	#include <SDL/SDL.h>
#endif

#include <CEGUI.h>



 /************************************************************************
     Translate a SDLKey to the proper CEGUI::Key
 *************************************************************************/
 CEGUI::uint SDLKeyToCEGUIKey(SDLKey key)
 {
     using namespace CEGUI;
     switch (key)
     {
     case SDLK_BACKSPACE:    return Key::Backspace;
     case SDLK_TAB:          return Key::Tab;
     case SDLK_RETURN:       return Key::Return;
     case SDLK_PAUSE:        return Key::Pause;
     case SDLK_ESCAPE:       return Key::Escape;
     case SDLK_SPACE:        return Key::Space;
     case SDLK_COMMA:        return Key::Comma;
     case SDLK_MINUS:        return Key::Minus;
     case SDLK_PERIOD:       return Key::Period;
     case SDLK_SLASH:        return Key::Slash;
     case SDLK_0:            return Key::Zero;
     case SDLK_1:            return Key::One;
     case SDLK_2:            return Key::Two;
     case SDLK_3:            return Key::Three;
     case SDLK_4:            return Key::Four;
     case SDLK_5:            return Key::Five;
     case SDLK_6:            return Key::Six;
     case SDLK_7:            return Key::Seven;
     case SDLK_8:            return Key::Eight;
     case SDLK_9:            return Key::Nine;
     case SDLK_COLON:        return Key::Colon;
     case SDLK_SEMICOLON:    return Key::Semicolon;
     case SDLK_EQUALS:       return Key::Equals;
     case SDLK_LEFTBRACKET:  return Key::LeftBracket;
     case SDLK_BACKSLASH:    return Key::Backslash;
     case SDLK_RIGHTBRACKET: return Key::RightBracket;
     case SDLK_a:            return Key::A;
     case SDLK_b:            return Key::B;
     case SDLK_c:            return Key::C;
     case SDLK_d:            return Key::D;
     case SDLK_e:            return Key::E;
     case SDLK_f:            return Key::F;
     case SDLK_g:            return Key::G;
     case SDLK_h:            return Key::H;
     case SDLK_i:            return Key::I;
     case SDLK_j:            return Key::J;
     case SDLK_k:            return Key::K;
     case SDLK_l:            return Key::L;
     case SDLK_m:            return Key::M;
     case SDLK_n:            return Key::N;
     case SDLK_o:            return Key::O;
     case SDLK_p:            return Key::P;
     case SDLK_q:            return Key::Q;
     case SDLK_r:            return Key::R;
     case SDLK_s:            return Key::S;
     case SDLK_t:            return Key::T;
     case SDLK_u:            return Key::U;
     case SDLK_v:            return Key::V;
     case SDLK_w:            return Key::W;
     case SDLK_x:            return Key::X;
     case SDLK_y:            return Key::Y;
     case SDLK_z:            return Key::Z;
     case SDLK_DELETE:       return Key::Delete;
     case SDLK_KP0:          return Key::Numpad0;
     case SDLK_KP1:          return Key::Numpad1;
     case SDLK_KP2:          return Key::Numpad2;
     case SDLK_KP3:          return Key::Numpad3;
     case SDLK_KP4:          return Key::Numpad4;
     case SDLK_KP5:          return Key::Numpad5;
     case SDLK_KP6:          return Key::Numpad6;
     case SDLK_KP7:          return Key::Numpad7;
     case SDLK_KP8:          return Key::Numpad8;
     case SDLK_KP9:          return Key::Numpad9;
     case SDLK_KP_PERIOD:    return Key::Decimal;
     case SDLK_KP_DIVIDE:    return Key::Divide;
     case SDLK_KP_MULTIPLY:  return Key::Multiply;
     case SDLK_KP_MINUS:     return Key::Subtract;
     case SDLK_KP_PLUS:      return Key::Add;
     case SDLK_KP_ENTER:     return Key::NumpadEnter;
     case SDLK_KP_EQUALS:    return Key::NumpadEquals;
     case SDLK_UP:           return Key::ArrowUp;
     case SDLK_DOWN:         return Key::ArrowDown;
     case SDLK_RIGHT:        return Key::ArrowRight;
     case SDLK_LEFT:         return Key::ArrowLeft;
     case SDLK_INSERT:       return Key::Insert;
     case SDLK_HOME:         return Key::Home;
     case SDLK_END:          return Key::End;
     case SDLK_PAGEUP:       return Key::PageUp;
     case SDLK_PAGEDOWN:     return Key::PageDown;
     case SDLK_F1:           return Key::F1;
     case SDLK_F2:           return Key::F2;
     case SDLK_F3:           return Key::F3;
     case SDLK_F4:           return Key::F4;
     case SDLK_F5:           return Key::F5;
     case SDLK_F6:           return Key::F6;
     case SDLK_F7:           return Key::F7;
     case SDLK_F8:           return Key::F8;
     case SDLK_F9:           return Key::F9;
     case SDLK_F10:          return Key::F10;
     case SDLK_F11:          return Key::F11;
     case SDLK_F12:          return Key::F12;
     case SDLK_F13:          return Key::F13;
     case SDLK_F14:          return Key::F14;
     case SDLK_F15:          return Key::F15;
     case SDLK_NUMLOCK:      return Key::NumLock;
     case SDLK_SCROLLOCK:    return Key::ScrollLock;
     case SDLK_RSHIFT:       return Key::RightShift;
     case SDLK_LSHIFT:       return Key::LeftShift;
     case SDLK_RCTRL:        return Key::RightControl;
     case SDLK_LCTRL:        return Key::LeftControl;
     case SDLK_RALT:         return Key::RightAlt;
     case SDLK_LALT:         return Key::LeftAlt;
     case SDLK_LSUPER:       return Key::LeftWindows;
     case SDLK_RSUPER:       return Key::RightWindows;
     case SDLK_SYSREQ:       return Key::SysRq;
     case SDLK_MENU:         return Key::AppMenu;
     case SDLK_POWER:        return Key::Power;
     default:                return 0;
     }
     return 0;
 }



bool handle_mouse_down(Uint8 button)
{
	switch ( button )
	{
		case SDL_BUTTON_LEFT:
			return CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::LeftButton);
			break;
		case SDL_BUTTON_MIDDLE:
			return CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::MiddleButton);
			break;
		case SDL_BUTTON_RIGHT:
			return CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::RightButton);
			break;

		case SDL_BUTTON_WHEELDOWN:
			return CEGUI::System::getSingleton().injectMouseWheelChange( -1 );
			break;
		case SDL_BUTTON_WHEELUP:
			return CEGUI::System::getSingleton().injectMouseWheelChange( +1 );
			break;
	}

	return false;
}


bool handle_mouse_up(Uint8 button)
{
	switch ( button )
	{
		case SDL_BUTTON_LEFT:
			return CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::LeftButton);
			break;
		case SDL_BUTTON_MIDDLE:
			return CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::MiddleButton);
			break;
		case SDL_BUTTON_RIGHT:
			return CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::RightButton);
			break;
	}

	return false;
}



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
	//int anim = 0;
	CEGUI::uint kc;

	if(ThreadSafeWorkpile::getInstance()->GameQuitted())
		return true;

	switch( flevent.type )
	{
		//***** mouse events
        case SDL_MOUSEBUTTONDOWN:	// a mouse button has been pushed
			if(handle_mouse_down(flevent.button.button))
				return false;

			_lbaNetEngine->FocusChatbox(false);


			if(flevent.button.button == 4)	// mouse wheel up
				_lbaNetEngine->DeltaUpdateZoom(false);

			if(flevent.button.button == 5)	// mouse wheel down
				_lbaNetEngine->DeltaUpdateZoom(true);

			_button_2_pressed = (flevent.button.button == 3);
			if(_button_2_pressed)
				_current_mouse_y = flevent.button.y;
		break;

		case SDL_MOUSEBUTTONUP:	// a mouse button has been pushed
			if(handle_mouse_up(flevent.button.button))
				return false;

			if(flevent.button.button == 3)
				_button_2_pressed = false;
		break;

		case SDL_MOUSEMOTION:	// the user move the mouse with a button activated
			if(CEGUI::System::getSingleton().injectMousePosition(static_cast<float>(flevent.motion.x),static_cast<float>(flevent.motion.y)))
				return false;

			if(_button_2_pressed)
			{
				_lbaNetEngine->DeltaUpdateZenit(flevent.motion.y-_current_mouse_y);
				_current_mouse_y = flevent.motion.y;
			}
		break;


		//***** keyboard events
		case SDL_KEYDOWN:	// in case of keyboard key down

			/* to tell CEGUI that a key was pressed, we inject the scancode. */
			kc = SDLKeyToCEGUIKey(flevent.key.keysym.sym);
			if(CEGUI::System::getSingleton().injectKeyDown(kc))
				return false;
			if(CEGUI::System::getSingleton().injectChar(flevent.key.keysym.unicode))
				return false;


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
					_lbaNetEngine->DisplayGUI(3);
					break;


				case SDLK_F10:
					_lbaNetEngine->ReplaceMain();
					break;

				case SDLK_F12:
					_lbaNetEngine->ChangeFullScreen();
					break;

				case SDLK_F1:
					_lbaNetEngine->PlayerChangeStance(1);
					break;

				case SDLK_F2:
					_lbaNetEngine->PlayerChangeStance(2);
					break;

				case SDLK_F3:
					_lbaNetEngine->PlayerChangeStance(3);
					break;

				case SDLK_F4:
					_lbaNetEngine->PlayerChangeStance(4);
					break;

				case SDLK_F5:
					_lbaNetEngine->PlayerChangeStance(5);
					break;

				case SDLK_F6:
					_lbaNetEngine->PlayerChangeStance(6);
					break;

				case SDLK_F7:
					_lbaNetEngine->PlayerChangeStance(7);
					break;


				case SDLK_PAGEUP:
					_lbaNetEngine->PlayerStartMoveUpDown(true);
					break;

				case SDLK_PAGEDOWN:
					_lbaNetEngine->PlayerStartMoveUpDown(false);
					break;


				case SDLK_SPACE:
					_lbaNetEngine->PlayerDoAction(false);
					break;

				case SDLK_w:
					_lbaNetEngine->PlayerDoAction(true);
					break;


				case SDLK_RETURN:
					_lbaNetEngine->FocusChatbox(true);
					break;


				case SDLK_KP_PLUS:
					//_lbaNetEngine->GoNextRoom();
					break;

			}
		break;


		case SDL_KEYUP:	// in case of keyboard key down
			 kc = SDLKeyToCEGUIKey(flevent.key.keysym.sym);
			 if(CEGUI::System::getSingleton().injectKeyUp(kc))
				 return false;

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

				case SDLK_PAGEUP:
					_lbaNetEngine->PlayerStopMoveUpDown(true);
					break;

				case SDLK_PAGEDOWN:
					_lbaNetEngine->PlayerStopMoveUpDown(false);
					break;
			}
		break;


		// SDL_QUIT event (window close)
		case SDL_QUIT:
			return true;
	}

	return false;
}