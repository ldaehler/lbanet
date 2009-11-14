#ifndef LBANET_MAP_OBSERVER
#define LBANET_MAP_OBSERVER


#include <LbaTypes.ice>
#include <ClientSession.ice>

module LbaNet
{
	interface MapObserver
	{    
	    void ActivateActor(ActorActivationInfo ai, ClientSession* callback);  
	    void SignalActor(ActorSignalInfo ai);
	    UpdateSeq GetUpdatedInfo();
	    PlayerSeq GetPlayersInfo();
	    
	    
	    void GotHurtByActor(long ActorId, long HurtingActorId);    
	    void GotHurtByFalling(long ActorId, float FallingDistance);	   
	    
	    void RaisedFromDead(long ActorId);
	    
	    void AskForContainer(long ActorId, long ContainerId, ClientSession* callback);
	    
	    void UpdateContainer(long ContainerId, long ActorId, ItemList Taken, ItemList Put, ClientSession* callback);
	    
	    void UpdateLifeMana(long ActorId, int LifeDelta, int ManaDelta);
	};
};

#endif

