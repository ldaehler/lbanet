#ifndef __LBA_NET_MAP_INFO_XML_WRITER_H__
#define __LBA_NET_MAP_INFO_XML_WRITER_H__

#include "WorldInfo.h"
#include <vector>
class Actor;

//*************************************************************************************************
//*                                      class MapInfoXmlWriter
//*************************************************************************************************

class MapInfoXmlWriter
{
public:
	// save world information into file
	static void SaveWorld(const std::string Filename, const WorldInfo & wi);

	// save text information into file
	static void SaveTexts(const std::string Filename, const std::map<long, std::string> &txts);


	// save map actors into memory
	static void SaveActors(const std::string &Filename, std::map<long, Actor *> * vec);



	// save all sprites info
	static void SaveSprites(const std::string &Filename, std::map<long, SpriteInfo> *vec);
};


#endif
