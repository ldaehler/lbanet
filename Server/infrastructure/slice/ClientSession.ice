#ifndef CLIENT_SESSION_ICE
#define CLIENT_SESSION_ICE

#include <Glacier2/Session.ice>
#include <ActorInfo.ice>
#include <ChatInfo.ice>
#include <LbaTypes.ice>

module LbaNet
{
	sequence<string> FriendsSeq;	

	interface ClientSession extends Glacier2::Session
	{
	    ChatRoomParticipant* JoinChat(string room, ChatRoomObserver* view);
	    void LeaveChat(string room, ChatRoomObserver* view);
	    
	    ActorsParticipant* ChangeRoom(string newroom, string actorname,  ActorsObserver* view);
	    
	    SavedWorldInfo ChangeWorld(string WorldName);
	    void UpdatePositionInWorld(PlayerPosition Position);
	    void UpdateInventory(InventoryInfo Inventory);

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
	    
	    
	    void UseItem(long ItemId);
	    
	    
	    void AskForContainerContent(long ContainerId);
	    void UpdateInventoryFromContainer(long ContainerId, ItemList Taken, ItemList Put);
	    
	    
	    bool HasItem(long ItemId, int QUantity);
	   
	   // callback functions
	    void ApplyInventoryChanges(UpdatedItemSeq InventoryChanges);
	    
	    void UpdateContainerInfo(ContainerInfo container);
	    
	    
	    void SetWhisperInterface(ChatRoomObserver * winterface);
	    bool Whisper(string To, string Message);
	    
	    
	    // friends functions
	    void AddFriend(string name);
	    void RemoveFriend(string name);
	    FriendsSeq GetFriends();
	};

};

#endif
