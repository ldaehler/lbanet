#ifndef ROOM_MANAGER_ICE
#define ROOM_MANAGER_ICE

module LbaNet
{
	dictionary<string, long> ConnectedL;


	interface ConnectedTracker
	{	
		// return the id of the player
		long Connect(string Nickname);
		void Disconnect(long PlayerId);

		ConnectedL GetConnected();   
	};

};

#endif
