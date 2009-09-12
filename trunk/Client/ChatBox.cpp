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

#include "ChatBox.h"
#include <iostream>
#include <algorithm>
#include <CEGUI.h>

#include "ThreadSafeWorkpile.h"
#include "IrcThread.h"
#include "GameGUI.h"
#include "ConfigurationManager.h"
#include "LogHandler.h"

// Sample sub-class for ListboxTextItem that auto-sets the selection brush
// image.  This saves doing it manually every time in the code.
class MyListItem : public CEGUI::ListboxTextItem
{
public:
    MyListItem (const CEGUI::String& text) : CEGUI::ListboxTextItem(text)
    {
        setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
    }
};


/***********************************************************
constructor
***********************************************************/
ChatBox::ChatBox(GameGUI * gamgui)
: _control_key_on(false), _shift_key_on(false),
	_gamgui(gamgui), _IRC(NULL), _currSelectedch(0)
{
	_channels.push_back("World");
	_channels.push_back("Map");
	_channels.push_back("IRC");
}


/***********************************************************
destructor
***********************************************************/
ChatBox::~ChatBox()
{
	try
	{
		CEGUI::FrameWindow * frw = static_cast<CEGUI::FrameWindow *> (
			CEGUI::WindowManager::getSingleton().getWindow("ChatFrame"));
		frw->subscribeEvent (CEGUI::FrameWindow::EventCloseClicked,
			CEGUI::Event::Subscriber (&ChatBox::HandleCloseChatbox, this));

		CEGUI::UVector2 vec = frw->getPosition();
		ConfigurationManager::GetInstance()->SetFloat("Gui.Chatbox.PosX", vec.d_x.d_scale);
		ConfigurationManager::GetInstance()->SetFloat("Gui.Chatbox.PosY", vec.d_y.d_scale);
		ConfigurationManager::GetInstance()->SetFloat("Gui.Chatbox.SizeX", frw->getWidth().d_scale);
		ConfigurationManager::GetInstance()->SetFloat("Gui.Chatbox.SizeY", frw->getHeight().d_scale);
		ConfigurationManager::GetInstance()->SetFloat("Gui.Chatbox.OffsetPosX", vec.d_x.d_offset);
		ConfigurationManager::GetInstance()->SetFloat("Gui.Chatbox.OffsetPosY", vec.d_y.d_offset);
		ConfigurationManager::GetInstance()->SetFloat("Gui.Chatbox.OffsetSizeX", frw->getWidth().d_offset);
		ConfigurationManager::GetInstance()->SetFloat("Gui.Chatbox.OffsetSizeY", frw->getHeight().d_offset);
		ConfigurationManager::GetInstance()->SetBool("Gui.Chatbox.Visible", frw->isVisible());
	}
	catch(CEGUI::Exception &ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Exception destructor chatbox: ") + ex.getMessage().c_str());
	}
}


