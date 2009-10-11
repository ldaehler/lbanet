#ifndef CLIENT_SESSION_ICE
#define CLIENT_SESSION_ICE

#include <Glacier2/Session.ice>
#include <ActorInfo.ice>
#include <ChatInfo.ice>
#include <MapObserver.ice>
#include <ConnectedTracker.ice>

module LbaNet
{
	struct PlayerPosition
	{
		// actor name
		string			MapName;		

		// actor coordinates
		float			X;
		float			Y;
		float			Z;
		float			Rotation;
	};


	interface ClientSession extends Glacier2::Session
	{
	    ChatRoomParticipant* JoinChat(string room, ChatRoomObserver* view);
	    void LeaveChat(string room, ChatRoomObserver* view);
	    
	    ActorsParticipant* ChangeRoom(string newroom, string actorname,  ActorsObserver* view);
	    
	    PlayerPosition ChangeWorld(string WorldName);
	    void UpdatePositionInWorld(PlayerPosition Position);

	    ConnectedL GetConnected(out long ownid);       
	    
	    
	    void ActivateActor(ActorActivationInfo ai);  
	    void SignalActor(ActorSignalInfo ai);
	    
	    UpdateSeq GetUpdatedInfo();	   
	    PlayerSeq GetPlayersInfo();		    
	    
	    void ChangeStatus(string Status);
	    
	    void ChangeNameColor(string Color);
	    
	    long GetTime();
	    
	    string GetVersion();
	    
	    ActorLifeInfo GetLifeInfo();
	    
	    void GotHurtByActor(long HurtingActorId);
	    
	    void GotHurtByFalling(float FallingDistance);
	    
	    void PlayerRaisedFromDead();
	};

};

#endif
