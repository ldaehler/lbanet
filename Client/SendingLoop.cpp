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

#include "SendingLoop.h"
#include "InfosReceiverServant.h"
#include "ChatReceiverServant.h"
#include "ThreadSafeWorkpile.h"
#include "LogHandler.h"
#include "GameEvents.h"

#include <cctype>
#include <Ice/Application.h>

/***********************************************************
constructor
***********************************************************/
IceConnectionManager::IceConnectionManager(	const Ice::ObjectAdapterPtr& adapter,
											const LbaNet::ClientSessionPrx& session,
											const std::string& category)
	: _adapter(adapter), _session(session), _category(category)
{
	try
	{
		// create the actor observer - we keep for the whole game as we only need one
		Ice::Identity id;
		id.name = "ActorObserver";
		id.category = _category;
		_actors_observer = LbaNet::ActorsObserverPrx::uncheckedCast(
												_adapter->add(new InfosReceiverServant(), id)->ice_oneway());
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Exception creating Actor observer: ") + ex.what());
    }
    catch(...)
    {
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception creating Actor observer. "));
    }

	JoinChat("World");
}


/***********************************************************
find a chat room if we already joined
***********************************************************/
LbaNet::ChatRoomParticipantPrx IceConnectionManager::FindChatRoom(const std::string& room) const
{
	std::map<std::string, LbaNet::ChatRoomParticipantPrx>::const_iterator p = _chat_publishers.find(room);
	LbaNet::ChatRoomParticipantPrx r;
	if(p != _chat_publishers.end())
	{
		r = p->second;
	}
	else
	{
		std::string upercroom = room;
		upercroom[0] = std::toupper(upercroom[0]);

		p = _chat_publishers.find(upercroom);
		if(p != _chat_publishers.end())
		{
			r = p->second;
		}
	}
	return r;
}


/***********************************************************
join a chat channel
***********************************************************/
bool IceConnectionManager::JoinChat(const std::string& roomName)
{
	bool succed = true;

	try
	{
		Ice::Identity id;
		id.name = "ChatObserver." + roomName;
		id.category = _category;
		LbaNet::ChatRoomObserverPrx observer = LbaNet::ChatRoomObserverPrx::uncheckedCast(
		_adapter->add(new ChatReceiverServant(roomName), id)->ice_oneway());

		LbaNet::ChatRoomParticipantPrx pub = _session->JoinChat(roomName, observer);

		_chat_observers.insert(make_pair(roomName, observer));
		_chat_publishers.insert(make_pair(roomName, pub));
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Exception entering chat room ") + ex.what());
		succed = false;
    }
    catch(...)
    {
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception entering chat room ") + roomName);
		succed = false;
    }

	return succed;
}


/***********************************************************
leave a chat channel
***********************************************************/
bool IceConnectionManager::LeaveChat(const std::string& roomName)
{
	bool succed = true;

	try
	{
		std::map<std::string, LbaNet::ChatRoomObserverPrx>::iterator it = _chat_observers.find(roomName);
		if(it != _chat_observers.end())
		{
			_session->LeaveChat(roomName, it->second);
			_adapter->remove(it->second->ice_getIdentity());
			_chat_observers.erase(roomName);
			_chat_publishers.erase(roomName);
		}
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Exception leaving chat room ") + roomName + ex.what());
		succed = false;
    }
    catch(...)
    {
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception leaving chat room ") + roomName);
		succed = false;
    }

	return succed;
}


/***********************************************************
change the room for actor info
***********************************************************/
bool IceConnectionManager::ChangeActorsRoom(const std::string& newRoom, const std::string& actorName)
{
	bool succed = true;


	// change the actor connection
	try
	{
		_actors_publisher = _session->ChangeRoom(newRoom, actorName, _actors_observer);

		LbaNet::UpdateSeq actorsstates =_session->GetUpdatedInfo();
		std::vector<ActorStateInfo> newstate;
		for(size_t i=0; i<actorsstates.size(); ++i)
		{
			ActorStateInfo asi;
			asi.ActorId = actorsstates[i].ActorId;

			// used for switch
			asi.On = actorsstates[i].On;

			//used for door
			asi.Open = actorsstates[i].Open;
			asi.Counter = actorsstates[i].Counter;
			asi.SignalOn = actorsstates[i].SignalOn;

			//used for lift
			asi.CurrentScript = actorsstates[i].CurrentScript;
			LbaNet::TargetSeq::iterator itcs = actorsstates[i].CurrentSignals.begin();
			LbaNet::TargetSeq::iterator endcs = actorsstates[i].CurrentSignals.end();
			for(; itcs != endcs; ++itcs)
				asi.CurrentSignals.push_back(*itcs);

			asi.X = actorsstates[i].X;
			asi.Y = actorsstates[i].Y;
			asi.Z = actorsstates[i].Z;
			asi.Rotation = actorsstates[i].Rotation;

			newstate.push_back(asi);
		}
		ThreadSafeWorkpile::getInstance()->UpdateActorState(newstate);

		// add players already present in the map
		LbaNet::PlayerSeq players =_session->GetPlayersInfo();
		for(size_t i=0; i<players.size(); ++i)
		{
			ThreadSafeWorkpile::getInstance()->UpdateActor(players[i].ai);
			ThreadSafeWorkpile::getInstance()->UpdateActorLife(players[i].li);
		}

	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Exception changing room connection: ")+ ex.what());
		succed = false;
    }
    catch(...)
    {
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception changing room connention: "));
		succed = false;
    }

	return succed;
}