/***********************************************************
initalize the box
***********************************************************/
void ChatBox::Initialize(CEGUI::Window* Root)
{
	try
	{
		_myChat = CEGUI::WindowManager::getSingleton().loadWindowLayout( "chatbox.layout" );
		Root->addChildWindow(_myChat);
		//tc->setProperty( "InheritsAlpha", "false" );

		AddTab("All");
		AddTab("World");
		AddTab("Map");
		AddTab("IRC");

		static_cast<CEGUI::PushButton *> (
			CEGUI::WindowManager::getSingleton().getWindow("Chat/bChannel"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&ChatBox::HandleBChannel, this));

		static_cast<CEGUI::PushButton *> (
			CEGUI::WindowManager::getSingleton().getWindow("Chat/bSend"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&ChatBox::HandleSend, this));

		static_cast<CEGUI::Editbox *> (
			CEGUI::WindowManager::getSingleton().getWindow("Chat/edit"))->subscribeEvent (
			CEGUI::Editbox::EventKeyDown,
			CEGUI::Event::Subscriber (&ChatBox::HandleEnterKey, this));

		static_cast<CEGUI::Editbox *> (
			CEGUI::WindowManager::getSingleton().getWindow("Chat/edit"))->subscribeEvent (
			CEGUI::Editbox::EventKeyUp,
			CEGUI::Event::Subscriber (&ChatBox::HandleReleaseKey, this));

		_lb = static_cast<CEGUI::Listbox *>
			(CEGUI::WindowManager::getSingleton().createWindow( "TaharezLook/Listbox", "Chat/listchannel" ));

		_lb->subscribeEvent(CEGUI::Listbox::EventSelectionChanged,
							CEGUI::Event::Subscriber (&ChatBox::HandleLbSelected, this));


		_lb->setProperty("Text", "Channels");
		_lb->setProperty("UnifiedMaxSize", "{{1,0},{1,0}}");
		_lb->setProperty("UnifiedAreaRect", "{{0,10},{1,-140},{0,95},{1,-40}}");
		_lb->setProperty("AlwaysOnTop", "True");

		_myChat->addChildWindow(_lb);
		_lb->hide();

		_myChannels = CEGUI::WindowManager::getSingleton().loadWindowLayout( "choosechannel.layout" );
		_myChannels->setProperty("AlwaysOnTop", "True");
		Root->addChildWindow(_myChannels);
		_myChannels->hide();

		CEGUI::FrameWindow * fw = static_cast<CEGUI::FrameWindow *>(_myChannels);
		fw->subscribeEvent (	CEGUI::FrameWindow::EventCloseClicked,
								CEGUI::Event::Subscriber (&ChatBox::HandleCloseChannels, this) );

		static_cast<CEGUI::PushButton *> (
			CEGUI::WindowManager::getSingleton().getWindow("Chat/chooseChannel/bOk"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&ChatBox::HandleCCOk, this));

		static_cast<CEGUI::PushButton *> (
			CEGUI::WindowManager::getSingleton().getWindow("Chat/chooseChannel/bCancel"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&ChatBox::HandleCCCancel, this));



		CEGUI::FrameWindow * frw = static_cast<CEGUI::FrameWindow *> (
			CEGUI::WindowManager::getSingleton().getWindow("ChatFrame"));
		frw->subscribeEvent (CEGUI::FrameWindow::EventCloseClicked,
			CEGUI::Event::Subscriber (&ChatBox::HandleCloseChatbox, this));

		float PosX, PosY, SizeX, SizeY, OPosX, OPosY, OSizeX, OSizeY;
		bool Visible;
		ConfigurationManager::GetInstance()->GetFloat("Gui.Chatbox.PosX", PosX);
		ConfigurationManager::GetInstance()->GetFloat("Gui.Chatbox.PosY", PosY);
		ConfigurationManager::GetInstance()->GetFloat("Gui.Chatbox.SizeX", SizeX);
		ConfigurationManager::GetInstance()->GetFloat("Gui.Chatbox.SizeY", SizeY);
		ConfigurationManager::GetInstance()->GetFloat("Gui.Chatbox.OffsetPosX", OPosX);
		ConfigurationManager::GetInstance()->GetFloat("Gui.Chatbox.OffsetPosY", OPosY);
		ConfigurationManager::GetInstance()->GetFloat("Gui.Chatbox.OffsetSizeX", OSizeX);
		ConfigurationManager::GetInstance()->GetFloat("Gui.Chatbox.OffsetSizeY", OSizeY);
		ConfigurationManager::GetInstance()->GetBool("Gui.Chatbox.Visible", Visible);
		frw->setPosition(CEGUI::UVector2(CEGUI::UDim(PosX, OPosX), CEGUI::UDim(PosY, OPosY)));
		frw->setWidth(CEGUI::UDim(SizeX, OSizeX));
		frw->setHeight(CEGUI::UDim(SizeY, OSizeY));

		if(Visible)
			frw->show();
		else
			frw->hide();

	}
	catch(CEGUI::Exception &ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Exception init chatbox: ") + ex.getMessage().c_str());
	}
}


