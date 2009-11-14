#ifndef MAP_MANAGER_ICE
#define MAP_MANAGER_ICE

#include <MapObserver.ice>

module LbaNet
{
	interface MapManager
	{
	    MapObserver* JoinMap(string mapName, long PlayerId, ActorLifeInfo lifeinfo);
	    ActorLifeInfo LeaveMap(string mapName, long PlayerId);	    
	};

};

#endif