/***********************************************************
get the publisher to the actors channel
***********************************************************/
LbaNet::ActorsParticipantPrx IceConnectionManager::GetActorPublisher()
{
	return _actors_publisher;
}



/***********************************************************
activate an actor
***********************************************************/
void IceConnectionManager::ActivateActor(const LbaNet::ActorActivationInfo & info)
{
	_session->ActivateActor(info);
}

/***********************************************************
signal an actor
***********************************************************/
void IceConnectionManager::SignalActor(const LbaNet::ActorSignalInfo & info)
{
	_session->SignalActor(info);
}

/***********************************************************
change player status
***********************************************************/
void IceConnectionManager::ChangeStatus(const std::string& status)
{
	try
	{
		_session->ChangeStatus(status);
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Exception by ChangeStatus: ")+ ex.what());
    }
    catch(...)
    {
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception by ChangeStatus "));
    }
}



/***********************************************************
change player color name
***********************************************************/
void IceConnectionManager::ChangeColor(const std::string& color)
{
	try
	{
		_session->ChangeNameColor(color);
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Exception by ChangeColor: ")+ ex.what());
    }
    catch(...)
    {
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception by ChangeColor "));
    }
}


/***********************************************************
return current player life state
***********************************************************/
LbaNet::ActorLifeInfo IceConnectionManager::GetPlayerLife()
{
	try
	{
		return _session->GetLifeInfo();
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Exception GetPlayerLife: ")+ ex.what());
    }
    catch(...)
    {
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception GetPlayerLife "));
    }

	return LbaNet::ActorLifeInfo ();
}


/***********************************************************
send player hurt event
***********************************************************/
void IceConnectionManager::AddPlayerHurt(long hurtingid)
{
	try
	{
		_session->GotHurtByActor(hurtingid);
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Exception AddPlayerHurt: ")+ ex.what());
    }
    catch(...)
    {
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception AddPlayerHurt "));
    }
}

/***********************************************************
send player hurt fall event
***********************************************************/
void IceConnectionManager::AddPlayerHurtFall(float fallingdistance)
{
	try
	{
		_session->GotHurtByFalling(fallingdistance);
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Exception AddPlayerHurtFall: ")+ ex.what());
    }
    catch(...)
    {
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception AddPlayerHurtFall "));
    }
}


/***********************************************************
player has been raised
***********************************************************/
void IceConnectionManager::PlayerRaised()
{
	try
	{
		_session->PlayerRaisedFromDead();
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Exception PlayerRaised: ")+ ex.what());
    }
    catch(...)
    {
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception PlayerRaised "));
    }
}



/***********************************************************
constructor
***********************************************************/
SendingLoopThread::SendingLoopThread(	const Ice::ObjectAdapterPtr& adapter, const LbaNet::ClientSessionPrx& session,
										const std::string& category,
										const std::string & MainName)
	: _main_name(MainName), _refresh_counter(0), _afk_counter(0),
			_connectionMananger(adapter, session, category), _afked(false)
{

}

