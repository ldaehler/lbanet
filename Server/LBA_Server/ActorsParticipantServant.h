#ifndef _ACTORS_PARTICIPANT_SERVANT_H
#define _ACTORS_PARTICIPANT_SERVANT_H

#include <ActorInfo.h>
#include <RoomManager.h>

using namespace LbaNet;


class ActorsParticipantServant : public ActorsParticipant
{
public:
	//! constructor
	ActorsParticipantServant(const std::string& room, std::string actor_name,
								const ActorsObserverPrx& observer,
								const RoomManagerPrx& manager);
    
	//! destructor
	~ActorsParticipantServant();

	//! update
	virtual void Update(const ActorInfo& asi, const Ice::Current &current);
	
	//! update
	virtual void Activate(const ActivationInfo &ai, const Ice::Current &current);

private:
	const std::string _room;
	const std::string _actor_name;
    const RoomManagerPrx _manager;
    const ActorsObserverPrx _observer;
    const ActorsObserverPrx _publish;
};

#endif
