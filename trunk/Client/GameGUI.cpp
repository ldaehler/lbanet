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

#include "GameGUI.h"
#include <CEGUI.h>
#include "LogHandler.h"
#include "MusicHandler.h"
#include "ThreadSafeWorkpile.h"
#include "GameEvents.h"
#include "DataLoader.h"
#include "MessageBox.h"
#include "ChooseNumberBox.h"

//#ifndef _LBANET_SET_EDITOR_
//#define _LBANET_SET_EDITOR_
//#endif

/***********************************************************
constructor
***********************************************************/
GameGUI::GameGUI()
: _cb(this), _comb(this), _telb(this), _invb(this, 50), 
	_shortb(this, 50), _containerb(this, 50), _editb(NULL), _lettereditb(this), 
	_letterviewb(this), _dialogb(this, 50)
{
	#ifdef _LBANET_SET_EDITOR_
	_editb = new EditorBox(this);
	#endif
}


/***********************************************************
destructor
***********************************************************/
GameGUI::~GameGUI()
{
}

/***********************************************************
initialize the GUI
***********************************************************/
void GameGUI::Initialize()
{
	try
	{
		_root = CEGUI::WindowManager::getSingleton().loadWindowLayout( "GameGuiRoot.layout" );
		_cb.Initialize(_root);
		_comb.Initialize(_root);
		_telb.Initialize(_root);
		_invb.Initialize(_root);
		_shortb.Initialize(_root);
		_containerb.Initialize(_root);
		_lettereditb.Initialize(_root);
		_letterviewb.Initialize(_root);
		_dialogb.Initialize(_root);
		CGMessageBox::getInstance()->Initialize(_root);
		ChooseNumberBox::getInstance()->Initialize(_root);


		if(_editb)_editb->Initialize(_root);
		CEGUI::WindowManager::getSingleton().getWindow("HeadInterfaceBG")->disable();
		CEGUI::WindowManager::getSingleton().getWindow("MenuCharInterfaceBG")->disable();

		static_cast<CEGUI::PushButton *> (CEGUI::WindowManager::getSingleton().getWindow("btnchaticon"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&GameGUI::HandleChatIconClicked, this));

		static_cast<CEGUI::PushButton *> (CEGUI::WindowManager::getSingleton().getWindow("btncomicon"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&GameGUI::HandleComIconClicked, this));

		static_cast<CEGUI::PushButton *> (CEGUI::WindowManager::getSingleton().getWindow("btnteleporticon"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&GameGUI::HandleTeleIconClicked, this));

		static_cast<CEGUI::PushButton *> (CEGUI::WindowManager::getSingleton().getWindow("btnquit"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&GameGUI::HandleQuitIconClicked, this));

		static_cast<CEGUI::PushButton *> (CEGUI::WindowManager::getSingleton().getWindow("btnsound"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&GameGUI::HandleSoundIconClicked, this));

		static_cast<CEGUI::PushButton *> (CEGUI::WindowManager::getSingleton().getWindow("btnchangeworld"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&GameGUI::HandleChangeWorldIconClicked, this));

		static_cast<CEGUI::PushButton *> (CEGUI::WindowManager::getSingleton().getWindow("btnoption"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&GameGUI::HandleOptionIconClicked, this));

		static_cast<CEGUI::PushButton *> (CEGUI::WindowManager::getSingleton().getWindow("DisplayGameTextFrame/bok"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&GameGUI::HandleCloseTextClicked, this));

		static_cast<CEGUI::PushButton *> (CEGUI::WindowManager::getSingleton().getWindow("btntunic"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&GameGUI::HandleInventoryClicked, this));

		static_cast<CEGUI::PushButton *> (CEGUI::WindowManager::getSingleton().getWindow("btnweapon"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&GameGUI::HandleShortcutClicked, this));


		
		CEGUI::FrameWindow * frw = static_cast<CEGUI::FrameWindow *> (
			CEGUI::WindowManager::getSingleton().getWindow("DisplayGameTextFrame"));
		frw->subscribeEvent (CEGUI::FrameWindow::EventCloseClicked,
			CEGUI::Event::Subscriber (&GameGUI::HandleCloseTextClicked, this));


		CEGUI::WindowManager::getSingleton().getWindow("DisplayGameTextFrame")->hide();
	}
	catch(CEGUI::Exception &ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Exception init login gui: ") + ex.getMessage().c_str());
		_root = NULL;
	}

	RefreshSOundButton();
}


