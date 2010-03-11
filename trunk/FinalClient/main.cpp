#include "OSGHandler.h"
#include <osg/Node>

int main( int argc, char **argv )
{
	// init OSG
	OsgHandler::getInstance()->Initialize("LbaNet", "./Data");

	//load map
	OsgHandler::getInstance()->SetMap(OsgHandler::getInstance()->LoadOSGFile("Lba1/Maps/map0.osgt"));

	while(!OsgHandler::getInstance()->Update());

    return 0;
}

