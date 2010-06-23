#ifndef LBANET_ACTOR_HANDLER
#define LBANET_ACTOR_HANDLER


#include <LbaTypes.ice>


module LbaNet
{
	interface ActorsObserver
	{
	    void UpdatedInfo(ActorInfo asi);
	    void Quitted(long PlayerId);
	    
	    void ActivatedActor(ActorActivationInfo ai);  
	    void SignaledActor(ActorSignalInfo ai);	
	    
	    void UpdatedLife(ActorLifeInfo ali);
	    
	    void ApplyInventoryChanges(UpdatedItemSeq InventoryChanges);
	    
	    void UpdateContainerInfo(ContainerInfo container);
	    
	    void InformInventoryChanges(UpdatedItemSeq InventoryChanges);
	    
	    void UpdateActorState(ActorUpdateInfo newinfo);   
	    
	    void InitQuestStartedFinished(QuestSeq Started, QuestSeq Finished);
	    
	    void InformQuestStarted(long QuestId);
	    
	    void InformQuestFinished(long QuestId);	    
	   
	    void ActivationAborted(ActorActivationInfo ai);
	    
		    
	    // called when player throw magic ball
	    void MagicBallPlayed(long PlayerId, LaunchInfo Linfo);	    
	};
	
	
	interface ActorsParticipant
	{
	    void Update(ActorInfo asi);
	};		
};

#endif