/***********************************************************
running function of the thread
***********************************************************/
void SendingLoopThread::run()
{
	// for each cycle
	while(ThreadSafeWorkpile::getInstance()->WaitOneSendingCycle())
	{
		HandleChatText();


		//-----------------------------------
		// player raised
		{
			if(ThreadSafeWorkpile::getInstance()->IsRaised())
				_connectionMananger.PlayerRaised();
		}

		//-----------------------------------
		// process hurt info
		{
			std::vector<long>  vechurt;
			std::vector<float>  vechurtfall;
			ThreadSafeWorkpile::getInstance()->GetPlayerHurts(vechurt);
			ThreadSafeWorkpile::getInstance()->GetPlayerHurtFalls(vechurtfall);

			for(size_t i=0; i<vechurt.size(); ++i)
				_connectionMananger.AddPlayerHurt(vechurt[i]);

			for(size_t i=0; i<vechurtfall.size(); ++i)
				_connectionMananger.AddPlayerHurtFall(vechurtfall[i]);
		}


		//-----------------------------------
		// process color change 
		{
			std::string newcolor;
			if(ThreadSafeWorkpile::getInstance()->NameColorChanged(newcolor))
				_connectionMananger.ChangeColor(newcolor);
		}

		//-----------------------------------
		// process info
		std::string NewWorldName, NewMapName;
		if(ThreadSafeWorkpile::getInstance()->HasMapChanged(NewWorldName, NewMapName))
		{
			ChangeMap(NewWorldName, NewMapName);
		}


		if(ThreadSafeWorkpile::getInstance()->HasUpdatedInfo(_last_ai))
		{
			_last_ai.ActorId = ThreadSafeWorkpile::getInstance()->GetPlayerId();
			UpdateActorInfo(_last_ai);
			_refresh_counter = 0;
			_afk_counter = 0;
			if(_afked)
			{
				_afked = false;
				_connectionMananger.ChangeStatus("");
			}
		}
		else // refresh info to server every 5sec in case nothing is happening
		{
			++_refresh_counter;
			if(_refresh_counter == 200)
			{
				UpdateActorInfo(_last_ai);
				_refresh_counter = 0;
				++_afk_counter;
				if(_afk_counter == 60) // afk after 5min
				{
					_afked = true;
					_connectionMananger.ChangeStatus("away");
				}
			}
		}

		//send activation events
		{
			std::vector<LbaNet::ActorActivationInfo> vec;
			ThreadSafeWorkpile::getInstance()->GetAllActivationEvents(vec);
			for(size_t i=0; i<vec.size(); ++i)
			{
				vec[i].ActorId = ThreadSafeWorkpile::getInstance()->GetPlayerId();
				AddActivation(vec[i]);
			}
		}

		//send signal events
		{
			std::vector<LbaNet::ActorSignalInfo> vecsig;
			ThreadSafeWorkpile::getInstance()->GetAllSignalEvents(vecsig);
			for(size_t i=0; i<vecsig.size(); ++i)
			{
				vecsig[i].ActorId = ThreadSafeWorkpile::getInstance()->GetPlayerId();
				AddSignal(vecsig[i]);
			}
		}

	}
}


/***********************************************************
ChangeMap
***********************************************************/
bool SendingLoopThread::ChangeMap(const std::string & NewWorldName, const std::string & NewMapName)
{
	// change the actor connection
	bool succeed = _connectionMananger.ChangeActorsRoom(NewWorldName + "_" + NewMapName+"_Actors", _main_name);


	// disconnect the old map chat channel
	_connectionMananger.LeaveChat(_current_world + "_" + _current_map);

	// connect to the new map channel
	_connectionMananger.JoinChat(NewWorldName + "_" + NewMapName);


	// update the current map name
	if(succeed)
	{
		_current_world = NewWorldName;
		_current_map = NewMapName;

		LbaNet::ActorLifeInfo ai = _connectionMananger.GetPlayerLife();
		ThreadSafeWorkpile::getInstance()->AddEvent(new PlayerLifeChangedEvent(ai.CurrentLife, 
													ai.MaxLife, ai.CurrentMana, ai.MaxMana));
	}
	else
	{
		_current_world = "";
		_current_map = "";
	}

	return succeed;
}

/***********************************************************
UpdateInfo
***********************************************************/
void SendingLoopThread::UpdateActorInfo(const LbaNet::ActorInfo & MainInfo)
{
	if(_current_map == "")
		return;

	try
	{
		_connectionMananger.GetActorPublisher()->Update(MainInfo);
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Disconnected from room : ") + ex.what());
		_current_map = "";
    }
	catch(...)
	{
		LogHandler::getInstance()->LogToFile(std::string("Disconnected from room : ") + _current_map);
		_current_map = "";
	}
}


