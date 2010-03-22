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

#include "DataLoader.h"
#include "LogHandler.h"
#include "MapInfoXmlReader.h"

//#include "MapInfoXmlWriter.h"
//#include "DataFileHandler.h"
//#include "Entities.h"
//#include "Actor.h"
//#include "InventoryHandler.h"
//#include "QuestHandler.h"

//#include <boost/filesystem/path.hpp>
//#include <boost/filesystem/operations.hpp>
//namespace fs = boost::filesystem;


DataLoader* DataLoader::_singletonInstance = NULL;



/***********************************************************
	Constructor
***********************************************************/
DataLoader::DataLoader()
//: _lang("fr"), _brickInfo(NULL), 
{
	LogHandler::getInstance()->LogToFile("Initializing data handler class...");
	//_estruct = parseEntities(DataFileHandler::GetPath("FILE3D"));
}


/***********************************************************
	Destructor
***********************************************************/
DataLoader::~DataLoader()
{
	//if(_brickInfo)
	//	delete _brickInfo;

	//free(_estruct->entitiesTable);
	//free(_estruct);
}


/***********************************************************
singleton pattern
***********************************************************/
DataLoader * DataLoader::getInstance()
{
	if(!_singletonInstance)
		_singletonInstance = new DataLoader();

	return _singletonInstance;
}




/***********************************************************
get the list of available worlds
***********************************************************/
void DataLoader::GetAvailableWorlds(std::vector<WorldDesc> & list)
{
	MapInfoXmlReader::GetAvailableWorlds("./Data/Worlds.xml", list);
}



