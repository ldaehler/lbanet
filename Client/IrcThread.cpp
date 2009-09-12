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

#include "IrcThread.h"
#include "libircclient.h"
#include "LogHandler.h"
#include "ThreadSafeWorkpile.h"

#include <sstream>



void event_msg (irc_session_t * session, const char * event, const char * origin,
					const char ** params, unsigned int count)
{
	if(origin != NULL)
	{
		ThreadSafeWorkpile::ChatTextData ctd;
		ctd.Channel = "IRC";
		ctd.Sender = origin;
		ctd.Text = params[1];
		ThreadSafeWorkpile::getInstance()->AddChatData(ctd);
	}
}


void event_quit (irc_session_t * session, const char * event, const char * origin,
				 const char ** params, unsigned int count)
{
	if(origin != NULL)
	{
		ThreadSafeWorkpile::JoinEvent ev;
		ev.ListName = "IRC";
		ev.Joined = false;
		ev.Clear = false;
		ev.Nickname = origin;
		ThreadSafeWorkpile::getInstance()->HappenedJoinEvent(ev);

		ThreadSafeWorkpile::ChatTextData ctd;
		ctd.Channel = "IRC";
		ctd.Sender = "info";
		ctd.Text = origin + std::string(" left the channel.");
		ThreadSafeWorkpile::getInstance()->AddChatData(ctd);

		IrcThread::IrcCoInfo * coi =  (IrcThread::IrcCoInfo *) irc_get_ctx(session);
		if(coi && coi->Nickname == origin)
			irc_cmd_join (session, coi->Channel.c_str(), 0);
	}
}


void event_kick (irc_session_t * session, const char * event, const char * origin,
				 const char ** params, unsigned int count)
{

	if(count >= 2)
	{
		ThreadSafeWorkpile::JoinEvent ev;
		ev.ListName = "IRC";
		ev.Joined = false;
		ev.Clear = false;
		ev.Nickname = params[1];
		ThreadSafeWorkpile::getInstance()->HappenedJoinEvent(ev);

		ThreadSafeWorkpile::ChatTextData ctd;
		ctd.Channel = "IRC";
		ctd.Sender = "info";
		ctd.Text = params[1] + std::string(" was kicked from the channel.");
		ThreadSafeWorkpile::getInstance()->AddChatData(ctd);

		IrcThread::IrcCoInfo * coi =  (IrcThread::IrcCoInfo *) irc_get_ctx(session);
		if(coi && coi->Nickname == params[1])
			irc_cmd_join (session, coi->Channel.c_str(), 0);
	}
}


void event_join (irc_session_t * session, const char * event, const char * origin,
				 const char ** params, unsigned int count)
{
	if(origin != NULL)
	{
		ThreadSafeWorkpile::JoinEvent ev;
		ev.ListName = "IRC";
		ev.Joined = true;
		ev.Clear = false;
		ev.Nickname = origin;
		ThreadSafeWorkpile::getInstance()->HappenedJoinEvent(ev);

		ThreadSafeWorkpile::ChatTextData ctd;
		ctd.Channel = "IRC";
		ctd.Sender = "info";
		ctd.Text = origin + std::string(" joined the channel.");
		ThreadSafeWorkpile::getInstance()->AddChatData(ctd);

	}
}


void event_nick (irc_session_t * session, const char * event, const char * origin,
				 const char ** params, unsigned int count)
{
	if(origin != NULL)
	{
		{
		ThreadSafeWorkpile::JoinEvent ev;
		ev.ListName = "IRC";
		ev.Joined = false;
		ev.Clear = false;
		ev.Nickname = origin;
		ThreadSafeWorkpile::getInstance()->HappenedJoinEvent(ev);
		}

		{
		ThreadSafeWorkpile::JoinEvent ev;
		ev.ListName = "IRC";
		ev.Joined = true;
		ev.Clear = false;
		ev.Nickname = params[0];
		ThreadSafeWorkpile::getInstance()->HappenedJoinEvent(ev);
		}


		ThreadSafeWorkpile::ChatTextData ctd;
		ctd.Channel = "IRC";
		ctd.Sender = "info";
		ctd.Text = origin + std::string(" changed nick to ") + params[0];
		ThreadSafeWorkpile::getInstance()->AddChatData(ctd);
	}
}


