#ifndef LBANET_ACTOR_HANDLER
#define LBANET_ACTOR_HANDLER


#include <LbaTypes.ice>


module LbaNet
{
	interface ActorsObserver
	{
	    void UpdatedInfo(ActorInfo asi);
	    void Quitted(string ActorName);
	    
	    void ActivatedActor(ActorActivationInfo ai);  
	    void SignaledActor(ActorSignalInfo ai);	
	    
	    void UpdatedLife(ActorLifeInfo ali);
	    
	    void ApplyInventoryChanges(UpdatedItemSeq InventoryChanges);
	    
	    void UpdateContainerInfo(ContainerInfo container);
	    
	    void InformInventoryChanges(UpdatedItemSeq InventoryChanges);	    
	};
	
	
	interface ActorsParticipant
	{
	    void Update(ActorInfo asi);
	};		
};

#endif
