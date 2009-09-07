#ifndef LBANET_MAP_OBSERVER
#define LBANET_MAP_OBSERVER

module LbaNet
{
	struct ActorActivationInfo
	{
		// id of the activated object
		long			ActivatedId;
		
		// true if activating - false if desactivating
		bool			Activate;
		
	
		// activator id
		long			ActorId;
		
		// activator coordinates
		float			X;
		float			Y;
		float			Z;
		float			Rotation;	
	};
	
	
	sequence<long> TargetSeq;
	
	struct ActorSignalInfo
	{
		// id of the signaled object
		TargetSeq		Targets;
		
		
		// id of the signal
		long			SignalId;
	
	
		// activator id
		long			ActorId;
	};
	
	struct ActorUpdateInfo
	{
		// id of the activated object
		long			ActorId;
	
		// switch info
		bool			On;
		
		//door info
		bool			Open;
		int			Counter;
		bool			SignalOn;
		
		//lift info
		long			CurrentScript;
		TargetSeq		CurrentSignals;
		float			X;
		float			Y;
		float			Z;
		float			Rotation;
	};
	
	sequence<ActorUpdateInfo> UpdateSeq;
	
	interface MapObserver
	{    
	    void ActivateActor(ActorActivationInfo ai);  
	    void SignalActor(ActorSignalInfo ai);
	    UpdateSeq GetUpdatedInfo();
	};	
};

#endif
