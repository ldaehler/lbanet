#ifndef MAP_MANAGER_ICE
#define MAP_MANAGER_ICE

#include <MapObserver.ice>

module LbaNet
{
	interface MapManager
	{
   
	    MapObserver* JoinMap(string mapName, long PlayerId);
	    void LeaveMap(string mapName, long PlayerId);	    
	};

};

#endif
