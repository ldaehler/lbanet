#ifndef LBANET_MAP_OBSERVER
#define LBANET_MAP_OBSERVER

module LbaNet
{
	struct ActorActivationInfo
	{
		// id of the activate object
		long			ActorId;
	
		// activator id
		long			PlayerId;
		
		// activator coordinates
		float			X;
		float			Y;
		float			Z;
		float			Rotation;
	
	
		// flag to tell if the actor needs to be desactivated
		bool			NeedDesactivation;
	};
	
	

	
	interface MapObserver
	{    
	    void ActivateActor(ActorActivationInfo ai);    
	    void DesactivateActor(ActorActivationInfo ai);    	    
	};	
};

#endif
