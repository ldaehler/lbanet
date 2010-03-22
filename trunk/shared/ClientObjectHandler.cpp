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


#include "ClientObjectHandler.h"
#include "ClientObject.h"


/************************************************************************/
/* constructor                                        
/************************************************************************/
ClientObjectHandler::ClientObjectHandler()
{

}

/************************************************************************/
/* destructor                                    
/************************************************************************/
ClientObjectHandler::~ClientObjectHandler()
{
	std::map<unsigned int, ClientObject *>::iterator it = _clients.begin();
	for(;it != _clients.end();++it)
		delete it->second;
}


/************************************************************************/
/* do a process step                                  
/************************************************************************/
void ClientObjectHandler::Process()
{
	std::map<unsigned int, ClientObject *>::iterator it = _clients.begin();
	while(it != _clients.end())
	{
		it->second->Process();
		if(it->second->isGarbage())
		{
			delete it->second;
			it = _clients.erase(it);
		}
		else
			++it;
	}
}


/************************************************************************/
/* add a client                                
/************************************************************************/
void ClientObjectHandler::Addclient(unsigned int id, ClientObject * cl)
{
	// if channel already exist then just return
	std::map<unsigned int , ClientObject *>::iterator it = _clients.find(id);
	if(it != _clients.end())
		return;

}

/************************************************************************/
/* remove a client                            
/************************************************************************/
void ClientObjectHandler::Removeclient(unsigned int id)
{
	std::map<unsigned int , ClientObject *>::iterator it = _clients.find(id);
	if(it != _clients.end())
	{
		delete it->second;
		_clients.erase(it);
	}
}

 
/************************************************************************/
/*  get a client
/*	return NULL if does not exist                        
/************************************************************************/
ClientObject * ClientObjectHandler::Getclient(unsigned int id)
{
	// if channel already exist then just return it
	std::map<unsigned int , ClientObject *>::iterator it = _clients.find(id);
	if(it != _clients.end())
		return it->second;

	// else return NULL
	return NULL;
}