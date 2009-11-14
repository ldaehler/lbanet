#ifndef ROOM_MANAGER_ICE
#define ROOM_MANAGER_ICE

#include <ActorInfo.ice>
#include <ChatInfo.ice>

module LbaNet
{
	interface RoomManager
	{
	    ChatRoomObserver* JoinChat(string room, ChatRoomObserver* view);
	    void LeaveChat(string room, ChatRoomObserver* view);
	    
	    ActorsObserver* JoinActorRoom(string room, string actorName, ActorsObserver* view);
	    void LeaveActorRoom(string room, ActorsObserver* view);	    
	};

};

#endif