void event_numeric (irc_session_t * session, unsigned int eventi, const char * origin,
					const char ** params, unsigned int count)
{
	ThreadSafeWorkpile::JoinEvent ev;


	switch(eventi)
	{
		case LIBIRC_RFC_RPL_NAMREPLY:
		if(count >= 4)
		{
			std::string nicks = params[3];
			std::string buf;
			std::stringstream ss(nicks);
			while (ss >> buf)
			{
				ev.ListName = "IRC";
				ev.Joined = true;
				ev.Clear = false;
				ev.Nickname = buf;
				ThreadSafeWorkpile::getInstance()->HappenedJoinEvent(ev);
			}
		}

		break;


		case LIBIRC_RFC_ERR_NEEDMOREPARAMS:
		LogHandler::getInstance()->LogToFile("LIBIRC_RFC_ERR_NEEDMOREPARAMS");
		break;

		case LIBIRC_RFC_ERR_BANNEDFROMCHAN:
		LogHandler::getInstance()->LogToFile("LIBIRC_RFC_ERR_BANNEDFROMCHAN");
		break;

		case LIBIRC_RFC_ERR_INVITEONLYCHAN:
		LogHandler::getInstance()->LogToFile("LIBIRC_RFC_ERR_INVITEONLYCHAN");
		break;

		case LIBIRC_RFC_ERR_BADCHANNELKEY:
		LogHandler::getInstance()->LogToFile("LIBIRC_RFC_ERR_BADCHANNELKEY");
		break;

		case LIBIRC_RFC_ERR_CHANNELISFULL:
		LogHandler::getInstance()->LogToFile("LIBIRC_RFC_ERR_CHANNELISFULL");
		break;

		case LIBIRC_RFC_ERR_BADCHANMASK:
		LogHandler::getInstance()->LogToFile("LIBIRC_RFC_ERR_BADCHANMASK");
		break;

		case LIBIRC_RFC_ERR_NOSUCHCHANNEL:
		LogHandler::getInstance()->LogToFile("LIBIRC_RFC_ERR_NOSUCHCHANNEL");
		break;

		case LIBIRC_RFC_ERR_TOOMANYCHANNELS:
		LogHandler::getInstance()->LogToFile("LIBIRC_RFC_ERR_TOOMANYCHANNELS");
		break;
	}
}

// join channel lba on connection
void event_connect (irc_session_t * session, const char * event, const char * origin,
						const char ** params, unsigned int count)
{
	IrcThread::IrcCoInfo * coi =  (IrcThread::IrcCoInfo *) irc_get_ctx(session);
	if(coi)
	{
		if ( irc_cmd_join (session, coi->Channel.c_str(), 0))
			LogHandler::getInstance()->LogToFile("Could not connect to the channel: " + std::string(irc_strerror (irc_errno(session))));
		else
		{
			ThreadSafeWorkpile::ChatTextData ctd;
			ctd.Channel = "IRC";
			ctd.Sender = "info";
			ctd.Text = "Connected to IRC.";
			ThreadSafeWorkpile::getInstance()->AddChatData(ctd);
		}
	}
}



/***********************************************************
constructor
***********************************************************/
IrcThread::IrcThread(const IrcCoInfo & confinfo)
: _session(NULL), _coInfo(confinfo), _wait_time(10000)
{
}


/***********************************************************
running function of the thread
***********************************************************/
void IrcThread::run()
{
	irc_callbacks_t	callbacks;

	// Initialize the callbacks
	memset (&callbacks, 0, sizeof(callbacks));

	// Set up the callbacks we will use
	callbacks.event_channel = event_msg;
	callbacks.event_privmsg = event_msg;
	callbacks.event_notice = event_msg;

	callbacks.event_join = event_join;
	callbacks.event_quit = event_quit;
	callbacks.event_part = event_quit;
	callbacks.event_kick = event_kick;
	callbacks.event_connect = event_connect;

	callbacks.event_numeric = event_numeric;
	callbacks.event_nick = event_nick;


	bool quit = false;

	// try reconnect in case of disconnection
	while(ThreadSafeWorkpile::getInstance()->WaitOneIrcCycle(_wait_time))
	{
		_wait_time *= 2;

		// And create the IRC session; 0 means error
		_session = irc_create_session (&callbacks);

		irc_set_ctx(_session, &_coInfo);

		if ( !_session )
		{
			LogHandler::getInstance()->LogToFile("Could not create IRC session");
			continue;
		}

		irc_option_set (_session, LIBIRC_OPTION_STRIPNICKS);

		// Initiate the IRC server connection
		if ( irc_connect (_session, _coInfo.Server.c_str(), 6667, 0, _coInfo.Nickname.c_str(),
							"LBANetPlayer", "http://forum.magicball.net/showthread.php?t=14792") )
		{
			LogHandler::getInstance()->LogToFile("Could not connect to irc: " + std::string(irc_strerror (irc_errno(_session))));
		}
		else
		{
			// and run into forever loop, generating events
			if(irc_run (_session))
				LogHandler::getInstance()->LogToFile("Irc loop returned: " + std::string(irc_strerror (irc_errno(_session))));


			ThreadSafeWorkpile::ChatTextData ctd;
			ctd.Channel = "IRC";
			ctd.Sender = "info";
			ctd.Text = "Disconnected from IRC.";
			ThreadSafeWorkpile::getInstance()->AddChatData(ctd);

			ThreadSafeWorkpile::JoinEvent ev;
			ev.ListName = "IRC";
			ev.Joined = false;
			ev.Clear = true;
			ev.Nickname = "";
			ThreadSafeWorkpile::getInstance()->HappenedJoinEvent(ev);


			// exit and cleanup
			irc_cmd_part  ( _session, _coInfo.Channel.c_str());
		}

		irc_destroy_session (_session);
	}
}



/***********************************************************
send message to irc
***********************************************************/
void IrcThread::sendMessage(const std::string & Text)
{
	if(irc_is_connected  ( _session )  )
	{
		irc_cmd_msg  ( _session, _coInfo.Channel.c_str(), Text.c_str());

		ThreadSafeWorkpile::ChatTextData ctd;
		ctd.Channel = "IRC";
		ctd.Sender = _coInfo.Nickname;
		ctd.Text = Text;
		ThreadSafeWorkpile::getInstance()->AddChatData(ctd);
	}
}


/***********************************************************
used to quit irc
***********************************************************/
void IrcThread::Quit()
{
	if(irc_is_connected  ( _session )  )
		irc_cmd_quit( _session, NULL);
}

