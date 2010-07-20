#ifndef CLIENT_SESSION_ICE
#define CLIENT_SESSION_ICE

#include <Glacier2/Session.ice>
#include <ActorInfo.ice>
#include <ChatInfo.ice>
#include <LbaTypes.ice>

module LbaNet
{
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
	    
	    //called when player is drowning
	    void PlayerDrowning();
	    
	    
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
	    void AskFriend(string friendname);
	    void AcceptFriend(long friendid);    
	    void RemoveFriend(long friendid);
	    void GetFriends();
	    
	    // send a pm to someone
	    void SendPM(PMInfo pm);
	    
	    // delete a pm
	    void DeletePM(long pmid);
	    
	    // mark pm as read
	    void MarkReadPM(long pmid);
	    
	    //get all pm in your mailbox
	    void GetInboxPM();
	    
	    // store letter to the server and return the letter id
	    void AddLetter(string title, string message);
	    
	    // return letter info
	    LetterInfo GetLetterInfo(long LetterId);
	    
	    // destroy an inventory item
	    void DestroyItem(long Id);
	    
	    
	    // set player targeted by actor
	    void SetTargeted(long ActorId);
	    
		    
	    // set player untargeted by actor
	    void SetUnTargeted(long ActorId); 
	    
	    // buy item
	    void BuyItem(long FromActorId, long Itemid);
	    
	    // tell client only if actor is activated
	    void ActivatedActor(ActorActivationInfo ai, bool succeded);
	    
	    
	    
	    // called when quest should start
	    void StartQuest(long QuestId);
	    
	    
	    // called when quest should end
	    void EndQuest(long QuestId);	
	    
	    // called when player throw magic ball
	    void MagicBallStart(LaunchInfo Linfo);
	    
	    // called when magic ball is back
	    void MagicBallEnd();
	    
	    // called when magic ball touch an actor
	    void MagicBallTouchActor(long ActorId);
	    
	    // called when magic ball touch a player
	    void MagicBallTouchPlayer(long ActorId);	    
	 
	    
	    // update current life of player - called by map server
	    void UpdatedLife(ActorLifeInfo ali);	 
	 
	};

};

#endif

