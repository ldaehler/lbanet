#ifndef _MAP_MANAGER_SERVANT_H
#define _MAP_MANAGER_SERVANT_H

#include "MapHandler.h"

#include <MapManager.h>
#include <Ice/Ice.h>

using namespace LbaNet;


class MapManagerServant : public MapManager, public IceUtil::Mutex
{
public:
	//! constructor
    MapManagerServant(const Ice::CommunicatorPtr& communicator, const Ice::ObjectAdapterPtr & adapter);

	//! destructor
    ~MapManagerServant();

	//! a player join a map
	virtual LbaNet::MapObserverPrx JoinMap(const std::string& mapName, 
											Ice::Long PlayerId, const Ice::Current&);
	
	//! a player leave a map
    virtual void LeaveMap(const std::string& mapName, Ice::Long PlayerId, const Ice::Current&);


private:
	std::map<std::string, MapHandler *> _running_maps;
	const Ice::CommunicatorPtr&			_communicator;
	Ice::ObjectAdapterPtr				_adapter;
};

#endif
