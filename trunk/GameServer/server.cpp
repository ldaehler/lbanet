/*
------------------------[ Lbanet Source ]-------------------------
Copyright (C) 2009
Author: Vivien Delage [Rincevent_123]
Email : vdelage@gmail.com

-------------------------------[ GNU License ]-------------------------------

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

-----------------------------------------------------------------------------
*/

#include "server.h"
#include "LogHandler.h"
#include "PhysXEngine.h"
#include "MainClientThread.h"
#include "MapInfoObject.h"
#include "ActorObject.h"
#include "PlayerObject.h"

#include <boost/thread/thread.hpp>

#define	_CUR_LBANET_SERVER_VERSION_ "v0.8"


/***********************************************************
Constructor
***********************************************************/
Server::Server( int _internalport, int _udpport, 
				unsigned int uplimittotal, unsigned int uplimitperconnection,
				unsigned short downpacketpersecond, unsigned short downbyteperpacket, 
				boost::shared_ptr<ServerDataHandler> dataH, const std::string & MainServerAddress,
				const std::string & MyAddress, DatabaseHandlerBase *dbh)
: m_conncount(0), m_uplimittotal(uplimittotal), m_uplimitperconnection(uplimitperconnection), 
	m_downpacketpersecond(downpacketpersecond), m_downbyteperpacket(downbyteperpacket),
	_dataH(dataH), m_MainServerAddress(MainServerAddress), m_MyAddress(MyAddress), _dbh(dbh)
{
	// this will allocate the sockets and create local bindings
	if ( !ZCom_initSockets( eZCom_EnableUDP, _udpport, _internalport, 0 ) )
	{
		LogHandler::getInstance()->LogToFile("Zoid: Failed to initialize sockets!", 2);
	}

	// string shown in log output
	ZCom_setDebugName("ZCOM_GAME_SRV");

	// maximum 8k/sec upstream and 2k/sec/connection
	ZCom_setUpstreamLimit(m_uplimittotal, m_uplimitperconnection);

	std::stringstream strs;
	strs<<"Server running and listening on udp port: "<<_udpport;
	LogHandler::getInstance()->LogToFile(strs.str(), 2);

	//send advertisement to main server
	AdvertizeToMainServer();


	// register classes
	MapInfoObject::registerClass(this);
	ActorObject::registerClass(this);
	PlayerObject::registerClass(this);

	// populate free map slot list
	// from 10 to 70
	for(int i=10; i<71; ++i)
		m_free_map_slots.push_back(i);

	//populate physic engines
	//TODO - reset to 60
	for(int i=0; i<3/*60*/; ++i)
		m_free_phys_slots.push_back(boost::shared_ptr<PhysXEngine>(new PhysXEngine()));
	

}


/***********************************************************
Destructor
***********************************************************/
Server::~Server()
{
	DeadvertizeToMainServer();
}




/***********************************************************
called on incoming connections
***********************************************************/
eZCom_RequestResult Server::ZCom_cbConnectionRequest( ZCom_ConnID _id, ZCom_BitStream &_request, ZCom_BitStream &_reply )
{
	// retrieve request for login and password
	char login[20];
	_request.getString(login, 20);
	char password[50];
	_request.getString(password, 50);
	char version[20];
	_request.getString(version, 20);

	std::string loginS(login);
	std::string passwordS(password);
	std::string versionS(version);

	// address information
	const ZCom_Address* addr = ZCom_getPeer( _id );
	if ( addr )
	{
		if ( addr->getType() == eZCom_AddressLocal )
		{
			std::stringstream strs;
			strs<<"Server: Incoming connection from localport: "<<addr->getPort();
			LogHandler::getInstance()->LogToFile(strs.str(), 2);    
		}
		else if ( addr->getType() == eZCom_AddressUDP )
		{
			std::stringstream strs;
			strs<<"Server: Incoming connection from UDP: "<<(int)addr->getIP( 0 )<<"."<<(int)addr->getIP( 1 )
				<<"."<<(int)addr->getIP( 2 )<<"."<<(int)addr->getIP( 3 )<<":"<<(int)addr->getPort();
			LogHandler::getInstance()->LogToFile(strs.str(), 2);    
		}
	}

	// check what the client is requesting
	if ( login && password && version)
	{
		if(versionS == _CUR_LBANET_SERVER_VERSION_)
		{
			long db_id = _dbh->CheckLogin(login, password);
			if(db_id >= 0)
			{
				std::stringstream strs;
				strs<<"Server: Incoming connection with ID: "<<_id<<" accepted";
				LogHandler::getInstance()->LogToFile(strs.str(), 2);    

				//add to client list
				_playerDbMap[_id] = db_id;

				//send reply
				_reply.addInt(_id, 32);
				_reply.addString( "Good" );
				return eZCom_AcceptRequest;
			}
			else
			{
				std::stringstream strs;
				strs<<"Server: Incoming connection with ID: "<<_id<<" denied";
				LogHandler::getInstance()->LogToFile(strs.str(), 2);    

				// deny connection request and send reason back to requester
				_reply.addInt(_id, 32);
				_reply.addString( "Incorrect username or password" );
				return eZCom_DenyRequest;
			}
		}
		else
		{
			std::stringstream strs;
			strs<<"Server: Incoming connection with ID: "<<_id<<" denied";
			LogHandler::getInstance()->LogToFile(strs.str(), 2);    

			// deny connection request and send reason back to requester
			_reply.addString( "Server version mismatch - please update your game." );
			return eZCom_DenyRequest;
		}
	}
	else
	{
		std::stringstream strs;
		strs<<"Server: Incoming connection with ID: "<<_id<<" denied";
		LogHandler::getInstance()->LogToFile(strs.str(), 2);    

		// deny connection request and send reason back to requester
		_reply.addString( "Empty username or password" );
		return eZCom_DenyRequest;
	}
}


