#ifndef LBANET_TYPES
#define LBANET_TYPES

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
		int			NameR;
		int			NameG;		
		int			NameB;
		
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
	
	
	
	dictionary<long, int> ItemList;	
	struct ContainerInfo
	{
		long ContainerId;
		long LockedById;
		ItemList Content;
	};	
	
	struct InventoryItem
	{
		int Number;
		int PlaceInInventory;
	};
	
	dictionary<long, InventoryItem> InventoryMap;
	
	
	struct UpdatedItem
	{
		long ItemId;
		int NewCount;
	};
	sequence<UpdatedItem> UpdatedItemSeq;	
	
	struct PlayerPosition
	{
		// actor name
		string			MapName;		

		// actor coordinates
		float			X;
		float			Y;
		float			Z;
		float			Rotation;
	};
	
	
	sequence<long> ShortcutSeq;

	
	struct InventoryInfo
	{
		int InventorySize;
		InventoryMap InventoryStructure;
		ShortcutSeq UsedShorcuts;
	};
	
	
	struct SavedWorldInfo
	{
		PlayerPosition ppos;
		InventoryInfo inventory;
	};
	
	
	struct PlayerInfo
	{
		long Id;
		string Status;
		string NameColor;
	};


	dictionary<string, PlayerInfo> ConnectedL;
	
	
	interface FooLba
	{
	    void Update();
	};	
};

#endif	
	