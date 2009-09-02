#ifndef CLIENT_SESSION_ICE
#define CLIENT_SESSION_ICE

#include <Glacier2/Session.ice>
#include <ActorInfo.ice>
#include <ChatInfo.ice>

module LbaNet
{
	dictionary<string, long> ConnectedList;
	

	interface ClientSession extends Glacier2::Session
	{
	    ChatRoomParticipant* JoinChat(string room, ChatRoomObserver* view);
	    void LeaveChat(string room, ChatRoomObserver* view);
	    
	    ActorsParticipant* ChangeRoom(string newroom, string actorname,  ActorsObserver* view);
	    

	    ConnectedList GetConnected(out long ownid);       
	    
	    
	    long GetTime();
	};

};

#endif
