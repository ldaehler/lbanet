#include "MapHandler.h"
#include "InfosReceiverServant.h"
#include "MapInfoXmlReader.h"
#include "ServerSignaler.h"

/***********************************************************
	Constructor
***********************************************************/
MapHandler::MapHandler(const Ice::CommunicatorPtr& communicator, const Ice::ObjectAdapterPtr & adapter,
								const std::string mapName)
: _communicator(communicator), _adapter(adapter), _mapName(mapName), _publisher(NULL), _stop(false)
{
	_signaler = new ServerSignaler(this);

	Ice::ObjectPrx proxy = _adapter->addWithUUID(this);
	_proxy = LbaNet::MapObserverPrx::uncheckedCast(proxy);

	IcestormSubscribe();

	std::string worldname = mapName.substr(0, mapName.find("_"));
	std::string mapS = mapName.substr(mapName.find("_")+1);
	mapS = mapS.substr(0, mapS.find_last_of("_"));
	WorldInfo wi;
	MapInfoXmlReader::LoadWorld("Data/" + worldname + ".xml", wi);
	std::string localfile = wi.Maps[mapS].Files["LocalActors"];
	std::string externalfile = wi.Maps[mapS].Files["ExternalActors"];

	std::map<long, SpriteInfo> tmp;
	std::map<long, ModelInfo> tmp2;
	MapInfoXmlReader::LoadActors("Data/" + localfile, tmp, tmp, tmp2, _actors, _signaler);

	std::map<long, Actor *> map;
	MapInfoXmlReader::LoadActors("Data/" + externalfile, tmp, tmp, tmp2, map, _signaler);
	std::map<long, Actor *>::iterator it = map.begin();
	std::map<long, Actor *>::iterator end = map.end();
	for(;it != end; ++it)
		_actors[it->first] = it->second;


	_threadC = this->start();
}


/***********************************************************
	destructor
***********************************************************/
MapHandler::~MapHandler()
{
}



/***********************************************************
	destroy the map handler
***********************************************************/
void MapHandler::Destroy()
{
	Lock sync(*this);
	_stop = true;

	IcestormUnsubscribe();
	_adapter->remove(_proxy->ice_getIdentity());
	_adapter->remove(_observer->ice_getIdentity());

	std::map<long, Actor *>::iterator it = _actors.begin();
	std::map<long, Actor *>::iterator end = _actors.end();
	for(;it != end; ++it)
		delete it->second;

	delete _signaler;
}



/***********************************************************
	callback function called when an actor id activated
***********************************************************/
void MapHandler::ActivateActor(const LbaNet::ActorActivationInfo& ai, const Ice::Current&)
{
	Lock sync(*this);
	if(_publisher)
		_publisher->ActivatedActor(ai);

	std::map<long, Actor *>::iterator it =	_actors.find((long)ai.ActivatedId);
	if(it != _actors.end())
	{
		if(ai.Activate)
		{
			it->second->ProcessActivation(ai.X, ai.Y, ai.Z, ai.Rotation);

			if(it->second->NeedDesactivation())
				_todeactivate[ai.ActorId] = ai.ActivatedId;
		}
		else
		{
			it->second->ProcessDesactivation(ai.X, ai.Y, ai.Z, ai.Rotation);
			std::map<Ice::Long, Ice::Long>::iterator itpd =_todeactivate.find(ai.ActorId);
			if(itpd != _todeactivate.end())
				_todeactivate.erase(itpd);
		}
	}

}

/***********************************************************
	callback function called when an actor id signaled
***********************************************************/
void MapHandler::SignalActor(const LbaNet::ActorSignalInfo& ai, const Ice::Current&)
{
	Lock sync(*this);
	if(_publisher)
		_publisher->SignaledActor(ai);

	for(size_t i=0; i< ai.Targets.size(); ++i)
	{
		std::map<long, Actor *>::iterator it =	_actors.find((long)ai.Targets[i]);
		if(it != _actors.end())
			it->second->OnSignal(ai.SignalId);
	}
}

/***********************************************************
	callback function called when a message is received from IceStorm
***********************************************************/
void MapHandler::UpdatedInfo(const LbaNet::ActorInfo& asi)
{
	IceUtil::Mutex::Lock lock(m_mutex_players);
	std::map<Ice::Long, LbaNet::ActorInfo>::iterator it = _players.find(asi.ActorId);
	if(it != _players.end())
		it->second = asi;
}



/***********************************************************
	get proxy to the map observer
***********************************************************/
LbaNet::MapObserverPrx MapHandler::GetMapProxy()
{
	return _proxy;
}


/***********************************************************
	a player join a map
***********************************************************/
void MapHandler::Join(Ice::Long PlayerId)
{
	IceUtil::Mutex::Lock lock(m_mutex_players);
	_players[PlayerId] = LbaNet::ActorInfo();
}

/***********************************************************
 a player leave a map
***********************************************************/
bool MapHandler::Leave(Ice::Long PlayerId)
{
	IceUtil::Mutex::Lock lock(m_mutex_players);
	Lock sync(*this);

	std::map<Ice::Long, Ice::Long>::iterator it =_todeactivate.find(PlayerId);
	std::map<Ice::Long, LbaNet::ActorInfo>::iterator itp = _players.find(PlayerId);

	if(it != _todeactivate.end())
	{
		if(itp != _players.end())
		{
			_actors[(long)it->second]->ProcessDesactivation(itp->second.X, itp->second.Y, itp->second.Z, itp->second.Rotation);

			if(_publisher)
			{
				LbaNet::ActorActivationInfo ai;
				ai.Activate = false;
				ai.ActivatedId = it->second;
				ai.ActorId = it->first;
				ai.X = itp->second.X;
				ai.Y = itp->second.Y;
				ai.Z = itp->second.Z;
				ai.Rotation = itp->second.Rotation;
				_publisher->ActivatedActor(ai);
			}
		}

		_todeactivate.erase(it);
	}


	if(itp != _players.end())
		_players.erase(itp);


	return (_players.size() == 0);
}

