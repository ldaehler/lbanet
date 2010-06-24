#ifndef MAP_MANAGER_ICE
#define MAP_MANAGER_ICE

#include <MapObserver.ice>
#include <ClientSession.ice>

module LbaNet
{
	interface MapManager
	{
	    MapObserver* JoinMap(string mapName, long PlayerId, ActorLifeInfo lifeinfo, ClientSession* callback);
	    ActorLifeInfo LeaveMap(string mapName, long PlayerId);	    
	};

};

#endif

