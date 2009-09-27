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


#ifndef __LBANET_GAME_EVENTS_H__
#define __LBANET_GAME_EVENTS_H__


/*
************************************************************************************************************************
*                                                  class GameEvent
************************************************************************************************************************
*/
class GameEvent
{

public:
	int GetType()
	{ return _type;}


protected:
	int	_type;
};


/*
************************************************************************************************************************
*                                                  class LoginEvent
*
*	class used during login
************************************************************************************************************************
*/
class LoginEvent : public GameEvent
{
public:
	//! constructor
	LoginEvent(const std::string &Name, const std::string &Password, bool Local)
		: _Name(Name), _Password(Password), _Local(Local)
	{
		_type = 1;
	}

	std::string _Name;
	std::string _Password;
	bool _Local;
};



/*
************************************************************************************************************************
*                                                  class GuiExitEvent
*
*	used when the active gui is being quitted by the user
************************************************************************************************************************
*/
class GuiExitEvent : public GameEvent
{
public:
	//! constructor
	GuiExitEvent()
	{
		_type = 2;
	}
};

/*
************************************************************************************************************************
*                                                  class ChangeWorldEvent
*
*	used when the user is changing world
************************************************************************************************************************
*/
class ChangeWorldEvent : public GameEvent
{
public:
	//! constructor
	ChangeWorldEvent(const std::string & NewWorld)
		: _NewWorld(NewWorld)
	{
		_type = 3;
	}

	std::string _NewWorld;
};

/*
************************************************************************************************************************
*                                                  class DisplayGUIEvent
*
*	used when the user is switching gui
************************************************************************************************************************
*/
class DisplayGUIEvent : public GameEvent
{
public:
	//! constructor
	DisplayGUIEvent(int GuiNumber)
		: _GuiNumber(GuiNumber)
	{
		_type = 4;
	}

	int _GuiNumber;
};

/*
************************************************************************************************************************
*                                                  class ScreenResEvent
*
*	used when the user is changing screen resolution
************************************************************************************************************************
*/
class ScreenResEvent : public GameEvent
{
public:
	//! constructor
	ScreenResEvent(int screenX, int screenY, bool fullscreen)
		: _screenX(screenX), _screenY(screenY), _fullscreen(fullscreen)
	{
		_type = 5;
	}

	int _screenX;
	int _screenY;
	bool _fullscreen;
};

/*
************************************************************************************************************************
*                                                  class DisplayFPSEvent
*
*	used when the user is changing screen resolution
************************************************************************************************************************
*/
class DisplayFPSEvent : public GameEvent
{
public:
	//! constructor
	DisplayFPSEvent(bool display)
		: _display(display)
	{
		_type = 6;
	}

	bool _display;
};


/*
************************************************************************************************************************
*                                                  class ChangeMainBodyEvent
*
*	used when the user is changing screen resolution
************************************************************************************************************************
*/
class ChangeMainBodyEvent : public GameEvent
{
public:
	//! constructor
	ChangeMainBodyEvent(bool plus)
		: _plus(plus)
	{
		_type = 7;
	}

	bool _plus;
};

/*
************************************************************************************************************************
*                                                  class NewFontSizeEvent
*
*	used when the user is changing screen resolution
************************************************************************************************************************
*/
class NewFontSizeEvent : public GameEvent
{
public:
	//! constructor
	NewFontSizeEvent()
	{
		_type = 8;
	}
};


/*
************************************************************************************************************************
*                                                  class TeleportEvent
*
*	used when the user is changing screen resolution
************************************************************************************************************************
*/
class TeleportEvent : public GameEvent
{
public:
	//! constructor
	TeleportEvent(const std::string &NewMap, const std::string &Spawning)
		: _NewMap(NewMap), _Spawning(Spawning)
	{
		_type = 9;
	}

	std::string _NewMap;
	std::string _Spawning;
};



/*
************************************************************************************************************************
*                                                  class ChangeMainBodyColorEvent
*
*	used when the user is changing screen resolution
************************************************************************************************************************
*/
class ChangeMainBodyColorEvent : public GameEvent
{
public:
	//! constructor
	ChangeMainBodyColorEvent(bool plus)
		: _plus(plus)
	{
		_type = 10;
	}

	bool _plus;
};