/***********************************************************
add new text to the chatbox
***********************************************************/
void ChatBox::AddText(std::string channel, const std::string & Sender, const std::string & Text)
{
	if(channel == (_currentWorld + "_" + _currentMap))
		channel = "Map";

	if(CEGUI::WindowManager::getSingleton().isWindowPresent("Chat/Tab_"+channel+"/editMulti"))
	{
		CEGUI::String tmp((const unsigned char *)Text.c_str());
		CEGUI::String tmp2((const unsigned char *)Sender.c_str());

		CEGUI::MultiLineEditbox* txt = static_cast<CEGUI::MultiLineEditbox *>(CEGUI::WindowManager::getSingleton().getWindow("Chat/Tab_"+channel+"/editMulti"));
		txt->hide();
		//float pos = txt->getVertScrollbar()->getScrollPosition();
		txt->appendText(tmp2 + ": "+ tmp);

		if(txt->getText().size() > 3000)
			txt->setText(txt->getText().substr(500));
		txt->show();

		{
			float maxpos = (txt->getVertScrollbar()->getDocumentSize() - txt->getVertScrollbar()->getPageSize());
			//if(pos >= maxpos*9/10)
				txt->getVertScrollbar()->setScrollPosition(maxpos);
		}

		if(CEGUI::WindowManager::getSingleton().isWindowPresent("Chat/Tab_All/editMulti"))
		{
			CEGUI::MultiLineEditbox* txt2 = static_cast<CEGUI::MultiLineEditbox *>(CEGUI::WindowManager::getSingleton().getWindow("Chat/Tab_All/editMulti"));
			CEGUI::String tmp3((const unsigned char *)channel.c_str());

			//float pos2 = txt2->getVertScrollbar()->getScrollPosition();
			txt2->hide();
			txt2->appendText(tmp2 + "@" + tmp3+ ": "+ tmp);
			if(txt2->getText().size() > 3000)
				txt2->setText(txt2->getText().substr(500));
			txt2->show();

			float maxpos = (txt2->getVertScrollbar()->getDocumentSize() - txt2->getVertScrollbar()->getPageSize());
			//if(pos2 >= maxpos*9/10)
				txt2->getVertScrollbar()->setScrollPosition(maxpos);
		}
	}

}

/***********************************************************
add a tab to the chat
***********************************************************/
void ChatBox::AddTab(const std::string & tabName)
{
    CEGUI::TabControl *tc = static_cast<CEGUI::TabControl *>(CEGUI::WindowManager::getSingleton().getWindow ("Chat/TabControl"));

	CEGUI::FrameWindow* fWnd = static_cast<CEGUI::FrameWindow *>(CEGUI::WindowManager::getSingleton().createWindow( "DefaultGUISheet", "Chat/Tab_"+tabName ));
	fWnd->setProperty("Text", (const unsigned char *)tabName.c_str());

	CEGUI::MultiLineEditbox* txt = static_cast<CEGUI::MultiLineEditbox *>(CEGUI::WindowManager::getSingleton().createWindow( "TaharezLook/MultiLineEditbox", "Chat/Tab_"+tabName+"/editMulti" ));
	txt->setProperty("MaxTextLength", "1073741823");
	txt->setProperty("UnifiedMaxSize", "{{1,0},{1,0}}");
	txt->setProperty("UnifiedAreaRect", "{{0,1},{0,1},{1,-1},{1,-1}}");
	txt->setReadOnly(true);
	fWnd->addChildWindow(txt);
	tc->addTab (fWnd);

   txt->subscribeEvent(CEGUI::Editbox::EventKeyDown, CEGUI::Event::Subscriber (&ChatBox::HandleEnterKey, this));
}