/***********************************************************
called when incoming connection has been established
***********************************************************/
void Server::ZCom_cbConnectionSpawned( ZCom_ConnID _id )
{
	std::stringstream strs;
	strs<<"Server: Incoming connection with ID: "<<_id<<" has been established.";
	LogHandler::getInstance()->LogToFile(strs.str(), 2);

	
	// create player info
	std::map<unsigned int, long>::iterator itm = _playerDbMap.find(_id);
	if(itm != _playerDbMap.end())
	{
		m_players_infos[_id] = boost::shared_ptr<PlayerInfoHandler> 
			(new PlayerInfoHandler(_playerDbMap[_id], _dbh, _dataH.get()));

		// remove from tmp map
		_playerDbMap.erase(itm);
	}


	// connect player to first map
	ChangePlayerMap(_id);


	// request 20 packets/second and 200 bytes per packet from client (maximum values of course)
	ZCom_requestDownstreamLimit(_id, m_downpacketpersecond, m_downbyteperpacket);

	++m_conncount;
}


/***********************************************************
called when a connection closed
***********************************************************/
void Server::ZCom_cbConnectionClosed( ZCom_ConnID _id, eZCom_CloseReason _reason, ZCom_BitStream &_reasondata )
{
	std::stringstream strs;
	strs<<"Server: Incoming connection with ID: "<<_id<<" has been closed.";
	LogHandler::getInstance()->LogToFile(strs.str(), 2);  

	// make player leave
	PlayerLeave(_id);


	//finalize player status
	std::map<unsigned int, boost::shared_ptr<PlayerInfoHandler> >::iterator itm = m_players_infos.find(_id);
	if(itm != m_players_infos.end())
	{
		//do last save to DB
		itm->second->SaveToDatabase();

		//remove it from map
		m_players_infos.erase(itm);
	}



	//decrease client connected count
	--m_conncount;
}



/***********************************************************
called when a connection wants to enter a channel
***********************************************************/
#ifdef _ZOID_USED_NEW_VERSION_
eZCom_RequestResult Server::ZCom_cbChannelSubscriptionChangeRequest( ZCom_ConnID _id, 
											zU32 _requested_channel, ZCom_BitStream &_reason )
#else
bool Server::ZCom_cbZoidRequest( ZCom_ConnID _id, zU8 _requested_channel, ZCom_BitStream &_reason)
#endif
{
	std::stringstream strs;
	strs<<"Server: Incoming connection with ID: "<<_id<<" wants to enter the channel "<<_requested_channel;
	LogHandler::getInstance()->LogToFile(strs.str(), 2);

	return eZCom_AcceptRequest;
}


/***********************************************************
called when a connection enters a channel
***********************************************************/
#ifdef _ZOID_USED_NEW_VERSION_
void Server::ZCom_cbChannelSubscriptionChangeResult( ZCom_ConnID _id, eZCom_SubscriptionResult _result, 
													zU32 _new_channel, ZCom_BitStream &_reason )