/***********************************************************
handle send button event
***********************************************************/
bool GameGUI::HandleChatIconClicked (const CEGUI::EventArgs& e)
{
	_cb.Show();
	return true;
}



/***********************************************************
handle send button event
***********************************************************/
bool GameGUI::HandleComIconClicked (const CEGUI::EventArgs& e)
{
	_comb.Show();
	return true;
}




/***********************************************************
handle send button event
***********************************************************/
bool GameGUI::HandleTeleIconClicked (const CEGUI::EventArgs& e)
{
	_telb.Show();
	return true;
}

/***********************************************************
handle send button event
***********************************************************/
bool GameGUI::HandleQuitIconClicked (const CEGUI::EventArgs& e)
{
	ThreadSafeWorkpile::getInstance()->AddEvent(new DisplayGUIEvent(3));
	return true;
}

/***********************************************************
handle send button event
***********************************************************/
bool GameGUI::HandleSoundIconClicked (const CEGUI::EventArgs& e)
{
	MusicHandler::getInstance()->EnableDisableSound();
	RefreshSOundButton();
	return true;
}


/***********************************************************
handle send button event
***********************************************************/
bool GameGUI::HandleChangeWorldIconClicked (const CEGUI::EventArgs& e)
{
	ThreadSafeWorkpile::getInstance()->AddEvent(new DisplayGUIEvent(1));
	return true;
}


/***********************************************************
handle send button event
***********************************************************/
bool GameGUI::HandleOptionIconClicked (const CEGUI::EventArgs& e)
{
	ThreadSafeWorkpile::getInstance()->AddEvent(new DisplayGUIEvent(4));
	return true;
}


/***********************************************************
handle send button event
***********************************************************/
bool GameGUI::HandleInventoryClicked (const CEGUI::EventArgs& e)
{
	_invb.Show();
	return true;
}

/***********************************************************
handle send button event
***********************************************************/
bool GameGUI::HandleShortcutClicked (const CEGUI::EventArgs& e)
{
	_shortb.Show();
	return true;
}

/***********************************************************
process what is needed in the game GUI
***********************************************************/
void GameGUI::Process()
{
	_cb.Process();
	_comb.Process();
	_shortb.Process();
	_invb.Process();
	_containerb.Process();
	_dialogb.Process();
}





/***********************************************************
set the list of teleport places
***********************************************************/
void GameGUI::SetTeleportList(const std::map<std::string, TPInfo> &_lists)
{
	_telb.SetTeleportList(_lists);
}


/***********************************************************
refresh sound button
***********************************************************/
void GameGUI::RefreshSOundButton()
{
	try
	{
		if(!MusicHandler::getInstance()->IsSoundEnabled())
			static_cast<CEGUI::PushButton *> (CEGUI::WindowManager::getSingleton().getWindow("btnsound"))->
												setProperty("NormalImage", "set:soundbutton image:disabled");
		else
			static_cast<CEGUI::PushButton *> (CEGUI::WindowManager::getSingleton().getWindow("btnsound"))->
												setProperty("NormalImage", "set:soundbutton image:normal");
	}
	catch(CEGUI::Exception &ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Exception refreshing the sound button: ") + ex.getMessage().c_str());
		_root = NULL;
	}
}