/***********************************************************
handle send button event
***********************************************************/
bool ChatBox::HandleSend (const CEGUI::EventArgs& e)
{
    CEGUI::PushButton * bch = static_cast<CEGUI::PushButton *>
		(CEGUI::WindowManager::getSingleton().getWindow("Chat/bChannel"));
	std::string curChannel = bch->getProperty("Text").c_str();

	CEGUI::Editbox * bed = static_cast<CEGUI::Editbox *>
		(CEGUI::WindowManager::getSingleton().getWindow("Chat/edit"));


	std::string currText = bed->getProperty("Text").c_str();
	if(currText != "")
	{
		SendText(curChannel, currText);
		bed->setProperty("Text", "");
	}

    return true;
}


/***********************************************************
handle send button event
***********************************************************/
bool ChatBox::HandleBChannel(const CEGUI::EventArgs& e)
{
	if(!_lb->isVisible())
	{
		_lb->resetList();

		std::list<std::string>::const_iterator it = _channels.begin();
		std::list<std::string>::const_iterator end = _channels.end();
		for(; it != end; ++it)
			_lb->addItem(new MyListItem((const unsigned char *)it->c_str()));

		_lb->addItem(new MyListItem("New.."));

		_lb->show();
	}
	else
	{
		_lb->hide();
	}
    return true;
}


/***********************************************************
send text outside
***********************************************************/
void ChatBox::SendText(const std::string & channel, const std::string & Text)
{
	if(Text == "")
		return;

	if(channel == "IRC")
	{
		if(_IRC)
			_IRC->sendMessage(Text);
	}
	else
	{
		if(Text[0] == '/')
			ThreadSafeWorkpile::getInstance()->AddChatText(Text);
		else
		{
			std::string tosend("/");
			if(channel == "Map")
				tosend += _currentWorld + "_" + _currentMap;
			else
				tosend +=channel;

			tosend += " ";
			tosend += Text;

			ThreadSafeWorkpile::getInstance()->AddChatText(tosend);
		}
	}
}


/***********************************************************
set the current map of the game
***********************************************************/
void ChatBox::SetCurrentMap(const std::string & WorldName, const std::string & MapName)
{
	_currentWorld = WorldName;
	_currentMap = MapName;

	CEGUI::WindowManager::getSingleton().getWindow("ChatFrame")->setProperty("Text", "Chat - " + _currentWorld + "/" + _currentMap);

}