//
//
///***********************************************************
//load world information into memory
//***********************************************************/
//bool DataLoader::LoadWorld(std::string WorldName)
//{
//	CleanUpWolrd();
//	_currentworldfile = WorldName;
//
//	if(MapInfoXmlReader::LoadWorld(WorldName, _currentWorld))
//	{
//		if(_brickInfo)
//			delete _brickInfo;
//
//		std::string brfile = _currentWorld.Files["bricks"];
//		_spritefile = _currentWorld.Files["sprites"];
//		_videofile = _currentWorld.Files["videos"];
//		_soundfile = _currentWorld.Files["sounds"];
//		_modelsfile = _currentWorld.Files["models"];
//		_inventoryfile = _currentWorld.Files["inventory"];
//		_questfile = _currentWorld.Files["quests"];
//
//		_inventorytextfile = _currentWorld.Files["inventorytext"];
//		_questtextfile = _currentWorld.Files["questtext"];
//
//		_inventorytextfile.insert(_inventorytextfile.find("Texts/") + 6, _lang + "/");
//		_questtextfile.insert(_questtextfile.find("Texts/") + 6, _lang + "/");
//
//		_brickInfo = new BrickInfoHandler(brfile);
//
//		MapInfoXmlReader::LoadSprites(_spritefile, _sprites);
//		MapInfoXmlReader::LoadSprites(_videofile, _videos);
//		MapInfoXmlReader::LoadModels(_modelsfile, _models);
//		MapInfoXmlReader::LoadInventory(_inventoryfile, _inventory);
//
//		_inventory_texts = MapInfoXmlReader::LoadTextFile(_inventorytextfile);
//		_quests_texts = MapInfoXmlReader::LoadTextFile(_questtextfile);
//
//		// load quests for this world
//		std::map<long, QuestPtr> quests;
//		MapInfoXmlReader::LoadQuests(_questfile, quests, InventoryHandler::getInstance(),
//										QuestHandler::getInstance());
//		QuestHandler::getInstance()->Initialize(quests);
//
//		_currentMap = _currentWorld.FirstMap;
//		return true;
//	}
//	else
//		return false;
//}
//
//
//
///***********************************************************
//get the first map of the world
//***********************************************************/
//std::string DataLoader::GetFirstMapName(std::string & spawning)
//{
//	spawning = _currentWorld.FirstSpawning;
//	return _currentWorld.FirstMap;
//}
//
//
//
///***********************************************************
//load a map into memory
//***********************************************************/
//bool DataLoader::LoadMap(std::string MapName)
//{
//	_currentMap = MapName;
//	MapInfo * MI = GetMapInfo();
//	_textfile = "Data/" + MI->Files["Texts"];
//
//	_textfile.insert(_textfile.find("Texts/") + 6, _lang + "/");
//
//	_map_texts = MapInfoXmlReader::LoadTextFile(_textfile);
//
//	return true;
//}
//
//
///***********************************************************
//return the current world info
//***********************************************************/
//WorldInfo & DataLoader::GetWorldInfo()
//{
//	return _currentWorld;
//}
//
///***********************************************************
//return the current map info
//***********************************************************/
//MapInfo * DataLoader::GetMapInfo()
//{
//	return &(_currentWorld.Maps[_currentMap]);
//}
//
//
//
//
///***********************************************************
//get the brick infor handler
//***********************************************************/
//const BrickInfoHandler * DataLoader::GetBrickInfo()
//{
//	return _brickInfo;
//}
//
//
//
///***********************************************************
//used to get the character entities info
//***********************************************************/
//entitiesTableStruct* DataLoader::GetEntitiesInfo()
//{
//	return _estruct;
//}
//
//
///***********************************************************
//cleanup actual world
//***********************************************************/
//void DataLoader::CleanUpWolrd()
//{
//	_currentWorld.Name = "";
//	_currentWorld.Description = "";
//	_currentWorld.FirstMap = "";
//	_currentWorld.FirstSpawning = "";
//	_currentWorld.Maps.clear();
//	_currentWorld.Teleports.clear();
//
//}
//
//
///***********************************************************
//get the current actors of the map
//***********************************************************/
//bool DataLoader::GetLocalMapActors(std::map<long, Actor *> & vec, float AnimationSpeed)
//{
//	MapInfo * MI = GetMapInfo();
//	std::string file = MI->Files["LocalActors"];
//	if(file == "")
//		return false;
//
//	return MapInfoXmlReader::LoadActors("Data/" + file, _sprites, _videos, _models, vec, 
//										&_signaler, AnimationSpeed, InventoryHandler::getInstance(),
//										QuestHandler::getInstance());
//}
//
//
///***********************************************************
//get the current external actors of the map
//***********************************************************/
//bool DataLoader::GetExternalMapActors(std::map<long, Actor *> & vec, float AnimationSpeed)
//{
//	MapInfo * MI = GetMapInfo();
//	std::string file = MI->Files["ExternalActors"];
//	if(file == "")
//		return false;
//
//	return MapInfoXmlReader::LoadActors("Data/" + file, _sprites, _videos, _models, vec, 
//										&_signaler, AnimationSpeed, InventoryHandler::getInstance(),
//										QuestHandler::getInstance());
//}
//
//
///***********************************************************
//get the text given a text id
//***********************************************************/
//std::string DataLoader::GetText(long TextId)
//{
//	return _map_texts[TextId];
//}
//
//
///***********************************************************
//get the text given a text id
//***********************************************************/
//std::string DataLoader::GetInventoryText(long TextId)
//{
//	return _inventory_texts[TextId];
//}
//
///***********************************************************
//get the text given a text id
//***********************************************************/
//std::string DataLoader::GetQuestText(long TextId)
//{
//	return _quests_texts[TextId];
//}
//
//
//
///***********************************************************
//get the text given a text id
//***********************************************************/
//long DataLoader::AddInventoryText(std::string Text)
//{
//	long id = 0;
//
//	if(_inventory_texts.size() > 0)	
//	{
//		id = _inventory_texts.rbegin()->first + 1;
//		while(_inventory_texts.find(id) != _inventory_texts.end())
//			++id;
//	}
//
//	_inventory_texts[id] = Text;
//
//	return id;
//}
//
//
///***********************************************************
//save back the current world into file
//***********************************************************/
//bool DataLoader::SaveCurrentWorld()
//{
//	MapInfoXmlWriter::SaveWorld(_currentworldfile, _currentWorld);
//	return true;
//}
//
//
//
//
///***********************************************************
//helper function
//***********************************************************/
//void DataLoader::GetRecursivefiles(const std::string & dirpath, const std::string & dirname,
//								   std::vector<std::string> & list)
//{
//	// get all xml file of the directory
//	{
//		fs::path full_path( fs::system_complete( dirpath ) );
//
//		if ( !fs::exists( full_path ) )
//		{
//			LogHandler::getInstance()->LogToFile(std::string("\nData directory Not found: ") + full_path.file_string());
//			return;
//		}
//
//		if ( fs::is_directory( full_path ) )
//		{
//			fs::directory_iterator end_iter;
//			for ( fs::directory_iterator dir_itr( full_path ); dir_itr != end_iter;	++dir_itr )
//			{
//				if ( fs::is_regular_file( dir_itr->status() ) )
//				{
//					list.push_back(dirname + "/" + dir_itr->path().filename());
//				}
//
//				if ( fs::is_directory( dir_itr->status() ) && dir_itr->path().filename()[0] != '.')
//				{
//					GetRecursivefiles(dirpath + "/" + dir_itr->path().filename(), dirname  + "/" + dir_itr->path().filename(), list);
//				}
//			}
//		}
//	}
//}
//
///***********************************************************
//get the list of available maps
//***********************************************************/
//void DataLoader::GetAvailableMaps(std::vector<std::string> & list)
//{
//	GetRecursivefiles("./Data/Maps", "Maps", list);
//}
//
///***********************************************************
//get the list of available music
//***********************************************************/
//void DataLoader::GetAvailableMusics(std::vector<std::string> & list)
//{
//	GetRecursivefiles("./Data/Music", "Music", list);
//}
//
///***********************************************************
//get the list of available music
//***********************************************************/
//void DataLoader::GetAvailableLActors(std::vector<std::string> & list)
//{
//	GetRecursivefiles("./Data/LocalActors", "LocalActors", list);
//}
//
///***********************************************************
//get the list of available music
//***********************************************************/
//void DataLoader::GetAvailableEActors(std::vector<std::string> & list)
//{
//	GetRecursivefiles("./Data/ExternalActors", "ExternalActors", list);
//}
//
///***********************************************************
//get the list of available music
//***********************************************************/
//void DataLoader::GetAvailableTexts(std::vector<std::string> & list)
//{
//	GetRecursivefiles("./Data/Texts/"+_lang, "Texts", list);
//}
//
//
///***********************************************************
//get the list of available music
//***********************************************************/
//void DataLoader::GetAvailableSprites(std::vector<std::string> & list)
//{
//	GetRecursivefiles("./Data/Sprites/sprites", "Sprites/sprites", list);
//}
//
//
///***********************************************************
//get the list of available music
//***********************************************************/
//void DataLoader::GetAvailableVideos(std::vector<std::string> & list)
//{
//	GetRecursivefiles("./Data/Sprites/videos", "Sprites/videos", list);
//}
//
//
///***********************************************************
//set game language
//***********************************************************/
//void DataLoader::SetLanguage(std::string lang)
//{
//	_lang = lang;
//}
//
///***********************************************************
//set game language
//***********************************************************/
//std::string DataLoader::GetSoundPath(long id)
//{
//	return MapInfoXmlReader::GetSoundPath(_soundfile, id);
//}
//
//
