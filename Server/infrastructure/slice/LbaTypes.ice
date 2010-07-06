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
		bool			Visible;
		
		// actor coordinates
		float			X;
		float			Y;
		float			Z;
		float			Rotation;
		
		// actor size
		float			SizeX;
		float			SizeY;
		float			SizeZ;

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
		
		// life/mana info
		float			CurrentLife;
		float			MaxLife;
		float			CurrentMana;
		float			MaxMana;
		
		
		// reason why life is changing
		// 1- raised from dead
		// 2- hurt by falling down		
		// 3- hurt by actor
		// 4- hurt by player
		// 5- potion
		// 6- drowning
		// 7- other reason
		int			ChangeReason;
		long			ChangeActorId;
		
		// true if change should ake actor play hurt animation
		bool			ShouldHurt;
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
	
	struct TargetedInfo
	{
		long TargetActorId;
		long TargetPlayerId;
	};
	
	sequence<TargetedInfo> TargetedSeq;	
	
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
		
		// NPC info
		bool			Visible;
		TargetedSeq		Targets;
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
		bool InformPlayer;
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
		int 			InventorySize;
		InventoryMap 		InventoryStructure;
		ShortcutSeq 		UsedShorcuts;
	};
	
	
	struct SavedWorldInfo
	{
		PlayerPosition 		ppos;
		InventoryInfo 		inventory;
		
		// life/mana info
		float			CurrentLife;
		float			MaxLife;
		float			CurrentMana;
		float			MaxMana;
	};
	
	
	struct PlayerInfo
	{
		long 			Id;
		string 			Status;
		string 			NameColor;
	};


	dictionary<string, PlayerInfo> ConnectedL;
	
	
	interface FooLba
	{
	    void Update();
	};
	
	
	
	struct LetterInfo
	{
		long Id;
		string Date;
		string Writter;
		string Title;
		string Message;
	};
	
	
	struct LaunchInfo
	{
		float PosX;
		float PosY;
		float PosZ;
		
		float DirX;
		float DirZ;	
		
		int   Mode;
		bool  Enoughmana;
	};
	
	sequence<long> QuestSeq;
	
	
	struct FriendInfo
	{
		long Id;
		string Name;
	};
	
	sequence<FriendInfo> FriendsSeq;
};	

#endif