#else
void Server::ZCom_cbZoidResult(ZCom_ConnID _id, eZCom_ZoidResult _result, zU8 _new_channel, ZCom_BitStream &_reason)
#endif
{
	//channel connection accepted
	if(_result == eZCom_RequestedChannelSubscribed || _result == eZCom_AutomaticChannelSubscribed)
	{
		std::stringstream strs;
		strs<<"Server: Incoming connection with ID: "<<_id<<" entered the channel "<<_new_channel;
		LogHandler::getInstance()->LogToFile(strs.str(), 2);
	}
}




/***********************************************************
process server internal stuff
***********************************************************/
void Server::Process()
{
	std::map<std::string, std::vector<boost::shared_ptr<ServerMapManager> > >::iterator it = m_opened_maps.begin();
	while(it != m_opened_maps.end())
	{
		std::vector<boost::shared_ptr<ServerMapManager> > & tmpvec = it->second;
		std::vector<boost::shared_ptr<ServerMapManager> >::iterator itvec = tmpvec.begin();
		
		// process map along the map vector
		while(itvec != tmpvec.end())
		{
			(*itvec)->Process();

			//check if need to be deleted
			if((*itvec)->DeleteMe())
			{
				// add slot back to the free slot list
				m_free_map_slots.push_back((*itvec)->GetZoidLevel());

				//add physic slot back
				m_free_phys_slots.push_back((*itvec)->GetPhysicEngine());

				// delete map
				itvec = tmpvec.erase(itvec);
			}
			else
				++itvec;
		}

		//remove empty vector
		if(tmpvec.size() > 0)
			++it;
		else
			it = m_opened_maps.erase(it);
	}
	

	//process queue of waiting player
	ProcessPlayerQueue();
}



/***********************************************************
called when player enter the map
***********************************************************/
void Server::PlayerEnter(unsigned int PlayerId, const std::string & MapName, 
							boost::shared_ptr<PlayerInfoHandler> pinfo)
{
	// make player leave previous map
	PlayerLeave(PlayerId);

	//push player on connection queue
	m_player_queue.push_back(WaitingPlayer(PlayerId, MapName, pinfo, ZoidCom::getTime()));
}


/***********************************************************
called when player leave the map
***********************************************************/
void Server::PlayerLeave(unsigned int PlayerId)
{
	//remove player from waiting list
	std::list<WaitingPlayer>::iterator itwp = 
		std::find(m_player_queue.begin(), m_player_queue.end(), WaitingPlayer(PlayerId));
	if(itwp != m_player_queue.end())
		m_player_queue.erase(itwp);


	// remove player from list
	std::map<unsigned int, boost::shared_ptr<ServerMapManager> >::iterator itp = m_connected_players.find(PlayerId);
	if(itp != m_connected_players.end())
	{
		// make player leave map
		itp->second->PlayerLeave(PlayerId);

		//remove player from connected list
		m_connected_players.erase(itp);	
	}


	//tell player to change to default zoid level
	ZCom_BitStream *stre = new ZCom_BitStream();

	//send event 1
	stre->addInt(1, 4);
	stre->addInt(1, 32);
	ZCom_sendData(PlayerId, stre);
}




/***********************************************************
called to connect the player to a certain map
return true if connected correctly
***********************************************************/
bool Server::ConnectPlayerToMap(unsigned int PlayerId, const std::string & MapName, 
									boost::shared_ptr<PlayerInfoHandler> pinfo)
{
	//check if map is already open
	std::map<std::string, std::vector<boost::shared_ptr<ServerMapManager> > >::iterator itm = 
																		m_opened_maps.find(MapName);
	if(itm != m_opened_maps.end())
	{
		//in this case check if map is already full
		std::vector<boost::shared_ptr<ServerMapManager> > & tmpvec = itm->second;
		std::vector<boost::shared_ptr<ServerMapManager> >::iterator itvec = tmpvec.begin();
			std::vector<boost::shared_ptr<ServerMapManager> >::iterator endvec = tmpvec.end();

		// process map along the map vector
		for(;itvec != endvec; ++itvec)
		{	
			if(!(*itvec)->IsFull())
			{
				// add player to the map
				(*itvec)->PlayerEnter(PlayerId, pinfo);

				//add player to the connected list
				m_connected_players[PlayerId] = *itvec;

				//tell player to change zoid level
				ZCom_BitStream *stre = new ZCom_BitStream();

				//send event 1
				stre->addInt(1, 4);
				stre->addInt((*itvec)->GetZoidLevel(), 32);
				ZCom_sendData(PlayerId, stre);

				return true;
			}
		}
	}


	//if no map avaiable or map are full - try to create a new map
	boost::shared_ptr<ServerMapManager> map = TryCreateMap(MapName);
	if(map)
	{
		//add map to the map list
		m_opened_maps[MapName].push_back(map);


		// add player to the map
		map->PlayerEnter(PlayerId, pinfo);	

		//add player to the connected list
		m_connected_players[PlayerId] = map;

		//tell player to change zoid level
		ZCom_BitStream *stre = new ZCom_BitStream();

		//send event 1
		stre->addInt(1, 4);
		stre->addInt(map->GetZoidLevel(), 32);
		ZCom_sendData(PlayerId, stre);

		return true;
	}

	// if nothing worked then return false
	return false;
}