/***********************************************************
when the engine update information
***********************************************************/
void SendingLoopThread::AddActivation(const LbaNet::ActorActivationInfo & info)
{
	if(_current_map == "")
		return;

	try
	{
		_connectionMananger.ActivateActor(info);
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Disconnected from room : ") + ex.what());
		_current_map = "";
    }
	catch(...)
	{
		LogHandler::getInstance()->LogToFile(std::string("Disconnected from room : ") + _current_map);
		_current_map = "";
	}
}


/***********************************************************
when the engine update information
***********************************************************/
void SendingLoopThread::AddSignal(const LbaNet::ActorSignalInfo & info)
{
	if(_current_map == "")
		return;

	try
	{
		_connectionMananger.SignalActor(info);
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Disconnected from room : ") + ex.what());
		_current_map = "";
    }
	catch(...)
	{
		LogHandler::getInstance()->LogToFile(std::string("Disconnected from room : ") + _current_map);
		_current_map = "";
	}
}




/***********************************************************
SendText
***********************************************************/
void SendingLoopThread::HandleChatText()
{
	std::vector<std::string> texts;
	ThreadSafeWorkpile::getInstance()->GetChatText(texts);

	try
	{
		if(texts.size() > 0)
		{
			_refresh_counter = 0;
			_afk_counter = 0;
			if(_afked)
			{
				_afked = false;
				_connectionMananger.ChangeStatus("");
			}
		}

		std::vector<std::string>::const_iterator it = texts.begin();
		std::vector<std::string>::const_iterator end = texts.end();
		for(;it != end; ++it)
		{
			ProcessText(*it);
		}
	}
    catch(const IceUtil::Exception& ex)
    {
		LogHandler::getInstance()->LogToFile(std::string("Disconnected from chat : ") + ex.what());
		_current_map = "";
    }
	catch(...)
	{
		LogHandler::getInstance()->LogToFile(std::string("Unknown exception sending text to chat"));
	}
}


/***********************************************************
process a line of text
***********************************************************/
void SendingLoopThread::ProcessText(const std::string & Text)
{
	// return if text is empty
	if(Text == "")
		return;

	// if it is a command - preproccess it
	if(Text[0] == '/')
	{
		std::vector<std::string> tok;
		std::stringstream ss(Text);
		std::string buf;
		while(ss >> buf)
		{
			tok.push_back(buf);
		}

		if(tok.size() > 0)
		{
			if(tok[0] == "/join")
			{
				if(tok.size() != 2)
				{
					ThreadSafeWorkpile::ChatTextData cdata;
					cdata.Channel = "World";
					cdata.Sender = "info";
					cdata.Text = "You need to specify a room to join.";
					ThreadSafeWorkpile::getInstance()->AddChatData(cdata);
					return;
				}

				std::string roomName = tok[1];
				if(_connectionMananger.FindChatRoom(roomName))
				{
					ThreadSafeWorkpile::ChatTextData cdata;
					cdata.Channel = "World";
					cdata.Sender = "info";
					cdata.Text = "You already joined channel "+roomName;
					ThreadSafeWorkpile::getInstance()->AddChatData(cdata);
					return;
				}

				_connectionMananger.JoinChat(roomName);
				//_cb->AddText("Joined channel "+roomName);
				return;
			}

			if(tok[0] == "/leave")
			{
				if(tok.size() != 2)
				{
					ThreadSafeWorkpile::ChatTextData cdata;
					cdata.Channel = "World";
					cdata.Sender = "info";
					cdata.Text = "You need to specify a room to leave.";
					ThreadSafeWorkpile::getInstance()->AddChatData(cdata);
					return;
				}

				std::string roomName = tok[1];
				_connectionMananger.LeaveChat(roomName);
				//_cb->AddText("Left channel "+roomName);
				return;
			}

			if(tok[0] == "/afk")
			{
				_connectionMananger.ChangeStatus("away");
				return;
			}

			if(tok[0] == "/back")
			{
				_connectionMananger.ChangeStatus("");
				return;
			}

			LbaNet::ChatRoomParticipantPrx room = _connectionMananger.FindChatRoom(tok[0].substr(1));
			if(room)
			{
				room->Say(_main_name, Text.substr(Text.find_first_of(' ')+1));
				return;
			}
			else
			{
					ThreadSafeWorkpile::ChatTextData cdata;
					cdata.Channel = "World";
					cdata.Sender = "info";
					cdata.Text = "Invalid command: " + tok[0];
					ThreadSafeWorkpile::getInstance()->AddChatData(cdata);
				return;
			}
		}
	}
	else
	{
		LbaNet::ChatRoomParticipantPrx room = _connectionMananger.FindChatRoom(_current_map);
		if(room)
			room->Say(_main_name, Text);
	}
}

