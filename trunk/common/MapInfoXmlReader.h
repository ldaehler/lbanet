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


protected:
	// load a map information into memory
	static MapInfo LoadMap(TiXmlElement* pElem);

};


#endif
