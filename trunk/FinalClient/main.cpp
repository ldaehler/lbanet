#include "LbaNetEngine.h"
#include "UserAllocatorHandler.h"


int main( int argc, char **argv )
{
	// init memory allocator
	UserAllocatorHandler::getInstance()->Initialize();

	LbaNetEngine engine;
	engine.run();

	return 0;
}

//#include <osg/Node>
////load map
//OsgHandler::getInstance()->SetMap(OsgHandler::getInstance()->LoadOSGFile("Lba1/Maps/map0.osgt"));

