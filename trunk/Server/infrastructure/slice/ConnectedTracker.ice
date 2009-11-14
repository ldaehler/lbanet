#ifndef ROOM_MANAGER_ICE
#define ROOM_MANAGER_ICE

#include <LbaTypes.ice>
#include <ChatInfo.ice>

module LbaNet
{
	interface ConnectedTracker
	{	
		// return the id of the player
		long Connect(string Nickname);
		void Disconnect(long PlayerId);
		void ChangeStatus(string Nickname, string NewStatus);
		void ChangeNameColor(string Nickname, string Color);		

		ConnectedL GetConnected();
		
		void SetWhisperInterface(string Nickname, ChatRoomObserver * winterface);
		bool Whisper(string From, string To, string Message);
	};

};

#endif

