#include "LbaNetEngine.h"
#include "UserAllocatorHandler.h"
#include "SendingLoop.h"

int main( int argc, char **argv )
{
	// init memory allocator
	UserAllocatorHandler::getInstance()->Initialize();

	//start sending loop thread with a 50ms scan cycle
	SendingLoopThread* sendth;// = new SendingLoopThread(50, );
	IceUtil::ThreadControl _tc = sendth->start();


	// start main thread engine
	LbaNetEngine engine;
	engine.run();

	// wait for all thread to terminate
	sendth->Stop();
	_tc.join();


	return 0;
}

//#include <osg/Node>
////load map
//OsgHandler::getInstance()->SetMap(OsgHandler::getInstance()->LoadOSGFile("Lba1/Maps/map0.osgt"));