/*
************************************************************************************************************************
*                                                  class DisplayGameTextEvent
*
*	used when the user has activated a text in game
************************************************************************************************************************
*/
class DisplayGameTextEvent : public GameEvent
{
public:
	//! constructor
	DisplayGameTextEvent(long textid)
		: _textid(textid)
	{
		_type = 11;
	}

	long _textid;
};



/*
************************************************************************************************************************
*                                                  struct PlayerScriptPart
*
*	used for scripted event
************************************************************************************************************************
*/
struct PlayerScriptPart
{
	int Type;	//0 = rotation - 1=translation - 2=animation - 3=inform targetA -
				// 4=wait fo signal -5= wait number of ms
	int Animation;
	int Sound;
	float ValueA;
	float ValueB;
	float ValueC;

	float Speed;
};

/*
************************************************************************************************************************
*                                                  class MainPlayerScriptedEvent
*
*	used when the user has activated a scripted event in game
************************************************************************************************************************
*/
class MainPlayerScriptedEvent : public GameEvent
{
public:


	//! constructor
	MainPlayerScriptedEvent(const std::vector<PlayerScriptPart> &script)
		: _script(script)
	{
		_type = 12;
	}

	std::vector<PlayerScriptPart> _script;
};



/*
************************************************************************************************************************
*                                                  class GameSignalvent
*
*	used when the user has activated a scripted event in game
************************************************************************************************************************
*/
//1 -> open door signal
//2 -> close door signal
//3 -> animation finished signal
//4 -> switch open/close door signal
class GameSignalvent : public GameEvent
{
public:


	//! constructor
	GameSignalvent(long signal, const std::vector<long> &targets)
		: _signal(signal), _targets(targets)
	{
		_type = 13;
	}

	long _signal;
	std::vector<long> _targets;
};


/*
************************************************************************************************************************
*                                                  class TeleportEvent
*
*	used when the user is changing screen resolution
************************************************************************************************************************
*/
class TeleportAnyEvent : public GameEvent
{
public:
	//! constructor
	TeleportAnyEvent(const std::string &NewMap, float X, float Y, float Z)
		: _NewMap(NewMap), _X(X), _Y(Y), _Z(Z)
	{
		_type = 14;
	}

	std::string _NewMap;
	float _X;
	float _Y;
	float _Z;
};


/*
************************************************************************************************************************
*                                                  class ChangePerspectiveEvent
*
*	used when the user is changing screen resolution
************************************************************************************************************************
*/
class ChangePerspectiveEvent : public GameEvent
{
public:
	//! constructor
	ChangePerspectiveEvent(bool perspective)
		: _perspective(perspective)
	{
		_type = 15;
	}

	bool _perspective;
};




/*
************************************************************************************************************************
*                                                  class DisplayExitsEvent
*
*	used when the user is changing screen resolution
************************************************************************************************************************
*/
class DisplayExitsEvent : public GameEvent
{
public:
	//! constructor
	DisplayExitsEvent(bool display)
		: _display(display)
	{
		_type = 16;
	}

	bool _display;
};



/*
************************************************************************************************************************
*                                                  class PlayerHurtEvent
*
*	used when the user is changing screen resolution
************************************************************************************************************************
*/
class PlayerHurtEvent : public GameEvent
{
public:
	//! constructor
	PlayerHurtEvent(long fromactorid)
		: _fromactorid(fromactorid)
	{
		_type = 17;
	}

	long _fromactorid;
};

/*
************************************************************************************************************************
*                                                  class DoFullCheckEvent
*
*	used to ask engine to do a full position check on the map
************************************************************************************************************************
*/
class DoFullCheckEvent : public GameEvent
{
public:
	//! constructor
	DoFullCheckEvent()
	{
		_type = 18;	
	}
	
};



/*
************************************************************************************************************************
*                                                  class PlayerLifeChangedEvent
*
*	used to update player life
************************************************************************************************************************
*/
class PlayerLifeChangedEvent : public GameEvent
{
public:
	//! constructor
	PlayerLifeChangedEvent(float CurLife, float MaxLife, float CurMana, float MaxMana)
		: _CurLife(CurLife), _MaxLife(MaxLife), _CurMana(CurMana), _MaxMana(MaxMana)
	{
		_type = 19;	
	}
	
	float _CurLife;
	float _MaxLife;
	float _CurMana;
	float _MaxMana;
};

#endif