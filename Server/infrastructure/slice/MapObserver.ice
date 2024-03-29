#ifndef LBANET_MAP_OBSERVER
#define LBANET_MAP_OBSERVER


#include <LbaTypes.ice>

module LbaNet
{
	interface MapObserver
	{    
	    void ActivateActor(ActorActivationInfo ai);  
	    void SignalActor(ActorSignalInfo ai);
	    UpdateSeq GetUpdatedInfo();
	    PlayerSeq GetPlayersInfo();
	    
	    
	    void GotHurtByActor(long ActorId, long HurtingActorId);    
	    void GotHurtByFalling(long ActorId, float FallingDistance);	   
	    
	    void RaisedFromDead(long ActorId);
	    
	    void AskForContainer(long ActorId, long ContainerId);
	    
	    void UpdateContainer(long ContainerId, long ActorId, ItemList Taken, ItemList Put);
	    
	    void UpdateLifeMana(long ActorId, int LifeDelta, int ManaDelta);
	    
		    
	    // set player targeted by actor
	    void SetTargeted(long PlayerId, long ActorId);
	    	    
	    // set player untargeted by actor
	    void SetUnTargeted(long PlayerId, long ActorId);
	    
		    
	    // called when player throw magic ball
	    void MagicBallPlayed(long PlayerId, LaunchInfo Linfo);
	    
	    // called when magic ball touch an actor
	    void MagicBallTouchActor(long PlayerId, long ActorId);
	    
	    // called when magic ball touch a player
	    void MagicBallTouchPlayer(long PlayerId, long ActorId);	    
	};
};

#endif

