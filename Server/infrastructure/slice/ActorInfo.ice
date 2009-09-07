#ifndef LBANET_ACTOR_HANDLER
#define LBANET_ACTOR_HANDLER


#include <MapObserver.ice>

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

	
	

	
	interface ActorsObserver
	{
	    void UpdatedInfo(ActorInfo asi);
	    void Quitted(string ActorName);
	    
	    void ActivatedActor(ActorActivationInfo ai);  
	    void SignaledActor(ActorSignalInfo ai);	
	};
	
	
	interface ActorsParticipant
	{
	    void Update(ActorInfo asi);
	};		
};

#endif
