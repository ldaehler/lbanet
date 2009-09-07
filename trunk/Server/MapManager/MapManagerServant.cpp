#include "MapManagerServant.h"


//! constructor
MapManagerServant::MapManagerServant(const Ice::CommunicatorPtr& communicator, 
									 const Ice::ObjectAdapterPtr & adapter)
: _communicator(communicator), _adapter(adapter)
{

}

//! destructor
MapManagerServant::~MapManagerServant()
{
	std::map<std::string, MapHandler *>::iterator it = _running_maps.begin();
	std::map<std::string, MapHandler *>::iterator end = _running_maps.end();

	for(; it != end; ++it)
		delete it->second;
}

//! a player join a map
LbaNet::MapObserverPrx MapManagerServant::JoinMap(const std::string& mapName, 
										Ice::Long PlayerId, const Ice::Current&)
{
	Lock sync(*this);
	MapHandler * MH = NULL;

	std::map<std::string, MapHandler *>::iterator it = _running_maps.find(mapName);
	if(it != _running_maps.end())
	{
		MH = it->second;
	}
	else
	{
		MH = new MapHandler(_communicator, _adapter, mapName);
		_running_maps[mapName] = MH;
	}

	MH->Join(PlayerId);
	return MH->GetMapProxy();
}

//! a player leave a map
void MapManagerServant::LeaveMap(const std::string& mapName, Ice::Long PlayerId, const Ice::Current&)
{
	Lock sync(*this);
	std::map<std::string, MapHandler *>::iterator it = _running_maps.find(mapName);
	if(it != _running_maps.end())
	{
		if(it->second->Leave(PlayerId))
		{
			it->second->Destroy();
			_running_maps.erase(it);
		}
	}
}