/***********************************************************
subscribe to icestorm
***********************************************************/
void MapHandler::IcestormSubscribe()
{
	Ice::ObjectPrx proxy = _adapter->addWithUUID(new InfosReceiverServant(this));
	_observer = LbaNet::ActorsObserverPrx::uncheckedCast(proxy);
	
	IceStorm::TopicManagerPrx manager;
	try
	{
		manager = IceStorm::TopicManagerPrx::uncheckedCast(
		_communicator->propertyToProxy("TopicManagerProxy"));
	}
	catch(const IceUtil::Exception& ex)
	{
		std::cout<<"MapHandler - Exception getting the actor topic manager: "<<ex.what()<<std::endl;
	}
	catch(...)
	{
		std::cout<<"MapHandler - Unknown exception getting the actor topic manager. "<<std::endl;
	}

	try
	{
		_topic = manager->create(_mapName);
	}
	catch(const IceStorm::TopicExists&)
	{
		_topic = manager->retrieve(_mapName);
	}
	catch(const IceUtil::Exception& ex)
	{
		std::cout<<"MapHandler - Exception creating actor topic "<<ex.what()<<std::endl;
	}
	catch(...)
	{
		std::cout<<"MapHandler - Unknown exception creating actor topic. "<<std::endl;
	}



	try
	{
		_topic->subscribeAndGetPublisher(IceStorm::QoS(), _observer);
	}
	catch(const IceUtil::Exception& ex)
	{
		std::cout<<"MapHandler - Exception creating actor publisher "<<ex.what()<<std::endl;
	}
	catch(...)
	{
		std::cout<<"MapHandler - Unknown exception creating actor publisher. "<<std::endl;
	}



	Ice::ObjectPrx pub; 
	try
	{
		pub = _topic->getPublisher();
		_publisher = ActorsObserverPrx::uncheckedCast(pub->ice_oneway());
	}
	catch(const IceUtil::Exception& ex)
	{
		std::cout<<"MapHandler - Exception getting the publisher "<<ex.what()<<std::endl;
	}
	catch(...)
	{
		std::cout<<"MapHandler - Unknown exception getting the publisher. "<<std::endl;
	}
}


/***********************************************************
unsubscribe to icestorm
***********************************************************/
void MapHandler::IcestormUnsubscribe()
{
	try
	{
		_topic->unsubscribe(_observer);
	}
	catch(const IceUtil::Exception& ex)
	{
		std::cout<<"MapHandler - Exception unsubscribing actor"<<ex.what()<<std::endl;
	}
	catch(...)
	{
		std::cout<<"MapHandler - Unknown exception unsubscribing actor. "<<std::endl;
	}
}


/***********************************************************
send signal
***********************************************************/
void MapHandler::SendSignal(long signal, const std::vector<long> &targets)
{
	std::vector<long>::const_iterator it = targets.begin();
	std::vector<long>::const_iterator end = targets.end();
	for(; it != end; ++it)
	{
		std::map<long, Actor *>::iterator itm = _actors.find(*it);
		if(itm != _actors.end())
			itm->second->OnSignal(signal);
	}
}


/***********************************************************
running thread
***********************************************************/
void MapHandler::run()
{
	_lasttime = IceUtil::Time::now().toMilliSecondsDouble();
	while(true)
	{
		_threadC.sleep(IceUtil::Time::milliSeconds(15));
		Lock sync(*this);
		if(_stop)
			break;

		double tnow = IceUtil::Time::now().toMilliSecondsDouble();
		float tdiff = (float)(tnow - _lasttime);
		_lasttime = tnow;

		std::map<long, Actor *>::iterator it =_actors.begin();
		std::map<long, Actor *>::iterator end =_actors.end();
		for(; it != end; ++it)
			it->second->Process(tnow, tdiff);
	}
}


/***********************************************************
get updated info
***********************************************************/
LbaNet::UpdateSeq MapHandler::GetUpdatedInfo(const Ice::Current&)
{
	LbaNet::UpdateSeq res;
	Lock sync(*this);

	std::map<long, Actor *>::iterator it =_actors.begin();
	std::map<long, Actor *>::iterator end =_actors.end();
	ActorStateInfo actState;
	for(; it != end; ++it)
	{
		if(it->second->Getstate(actState))
		{
			LbaNet::ActorUpdateInfo ui;
			ui.ActorId = it->first;
			ui.On = actState.On;
			ui.Open = actState.Open;
			ui.Counter = actState.Counter;
			ui.SignalOn = actState.SignalOn;
			ui.CurrentScript = actState.CurrentScript;
			ui.X = actState.X;
			ui.Y = actState.Y;
			ui.Z = actState.Z;
			ui.Rotation = actState.Rotation;

			std::vector<long>::iterator itcs = actState.CurrentSignals.begin();
			std::vector<long>::iterator endcs = actState.CurrentSignals.end();
			for(; itcs != endcs; ++itcs)
				ui.CurrentSignals.push_back(*itcs);

			res.push_back(ui);
		}
	}

	return res;
}