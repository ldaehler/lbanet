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

#include "EditorBox.h"
#include <iostream>
#include <algorithm>
#include <CEGUI.h>

#include "GameGUI.h"
#include "LogHandler.h"
#include "DataLoader.h"
#include "ThreadSafeWorkpile.h"
#include "GameEvents.h"
#include "MapInfoXmlReader.h"
#include "MapInfoXmlWriter.h"
#include "Actor.h"
#include "3DObjectRenderer.h"
#include "SpriteRenderer.h"
#include "AviVideoRenderer.h"

#include "TextActor.h"
#include "LadderActor.h"
#include "ExitActor.h"
#include "DoorActor.h"
#include "ContainerActor.h"
#include "UpExitActor.h"
#include "SwitchActor.h"
#include "ms3d.h"
#include "FloorSwitch.h"
#include "AreaSwitch.h"
#include "ScriptableActor.h"
#include "HurtArea.h"



// Sample sub-class for ListboxTextItem that auto-sets the selection brush
// image.  This saves doing it manually every time in the code.
class MyEditorListItem : public CEGUI::ListboxTextItem
{
public:
    MyEditorListItem (const CEGUI::String& text) : CEGUI::ListboxTextItem(text)
    {
        setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
    }
};


// string helper function
static void Trim(std::string& str)
{
	std::string::size_type pos = str.find_last_not_of(' ');
	if(pos != std::string::npos)
	{
		str.erase(pos + 1);
		pos = str.find_first_not_of(' ');

		if(pos != std::string::npos)
			str.erase(0, pos);
	}
	else
		str.clear();

}

// string helper function
static void StringTokenize(const std::string& str,
						std::vector<std::string>& tokens,
						const std::string& delimiters)
{
	// Skip delimiters at beginning.
	std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// Find first "non-delimiter".
	std::string::size_type pos     = str.find_first_of(delimiters, lastPos);



	while (std::string::npos != pos || std::string::npos != lastPos)
	{
		// Found a token, add it to the vector.
		std::string tmp = str.substr(lastPos, pos - lastPos);
		Trim(tmp);
		tokens.push_back(tmp);

		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);

		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}
}




/***********************************************************
constructor
***********************************************************/
EditorBox::EditorBox(GameGUI * gamgui)
: _gamgui(gamgui), _wi(DataLoader::getInstance()->GetWorldInfo()), _currsp(NULL),
	_currex(NULL), _Lactors(NULL), _Eactors(NULL), _sprites(NULL), _videos(NULL)
{
}

/***********************************************************
destructor
***********************************************************/
EditorBox::~EditorBox()
{
}


/***********************************************************
initalize the box
***********************************************************/
void EditorBox::Initialize(CEGUI::Window* Root)
{
	try
	{
		_myBox = CEGUI::WindowManager::getSingleton().loadWindowLayout( "editortab.layout" );
		Root->addChildWindow(_myBox);


		static_cast<CEGUI::Listbox *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/list"))
			->subscribeEvent(CEGUI::Listbox::EventSelectionChanged,
							CEGUI::Event::Subscriber (&EditorBox::HandleLbmapSelected, this));

		static_cast<CEGUI::Combobox *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/combosp"))
				->subscribeEvent(CEGUI::Combobox::EventListSelectionChanged,
							CEGUI::Event::Subscriber (&EditorBox::HandleLbspawningSelected, this));

		static_cast<CEGUI::PushButton *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/go"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&EditorBox::HandleBokmap, this));

		static_cast<CEGUI::PushButton *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/create"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&EditorBox::Handlebcreatemap, this));


		{
			CEGUI::Combobox * cb = static_cast<CEGUI::Combobox *> (
					CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/editmusic"));
			cb->resetList();

			std::vector<std::string> mlist;
			DataLoader::getInstance()->GetAvailableMusics(mlist);
			for(size_t i=0; i<mlist.size(); ++i)
				cb->addItem(new MyEditorListItem((const unsigned char *)mlist[i].c_str()));
		}

		{
			CEGUI::Combobox * cb = static_cast<CEGUI::Combobox *> (
					CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/edtmapdata"));
			cb->resetList();

			std::vector<std::string> mlist;
			DataLoader::getInstance()->GetAvailableMaps(mlist);
			for(size_t i=0; i<mlist.size(); ++i)
				cb->addItem(new MyEditorListItem((const unsigned char *)mlist[i].c_str()));
		}

		{
			CEGUI::Combobox * cb = static_cast<CEGUI::Combobox *> (
					CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/edtlocalA"));
			cb->resetList();

			std::vector<std::string> mlist;
			DataLoader::getInstance()->GetAvailableLActors(mlist);
			for(size_t i=0; i<mlist.size(); ++i)
				cb->addItem(new MyEditorListItem((const unsigned char *)mlist[i].c_str()));
		}

		{
			CEGUI::Combobox * cb = static_cast<CEGUI::Combobox *> (
					CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/edtexta"));
			cb->resetList();

			std::vector<std::string> mlist;
			DataLoader::getInstance()->GetAvailableEActors(mlist);
			for(size_t i=0; i<mlist.size(); ++i)
				cb->addItem(new MyEditorListItem((const unsigned char *)mlist[i].c_str()));
		}

		{
			CEGUI::Combobox * cb = static_cast<CEGUI::Combobox *> (
					CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/edttext"));
			cb->resetList();

			std::vector<std::string> mlist;
			DataLoader::getInstance()->GetAvailableTexts(mlist);
			for(size_t i=0; i<mlist.size(); ++i)
				cb->addItem(new MyEditorListItem((const unsigned char *)mlist[i].c_str()));
		}

		static_cast<CEGUI::Listbox *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Spawnings/list"))
			->subscribeEvent(CEGUI::Listbox::EventSelectionChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlespawningselected, this));


		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Spawnings/spex"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlespawningplacechanged, this));

		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Spawnings/spey"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlespawningplacechanged, this));

		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Spawnings/spez"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlespawningplacechanged, this));


		static_cast<CEGUI::PushButton *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Spawnings/spsave"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&EditorBox::Handlesaveb, this));

		static_cast<CEGUI::PushButton *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/spsave"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&EditorBox::Handlesaveb, this));



		static_cast<CEGUI::PushButton *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Spawnings/spadd"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&EditorBox::Handleaddspaw, this));

		static_cast<CEGUI::PushButton *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Spawnings/sprem"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&EditorBox::Handleremspaw, this));




		static_cast<CEGUI::Listbox *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/list"))
			->subscribeEvent(CEGUI::Listbox::EventSelectionChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleexitselected, this));


		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/spex"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleexitplacechanged, this));

		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/spey"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleexitplacechanged, this));

		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/spez"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleexitplacechanged, this));

		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/sizeex"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleexitplacechanged, this));

		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/sizeey"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleexitplacechanged, this));

		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/sizeez"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleexitplacechanged, this));



		static_cast<CEGUI::Combobox *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/newmapex"))
				->subscribeEvent(CEGUI::Combobox::EventListSelectionChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleexitmapelected, this));

		static_cast<CEGUI::Combobox *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/newspax"))
				->subscribeEvent(CEGUI::Combobox::EventListSelectionChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleexitspanelected, this));




		static_cast<CEGUI::PushButton *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/spadd"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&EditorBox::Handleaddexit, this));

		static_cast<CEGUI::PushButton *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/brem"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&EditorBox::Handleremexit, this));




		static_cast<CEGUI::Listbox *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Texts/list"))
			->subscribeEvent(CEGUI::Listbox::EventSelectionChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handletextselected, this));


		static_cast<CEGUI::PushButton *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Texts/badd"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&EditorBox::Handleaddtxt, this));

		static_cast<CEGUI::PushButton *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Texts/brem"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&EditorBox::Handleremtxt, this));

		static_cast<CEGUI::PushButton *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Texts/bupd"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&EditorBox::Handleupdtxt, this));

		{
		CEGUI::Combobox * cbatype = static_cast<CEGUI::Combobox *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/LActors/cbtype"));
			cbatype->addItem(new MyEditorListItem("Static Actor"));
			cbatype->addItem(new MyEditorListItem("Text Actor"));
			cbatype->addItem(new MyEditorListItem("Ladder"));
			cbatype->addItem(new MyEditorListItem("Exit Down"));
			cbatype->addItem(new MyEditorListItem("Door"));
			cbatype->addItem(new MyEditorListItem("Container"));
			cbatype->addItem(new MyEditorListItem("Exit up"));
			cbatype->addItem(new MyEditorListItem("Switch"));
			cbatype->addItem(new MyEditorListItem("Area Switch"));
			cbatype->addItem(new MyEditorListItem("Floor Switch"));
			cbatype->addItem(new MyEditorListItem("Lift Actor"));
			cbatype->addItem(new MyEditorListItem("Hurting zone"));
		}

		{
		CEGUI::Combobox * cbatype = static_cast<CEGUI::Combobox *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/sptyper"));
			cbatype->addItem(new MyEditorListItem("Sprite"));
			cbatype->addItem(new MyEditorListItem("Video"));
			cbatype->addItem(new MyEditorListItem("Ms3d model"));
		}

		static_cast<CEGUI::Listbox *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/LActors/list"))
			->subscribeEvent(CEGUI::Listbox::EventSelectionChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleactorselected, this));




		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/LActors/spex"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleactorchanged, this));

		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/LActors/spey"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleactorchanged, this));

		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/LActors/spez"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleactorchanged, this));

		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/sper"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleactorchanged, this));

		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/LActors/sizeex"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleactorchanged, this));

		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/LActors/sizeey"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleactorchanged, this));

		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/LActors/sizeez"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleactorchanged, this));


		static_cast<CEGUI::Checkbox *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/passable"))
			->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleactorchanged, this));

		static_cast<CEGUI::Checkbox *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/depthm"))
			->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleactorchanged, this));


		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/spsig"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleactorchanged, this));

			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/etars")
				->subscribeEvent(CEGUI::Window::EventTextChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleactorchanged, this));

		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/sounded"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleactorchanged, this));


		static_cast<CEGUI::Checkbox *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/renderercb"))
			->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleactorrendererchanged, this));


		static_cast<CEGUI::Combobox *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/sptyper"))
				->subscribeEvent(CEGUI::Combobox::EventListSelectionChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleactorrendererchanged, this));


		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/sprtarget")
			->subscribeEvent(CEGUI::Window::EventTextChanged,
						CEGUI::Event::Subscriber (&EditorBox::Handleactorrendererchanged, this));



		static_cast<CEGUI::PushButton *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/LActors/spadd"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&EditorBox::Handleactoradded, this));


		static_cast<CEGUI::PushButton *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/LActors/sprem"))->subscribeEvent (
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber (&EditorBox::Handleactorremoved, this));

		HideAllactorviews();



		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/textawin/spactdis"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handletextchanged, this));

		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/textawin/txtidspr"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handletextchanged, this));



		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/switchawin/spactdis"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleswitchchanged, this));


		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/ladderawin/spactdis"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleladderchanged, this));

		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/ladderawin/direct"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleladderchanged, this));

		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/ladderawin/spex"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleladderchanged, this));

		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/ladderawin/spey"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleladderchanged, this));

		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/ladderawin/spez"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleladderchanged, this));



		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/exitactorwin/spactdis"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleexitchanged, this));

		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/exitactorwin/direct"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleexitchanged, this));

		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/exitactorwin/spex"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleexitchanged, this));

		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/exitactorwin/spey"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleexitchanged, this));

		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/exitactorwin/spez"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleexitchanged, this));




	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/zsxsp"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handledoorchanged, this));

	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/zsysp"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handledoorchanged, this));

	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/zszsp"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handledoorchanged, this));


	static_cast<CEGUI::Checkbox *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/cblocked"))
			->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handledoorchanged, this));

	static_cast<CEGUI::Checkbox *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/cbhide"))
			->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handledoorchanged, this));

	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/keyidsp"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handledoorchanged, this));

	static_cast<CEGUI::Checkbox *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/destroykey"))
			->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handledoorchanged, this));


	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/trxsp"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handledoorchanged, this));

	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/trysp"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handledoorchanged, this));

	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/trzsp"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handledoorchanged, this));


	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/spxsp"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handledoorchanged, this));

	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/spysp"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handledoorchanged, this));

	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/spzsp"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handledoorchanged, this));




	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/containeractorwin/zsxsp"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlecontainerchanged, this));

	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/containeractorwin/zsysp"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlecontainerchanged, this));

	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/containeractorwin/zszsp"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlecontainerchanged, this));




	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/areaswitchwin/zsxsp"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleareaswitchchanged, this));

	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/areaswitchwin/zsysp"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleareaswitchchanged, this));

	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/areaswitchwin/zszsp"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handleareaswitchchanged, this));



	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/floorswitchwin/zsxsp"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlefloorswitchchanged, this));

	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/floorswitchwin/zsysp"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlefloorswitchchanged, this));

	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/floorswitchwin/zszsp"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlefloorswitchchanged, this));




	static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/upexitactorwin/spactdis"))
		->subscribeEvent(CEGUI::Spinner::EventValueChanged,
						CEGUI::Event::Subscriber (&EditorBox::Handleupexitchanged, this));

	static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/upexitactorwin/direct"))
		->subscribeEvent(CEGUI::Spinner::EventValueChanged,
						CEGUI::Event::Subscriber (&EditorBox::Handleupexitchanged, this));



	static_cast<CEGUI::PushButton *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/save"))
		->subscribeEvent (CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber (&EditorBox::Handleactorsave, this));



		static_cast<CEGUI::Listbox *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/splist"))
			->subscribeEvent(CEGUI::Listbox::EventSelectionChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlespriteselected, this));


		static_cast<CEGUI::Listbox *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/sppartlist"))
			->subscribeEvent(CEGUI::Listbox::EventSelectionChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlespritepartselected, this));




	static_cast<CEGUI::PushButton *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/addspr"))
		->subscribeEvent (CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber (&EditorBox::Handleaddsprite, this));


	static_cast<CEGUI::PushButton *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/rmbt"))
		->subscribeEvent (CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber (&EditorBox::Handleremovesprite, this));


	static_cast<CEGUI::PushButton *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/AddPartbt"))
		->subscribeEvent (CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber (&EditorBox::Handleaddspritepart, this));


	static_cast<CEGUI::PushButton *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/rmpartbt"))
		->subscribeEvent (CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber (&EditorBox::Handleremovespritepart, this));

	static_cast<CEGUI::PushButton *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/savebt"))
		->subscribeEvent (CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber (&EditorBox::Handlespritesave, this));



	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/bottomLeftX"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlespritechanged, this));
	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/bottomLeftY"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlespritechanged, this));
	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/bottomLeftZ"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlespritechanged, this));

	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/topRightX"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlespritechanged, this));
	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/topRightY"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlespritechanged, this));
	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/topRightZ"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlespritechanged, this));


	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/topLeftX"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlespritechanged, this));
	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/topLeftY"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlespritechanged, this));
	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/topLeftZ"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlespritechanged, this));

	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/bottomRightX"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlespritechanged, this));
	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/bottomRightY"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlespritechanged, this));
	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/bottomRightZ"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlespritechanged, this));



	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/topLeftXText"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlespritechanged, this));
	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/topLeftYText"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlespritechanged, this));

	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/bottomLeftXText"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlespritechanged, this));
	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/bottomLeftYText"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlespritechanged, this));

	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/topRightXText"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlespritechanged, this));
	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/topRightYText"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlespritechanged, this));

	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/bottomRightXText"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlespritechanged, this));
	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/bottomRightYText"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlespritechanged, this));


	CEGUI::MultiColumnList* listscript =
	static_cast<CEGUI::MultiColumnList*> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/liftactorwin/listscript"));
	listscript->addColumn("Type", 0, CEGUI::UDim(0.16f, 0));
	listscript->addColumn("Value A", 1, CEGUI::UDim(0.16f, 0));
	listscript->addColumn("Value B", 2, CEGUI::UDim(0.16f, 0));
	listscript->addColumn("Value C", 3, CEGUI::UDim(0.16f, 0));
	listscript->addColumn("Speed", 4, CEGUI::UDim(0.16f, 0));
	listscript->addColumn("Sound", 5, CEGUI::UDim(0.16f, 0));

	listscript->subscribeEvent(CEGUI::MultiColumnList::EventSelectionChanged,
										CEGUI::Event::Subscriber (&EditorBox::Handleliftscriptsselected, this));

	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/liftactorwin/edtype")
		->subscribeEvent(CEGUI::Window::EventTextChanged,
					CEGUI::Event::Subscriber (&EditorBox::Handleliftscriptschanged, this));
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/liftactorwin/edva")
		->subscribeEvent(CEGUI::Window::EventTextChanged,
					CEGUI::Event::Subscriber (&EditorBox::Handleliftscriptschanged, this));
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/liftactorwin/edvb")
		->subscribeEvent(CEGUI::Window::EventTextChanged,
					CEGUI::Event::Subscriber (&EditorBox::Handleliftscriptschanged, this));
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/liftactorwin/edvc")
		->subscribeEvent(CEGUI::Window::EventTextChanged,
					CEGUI::Event::Subscriber (&EditorBox::Handleliftscriptschanged, this));
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/liftactorwin/edspeed" )
		->subscribeEvent(CEGUI::Window::EventTextChanged,
					CEGUI::Event::Subscriber (&EditorBox::Handleliftscriptschanged, this));
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/liftactorwin/edsound")
		->subscribeEvent(CEGUI::Window::EventTextChanged,
					CEGUI::Event::Subscriber (&EditorBox::Handleliftscriptschanged, this));



	static_cast<CEGUI::PushButton *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/liftactorwin/addb"))
		->subscribeEvent (CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber (&EditorBox::Handleliftscriptsadded, this));


	static_cast<CEGUI::PushButton *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/liftactorwin/remb"))
		->subscribeEvent (CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber (&EditorBox::Handleliftscriptsremoved, this));



	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/hurtareawin/zsxsp"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlehurtareachanged, this));

	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/hurtareawin/zsysp"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlehurtareachanged, this));

	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/hurtareawin/zszsp"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlehurtareachanged, this));

	static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/hurtareawin/lifetaken"))
			->subscribeEvent(CEGUI::Spinner::EventValueChanged,
							CEGUI::Event::Subscriber (&EditorBox::Handlehurtareachanged, this));




		static_cast<CEGUI::TabControl *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol"))->setSelectedTab("EditorWindow/tabcontrol/Maps");
	}
	catch(CEGUI::Exception &ex)
	{
		LogHandler::getInstance()->LogToFile(std::string("Exception init editorbox: ") + ex.getMessage().c_str());
	}


}



