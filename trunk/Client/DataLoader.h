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


#if !defined(__LbaNetModel_1_DataLoader_h)
#define __LbaNetModel_1_DataLoader_h

#include <string>
#include <vector>
#include "WorldInfo.h"
#include "BrickInfoHandler.h"
#include "ClientSignaler.h"

struct WorldDesc
{
	std::string FileName;
	std::string WorldName;
	std::string Description;
};

struct entitiesTableStruct;
class Actor;


/***********************************************************************
 * Module:  DataLoader.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:56:34
 * Purpose: Declaration of the class DataLoader
 ***********************************************************************/
class DataLoader
{
public:


	//! destructor
	~DataLoader();

	// singleton pattern
	static DataLoader * getInstance();

	//! get the list of available worlds
	void GetAvailableWorlds(std::vector<WorldDesc> & list);

	//! load world information into memory
	bool LoadWorld(std::string WorldName);

	//! get the first map of the world
	std::string GetFirstMapName(std::string & spawning);

	//! load a map into memory
	bool LoadMap(std::string MapName);

	//! return the current world info
	WorldInfo & GetWorldInfo();

	//! return the current map info
	MapInfo * GetMapInfo();

	//! get the brick infor handler
	const BrickInfoHandler * GetBrickInfo();

	//! used to get the character entities info
	entitiesTableStruct* GetEntitiesInfo();

	//! get the current actors of the map
	bool GetLocalMapActors(std::map<long, Actor *> & vec);

	//! get the current external actors of the map
	bool GetExternalMapActors(std::map<long, Actor *> & vec);

	//! get the text given a text id
	std::string GetText(long TextId);

	//! save back the current world into file
	bool SaveCurrentWorld();

	//! get the list of available music
	void GetAvailableMusics(std::vector<std::string> & list);

	//! get the list of available maps
	void GetAvailableMaps(std::vector<std::string> & list);

	//! get the list of available maps
	void GetAvailableLActors(std::vector<std::string> & list);

	//! get the list of available maps
	void GetAvailableEActors(std::vector<std::string> & list);

	//! get the list of available maps
	void GetAvailableTexts(std::vector<std::string> & list);

	//! get the list of available maps
	void GetAvailableSprites(std::vector<std::string> & list);

	//! get the list of available maps
	void GetAvailableVideos(std::vector<std::string> & list);

	//! sprites accessor
	std::map<long, SpriteInfo> * GetSpritesInfo()
	{return &_sprites;}

	//! video accessor
	std::map<long, SpriteInfo> * GetVideosInfo()
	{return &_videos;}

	//! video accessor
	std::map<long, ModelInfo> * GetModelsInfo()
	{return &_models;}

	//! set game language
	void SetLanguage(std::string lang);

	//! get language
	std::string GetLanguage()
	{return _lang;}


	//! get current text file
	std::string gettextfile()
	{return _textfile;}

	//! get current sprite file
	std::string getSpritesfile()
	{return _spritefile;}

	//! get current sprite file
	std::string getVideosfile()
	{return _videofile;}

	//! get current sprite file
	std::string getModelsfile()
	{return _modelsfile;}

	// get a sound path
	std::string GetSoundPath(long id);

	//! video accessor
	const std::map<long, ItemInfo> & GetInventory()
	{return _inventory;}


protected:
	//! constructor
   DataLoader();

	//! cleanup actual world
	void CleanUpWolrd();

	//! helper function
	void GetRecursivefiles(const std::string & dirpath, const std::string & dirname,
								   std::vector<std::string> & list);

private:
	static DataLoader *		_singletonInstance;
	WorldInfo				_currentWorld;
	std::string				_currentMap;
	BrickInfoHandler *		_brickInfo;
	std::map<long, SpriteInfo> _sprites;
	std::map<long, SpriteInfo> _videos;
	std::map<long, ModelInfo> _models;
	std::map<long, ItemInfo>	_inventory;

	std::string				_textfile;
	std::string				_currentworldfile;
	std::string				_spritefile;
	std::string				_videofile;
	std::string				_soundfile;
	std::string				_modelsfile;
	std::string				_inventoryfile;

	std::string				_lang;

	entitiesTableStruct*	_estruct;
	ClientSignaler			_signaler;
};

#endif