/***********************************************************
handle send button event
***********************************************************/
bool ChatBox::HandleEnterKey (const CEGUI::EventArgs& e)
{
	const CEGUI::KeyEventArgs& we =
    static_cast<const CEGUI::KeyEventArgs&>(e);

	const CEGUI::WindowEventArgs& wine =
    static_cast<const CEGUI::WindowEventArgs&>(e);


	if(we.scancode == CEGUI::Key::LeftControl || we.scancode == CEGUI::Key::RightControl)
	{
		_control_key_on = true;
		return true;
	}

	if(we.scancode == CEGUI::Key::LeftShift || we.scancode == CEGUI::Key::RightShift)
	{
		_shift_key_on = true;
		return true;
	}


	if(wine.window->getName() == "Chat/edit")
	{
		if(we.scancode == CEGUI::Key::Return)
		{
			HandleSend (e);

			CEGUI::Editbox * bed = static_cast<CEGUI::Editbox *>
			(CEGUI::WindowManager::getSingleton().getWindow("Chat/edit"));
			bed->deactivate();
			return true;
		}

		if(we.scancode == CEGUI::Key::ArrowUp)
		{


			++_currSelectedch;
			if(_currSelectedch >= (int)_channels.size())
				--_currSelectedch;
			else
			{
				std::list<std::string>::const_iterator it = _channels.begin();
				std::list<std::string>::const_iterator end = _channels.end();
				for(int cc=0; cc<_currSelectedch && it != end; ++it, ++cc);

				CEGUI::PushButton * bch = static_cast<CEGUI::PushButton *>
					(CEGUI::WindowManager::getSingleton().getWindow("Chat/bChannel"));
				bch->setProperty("Text", *it);
			}



			return true;
		}
		if(we.scancode == CEGUI::Key::ArrowDown)
		{
			--_currSelectedch;
			if(_currSelectedch < 0)
				++_currSelectedch;
			else
			{
				std::list<std::string>::const_iterator it = _channels.begin();
				std::list<std::string>::const_iterator end = _channels.end();
				for(int cc=0; cc<_currSelectedch && it != end; ++it, ++cc);

				CEGUI::PushButton * bch = static_cast<CEGUI::PushButton *>
					(CEGUI::WindowManager::getSingleton().getWindow("Chat/bChannel"));
				bch->setProperty("Text", *it);
			}

			return true;
		}


		if(we.scancode == CEGUI::Key::ArrowUp || we.scancode == CEGUI::Key::ArrowDown)
			return true;



		// paste text
		if(we.scancode == CEGUI::Key::V && _control_key_on)
		{
			CEGUI::Editbox * bed = static_cast<CEGUI::Editbox *>
				(CEGUI::WindowManager::getSingleton().getWindow("Chat/edit"));
			if(bed->isActive())
			{
				if(_text_copyed != "")
				{
					size_t selB = bed->getSelectionStartIndex();
					size_t selE = bed->getSelectionLength();
					CEGUI::String str = bed->getText();
					if(selE > 0)
					{
						str = str.erase(selB, selE);
					}

					if(str.size() + _text_copyed.size() < bed->getMaxTextLength())
					{
						size_t idx = bed->getCaratIndex();
						str = str.insert(idx, (unsigned char *)_text_copyed.c_str());
						bed->setText(str);
						bed->setCaratIndex(idx + _text_copyed.size());
					}
				}

				return true;
			}
		}
	}



	// copy text
	if(we.scancode == CEGUI::Key::C && _control_key_on)
	{
		CEGUI::Window * actw = _myChat->getActiveChild();
		if(actw != NULL)
		{
			if(actw->getName() == "Chat/edit")
			{
				CEGUI::Editbox * bed = static_cast<CEGUI::Editbox *> (actw);
				size_t selB = bed->getSelectionStartIndex();
				size_t selE = bed->getSelectionLength();
				if(selE > 0)
				{
					CEGUI::String str = bed->getText().substr(selB, selE);
					_text_copyed = str.c_str();
				}

				return true;
			}
			else
			{
				CEGUI::MultiLineEditbox* txt = static_cast<CEGUI::MultiLineEditbox *>(actw);
				size_t selB = txt->getSelectionStartIndex();
				size_t selE = txt->getSelectionLength();
				if(selE > 0)
				{
					CEGUI::String str = txt->getText().substr(selB, selE);
					_text_copyed = str.c_str();
				}

				return true;
			}
		}

	}

    return false;
}


/***********************************************************
handle event when list is selected
***********************************************************/
bool ChatBox::HandleLbSelected (const CEGUI::EventArgs& e)
{
	const CEGUI::ListboxTextItem * it = static_cast<const CEGUI::ListboxTextItem *>(_lb->getFirstSelectedItem());
	std::string txt = it->getText().c_str();
	if(txt == "New..")
	{
		_myChannels->show();
		CEGUI::Editbox * bed = static_cast<CEGUI::Editbox *>
			(CEGUI::WindowManager::getSingleton().getWindow("Chat/chooseChannel/edit"));
		bed->activate();
		_myChat->setEnabled(false);
	}
	else
	{
		CEGUI::PushButton * bch = static_cast<CEGUI::PushButton *>
			(CEGUI::WindowManager::getSingleton().getWindow("Chat/bChannel"));
		bch->setProperty("Text", it->getText());


		_currSelectedch= (int)_lb->getItemIndex(it);
	}

	_lb->hide();

	return true;
}