/***********************************************************
called when we change the world
***********************************************************/
void EditorBox::ChangeMap(const std::string &newworld, const std::string &newmap)
{
	CEGUI::Listbox * lb = static_cast<CEGUI::Listbox *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/list"));


	CEGUI::Combobox * cb = static_cast<CEGUI::Combobox *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/newmapex"));

	bool neww = false;
	if(_currentworld != newworld)
	{
		neww = true;
		_currentworld = newworld;
		_wi = DataLoader::getInstance()->GetWorldInfo();


		lb->resetList();
		cb->resetList();

		std::map<std::string, MapInfo>::iterator itm = _wi.Maps.begin();
		std::map<std::string, MapInfo>::iterator endm = _wi.Maps.end();
		for(; itm != endm; ++itm)
		{
			lb->addItem(new MyEditorListItem((const unsigned char *)itm->first.c_str()));
			cb->addItem(new MyEditorListItem((const unsigned char *)itm->first.c_str()));
		}

		InitSprites();
	}

	if(neww || _currentmap != newmap)
	{
		_currsp = NULL;
		_currex = NULL;
		_currentmap = newmap;
		CEGUI::ListboxItem* item = lb->findItemWithText(_currentmap, NULL);
		if(item)
			lb->setItemSelectState(item, true);



		CEGUI::Listbox * lbsp = static_cast<CEGUI::Listbox *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Spawnings/list"));
		lbsp->resetList();
		lbsp->setText("");

		CEGUI::Listbox * lbex = static_cast<CEGUI::Listbox *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/list"));
		lbex->resetList();
		lbex->setText("");

		CEGUI::Listbox * lbtxt = static_cast<CEGUI::Listbox *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Texts/list"));
		lbtxt->resetList();
		lbtxt->setText("");

		std::map<std::string, MapInfo>::iterator itm = _wi.Maps.find(_currentmap);
		if(itm != _wi.Maps.end())
		{
			std::map<std::string, SpawningInfo> & sps = _wi.Maps[_currentmap].Spawnings;
			std::map<std::string, SpawningInfo>::iterator its = sps.begin();
			std::map<std::string, SpawningInfo>::iterator ends = sps.end();
			for(int cc=0; its != ends; ++its, ++cc)
			{
				MyEditorListItem * item2 = new MyEditorListItem((const unsigned char *)its->first.c_str());
				lbsp->addItem(item2);
				if(cc == 0)
					lbsp->setItemSelectState(item2, true);
			}

			std::map<std::string, ExitInfo> & spex = _wi.Maps[_currentmap].Exits;
			std::map<std::string, ExitInfo>::iterator itex = spex.begin();
			std::map<std::string, ExitInfo>::iterator endex = spex.end();
			for(int cc=0; itex != endex; ++itex, ++cc)
			{
				MyEditorListItem * item = new MyEditorListItem((const unsigned char *)itex->first.c_str());
				lbex->addItem(item);
				if(cc == 0)
					lbex->setItemSelectState(item, true);
			}

			_txts.clear();
			_ctxtfile = DataLoader::getInstance()->gettextfile();

			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Texts/textfile")
				->setText(_ctxtfile);
			//_ctxtfile = "Data/" + itm->second.Files["Texts"];
			MapInfoXmlReader::GetAllTexts(_ctxtfile, _txts);
			std::map<long, std::string>::iterator ittxt = _txts.begin();
			std::map<long, std::string>::iterator endtxt = _txts.end();
			for(int cc=0; ittxt != endtxt; ++ittxt, ++cc)
			{
				char buff[255];
				long id = ittxt->first;
				ltoa(id, buff, 10);
				MyEditorListItem * item = new MyEditorListItem(buff);
				lbtxt->addItem(item);
				if(cc == 0)
					lbtxt->setItemSelectState(item, true);
			}


			CEGUI::Listbox * lbact = static_cast<CEGUI::Listbox *> (
					CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/LActors/list"));
			lbact->resetList();
			lbact->setText("");

			std::set<long> aids;

			if(_Lactors)
			{
				std::map<long, Actor *>::iterator it = _Lactors->begin();
				std::map<long, Actor *>::iterator end = _Lactors->end();
				for(; it != end; ++it)
					aids.insert(it->first);
			}

			if(_Eactors)
			{
				std::map<long, Actor *>::iterator it = _Eactors->begin();
				std::map<long, Actor *>::iterator end = _Eactors->end();
				for(; it != end; ++it)
					aids.insert(it->first);
			}

			{
				std::set<long>::iterator it = aids.begin();
				std::set<long>::iterator end = aids.end();
				for(int cc=0; it != end; ++it, ++cc)
				{
					char buff[255];
					ltoa(*it, buff, 10);

					MyEditorListItem * item = new MyEditorListItem(buff);
					lbact->addItem(item);
					if(cc == 0)
						lbact->setItemSelectState(item, true);
				}
			}
		}
	}

}


/***********************************************************
handle event when list is selected
***********************************************************/
bool EditorBox::HandleLbmapSelected (const CEGUI::EventArgs& e)
{
	CEGUI::Listbox * lb = static_cast<CEGUI::Listbox *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/list"));

	const CEGUI::ListboxTextItem * it = static_cast<const CEGUI::ListboxTextItem *>(lb->getFirstSelectedItem());

	if(it)
	{
		CEGUI::Combobox * cb = static_cast<CEGUI::Combobox *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/combosp"));
		cb->resetList();
		cb->setText("");


		_selectedmap = it->getText().c_str();
		std::map<std::string, MapInfo>::iterator itm = _wi.Maps.find(_selectedmap);
		if(itm != _wi.Maps.end())
		{
			std::map<std::string, SpawningInfo> & sps = _wi.Maps[_selectedmap].Spawnings;
			std::map<std::string, SpawningInfo>::iterator its = sps.begin();
			std::map<std::string, SpawningInfo>::iterator ends = sps.end();
			for(int cc=0; its != ends; ++its, ++cc)
			{
				MyEditorListItem * item = new MyEditorListItem((const unsigned char *)its->first.c_str());
				cb->addItem(item);
				if(cc == 0)
					cb->setItemSelectState(item, true);
			}


			if(sps.size() == 0)
			{
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/editX")->setText("0");
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/editY")->setText("0");
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/editZ")->setText("0");
			}

			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/editname")
					->setText(itm->first);

			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/editmusic")
				->setText(itm->second.Music);

			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/edtmapdata")
					->setText(itm->second.Files["Maps"]);

			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/edtlocalA")
					->setText(itm->second.Files["LocalActors"]);

			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/edtexta")
					->setText(itm->second.Files["ExternalActors"]);

			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/edttext")
					->setText(itm->second.Files["Texts"]);

			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/edtdesc")
					->setText(itm->second.Description);

			static_cast<CEGUI::Checkbox *>
				(CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/interior"))
				->setSelected(itm->second.Type == "interior");

			static_cast<CEGUI::Spinner *>
				(CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/mloop"))
				->setCurrentValue((float)itm->second.MusicLoop);

		}

	}

	return true;
}


/***********************************************************
handle event when list is selected
***********************************************************/
bool EditorBox::HandleLbspawningSelected (const CEGUI::EventArgs& e)
{
	CEGUI::Combobox * cb = static_cast<CEGUI::Combobox *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/combosp"));

	const CEGUI::ListboxTextItem * it = static_cast<const CEGUI::ListboxTextItem *>(cb->getSelectedItem());

	if(it)
	{
		std::string selsp = it->getText().c_str();

		std::map<std::string, MapInfo>::iterator itm = _wi.Maps.find(_selectedmap);
		if(itm != _wi.Maps.end())
		{

			std::map<std::string, SpawningInfo>::iterator its = itm->second.Spawnings.find(selsp);

			if(its != itm->second.Spawnings.end())
			{
				{
				std::stringstream strs;
				strs << its->second.PosX;
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/editX")
					->setText(strs.str());
				}

				{
				std::stringstream strs;
				strs << its->second.PosY;
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/editY")
					->setText(strs.str());
				}

				{
				std::stringstream strs;
				strs << its->second.PosZ;
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/editZ")
					->setText(strs.str());
				}
			}
		}


	}

	return true;
}


/***********************************************************
handle event when list is selected
***********************************************************/
bool EditorBox::HandleBokmap (const CEGUI::EventArgs& e)
{
	if(_selectedmap != "")
	{
		std::string strx = CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/editX")
				->getText().c_str();

		std::string stry = CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/editY")
			->getText().c_str();

		std::string strz = CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/editZ")
			->getText().c_str();

		float x = (float)atof(strx.c_str());
		float y = (float)atof(stry.c_str());
		float z = (float)atof(strz.c_str());

		ThreadSafeWorkpile::getInstance()->AddEvent(new TeleportAnyEvent(_selectedmap, x, y, z));
	}

	return true;
}


/***********************************************************
handle event when list is selected
***********************************************************/
bool EditorBox::Handlebcreatemap (const CEGUI::EventArgs& e)
{
	std::string name = CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/editname")
			->getText().c_str();

	std::string music = CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/editmusic")
			->getText().c_str();

	std::string mapdata = CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/edtmapdata")
			->getText().c_str();

	std::string lactors = CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/edtlocalA")
			->getText().c_str();

	std::string eactors = CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/edtexta")
			->getText().c_str();

	std::string texts = CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/edttext")
			->getText().c_str();

	std::string descs = CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/edtdesc")
			->getText().c_str();

	bool interior = static_cast<CEGUI::Checkbox *>
		(CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/interior"))->isSelected();

	int mloop = (int)static_cast<CEGUI::Spinner *>
		(CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/mloop"))->getCurrentValue();


	if(name != "")
	{
		if(eactors.find(".xml") != std::string::npos)
		{
			if(!std::ifstream(("Data/"+eactors).c_str()))
			{
				std::ofstream file(("Data/"+eactors).c_str())	;
				file<<"<actors>"<<std::endl<<"</actors>"<<std::endl;
				file.close();
			}

			if(lactors.find(".xml") != std::string::npos)
			{
				if(!std::ifstream(("Data/"+lactors).c_str()))
				{
					std::ofstream file(("Data/"+lactors).c_str())	;
					file<<"<actors>"<<std::endl<<"</actors>"<<std::endl;
					file.close();
				}

				if(texts.find(".xml") != std::string::npos)
				{
					//if(!std::ifstream(("Data/"+texts).c_str()))
					//{
					//	std::ofstream file(("Data/"+texts).c_str())	;
					//	file<<"<texts>"<<std::endl<<"</texts>"<<std::endl;
					//	file.close();
					//}

					MapInfo mi;
					if(_wi.Maps.find(name) == _wi.Maps.end())
					{
						CEGUI::Listbox * lb = static_cast<CEGUI::Listbox *> (
							CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Maps/list"));
						lb->addItem(new MyEditorListItem((const unsigned char *)name.c_str()));
					}
					else
					{
						mi.Spawnings = _wi.Maps[name].Spawnings;
						mi.Exits = _wi.Maps[name].Exits;
						mi.Lights = _wi.Maps[name].Lights;
						mi.Files = _wi.Maps[name].Files;
					}

					mi.Name = name;
					mi.Description = descs;
					mi.Type = interior ? "interior" : "exterior";
					mi.Music = music;
					mi.MusicLoop = mloop;
					mi.Files["ExternalActors"] = eactors;
					mi.Files["LocalActors"] = lactors;
					mi.Files["Maps"] = mapdata;
					mi.Files["Texts"] = texts;
					_wi.Maps[name] = mi;

					DataLoader::getInstance()->SaveCurrentWorld();
				}
			}
		}
	}

	return true;
}


/***********************************************************
handle event when list is selected
***********************************************************/
bool EditorBox::Handlespawningselected (const CEGUI::EventArgs& e)
{
	CEGUI::Listbox * lb = static_cast<CEGUI::Listbox *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Spawnings/list"));

	const CEGUI::ListboxTextItem * it = static_cast<const CEGUI::ListboxTextItem *>(lb->getFirstSelectedItem());

	if(it)
	{
		std::map<std::string, SpawningInfo>::iterator itsp = _wi.Maps[_currentmap].Spawnings.find(it->getText().c_str());
		if(itsp != _wi.Maps[_currentmap].Spawnings.end())
		{
			_currsp = &(itsp->second);
			float x = _currsp->PosX;
			float y = _currsp->PosY;
			float z = _currsp->PosZ;

			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Spawnings/spname")
				->setText(_currsp->Name);

			static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Spawnings/spex"))
				->setCurrentValue(x);


			static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Spawnings/spey"))
				->setCurrentValue(y);

			static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Spawnings/spez"))
				->setCurrentValue(z);
		}
		else
			_currsp = NULL;
	}

	return true;
}


/***********************************************************
handle event when list is selected
***********************************************************/
bool EditorBox::Handlespawningplacechanged (const CEGUI::EventArgs& e)
{
	if(_currsp)
	{
		_currsp->PosX = static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Spawnings/spex"))
			->getCurrentValue();


		_currsp->PosY = static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Spawnings/spey"))
			->getCurrentValue();

		_currsp->PosZ = static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Spawnings/spez"))
			->getCurrentValue();
	}


	return true;
}


/***********************************************************
handle event when list is selected
***********************************************************/
bool EditorBox::Handlesaveb (const CEGUI::EventArgs& e)
{
	DataLoader::getInstance()->SaveCurrentWorld();
	return true;
}



/***********************************************************
handle event when list is selected
***********************************************************/
bool EditorBox::Handleaddspaw (const CEGUI::EventArgs& e)
{
	std::string spname =
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Spawnings/spnameadd")
		->getText().c_str();
	if(spname == "")
		return true;

	if(_wi.Maps[_currentmap].Spawnings.find(spname) == _wi.Maps[_currentmap].Spawnings.end())
	{
		CEGUI::Listbox * lbsp = static_cast<CEGUI::Listbox *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Spawnings/list"));

		MyEditorListItem * item2 = new MyEditorListItem((const unsigned char *)spname.c_str());
		lbsp->addItem(item2);

		_currsp = &(_wi.Maps[_currentmap].Spawnings[spname.c_str()]);
		_currsp->Name = spname.c_str();
		_currsp->Rotation = 0;
		_currsp->PosX = 0;
		_currsp->PosY = 0;
		_currsp->PosZ = 0;


		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Spawnings/spex"))
			->setCurrentValue(0);


		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Spawnings/spey"))
			->setCurrentValue(0);

		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Spawnings/spez"))
			->setCurrentValue(0);

		lbsp->setItemSelectState(item2, true);
	}

	return true;
}



/***********************************************************
handle event when list is selected
***********************************************************/
bool EditorBox::Handleremspaw (const CEGUI::EventArgs& e)
{
	if(!_currsp)
		return true;

	_wi.Maps[_currentmap].Spawnings.erase(_currsp->Name);
	_currsp = NULL;

	CEGUI::Listbox * lbsp = static_cast<CEGUI::Listbox *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Spawnings/list"));

	lbsp->removeItem(lbsp->getFirstSelectedItem());

	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Spawnings/spname")
		->setText("");

	if(lbsp->getItemCount() > 0)
		lbsp->setItemSelectState(lbsp->getListboxItemFromIndex(0), true);

	return true;
}


/***********************************************************
handle event when list is selected
***********************************************************/
bool EditorBox::Handleexitselected (const CEGUI::EventArgs& e)
{
	CEGUI::Listbox * lb = static_cast<CEGUI::Listbox *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/list"));

	const CEGUI::ListboxTextItem * it = static_cast<const CEGUI::ListboxTextItem *>(lb->getFirstSelectedItem());

	if(it)
	{
		std::map<std::string, ExitInfo>::iterator itsp = _wi.Maps[_currentmap].Exits.find(it->getText().c_str());
		if(itsp != _wi.Maps[_currentmap].Exits.end())
		{
			_currex = &(itsp->second);
			float x = _currex->BottomLeftX;
			float y = _currex->BottomLeftY;
			float z = _currex->BottomLeftZ;

			float sx = _currex->TopRightX-x;
			float sy = _currex->TopRightY-y;
			float sz = _currex->TopRightZ-z;


			std::string	NewMap = _currex->NewMap;
			std::string	Spawning = _currex->Spawning;

			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/name")
				->setText(_currex->Name);

			static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/spex"))
				->setCurrentValue(x);

			static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/spey"))
				->setCurrentValue(y);

			static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/spez"))
				->setCurrentValue(z);


			static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/sizeex"))
				->setCurrentValue(sx);

			static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/sizeey"))
				->setCurrentValue(sy);

			static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/sizeez"))
				->setCurrentValue(sz);

			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/newmapex")
				->setText(NewMap);

			CEGUI::Combobox * cbnm = static_cast<CEGUI::Combobox *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/newmapex"));
			CEGUI::ListboxItem * lbi = cbnm->findItemWithText(NewMap, NULL);
			if(lbi)	cbnm->setItemSelectState(lbi, true);


			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/newspax")
				->setText(Spawning);

			CEGUI::Combobox * cbnm2 = static_cast<CEGUI::Combobox *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/newspax"));
			CEGUI::ListboxItem * lbi2 = cbnm2->findItemWithText(Spawning, NULL);
			if(lbi2) cbnm2->setItemSelectState(lbi2, true);

		}

		else
			_currex = NULL;
	}


	return true;
}


/***********************************************************
handle event when list is selected
***********************************************************/
bool EditorBox::Handleexitplacechanged (const CEGUI::EventArgs& e)
{
	if(_currex)
	{
		_currex->BottomLeftX = static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/spex"))
			->getCurrentValue();

		_currex->BottomLeftY = static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/spey"))
			->getCurrentValue();

		_currex->BottomLeftZ = static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/spez"))
			->getCurrentValue();


		_currex->TopRightX = _currex->BottomLeftX + static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/sizeex"))
			->getCurrentValue();

		_currex->TopRightY = _currex->BottomLeftY + static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/sizeey"))
			->getCurrentValue();

		_currex->TopRightZ = _currex->BottomLeftZ + static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/sizeez"))
			->getCurrentValue();
	}

	return true;
}


/***********************************************************
handle event when list is selected
***********************************************************/
bool EditorBox::Handleexitmapelected (const CEGUI::EventArgs& e)
{
	CEGUI::Combobox * cb2 = static_cast<CEGUI::Combobox *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/newmapex"));
	const CEGUI::ListboxTextItem * it = static_cast<const CEGUI::ListboxTextItem *>(cb2->getSelectedItem());

	CEGUI::Combobox * cb = static_cast<CEGUI::Combobox *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/newspax"));

	cb->resetList();

	if(it)
	{
		std::string selmap = it->getText().c_str();
		_currex->NewMap = selmap;

		std::map<std::string, MapInfo>::iterator itm = _wi.Maps.find(selmap);
		if(itm != _wi.Maps.end())
		{
			std::map<std::string, SpawningInfo> & sps = _wi.Maps[selmap].Spawnings;
			std::map<std::string, SpawningInfo>::iterator its = sps.begin();
			std::map<std::string, SpawningInfo>::iterator ends = sps.end();
			for(int cc=0; its != ends; ++its, ++cc)
			{
				MyEditorListItem * item2 = new MyEditorListItem((const unsigned char *)its->first.c_str());
				cb->addItem(item2);
				if(cc == 0)
					cb->setItemSelectState(item2, true);
			}
		}
	}

	return true;
}

/***********************************************************
handle event when list is selected
***********************************************************/
bool EditorBox::Handleexitspanelected (const CEGUI::EventArgs& e)
{
	CEGUI::Combobox * cb = static_cast<CEGUI::Combobox *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/newspax"));
	const CEGUI::ListboxTextItem * it = static_cast<const CEGUI::ListboxTextItem *>(cb->getSelectedItem());
	if(it)
	{
		std::string selsp = it->getText().c_str();
		_currex->Spawning = selsp;
	}

	return true;
}



/***********************************************************
handle event when list is selected
***********************************************************/
bool EditorBox::Handleaddexit (const CEGUI::EventArgs& e)
{
	std::string spname =
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/spnameadd")
		->getText().c_str();
	if(spname == "")
		return true;

	if(_wi.Maps[_currentmap].Exits.find(spname) == _wi.Maps[_currentmap].Exits.end())
	{
		CEGUI::Listbox * lbsp = static_cast<CEGUI::Listbox *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/list"));

		MyEditorListItem * item2 = new MyEditorListItem((const unsigned char *)spname.c_str());
		lbsp->addItem(item2);

		_currex = &(_wi.Maps[_currentmap].Exits[spname.c_str()]);
		_currex->Name = spname.c_str();
		_currex->BottomLeftX = 0;
		_currex->BottomLeftY = 0;
		_currex->BottomLeftZ = 0;
		_currex->TopRightX = 0;
		_currex->TopRightY = 0;
		_currex->TopRightZ = 0;


		lbsp->setItemSelectState(item2, true);
	}

	return true;
}

/***********************************************************
handle event when list is selected
***********************************************************/
bool EditorBox::Handleremexit (const CEGUI::EventArgs& e)
{
	if(!_currex)
		return true;

	_wi.Maps[_currentmap].Exits.erase(_currex->Name);
	_currex = NULL;

	CEGUI::Listbox * lbsp = static_cast<CEGUI::Listbox *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/list"));

	lbsp->removeItem(lbsp->getFirstSelectedItem());

	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Exits/name")
		->setText("");

	if(lbsp->getItemCount() > 0)
		lbsp->setItemSelectState(lbsp->getListboxItemFromIndex(0), true);

	return true;
}



/***********************************************************
handle event when list is selected
***********************************************************/
bool EditorBox::Handletextselected (const CEGUI::EventArgs& e)
{
	CEGUI::Listbox * lb = static_cast<CEGUI::Listbox *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Texts/list"));

	const CEGUI::ListboxTextItem * it = static_cast<const CEGUI::ListboxTextItem *>(lb->getFirstSelectedItem());

	if(it)
	{
		std::string id = it->getText().c_str();
		_seltxtid = atol(id.c_str());
		std::map<long, std::string>::iterator it = _txts.find(_seltxtid);
		if(it != _txts.end())
		{
			CEGUI::MultiLineEditbox * txs =
			static_cast<CEGUI::MultiLineEditbox *>
			(CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Texts/txt"));


			std::string str = it->second;
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
		}
	}

	return true;
}

/***********************************************************
handle event when list is selected
***********************************************************/
bool EditorBox::Handleaddtxt (const CEGUI::EventArgs& e)
{
	long newid = 0;
	if(_txts.size() > 0)
		newid = _txts.rbegin()->first + 1;

	_txts[newid] = "";

	CEGUI::Listbox * lbtxt = static_cast<CEGUI::Listbox *> (
					CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Texts/list"));
	char buff[255];
	ltoa(newid, buff, 10);
	MyEditorListItem * item = new MyEditorListItem(buff);
	lbtxt->addItem(item);
	lbtxt->setItemSelectState(item, true);

	MapInfoXmlWriter::SaveTexts(_ctxtfile, _txts);
	return true;
}

/***********************************************************
handle event when list is selected
***********************************************************/
bool EditorBox::Handleremtxt (const CEGUI::EventArgs& e)
{
	std::map<long, std::string>::iterator it = _txts.find(_seltxtid);
	if(it != _txts.end())
	{
		_txts.erase(it);

		CEGUI::Listbox * lbtxt = static_cast<CEGUI::Listbox *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Texts/list"));

		lbtxt->removeItem(lbtxt->getFirstSelectedItem());
		if(lbtxt->getItemCount() > 0)
			lbtxt->setItemSelectState(lbtxt->getListboxItemFromIndex(0), true);
	}

	MapInfoXmlWriter::SaveTexts(_ctxtfile, _txts);
	return true;
}

/***********************************************************
handle event when list is selected
***********************************************************/
bool EditorBox::Handleupdtxt (const CEGUI::EventArgs& e)
{
	std::map<long, std::string>::iterator it = _txts.find(_seltxtid);
	if(it != _txts.end())
	{
		CEGUI::MultiLineEditbox * txs =
		static_cast<CEGUI::MultiLineEditbox *>
		(CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Texts/txt"));

		std::string str = txs->getText().c_str();
		std::string res;
		int idxs = 0;
		bool firsttime=true;
		while((idxs = str.find("\n")) != std::string::npos)
		{
			if(firsttime)
			{
				firsttime = false;
				res = str.substr(0, idxs);
			}
			else
				res += " @ " + str.substr(0, idxs);
			str = str.substr(idxs+1);
		}

		it->second = res;
	}

	MapInfoXmlWriter::SaveTexts(_ctxtfile, _txts);
	return true;
}


/***********************************************************
set actors
***********************************************************/
void EditorBox::SetActors(std::map<long, Actor *> * Lactors, std::map<long, Actor *> * Eactors)
{
	_Lactors = Lactors;
	_Eactors = Eactors;
}


/***********************************************************
handle event when list is selected
***********************************************************/
bool EditorBox::Handleactorselected (const CEGUI::EventArgs& e)
{
	HideAllactorviews();

	CEGUI::Listbox * lb = static_cast<CEGUI::Listbox *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/LActors/list"));

	const CEGUI::ListboxTextItem * it = static_cast<const CEGUI::ListboxTextItem *>(lb->getFirstSelectedItem());

	std::string id = "";
	if(it)
	{
		id = it->getText().c_str();
		long idl = atol(id.c_str());
		std::map<long, Actor *>::iterator ita = _Lactors->find(idl);
		if(ita == _Lactors->end())
		{
			ita = _Eactors->find(idl);
			if(ita != _Eactors->end())
				_selActor = ita->second;
			else
				_selActor = NULL;
		}
		else
			_selActor = ita->second;
	}
	else
		_selActor = NULL;

	if(_selActor)
	{
		long type = _selActor->GetType();
		bool dm = _selActor->IsDepthMask();
		bool pass = _selActor->IsPassable();
		float x = _selActor->GetPosX();
		float y = _selActor->GetPosY();
		float z = _selActor->GetPosZ();
		float sx = _selActor->GetSizeX();
		float sy = _selActor->GetSizeY();
		float sz = _selActor->GetSizeZ();
		float r = _selActor->GetRotation();
		long sig = _selActor->GetSignal();
		D3ObjectRenderer * renderer = _selActor->GetRenderer();
		long rtype = _selActor->GetRendererType();
		long sound = _selActor->GetAttachedSound();

		std::vector<long> rtargetv = _selActor->GetRendererTarget();
		std::stringstream strrta;
		if(rtargetv.size() > 0)
			strrta<<rtargetv[0];
		for(size_t i=1; i<rtargetv.size(); ++i)
			strrta<<","<<rtargetv[i];

		std::vector<long> vect = _selActor->GetTargets();
		std::stringstream str;
		if(vect.size() > 0)
			str<<vect[0];

		for(size_t i=1; i<vect.size(); ++i)
			str<<","<<vect[i];



		std::string types = "";
		switch(type)
		{
			case 0:
				types = "Static Actor";
			break;
			case 1:
				types = "Text Actor";
			break;
			case 2:
				types = "Ladder";
			break;
			case 3:
				types = "Exit Down";
			break;
			case 4:
				types = "Door";
			break;
			case 5:
				types = "Container";
			break;
			case 6:
				types = "Exit up";
			break;
			case 7:
				types = "Switch";
			break;
			case 8:
				types = "Area Switch";
			break;
			case 9:
				types = "Floor Switch";
			break;
			case 10:
				types = "Lift Actor";
			break;
			case 11:
				types = "Hurting zone";
			break;
		}






		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/LActors/spname")
			->setText(id);

		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/stltype")
		->setText(types);

		static_cast<CEGUI::Checkbox *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/depthm"))
		->setSelected(dm);

		static_cast<CEGUI::Checkbox *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/passable"))
		->setSelected(!pass);


		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/LActors/spex"))
			->setCurrentValue(x);
		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/LActors/spey"))
			->setCurrentValue(y);
		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/LActors/spez"))
			->setCurrentValue(z);
		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/sper"))
			->setCurrentValue(r);

		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/LActors/sizeex"))
			->setCurrentValue(sx);
		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/LActors/sizeey"))
			->setCurrentValue(sy);
		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/LActors/sizeez"))
			->setCurrentValue(sz);

		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/spsig"))
			->setCurrentValue((float)sig);

		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/sounded"))
			->setCurrentValue((float)sound);



		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/etars")
			->setText(str.str());



		static_cast<CEGUI::Checkbox *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/renderercb"))
		->setSelected((renderer != NULL));



		CEGUI::Combobox * cbnm = static_cast<CEGUI::Combobox *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/sptyper"));
		if(rtype >=0)
		{
			CEGUI::ListboxItem * lbi = cbnm->getListboxItemFromIndex(rtype);
			if(lbi)	cbnm->setItemSelectState(lbi, true);
		}
		else
			cbnm->setText("");

		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/sprtarget")
		->setText(strrta.str());


		switch(type)
		{

			case 1:
				{
					TextActor * tmpa = static_cast<TextActor *>(_selActor);
					float ad = tmpa->GetActivationDistance();
					long tid = tmpa->GetTextId();

					static_cast<CEGUI::Spinner *> (
					CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/textawin/spactdis"))
					->setCurrentValue(ad);

					static_cast<CEGUI::Spinner *> (
					CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/textawin/txtidspr"))
					->setCurrentValue((float)tid);

					CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/textawin")->show();
				}
			break;
			case 2:
				{
				LadderActor * tmpa = static_cast<LadderActor *>(_selActor);

				float ad = tmpa->GetActivationDistance();
				float dx = tmpa->GetDX();
				float dy = tmpa->GetDY();
				float dz = tmpa->GetDZ();
				int dir = tmpa->GetDir();

				static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/ladderawin/spactdis"))
				->setCurrentValue(ad);

				static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/ladderawin/direct"))
				->setCurrentValue((float)dir);

				static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/ladderawin/spex"))
				->setCurrentValue(dx);

				static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/ladderawin/spey"))
				->setCurrentValue(dy);

				static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/ladderawin/spez"))
				->setCurrentValue(dz);

				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/ladderawin")->show();
				}
			break;
			case 3:
				{
				ExitActor * tmpa = static_cast<ExitActor *>(_selActor);

				float ad = tmpa->GetActivationDistance();
				float dx = tmpa->GetDX();
				float dy = tmpa->GetDY();
				float dz = tmpa->GetDZ();
				int dir = tmpa->GetDir();

				static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/exitactorwin/spactdis"))
				->setCurrentValue(ad);

				static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/exitactorwin/direct"))
				->setCurrentValue((float)dir);

				static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/exitactorwin/spex"))
				->setCurrentValue(dx);

				static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/exitactorwin/spey"))
				->setCurrentValue(dy);

				static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/exitactorwin/spez"))
				->setCurrentValue(dz);

				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/exitactorwin")->show();
				}
			break;
			case 4:
				{
				DoorActor * tmpa = static_cast<DoorActor *>(_selActor);

				float zx = tmpa->GetZoneX();
				float zy = tmpa->GetZoneY();
				float zz = tmpa->GetZoneZ();

				bool locked = tmpa->GetLocked();
				long keyid = tmpa->GetKeyId();
				bool destroyk = tmpa->GetDesKey();
				bool hide = tmpa->GetHide();

				float dtx = tmpa->GetOpenTransX();
				float dty = tmpa->GetOpenTransY();
				float dtz = tmpa->GetOpenTransZ();
				float spx = tmpa->GetOpenTransSpeedX();
				float spy = tmpa->GetOpenTransSpeedY();
				float spz = tmpa->GetOpenTransSpeedZ();

				static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/zsxsp"))
				->setCurrentValue(zx);

				static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/zsysp"))
				->setCurrentValue(zy);

				static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/zszsp"))
				->setCurrentValue(zz);


				static_cast<CEGUI::Checkbox *> (
					CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/cblocked"))
				->setSelected((locked));

				static_cast<CEGUI::Checkbox *> (
					CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/cbhide"))
				->setSelected((hide));

				static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/keyidsp"))
				->setCurrentValue((float)keyid);

				static_cast<CEGUI::Checkbox *> (
					CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/destroykey"))
				->setSelected((destroyk));


				static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/trxsp"))
				->setCurrentValue(dtx);

				static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/trysp"))
				->setCurrentValue(dty);

				static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/trzsp"))
				->setCurrentValue(dtz);


				static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/spxsp"))
				->setCurrentValue(spx);

				static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/spysp"))
				->setCurrentValue(spy);

				static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/spzsp"))
				->setCurrentValue(spz);


				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin")->show();
				}
			break;

			case 5: //container
				{
				ContainerActor * tmpa = static_cast<ContainerActor *>(_selActor);

				float zx = tmpa->GetZoneX();
				float zy = tmpa->GetZoneY();
				float zz = tmpa->GetZoneZ();

				static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/containeractorwin/zsxsp"))
				->setCurrentValue(zx);

				static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/containeractorwin/zsysp"))
				->setCurrentValue(zy);

				static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/containeractorwin/zszsp"))
				->setCurrentValue(zz);

				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/containeractorwin")->show();
				}
			break;

			case 6: // up exit
				{
				UpExitActor * tmpa = static_cast<UpExitActor *>(_selActor);

				float ad = tmpa->GetActivationDistance();
				int dir = tmpa->GetDir();

				static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/upexitactorwin/spactdis"))
				->setCurrentValue(ad);

				static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/upexitactorwin/direct"))
				->setCurrentValue((float)dir);

				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/upexitactorwin")->show();
				}
			break;

			case 7: // switch
				{
					SwitchActor * tmpa = static_cast<SwitchActor *>(_selActor);
					float ad = tmpa->GetActivationDistance();

					static_cast<CEGUI::Spinner *> (
					CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/switchawin/spactdis"))
					->setCurrentValue(ad);

					CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/switchawin")->show();
				}
			break;

			case 8: //area switch
				{
				AreaSwitch * tmpa = static_cast<AreaSwitch *>(_selActor);

				float zx = tmpa->GetZoneX();
				float zy = tmpa->GetZoneY();
				float zz = tmpa->GetZoneZ();

				static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/areaswitchwin/zsxsp"))
				->setCurrentValue(zx);

				static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/areaswitchwin/zsysp"))
				->setCurrentValue(zy);

				static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/areaswitchwin/zszsp"))
				->setCurrentValue(zz);

				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/areaswitchwin")->show();
				}
			break;

			case 9: //floor switch
				{
				FloorSwitch * tmpa = static_cast<FloorSwitch *>(_selActor);

				float zx = tmpa->GetZoneX();
				float zy = tmpa->GetZoneY();
				float zz = tmpa->GetZoneZ();

				static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/floorswitchwin/zsxsp"))
				->setCurrentValue(zx);

				static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/floorswitchwin/zsysp"))
				->setCurrentValue(zy);

				static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/floorswitchwin/zszsp"))
				->setCurrentValue(zz);

				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/floorswitchwin")->show();
				}
			break;

			case 10: //lift actor
				{
				ScriptableActor * tmpa = static_cast<ScriptableActor *>(_selActor);

				CEGUI::MultiColumnList* listscript =
				static_cast<CEGUI::MultiColumnList*> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/liftactorwin/listscript"));
				listscript->resetList();


				const std::vector<PlayerScriptPart> & script = tmpa->GetScripts();
				for(size_t i=0; i<script.size(); ++i)
				{
					listscript->addRow();

					{
					std::stringstream strs;
					strs<<script[i].Type;
					MyEditorListItem *itemMultiColumnList = new MyEditorListItem(strs.str());
					listscript->setItem(itemMultiColumnList, 0, i);
					}
					{
					std::stringstream strs;
					strs<<script[i].ValueA;
					MyEditorListItem *itemMultiColumnList = new MyEditorListItem(strs.str());
					listscript->setItem(itemMultiColumnList, 1, i);
					}
					{
					std::stringstream strs;
					strs<<script[i].ValueB;
					MyEditorListItem *itemMultiColumnList = new MyEditorListItem(strs.str());
					listscript->setItem(itemMultiColumnList, 2, i);
					}
					{
					std::stringstream strs;
					strs<<script[i].ValueC;
					MyEditorListItem *itemMultiColumnList = new MyEditorListItem(strs.str());
					listscript->setItem(itemMultiColumnList, 3, i);
					}
					{
					std::stringstream strs;
					strs<<script[i].Speed;
					MyEditorListItem *itemMultiColumnList = new MyEditorListItem(strs.str());
					listscript->setItem(itemMultiColumnList, 4, i);
					}
					{
					std::stringstream strs;
					strs<<script[i].Sound;
					MyEditorListItem *itemMultiColumnList = new MyEditorListItem(strs.str());
					listscript->setItem(itemMultiColumnList, 5, i);
					}
				}

				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/liftactorwin")->show();
				}
			break;

			case 11: //hurt zone
				{
				HurtArea * tmpa = static_cast<HurtArea *>(_selActor);

				float zx = tmpa->GetZoneX();
				float zy = tmpa->GetZoneY();
				float zz = tmpa->GetZoneZ();
				int lifetaken = tmpa->GetLifeTaken();

				static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/hurtareawin/zsxsp"))
				->setCurrentValue(zx);

				static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/hurtareawin/zsysp"))
				->setCurrentValue(zy);

				static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/hurtareawin/zszsp"))
				->setCurrentValue(zz);

				static_cast<CEGUI::Spinner *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/hurtareawin/lifetaken"))
				->setCurrentValue((float)lifetaken);

				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/hurtareawin")->show();
				}
			break;
		}


	}

	return true;
}


/***********************************************************
handle event when list is selected
***********************************************************/
bool EditorBox::Handleactorchanged (const CEGUI::EventArgs& e)
{
	if(!_selActor)
		return true;


	_selActor->SetDepthMask(static_cast<CEGUI::Checkbox *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/depthm"))
		->isSelected());

	_selActor->SetPassable(!static_cast<CEGUI::Checkbox *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/passable"))
		->isSelected());


	_selActor->SetPosition(static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/LActors/spex"))
		->getCurrentValue(),
	static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/LActors/spey"))
		->getCurrentValue(),
	static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/LActors/spez"))
		->getCurrentValue());

	_selActor->SetRotation(
	static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/sper"))
		->getCurrentValue());

	_selActor->SetSize(
	static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/LActors/sizeex"))
		->getCurrentValue(),
	static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/LActors/sizeey"))
		->getCurrentValue(),
	static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/LActors/sizeez"))
		->getCurrentValue());

	_selActor->SetAttachedSound((long)
		static_cast<CEGUI::Spinner *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/sounded"))
			->getCurrentValue());



	long sigo = (long)static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/spsig"))
		->getCurrentValue();

	std::string stars = CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/etars")
		->getText().c_str();

	std::vector<long> _targets;
	std::vector<std::string> tokens;
	StringTokenize(stars, tokens, ",");
	for(size_t i=0; i<tokens.size(); ++i)
		_targets.push_back(atol(tokens[i].c_str()));

	_selActor->SetOutputSignal(sigo, _targets);


	return true;
}


/***********************************************************
handle event when list is selected
***********************************************************/
bool EditorBox::Handleactorrendererchanged (const CEGUI::EventArgs& e)
{
	if(!_selActor)
		return true;


	bool render = static_cast<CEGUI::Checkbox *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/renderercb"))
	->isSelected();

	long rtype = 0;
	CEGUI::Combobox * cbnm = static_cast<CEGUI::Combobox *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/sptyper"));
	CEGUI::ListboxItem * lbi = cbnm->getSelectedItem();
	if(lbi)
		rtype = cbnm->getItemIndex(lbi);

	std::string rtar =
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/sprtarget")
		->getText().c_str();

	std::vector<long> _targets;
	std::vector<std::string> tokens;
	StringTokenize(rtar, tokens, ",");
	for(size_t i=0; i<tokens.size(); ++i)
		_targets.push_back(atol(tokens[i].c_str()));


	_selActor->SetRenderer(NULL);

	if(render && _targets.size() > 0)
	{
		if(cbnm->getText() == "")
		{
			CEGUI::ListboxItem * lbi = cbnm->getListboxItemFromIndex(0);
			if(lbi)	cbnm->setItemSelectState(lbi, true);
		}

		std::map<long, SpriteInfo> * spinfos = DataLoader::getInstance()->GetSpritesInfo();
		std::map<long, SpriteInfo> * vidinfos = DataLoader::getInstance()->GetVideosInfo();
		std::map<long, ModelInfo> * modinfos = DataLoader::getInstance()->GetModelsInfo();

		switch(rtype)
		{
			case 0: // sprite renderer
			{
				std::vector<SpriteInfo *> vectmp;
				for(size_t itar = 0; itar<_targets.size(); ++itar)
				{
					std::map<long, SpriteInfo>::iterator itsp = spinfos->find(_targets[itar]);
					if(itsp != spinfos->end())
						vectmp.push_back(&itsp->second);
				}

				if(vectmp.size() > 0)
				{
					SpriteRenderer * tmp = new SpriteRenderer();
					tmp->SetSprites(vectmp);
					_selActor->SetRenderer(tmp);
				}
			}
			break;
			case 1: // video renderer
				if(_targets[0] < (long)vidinfos->size())
				{
					std::vector<SpriteInfo *> vectmp;
					vectmp.push_back(&(*vidinfos)[_targets[0]]);
					AviVideoRenderer * tmp = new AviVideoRenderer();
					tmp->SetSprites(vectmp);
					_selActor->SetRenderer(tmp);
				}
			break;
			case 2: // ms3d renderer
				if(_targets[0] < (long)modinfos->size())
				{
					ModelInfo * minf = &(*modinfos)[_targets[0]];
					MS3DModel * tmp = new MS3DModel();
					tmp->loadModelData( minf->filename );
					tmp->SetScale(minf->ScaleX, minf->ScaleY, minf->ScaleZ);
					_selActor->SetRenderer(tmp);
				}
			break;

		}

		_selActor->SetRendererType(rtype, _targets);
	}
	else
	{
		CEGUI::ListboxItem * lbi = cbnm->getSelectedItem();
		if(lbi)
			cbnm->setItemSelectState(lbi, false);
	}

	return true;
}



/***********************************************************
handle event when list is selected
***********************************************************/
bool EditorBox::Handleactoradded(const CEGUI::EventArgs& e)
{
	CEGUI::Combobox * cb = static_cast<CEGUI::Combobox *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/LActors/cbtype"));
	const CEGUI::ListboxTextItem * it = static_cast<const CEGUI::ListboxTextItem *>(cb->getSelectedItem());
	if(it)
	{
		long newid = 0;
		CEGUI::Listbox * lbact = static_cast<CEGUI::Listbox *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/LActors/list"));
		size_t itc = lbact->getItemCount()-1;
		if(itc >= 0 && itc < 1000)
			newid = atol(lbact->getListboxItemFromIndex(itc)->getText().c_str()) + 1;

		size_t idx = cb->getItemIndex(it);

		Actor * act;
		switch(idx)
		{
			case 0:	//basic actor class
				act = new Actor();
				act->SetId(newid);
				act->SetType(idx);
				(*_Lactors)[newid] = act;
			break;
			case 1:	//text actor class
			{
				act = new TextActor(0, 0, 1);
				act->SetId(newid);
				act->SetType(idx);
				(*_Lactors)[newid] = act;
			}
			break;
			case 2:	//ladder actor class
			{
				float deltaX=0, deltaY=0, deltaZ=0;
				int direction=0;
				float activationdistance=0;
				act = new LadderActor(activationdistance, deltaX, deltaY, deltaZ, direction, 1);
				act->SetId(newid);
				act->SetType(idx);
				(*_Lactors)[newid] = act;
			}
			break;
			case 3:	//exit actor class
			{
				float deltaX=0, deltaY=0, deltaZ=0;
				int direction=0;
				float activationdistance=0;
				act = new ExitActor(activationdistance, deltaX, deltaY, deltaZ, direction, 1);
				act->SetId(newid);
				act->SetType(idx);
				(*_Lactors)[newid] = act;
			}
			break;

			case 4:	//door actor class
			{
				float zoneSizeX=0, zoneSizeY=0, zoneSizeZ=0;
				bool locked = false;
				bool hide = false;
				long keyid=0;
				float OpenTransX=0;
				float OpenTransY=0;
				float OpenTransZ=0;
				float OpenTransSpeedX=0;
				float OpenTransSpeedY=0;
				float OpenTransSpeedZ=0;
				bool destk=false;

				act = new DoorActor(zoneSizeX, zoneSizeY, zoneSizeZ, locked, keyid, hide,
										OpenTransX, OpenTransY, OpenTransZ,
										OpenTransSpeedX, OpenTransSpeedY, OpenTransSpeedZ, destk);
				act->SetPassable(false);
				act->SetId(newid);
				act->SetType(idx);
				(*_Eactors)[newid] = act;
			}
			break;

			case 5:	//container actor class
			{
				float zoneSizeX=0, zoneSizeY=0, zoneSizeZ=0;

				act = new ContainerActor(zoneSizeX, zoneSizeY, zoneSizeZ, 1);
				act->SetId(newid);
				act->SetType(idx);
				(*_Lactors)[newid] = act;
			}
			break;

			case 6:	//up exit actor class
			{
				int direction=0;
				float activationdistance=0;
				act = new UpExitActor(activationdistance, direction, 1);
				act->SetId(newid);
				act->SetType(idx);
				(*_Lactors)[newid] = act;
			}
			break;

			case 7:	//switch actor class
			{
				act = new SwitchActor(0, 1);
				act->SetId(newid);
				act->SetType(idx);
				(*_Lactors)[newid] = act;
			}
			break;

			case 8:	//area switch actor class
			{
				float zoneSizeX=0, zoneSizeY=0, zoneSizeZ=0;

				act = new AreaSwitch(zoneSizeX, zoneSizeY, zoneSizeZ);
				act->SetId(newid);
				act->SetType(idx);
				(*_Eactors)[newid] = act;
			}
			break;

			case 9:	//floor switch actor class
			{
				float zoneSizeX=0, zoneSizeY=0, zoneSizeZ=0;

				act = new FloorSwitch(zoneSizeX, zoneSizeY, zoneSizeZ, 1);
				act->SetId(newid);
				act->SetType(idx);
				(*_Lactors)[newid] = act;
			}
			break;

			case 10:	//lift actor class
			{
				std::vector<PlayerScriptPart> scripts;
				act = new ScriptableActor(scripts, true);
				act->SetId(newid);
				act->SetType(idx);
				(*_Eactors)[newid] = act;
			}
			break;

			case 11:	//hurt area actor class
			{
				float zoneSizeX=0, zoneSizeY=0, zoneSizeZ=0;
				int lifetaken=5;

				act = new HurtArea(zoneSizeX, zoneSizeY, zoneSizeZ, lifetaken);
				act->SetId(newid);
				act->SetType(idx);
				(*_Lactors)[newid] = act;
			}
			break;
		}


		char buff[255];
		ltoa(newid, buff, 10);
		MyEditorListItem * item = new MyEditorListItem(buff);
		lbact->addItem(item);
		lbact->setItemSelectState(item, true);
	}



	return true;
}



/***********************************************************
handle event when list is selected
***********************************************************/
bool EditorBox::Handleactorremoved(const CEGUI::EventArgs& e)
{
	_selActor = NULL;

	CEGUI::Listbox * lb = static_cast<CEGUI::Listbox *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/LActors/list"));

	const CEGUI::ListboxTextItem * it = static_cast<const CEGUI::ListboxTextItem *>(lb->getFirstSelectedItem());
	if(it)
	{
		std::string id = it->getText().c_str();
		long idl = atol(id.c_str());
		std::map<long, Actor *>::iterator ita = _Lactors->find(idl);
		if(ita == _Lactors->end())
		{
			ita = _Eactors->find(idl);
			if(ita != _Eactors->end())
			{
				delete ita->second;
				_Eactors->erase(ita);
			}
		}
		else
		{
			delete ita->second;
			_Lactors->erase(ita);
		}


		lb->removeItem(it);
		if(lb->getItemCount() > 0)
			lb->setItemSelectState(lb->getListboxItemFromIndex(0), true);
	}

	return true;
}


/***********************************************************
hide all specific views
***********************************************************/
void EditorBox::HideAllactorviews()
{
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/textawin")->hide();
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/ladderawin")->hide();
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/exitactorwin")->hide();
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin")->hide();
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/containeractorwin")->hide();
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/upexitactorwin")->hide();
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/switchawin")->hide();
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/areaswitchwin")->hide();
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/floorswitchwin")->hide();
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/liftactorwin")->hide();
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/hurtareawin")->hide();
}



/***********************************************************
hide all specific views
***********************************************************/
bool EditorBox::Handletextchanged (const CEGUI::EventArgs& e)
{
	if(!_selActor)
		return true;

	TextActor * tmpa = static_cast<TextActor *>(_selActor);

	tmpa->SetActivationDistance(static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/textawin/spactdis"))
		->getCurrentValue());

	tmpa->SetTextId((long)static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/textawin/txtidspr"))
		->getCurrentValue());

	return true;
}

/***********************************************************
hide all specific views
***********************************************************/
bool EditorBox::Handleladderchanged (const CEGUI::EventArgs& e)
{
	if(!_selActor)
		return true;

	LadderActor * tmpa = static_cast<LadderActor *>(_selActor);

	tmpa->SetActivationDistance(static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/ladderawin/spactdis"))
		->getCurrentValue());


	tmpa->SetDir((long)static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/ladderawin/direct"))
	->getCurrentValue());

	tmpa->SetDX(static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/ladderawin/spex"))
	->getCurrentValue());

	tmpa->SetDY(static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/ladderawin/spey"))
	->getCurrentValue());

	tmpa->SetDZ(static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/ladderawin/spez"))
	->getCurrentValue());


	return true;
}

/***********************************************************
hide all specific views
***********************************************************/
bool EditorBox::Handleexitchanged (const CEGUI::EventArgs& e)
{
	if(!_selActor)
		return true;

	ExitActor * tmpa = static_cast<ExitActor *>(_selActor);

	tmpa->SetActivationDistance(static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/exitactorwin/spactdis"))
		->getCurrentValue());


	tmpa->SetDir((long)static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/exitactorwin/direct"))
	->getCurrentValue());

	tmpa->SetDX(static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/exitactorwin/spex"))
	->getCurrentValue());

	tmpa->SetDY(static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/exitactorwin/spey"))
	->getCurrentValue());

	tmpa->SetDZ(static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/exitactorwin/spez"))
	->getCurrentValue());

	return true;
}


/***********************************************************
hide all specific views
***********************************************************/
bool EditorBox::Handledoorchanged (const CEGUI::EventArgs& e)
{
	if(!_selActor)
		return true;

	DoorActor * tmpa = static_cast<DoorActor *>(_selActor);

	tmpa->SetZoneX(static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/zsxsp"))
	->getCurrentValue());

	tmpa->SetZoneY(static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/zsysp"))
	->getCurrentValue());

	tmpa->SetZoneZ(static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/zszsp"))
	->getCurrentValue());


	tmpa->SetLocked(static_cast<CEGUI::Checkbox *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/cblocked"))
	->isSelected());

	tmpa->SetHide(static_cast<CEGUI::Checkbox *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/cbhide"))
	->isSelected());

	tmpa->SetKeyId((long)static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/keyidsp"))
	->getCurrentValue());

	tmpa->SetDesKey(static_cast<CEGUI::Checkbox *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/destroykey"))
	->isSelected());


	tmpa->SetOpenTransX(static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/trxsp"))
	->getCurrentValue());

	tmpa->SetOpenTransY(static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/trysp"))
	->getCurrentValue());

	tmpa->SetOpenTransZ(static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/trzsp"))
	->getCurrentValue());


	tmpa->SetOpenTransSpeedX(static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/spxsp"))
	->getCurrentValue());

	tmpa->SetOpenTransSpeedY(static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/spysp"))
	->getCurrentValue());

	tmpa->SetOpenTransSpeedZ(static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/dooractorwin/spzsp"))
	->getCurrentValue());

	return true;
}



/***********************************************************
hide all specific views
***********************************************************/
bool EditorBox::Handleactorsave (const CEGUI::EventArgs& e)
{
	MapInfoXmlWriter::SaveActors("Data/"+_wi.Maps[_selectedmap].Files["LocalActors"], _Lactors);
	MapInfoXmlWriter::SaveActors("Data/"+_wi.Maps[_selectedmap].Files["ExternalActors"], _Eactors);
	return true;
}



/***********************************************************
hide all specific views
***********************************************************/
bool EditorBox::Handlecontainerchanged (const CEGUI::EventArgs& e)
{
	if(!_selActor)
		return true;

	ContainerActor * tmpa = static_cast<ContainerActor *>(_selActor);

	tmpa->SetZoneX(static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/containeractorwin/zsxsp"))
	->getCurrentValue());

	tmpa->SetZoneY(static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/containeractorwin/zsysp"))
	->getCurrentValue());

	tmpa->SetZoneZ(static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/containeractorwin/zszsp"))
	->getCurrentValue());

	return true;
}



/***********************************************************
hide all specific views
***********************************************************/
bool EditorBox::Handleupexitchanged (const CEGUI::EventArgs& e)
{
	if(!_selActor)
		return true;

	UpExitActor * tmpa = static_cast<UpExitActor *>(_selActor);

	tmpa->SetActivationDistance(static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/upexitactorwin/spactdis"))
		->getCurrentValue());

	tmpa->SetDir((long)static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/upexitactorwin/direct"))
	->getCurrentValue());

	return true;
}




/***********************************************************
hide all specific views
***********************************************************/
bool EditorBox::Handleareaswitchchanged (const CEGUI::EventArgs& e)
{
	if(!_selActor)
		return true;

	AreaSwitch * tmpa = static_cast<AreaSwitch *>(_selActor);

	tmpa->SetZoneX(static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/areaswitchwin/zsxsp"))
	->getCurrentValue());

	tmpa->SetZoneY(static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/areaswitchwin/zsysp"))
	->getCurrentValue());

	tmpa->SetZoneZ(static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/areaswitchwin/zszsp"))
	->getCurrentValue());

	return true;
}


/***********************************************************
hide all specific views
***********************************************************/
bool EditorBox::Handlefloorswitchchanged (const CEGUI::EventArgs& e)
{
	if(!_selActor)
		return true;

	FloorSwitch * tmpa = static_cast<FloorSwitch *>(_selActor);

	tmpa->SetZoneX(static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/floorswitchwin/zsxsp"))
	->getCurrentValue());

	tmpa->SetZoneY(static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/floorswitchwin/zsysp"))
	->getCurrentValue());

	tmpa->SetZoneZ(static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/floorswitchwin/zszsp"))
	->getCurrentValue());

	return true;
}


/***********************************************************
init sprite info
***********************************************************/
void EditorBox::InitSprites()
{
	_sprites = DataLoader::getInstance()->GetSpritesInfo();
	_videos = DataLoader::getInstance()->GetVideosInfo();

	//{
	//	std::vector<std::string> spfilelist;
	//	DataLoader::getInstance()->GetAvailableSprites(spfilelist);
	//	std::vector<std::string>::iterator it = spfilelist.begin();
	//	std::vector<std::string>::iterator end = spfilelist.end();

	//	CEGUI::Combobox * lbsp = static_cast<CEGUI::Combobox *> (
	//			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/spritefile"));
	//	lbsp->resetList();
	//	lbsp->setText("");

	//	for(int cc=0; it != end; ++it, ++cc)
	//	{
	//		MyEditorListItem * item = new MyEditorListItem(*it);
	//		lbsp->addItem(item);
	//	}
	//}




	{
		std::map<long, SpriteInfo>::iterator it = _sprites->begin();
		std::map<long, SpriteInfo>::iterator end = _sprites->end();

		CEGUI::Listbox * lbsp = static_cast<CEGUI::Listbox *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/splist"));
		lbsp->resetList();
		lbsp->setText("");

		for(int cc=0; it != end; ++it, ++cc)
		{
			char buff[255];
			long id = it->first;
			ltoa(id, buff, 10);
			MyEditorListItem * item = new MyEditorListItem(buff);
			lbsp->addItem(item);
			if(cc == 0)
				lbsp->setItemSelectState(item, true);
		}
	}

}


/***********************************************************
handle event when list is selected
***********************************************************/
bool EditorBox::Handlespriteselected (const CEGUI::EventArgs& e)
{
	CEGUI::Listbox * lb = static_cast<CEGUI::Listbox *> (
			CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/splist"));

	const CEGUI::ListboxTextItem * it = static_cast<const CEGUI::ListboxTextItem *>(lb->getFirstSelectedItem());

	std::string id = "";
	if(it)
	{
		id = it->getText().c_str();
		long idl = atol(id.c_str());
		std::map<long, SpriteInfo>::iterator ita = _sprites->find(idl);
		if(ita == _sprites->end())
			_selSprite = NULL;
		else
			_selSprite = &ita->second;
	}
	else
		_selSprite = NULL;



	if(_selSprite)
	{
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/sprlblnumber")
			->setText(id);

		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/sprfilelbl")
			->setText(_selSprite->filename);


		CEGUI::Listbox * lbsp = static_cast<CEGUI::Listbox *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/sppartlist"));
		lbsp->resetList();
		lbsp->setText("");

		std::vector<QuadImageInfo>::iterator it = _selSprite->quadsInfo.begin();
		std::vector<QuadImageInfo>::iterator end = _selSprite->quadsInfo.end();
		for(long cc=0; it != end; ++it, ++cc)
		{
			char buff[255];
			ltoa(cc, buff, 10);
			MyEditorListItem * item = new MyEditorListItem(buff);
			lbsp->addItem(item);
			if(cc == 0)
				lbsp->setItemSelectState(item, true);
		}
	}

	return true;
}



/***********************************************************
handle event when list is selected
***********************************************************/
bool EditorBox::Handlespritepartselected (const CEGUI::EventArgs& e)
{
	if(_selSprite)
	{
		CEGUI::Listbox * lb = static_cast<CEGUI::Listbox *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/sppartlist"));

		const CEGUI::ListboxTextItem * it = static_cast<const CEGUI::ListboxTextItem *>(lb->getFirstSelectedItem());

		std::string id = "";
		if(it)
		{
			id = it->getText().c_str();
			long idl = atol(id.c_str());
			if(idl >= _selSprite->quadsInfo.size())
				_selSpritePart = NULL;
			else
				_selSpritePart = &(_selSprite->quadsInfo[idl]);
		}
		else
			_selSpritePart = NULL;
	}

	if(_selSpritePart)
	{
		QuadImageInfo qi = *_selSpritePart;

		static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow( "EditorWindow/tabcontrol/Sprites/bottomLeftX"))
		->setCurrentValue(qi.BottomLeftCornerX);

		static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/bottomLeftY"))
		->setCurrentValue(qi.BottomLeftCornerY);

		static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/bottomLeftZ"))
		->setCurrentValue(qi.BottomLeftCornerZ);

		static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/topRightX"))
		->setCurrentValue(qi.TopRightCornerX);

		static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/topRightY"))
		->setCurrentValue(qi.TopRightCornerY);

		static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/topRightZ"))
		->setCurrentValue(qi.TopRightCornerZ);


		static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow( "EditorWindow/tabcontrol/Sprites/topLeftX"))
		->setCurrentValue(qi.TopLeftCornerX);

		static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/topLeftY"))
		->setCurrentValue(qi.TopLeftCornerY);

		static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/topLeftZ"))
		->setCurrentValue(qi.TopLeftCornerZ);

		static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/bottomRightX"))
		->setCurrentValue(qi.BottomRightCornerX);

		static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/bottomRightY"))
		->setCurrentValue(qi.BottomRightCornerY);

		static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/bottomRightZ"))
		->setCurrentValue(qi.BottomRightCornerZ);



		static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/topLeftXText"))
		->setCurrentValue(qi.TopLeftTextcoordX);

		static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/topLeftYText"))
		->setCurrentValue(qi.TopLeftTextcoordY);

		static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow( "EditorWindow/tabcontrol/Sprites/bottomLeftXText"))
		->setCurrentValue(qi.BottomLeftTextcoordX);

		static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/bottomLeftYText"))
		->setCurrentValue(qi.BottomLeftTextcoordY);

		static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/topRightXText"))
		->setCurrentValue(qi.TopRightTextcoordX);

		static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/topRightYText"))
		->setCurrentValue(qi.TopRightTextcoordY);

		static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow( "EditorWindow/tabcontrol/Sprites/bottomRightXText"))
		->setCurrentValue(qi.BottomRightTextcoordX);

		static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/bottomRightYText"))
		->setCurrentValue(qi.BottomRightTextcoordY);
	}

	return true;
}



/***********************************************************
handle event when list is selected
***********************************************************/
bool EditorBox::Handleaddsprite (const CEGUI::EventArgs& e)
{
	std::string file = CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/spritefile")
		->getText().c_str();

	if(file != "")
	{
		long newid = 0;
		CEGUI::Listbox * lbact = static_cast<CEGUI::Listbox *> (
				CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/splist"));
		size_t itc = lbact->getItemCount()-1;
		if(itc >= 0 && itc < 1000)
			newid = atol(lbact->getListboxItemFromIndex(itc)->getText().c_str()) + 1;


		SpriteInfo spi;
		spi.filename = file;
		spi.id = newid;

		(*_sprites)[newid] = spi;

		char buff[255];
		ltoa(newid, buff, 10);
		MyEditorListItem * item = new MyEditorListItem(buff);
		lbact->addItem(item);
		lbact->setItemSelectState(item, true);

		Handleaddspritepart (e);
	}


	return true;
}


/***********************************************************
handle event when list is selected
***********************************************************/
bool EditorBox::Handleremovesprite (const CEGUI::EventArgs& e)
{
	//_selSprite = NULL;
	//_selSpritePart = NULL;

	//CEGUI::Listbox * lb = static_cast<CEGUI::Listbox *> (
	//	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/splist"));

	//const CEGUI::ListboxTextItem * it = static_cast<const CEGUI::ListboxTextItem *>(lb->getFirstSelectedItem());
	//if(it)
	//{
	//	std::string id = it->getText().c_str();
	//	long idl = atol(id.c_str());
	//	std::map<long, SpriteInfo>::iterator ita = _sprites->find(idl);
	//	if(ita != _sprites->end())
	//		_sprites->erase(ita);

	//	lb->removeItem(it);
	//	if(lb->getItemCount() > 0)
	//		lb->setItemSelectState(lb->getListboxItemFromIndex(0), true);
	//}



	return true;
}

/***********************************************************
handle event when list is selected
***********************************************************/
bool EditorBox::Handleaddspritepart (const CEGUI::EventArgs& e)
{
	if(!_selSprite)
		return true;

	QuadImageInfo qi;
	qi.BottomLeftCornerX=-2;
	qi.BottomLeftCornerY=0;
	qi.BottomLeftCornerZ=2;
	qi.TopRightCornerX=2;
	qi.TopRightCornerY=5;
	qi.TopRightCornerZ=-2;

	qi.TopLeftTextcoordX=0;
	qi.TopLeftTextcoordY=100;

	qi.TopRightTextcoordX=100;
	qi.TopRightTextcoordY=100;

	qi.BottomLeftTextcoordX=0;
	qi.BottomLeftTextcoordY=0;

	qi.BottomRightTextcoordX=100;
	qi.BottomRightTextcoordY=0;

	_selSprite->quadsInfo.push_back(qi);

	CEGUI::Listbox * lb = static_cast<CEGUI::Listbox *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/sppartlist"));

	long cc = (long) _selSprite->quadsInfo.size() - 1;
	char buff[255];
	ltoa(cc, buff, 10);
	MyEditorListItem * item = new MyEditorListItem(buff);
	lb->addItem(item);
	lb->setItemSelectState(item, true);

	return true;
}

/***********************************************************
handle event when list is selected
***********************************************************/
bool EditorBox::Handleremovespritepart (const CEGUI::EventArgs& e)
{
	if(!_selSprite)
		return true;

	_selSpritePart = NULL;

	CEGUI::Listbox * lb = static_cast<CEGUI::Listbox *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/sppartlist"));

	const CEGUI::ListboxTextItem * it = static_cast<const CEGUI::ListboxTextItem *>(lb->getFirstSelectedItem());
	if(it)
	{
		std::string id = it->getText().c_str();
		long idl = atol(id.c_str());
		if(idl < _selSprite->quadsInfo.size())
		{
			std::vector<QuadImageInfo>::iterator ita = _selSprite->quadsInfo.begin();
			for(long i=0; i<idl; ++i, ++ita);

			if(ita != _selSprite->quadsInfo.end())
				_selSprite->quadsInfo.erase(ita);
		}

		lb->removeItem(it);
		if(lb->getItemCount() > 0)
			lb->setItemSelectState(lb->getListboxItemFromIndex(0), true);
	}



	return true;
}

/***********************************************************
handle event when list is selected
***********************************************************/
bool EditorBox::Handlespritechanged (const CEGUI::EventArgs& e)
{
	if(_selSpritePart)
	{
		_selSpritePart->BottomLeftCornerX = static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow( "EditorWindow/tabcontrol/Sprites/bottomLeftX"))
		->getCurrentValue();

		_selSpritePart->BottomLeftCornerY = static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/bottomLeftY"))
		->getCurrentValue();

		_selSpritePart->BottomLeftCornerZ = static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/bottomLeftZ"))
		->getCurrentValue();

		_selSpritePart->TopRightCornerX = static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/topRightX"))
		->getCurrentValue();

		_selSpritePart->TopRightCornerY = static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/topRightY"))
		->getCurrentValue();

		_selSpritePart->TopRightCornerZ = static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/topRightZ"))
		->getCurrentValue();

		_selSpritePart->TopLeftCornerX = static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow( "EditorWindow/tabcontrol/Sprites/topLeftX"))
		->getCurrentValue();

		_selSpritePart->TopLeftCornerY = static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/topLeftY"))
		->getCurrentValue();

		_selSpritePart->TopLeftCornerZ = static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/topLeftZ"))
		->getCurrentValue();

		_selSpritePart->BottomRightCornerX = static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/bottomRightX"))
		->getCurrentValue();

		_selSpritePart->BottomRightCornerY = static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/bottomRightY"))
		->getCurrentValue();

		_selSpritePart->BottomRightCornerZ = static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/bottomRightZ"))
		->getCurrentValue();


		_selSpritePart->TopLeftTextcoordX = static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/topLeftXText"))
		->getCurrentValue();

		_selSpritePart->TopLeftTextcoordY = static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/topLeftYText"))
		->getCurrentValue();

		_selSpritePart->BottomLeftTextcoordX = static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow( "EditorWindow/tabcontrol/Sprites/bottomLeftXText"))
		->getCurrentValue();

		_selSpritePart->BottomLeftTextcoordY = static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/bottomLeftYText"))
		->getCurrentValue();

		_selSpritePart->TopRightTextcoordX = static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/topRightXText"))
		->getCurrentValue();

		_selSpritePart->TopRightTextcoordY = static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/topRightYText"))
		->getCurrentValue();

		_selSpritePart->BottomRightTextcoordX = static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow( "EditorWindow/tabcontrol/Sprites/bottomRightXText"))
		->getCurrentValue();

		_selSpritePart->BottomRightTextcoordY = static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Sprites/bottomRightYText"))
		->getCurrentValue();
	}


	return true;
}

/***********************************************************
handle event when list is selected
***********************************************************/
bool EditorBox::Handlespritesave (const CEGUI::EventArgs& e)
{
	MapInfoXmlWriter::SaveSprites(DataLoader::getInstance()->getSpritesfile(), _sprites);
	return true;
}



/***********************************************************
handle event when list is selected
***********************************************************/
bool EditorBox::Handleswitchchanged (const CEGUI::EventArgs& e)
{
	if(!_selActor)
		return true;

	SwitchActor * tmpa = static_cast<SwitchActor *>(_selActor);

	tmpa->SetActivationDistance(static_cast<CEGUI::Spinner *> (
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/switchawin/spactdis"))
		->getCurrentValue());

	return true;
}




/***********************************************************
handle event when list is selected
***********************************************************/
bool EditorBox::Handleliftscriptsselected (const CEGUI::EventArgs& e)
{
	CEGUI::MultiColumnList* listscript =
	static_cast<CEGUI::MultiColumnList*> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/liftactorwin/listscript"));

	 CEGUI::ListboxItem * it = listscript->getNextSelected(NULL);
	 if(it)
	 {
		_liftrowid = listscript->getItemRowIndex(it);

		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/liftactorwin/edtype")
		->setText(it->getText());
	 }
	 else
		 _liftrowid = 1000000;

	 it = listscript->getNextSelected(it);

	 if(it)
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/liftactorwin/edva")
		->setText(it->getText());
	 it = listscript->getNextSelected(it);

	 if(it)
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/liftactorwin/edvb")
		->setText(it->getText());
	 it = listscript->getNextSelected(it);

	 if(it)
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/liftactorwin/edvc")
		->setText(it->getText());
	 it = listscript->getNextSelected(it);

	 if(it)
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/liftactorwin/edspeed" )
		->setText(it->getText());
	 it = listscript->getNextSelected(it);

	 if(it)
		CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/liftactorwin/edsound")
		->setText(it->getText());

	return true;
}


/***********************************************************
handle event when list is selected
***********************************************************/
bool EditorBox::Handleliftscriptschanged (const CEGUI::EventArgs& e)
{
	if(!_selActor)
		return true;

	ScriptableActor* la = static_cast<ScriptableActor*> (_selActor);
	std::vector<PlayerScriptPart> & scripts = la->GetScripts();

	if(_liftrowid < scripts.size())
	{
		PlayerScriptPart & sp = scripts[_liftrowid];

		sp.Type =
		atoi(CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/liftactorwin/edtype")
		->getText().c_str());


		sp.ValueA =
		atof(CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/liftactorwin/edva")
		->getText().c_str());

		sp.ValueB =
		atof(CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/liftactorwin/edvb")
		->getText().c_str());

		sp.ValueC =
		atof(CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/liftactorwin/edvc")
		->getText().c_str());


		sp.Speed =
		atof(CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/liftactorwin/edspeed" )
		->getText().c_str());


		sp.Sound =
		atoi(CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/liftactorwin/edsound")
		->getText().c_str());

	}


	return true;
}



//! handle event when list is selected
bool EditorBox::Handleliftscriptsadded (const CEGUI::EventArgs& e)
{
	if(!_selActor)
		return true;

	ScriptableActor* la = static_cast<ScriptableActor*> (_selActor);

	CEGUI::MultiColumnList* listscript =
	static_cast<CEGUI::MultiColumnList*> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/liftactorwin/listscript"));


	std::vector<PlayerScriptPart> & script = la->GetScripts();
	PlayerScriptPart ps;
	ps.Sound = -1;
	ps.Speed = -1;
	ps.Type = -1;
	ps.ValueA = -1;
	ps.ValueB = -1;
	ps.ValueC = -1;
	script.push_back(ps);
	size_t i = script.size() - 1;

	listscript->addRow();

	{
	std::stringstream strs;
	strs<<-1;
	MyEditorListItem *itemMultiColumnList = new MyEditorListItem(strs.str());
	listscript->setItem(itemMultiColumnList, 0, i);
	}
	{
	std::stringstream strs;
	strs<<-1;
	MyEditorListItem *itemMultiColumnList = new MyEditorListItem(strs.str());
	listscript->setItem(itemMultiColumnList, 1, i);
	}
	{
	std::stringstream strs;
	strs<<-1;
	MyEditorListItem *itemMultiColumnList = new MyEditorListItem(strs.str());
	listscript->setItem(itemMultiColumnList, 2, i);
	}
	{
	std::stringstream strs;
	strs<<-1;
	MyEditorListItem *itemMultiColumnList = new MyEditorListItem(strs.str());
	listscript->setItem(itemMultiColumnList, 3, i);
	}
	{
	std::stringstream strs;
	strs<<-1;
	MyEditorListItem *itemMultiColumnList = new MyEditorListItem(strs.str());
	listscript->setItem(itemMultiColumnList, 4, i);
	}
	{
	std::stringstream strs;
	strs<<-1;
	MyEditorListItem *itemMultiColumnList = new MyEditorListItem(strs.str());
	listscript->setItem(itemMultiColumnList, 5, i);
	}



	return true;
}

//! handle event when list is selected
bool EditorBox::Handleliftscriptsremoved (const CEGUI::EventArgs& e)
{
	if(!_selActor)
		return true;

	ScriptableActor* la = static_cast<ScriptableActor*> (_selActor);
	std::vector<PlayerScriptPart> & script = la->GetScripts();

	CEGUI::MultiColumnList* listscript =
	static_cast<CEGUI::MultiColumnList*> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/liftactorwin/listscript"));

	 CEGUI::ListboxItem * it = listscript->getNextSelected(NULL);
	 if(it)
	 {
		size_t rowid = listscript->getItemRowIndex(it);

		std::vector<PlayerScriptPart>::iterator its = script.begin();
		for(size_t i=0; i<rowid; ++i, ++its);

		script.erase(its);

		Handleactorselected(e);
	 }


	return true;
}


/***********************************************************
hide all specific views
***********************************************************/
bool EditorBox::Handlehurtareachanged (const CEGUI::EventArgs& e)
{
	if(!_selActor)
		return true;

	HurtArea * tmpa = static_cast<HurtArea *>(_selActor);

	tmpa->SetZoneX(static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/hurtareawin/zsxsp"))
	->getCurrentValue());

	tmpa->SetZoneY(static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/hurtareawin/zsysp"))
	->getCurrentValue());

	tmpa->SetZoneZ(static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/hurtareawin/zszsp"))
	->getCurrentValue());

	tmpa->SetLifeTaken((int) static_cast<CEGUI::Spinner *> (
	CEGUI::WindowManager::getSingleton().getWindow("EditorWindow/tabcontrol/Actors/hurtareawin/lifetaken"))
	->getCurrentValue());

	return true;
}
