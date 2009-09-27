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


#ifndef _LBA_NET_EDITOR_BOX_
#define _LBA_NET_EDITOR_BOX_

class AgentSharedData;
class GameGUI;
class IrcThread;
class Actor;

namespace CEGUI
{
	class Window;
	class EventArgs;
	class Listbox;
}

#include <iostream>
#include <string>
#include <list>

#include "WorldInfo.h"

//*************************************************************************************************
//*                               class EditorBox
//*************************************************************************************************
/**
* @brief Class handling a chat box
*
*/
class EditorBox
{
 public:
	//! constructor
	 EditorBox(GameGUI * gamgui);

	//! destructor
	virtual ~EditorBox();

	//! initalize the box
	void Initialize(CEGUI::Window* Root);

	//! called when we change the world
	void ChangeMap(const std::string &newworld, const std::string &newmap);


	//! handle event when list is selected
	bool HandleLbmapSelected (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool HandleLbspawningSelected (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool HandleBokmap (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handlebcreatemap (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handlespawningselected (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handlespawningplacechanged (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handlesaveb (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handleaddspaw (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handleremspaw (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handleexitselected (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handleexitplacechanged (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handleexitmapelected (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handleexitspanelected (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handleaddexit (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handleremexit (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handletextselected (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handleaddtxt (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handleremtxt (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handleupdtxt (const CEGUI::EventArgs& e);

	//! set actors
	void SetActors(std::map<long, Actor *> * Lactors, std::map<long, Actor *> * Eactors);

	//! handle event when list is selected
	bool Handleactorselected (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handleactorchanged (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handleactorrendererchanged (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handleactoradded(const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handleactorremoved(const CEGUI::EventArgs& e);

	//! hide all specific views
	void HideAllactorviews();

	//! handle event when list is selected
	bool Handletextchanged (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handleladderchanged (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handleexitchanged (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handledoorchanged (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handleactorsave (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handlecontainerchanged (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handleupexitchanged (const CEGUI::EventArgs& e);

	//! init sprite info
	void InitSprites();

	//! handle event when list is selected
	bool Handlespriteselected (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handlespritepartselected (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handleaddsprite (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handleremovesprite (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handleaddspritepart (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handleremovespritepart (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handlespritechanged (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handlespritesave (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handleswitchchanged (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handleareaswitchchanged (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handlefloorswitchchanged (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handleliftscriptsselected (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handleliftscriptschanged (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handleliftscriptsadded (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handleliftscriptsremoved (const CEGUI::EventArgs& e);

	//! handle event when list is selected
	bool Handlehurtareachanged (const CEGUI::EventArgs& e);

private:
	CEGUI::Window*			_myBox;
	GameGUI *				_gamgui;

	std::string				_currentworld;
	std::string				_currentmap;
	WorldInfo &				_wi;
	SpawningInfo *			_currsp;
	ExitInfo *				_currex;
	std::map<long, std::string>	_txts;
	long					_seltxtid;
	std::string				_ctxtfile;

	std::string				_selectedmap;

	std::map<long, Actor *> * _Lactors;
	std::map<long, Actor *> * _Eactors;

	Actor *					_selActor;

	std::map<long, SpriteInfo> * _sprites;
	std::map<long, SpriteInfo> * _videos;
	SpriteInfo *			_selSprite;
	QuadImageInfo *			_selSpritePart;

	size_t					_liftrowid;
};

#endif
