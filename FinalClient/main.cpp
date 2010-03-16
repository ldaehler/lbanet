#include "LbaNetEngine.h"
#include "UserAllocatorHandler.h"
#include "SendingLoop.h"
#include "LocalSender.h"
#include "ServerThread.h"
#include "ServerReceiveingWorkpile.h"

int main( int argc, char **argv )
{
	// init memory allocator
	UserAllocatorHandler::getInstance()->Initialize();

	// server receiver buffer
	boost::shared_ptr<ServerReceivingWorkpile> receiverb(new ServerReceivingWorkpile());

	//start server loop thread with a 50ms scan cycle
	ServerThread* serverth = new ServerThread(50, receiverb);
	IceUtil::ThreadControl _tcserver = serverth->start();


	//start sending loop thread with a 50ms scan cycle
	SendingLoopThread* sendth = new SendingLoopThread(50, boost::shared_ptr<SenderBase>(new LocalSender(receiverb)));
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

