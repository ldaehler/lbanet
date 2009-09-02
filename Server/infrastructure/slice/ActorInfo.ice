#ifndef LBANET_ACTOR_HANDLER
#define LBANET_ACTOR_HANDLER

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

	
	struct CurrentAndFutureInfo
	{
		ActorInfo CurrentState;
		ActorInfo FutureState;		
	};

	
	struct ActivationInfo
	{
		// id of the activate object
		long			ActivatedId;
		
		// true if activating - false if desactivating
		bool			Activate;
		
		// send signal instead of activate
		bool			Signal;
		long			SignalId;
	
	
		// activator id
		long			ActorId;
		
		// activator coordinates
		float			X;
		float			Y;
		float			Z;
		float			Rotation;
	};
	
	

	
	interface ActorsObserver
	{
	    void UpdatedInfo(ActorInfo asi);
	    void Quitted(string ActorName);
	    
	    void Activated(ActivationInfo ai);    
	};
	
	
	interface ActorsParticipant
	{
	    void Update(ActorInfo asi);
	    
	    void Activate(ActivationInfo ai);
	};		
};

#endif