/***********************************************************
process player connection queue
***********************************************************/
void Server::ProcessPlayerQueue()
{
	//go along the player queue
	std::list<WaitingPlayer>::iterator itp = m_player_queue.begin();
	while(itp != m_player_queue.end())
	{
		//check if can connect successfully
		if(ConnectPlayerToMap(itp->Id, itp->MapWanted, itp->Playerinfo))
		{
			//then remove from queue
			itp = m_player_queue.erase(itp);
		}
		else // else skip for next time
		{
			// if player is waiting more than 10 seconds
			if((ZoidCom::getTime() - itp->Time) > 10000)
			{
				//if player waiting for long then inform the client
				ZCom_BitStream *stre = new ZCom_BitStream();

				//send event 0
				stre->addInt(0, 4);
				ZCom_sendData(itp->Id, stre);
			}

			++itp;
		}
	}


}


/***********************************************************
 try to create a new map
 return map handler if created
 else return NULL
***********************************************************/
boost::shared_ptr<ServerMapManager> Server::TryCreateMap(const std::string & MapName)
{
	//check if we still have free slots
	if(m_free_map_slots.size() > 0 && m_free_phys_slots.size() > 0)
	{
		//get map info
		MapInfo MI = _dataH->GetMapInfo(MapName);

		// get slots and remove them from list
		unsigned int lvl = m_free_map_slots.front();
		boost::shared_ptr<PhysXEngine> phys = m_free_phys_slots.front();
		m_free_map_slots.pop_front();
		m_free_phys_slots.pop_front();

		return boost::shared_ptr<ServerMapManager>(new ServerMapManager(this, lvl, MI, phys));
	}

	//if can not create then return NULL
	return boost::shared_ptr<ServerMapManager>();
}



/***********************************************************
advertize game server to main server when arriving
***********************************************************/
void Server::AdvertizeToMainServer()
{
	//be carefull - delete will never be called
	// should only be called once at the beginning of the program
	MainClientThread *mtt = new MainClientThread(m_MainServerAddress, _dataH->GetWorlName(), m_MyAddress, true);
	boost::thread thr( boost::bind( &MainClientThread::Run, mtt ));
}


/***********************************************************
deadvertize game server to main server when quitting
***********************************************************/
void Server::DeadvertizeToMainServer()
{
	// be carefull - delete will never be called
	// should only be called once at the end of the program
	MainClientThread *mtt = new MainClientThread(m_MainServerAddress, _dataH->GetWorlName(), m_MyAddress, false);
	boost::thread thr( boost::bind( &MainClientThread::Run, mtt ));
}



/***********************************************************
if needed, change the map the player is currently connected
***********************************************************/
void Server::ChangePlayerMap(unsigned int PlayerId)
{
	std::map<unsigned int, boost::shared_ptr<PlayerInfoHandler> >::iterator it = m_players_infos.find(PlayerId);
	if(it != m_players_infos.end())
	{
		//get map to connect
		std::string nextmap = it->second->GetNextMap();

		//connect to the map
		PlayerEnter(PlayerId, nextmap, it->second);
	}
	else
	{
		// we do not have player info - log the error
		std::stringstream strs;
		strs<<"Server: Player info missing for player id: "<<PlayerId;
		LogHandler::getInstance()->LogToFile(strs.str(), 2);    
	}
}