/***********************************************************
focus the chatbox
***********************************************************/
void GameGUI::FocusChatbox(bool focus)
{
	_cb.Focus(focus);
}


/***********************************************************
set irc thread
***********************************************************/
void GameGUI::SetIrcThread(IrcThread * IT)
{
	_cb.SetIrc(IT);
}



/***********************************************************
display game text
***********************************************************/
bool GameGUI::DisplayGameText(long textid, bool show)
{
	try
	{
		CEGUI::MultiLineEditbox * txs =
		static_cast<CEGUI::MultiLineEditbox *> (CEGUI::WindowManager::getSingleton().getWindow("DisplayGameTextFrame/text"));

		if(show)
		{
			std::string str = DataLoader::getInstance()->GetText(textid);
			int idxs = 0;
			bool firsttime=true;
			while((idxs = str.find(" @ ")) != std::string::npos)
			{
				std::string tmp = str.substr(0, idxs);
				if(tmp == "")
					tmp = "\n";

				if(firsttime)
				{
					firsttime = false;
					txs->setText((const unsigned char *)tmp.c_str());
				}
				else
					txs->appendText((const unsigned char *)tmp.c_str());

				str = str.substr(idxs+3);
			}

			if(firsttime)
			{
				firsttime = false;
				txs->setText((const unsigned char *)str.c_str());
			}
			else
				txs->appendText((const unsigned char *)str.c_str());


			if(!CEGUI::WindowManager::getSingleton().getWindow("DisplayGameTextFrame")->isVisible())
			{
				CEGUI::WindowManager::getSingleton().getWindow("DisplayGameTextFrame")->show();
				return true;
			}
			return false;
		}
		else
		{
			if(CEGUI::WindowManager::getSingleton().getWindow("DisplayGameTextFrame")->isVisible())
			{
				CEGUI::WindowManager::getSingleton().getWindow("DisplayGameTextFrame")->hide();
				return true;
			}
			return false;
		}

	}
	catch(CEGUI::Exception &ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Exception showing text window: ") + ex.getMessage().c_str());
		_root = NULL;
	}

	return true;
}



/***********************************************************
handle send button event
***********************************************************/
bool GameGUI::HandleCloseTextClicked (const CEGUI::EventArgs& e)
{
	try
	{
		CEGUI::WindowManager::getSingleton().getWindow("DisplayGameTextFrame")->hide();
	}
	catch(CEGUI::Exception &ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Exception hidding text window: ") + ex.getMessage().c_str());
		_root = NULL;
	}

	return true;
}



/***********************************************************
set player name
***********************************************************/
void GameGUI::SetPlayerName(const std::string & name)
{
	try
	{
		CEGUI::WindowManager::getSingleton().getWindow("PlayerName")->setText(name);
	}
	catch(CEGUI::Exception &ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Exception SetPlayerName: ") + ex.getMessage().c_str());
		_root = NULL;
	}
}



/***********************************************************
refreshthe gui
***********************************************************/
void GameGUI::Refresh()
{
	_cb.Show();_cb.Show();
	_comb.Show();_comb.Show();
	_telb.Show();_telb.Show();
	_invb.Show();_invb.Show();
	_shortb.Show();_shortb.Show();

	_containerb.Refresh();
	_invb.Refresh();
	_shortb.Refresh();

	CEGUI::Window * win = CEGUI::WindowManager::getSingleton().getWindow("PlayerName");
	win->hide();
	win->show();
}




/***********************************************************
display inventory
***********************************************************/
void GameGUI::ShowInventory()
{
	_invb.Show();
}


/***********************************************************
show dialog with NPC
***********************************************************/
void GameGUI::ShowDialog(long ActorId, const std::string &ActorName, const std::string & WelcomeSentence,
							bool IsTrader, bool Show,
							const std::map<long, TraderItem> &inventory)
{
	_dialogb.ShowDialog(ActorId, ActorName, WelcomeSentence, IsTrader, Show, inventory);
}