/***********************************************************
add a channel to the chat
***********************************************************/
void ChatBox::AddChannel(const std::string & channel)
{
	std::list<std::string>::iterator it = std::find(_channels.begin(), _channels.end(), channel);
	if(it != _channels.end())
		return;

	_channels.push_back(channel);
	AddTab(channel);

	CEGUI::PushButton * bch = static_cast<CEGUI::PushButton *>
		(CEGUI::WindowManager::getSingleton().getWindow("Chat/bChannel"));
	bch->setProperty("Text", (const unsigned char *)channel.c_str());

	std::string tosend("/join ");
	tosend +=channel;
	ThreadSafeWorkpile::getInstance()->AddChatText(tosend);
}


/***********************************************************
handle event when list is selected
***********************************************************/
bool ChatBox::HandleCCOk (const CEGUI::EventArgs& e)
{
	CEGUI::Editbox * bed = static_cast<CEGUI::Editbox *>
	(CEGUI::WindowManager::getSingleton().getWindow("Chat/chooseChannel/edit"));

	std::string strc = bed->getProperty("Text").c_str();

	if(strc != "")
	{
		AddChannel(strc);
		bed->setProperty("Text", "");
		_myChannels->hide();
		_myChat->setEnabled(true);
	}
	else
	{
		bed->activate();
	}

	return true;
}

/***********************************************************
handle event when list is selected
***********************************************************/
bool ChatBox::HandleCCCancel (const CEGUI::EventArgs& e)
{
	CEGUI::Editbox * bed = static_cast<CEGUI::Editbox *>
	(CEGUI::WindowManager::getSingleton().getWindow("Chat/chooseChannel/edit"));

	bed->setProperty("Text", "");
	_myChannels->hide();
	_myChat->setEnabled(true);
	return true;
}


/***********************************************************
handle release key
***********************************************************/
bool ChatBox::HandleReleaseKey (const CEGUI::EventArgs& e)
{
	const CEGUI::KeyEventArgs& we =
    static_cast<const CEGUI::KeyEventArgs&>(e);

	if(we.scancode == CEGUI::Key::LeftControl || we.scancode == CEGUI::Key::RightControl)
		_control_key_on = false;

	if(we.scancode == CEGUI::Key::LeftShift || we.scancode == CEGUI::Key::RightShift)
		_shift_key_on = false;

	return false;
}


/***********************************************************
handle event when the channel window is closed
***********************************************************/
bool ChatBox::HandleCloseChannels (const CEGUI::EventArgs& e)
{
	_myChannels->hide();
	_myChat->setEnabled(true);
	return true;
}



/***********************************************************
handle event when the channel window is closed
***********************************************************/
bool ChatBox::HandleCloseChatbox (const CEGUI::EventArgs& e)
{
	_myChat->hide();
	//_gamgui->ShowChatIcon();
	return true;
}


/***********************************************************
display the chatbox on screen
***********************************************************/
void ChatBox::Show()
{
	if(_myChat->isVisible())
		_myChat->hide();
	else
	{
		_myChat->show();
		_myChat->activate();
	}
}



/***********************************************************
used to process text to add
***********************************************************/
void ChatBox::Process()
{
	std::vector<ThreadSafeWorkpile::ChatTextData> data;
	ThreadSafeWorkpile::getInstance()->GetChatData(data);

	std::vector<ThreadSafeWorkpile::ChatTextData>::const_iterator it = data.begin();
	std::vector<ThreadSafeWorkpile::ChatTextData>::const_iterator end = data.end();

	for(; it != end; ++it)
		AddText(it->Channel, it->Sender, it->Text);
}


/***********************************************************
used to give focus to the chatbox
***********************************************************/

void ChatBox::Focus(bool focus)
{
	CEGUI::Editbox * bed = static_cast<CEGUI::Editbox *>
	(CEGUI::WindowManager::getSingleton().getWindow("Chat/edit"));

	if(focus)
		bed->activate();
	else
		bed->deactivate();
}


