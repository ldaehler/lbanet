#ifndef ROOM_MANAGER_ICE
#define ROOM_MANAGER_ICE

module LbaNet
{
	struct PlayerInfo
	{
		long Id;
		string Status;
	};


	dictionary<string, PlayerInfo> ConnectedL;


	interface ConnectedTracker
	{	
		// return the id of the player
		long Connect(string Nickname);
		void Disconnect(long PlayerId);
		void ChangeStatus(string Nickname, string NewStatus);

		ConnectedL GetConnected();   
	};

};

#endif
