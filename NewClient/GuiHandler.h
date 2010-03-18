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

#if !defined(__LbaNetModel_1_GuiHandler_h)
#define __LbaNetModel_1_GuiHandler_h

class GUI;
class GameGUI;
class IrcThread;
class LoginGUI;
class Actor;
class LbaNetEngine;
class OptionsGUI;

namespace CEGUI
{
	class OpenGLRenderer;
	class EventArgs;
}



#include <string>
#include <vector>
#include <list>
#include <map>
struct TPInfo;

/***********************************************************************
 * Module:  GuiHandler.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:59:34
 * Purpose: Declaration of the class GuiHandler
 ***********************************************************************/
class GuiHandler
{
public:
	//! constructor
	GuiHandler();

	//! destructor
	~GuiHandler();

	//! initialize function
	void Initialize(int screen_size_X, int screen_size_Y, bool ServerOn,
						const std::string &clientversion, LbaNetEngine * engine);

	// process function
	void process(void);

	//! switch from current gui to another one
	void SwitchGUI(int GuiNumber);

	//! redraw the scene on screen
	void Redraw(void);

	//! called when the windows is resized
	void Resize(int screen_size_X, int screen_size_Y);
	void grabTextures();
	void restoreTextures();



	//! called when font size changed
	void ReloadFontSize();



protected:
	//! inject time to the GUI
	void inject_time_pulse();

private:
	CEGUI::OpenGLRenderer *	_gui_renderer;
	std::vector<GUI *>		_guis;
	GameGUI *				_game_gui;
	LoginGUI *				_login_gui;
	OptionsGUI *			_option_gui;
	int						_currentGUI;
	double					m_last_time_pulse;
	LbaNetEngine *			_engine;
};

#endif