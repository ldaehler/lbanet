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


#ifndef __LBA_NET_MAP_INFO_XML_READER_H__
#define __LBA_NET_MAP_INFO_XML_READER_H__

#include "WorldInfo.h"
#include <vector>
#include <map>


class TiXmlElement;
class Actor;
class SignalerBase;

//*************************************************************************************************
//*                                      class MapInfoXmlReader
//*************************************************************************************************

class MapInfoXmlReader
{
public:
	// load a world information into memory
	static  bool LoadWorld(const std::string &Filename, WorldInfo & res);

	// get world description
	static void GetWorldDescription(const std::string &Filename,
										std::string &WorldName, std::string &WorldDesc);

	// load map actors into memory
	static  bool LoadActors(const std::string &Filename, std::map<long, SpriteInfo> &spinfos,
									std::map<long, SpriteInfo> &vidinfos,
									std::map<long, ModelInfo> &modelinfos,
									std::map<long, Actor *> & vec,
									SignalerBase * signaler);

	// load all sprites info
	static bool LoadSprites(const std::string &Filename, std::map<long, SpriteInfo> &vec);

	// get a text from file
	static std::string GetText(const std::string &Filename, long textid);

	// get a text from file
	static void GetAllTexts(const std::string &Filename, std::map<long, std::string> &txts);

	// get a sound path from file
	static std::string GetSoundPath(const std::string &Filename, long id);

	// load all models info
	static bool LoadModels(const std::string &Filename, std::map<long, ModelInfo> &vec);

	// load inventory info
	static bool LoadInventory(const std::string &Filename, std::map<long, ItemInfo> &mapinv);

protected:
	// load a map information into memory
	static MapInfo LoadMap(TiXmlElement* pElem);

};


#endif
