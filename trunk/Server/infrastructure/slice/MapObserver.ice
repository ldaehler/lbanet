#ifndef LBANET_MAP_OBSERVER
#define LBANET_MAP_OBSERVER

module LbaNet
{
	struct ActorInfo
	{
		double			Time;
	
		// actor name
		long			ActorId;
		string			MapName;		
		string			Name;
		bool			DisplayName;

		// actor coordinates
		float			X;
		float			Y;
		float			Z;
		float			Rotation;

		// actor display
		int			Model;
		int			Body;
		int			Animation;
		short			BodyColor;

		// actor velocity
		float			vX;
		float			vY;
		float			vZ;
		float			vRotation;		
	};

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

	struct ActorLifeInfo
	{
		// id of the activated object
		long			ActorId;
		string			Name;
		
		// activator coordinates
		float			CurrentLife;
		float			MaxLife;
		float			CurrentMana;
		float			MaxMana;	
	};	
	
	
	struct PlayerFullInfo
	{
		ActorInfo ai;
		ActorLifeInfo li;
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
	

	sequence<PlayerFullInfo> PlayerSeq;	
	
	interface MapObserver
	{    
	    void ActivateActor(ActorActivationInfo ai);  
	    void SignalActor(ActorSignalInfo ai);
	    UpdateSeq GetUpdatedInfo();
	    PlayerSeq GetPlayersInfo();
	    
	    
	    void GotHurtByActor(long ActorId, long HurtingActorId);    
	    void GotHurtByFalling(long ActorId, float FallingDistance);	   
	    
	    void RaisedFromDead(long ActorId);	    
	};	
};

#endif
