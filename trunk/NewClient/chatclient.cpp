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

#include "chatclient.h"
#include "LogHandler.h"

/***********************************************************************
 * Constructor
 ***********************************************************************/
ChatClient::ChatClient()
: m_id(ZCom_Invalid_ID), m_connected(false)
{
	// this will allocate the sockets and create local bindings
    if ( !ZCom_initSockets( true, 0, 0, 0 ) )
    {
		LogHandler::getInstance()->LogToFile("Zoid: Failed to initialize sockets!", 2);
    }

    // string shown in log output
    ZCom_setDebugName("ZCOM_CLI");
}


/***********************************************************************
 * Destructor
 ***********************************************************************/
ChatClient::~ChatClient()
{

}



/***********************************************************************
 * connect to a server given an address + port
 ***********************************************************************/
void ChatClient::ConnectToServer(const std::string & address, const std::string & login, 
								 const std::string & password)
{
	if(m_connected)
	{
		LogHandler::getInstance()->LogToFile("Zoid: ALready connected to chat server - skipping connection", 2);
		return;
	}


	// create target address 
	ZCom_Address dst_udp;
	dst_udp.setAddress( eZCom_AddressUDP, 0, address.c_str() );

	//prepare login info
    ZCom_BitStream *req = new ZCom_BitStream();
	req->addString( login.c_str() );
	req->addString( password.c_str() );

	// connect to server
	LogHandler::getInstance()->LogToFile("Chat client connecting to "+address, 2);
	if (!ZCom_Connect( dst_udp, req))
	{
		LogHandler::getInstance()->LogToFile("Chat client: unable to start connecting!", 2);
	}
}

/***********************************************************************
 * called when initiated connection process yields a result
 ***********************************************************************/
void ChatClient::ZCom_cbConnectResult( ZCom_ConnID _id, eZCom_ConnectResult _result, ZCom_BitStream &_reply )
{
	std::stringstream strs;
	strs<<"Client: The connection process for: "<<_id<<" returned with resultcode "
		<<_result<<", the reply was "<<_reply.getStringStatic();
	LogHandler::getInstance()->LogToFile(strs.str(), 2);   

	if ( _result != eZCom_ConnAccepted )
		m_connected = false;
	else
	{
		m_connected = true;
		ZCom_requestDownstreamLimit(_id, 30, 200);
		ZCom_requestZoidMode(_id, 2);
		m_id = _id;
	}
}


/***********************************************************************
 * connection has closed
 ***********************************************************************/
void ChatClient::ZCom_cbConnectionClosed( ZCom_ConnID _id, eZCom_CloseReason _reason, 
									 ZCom_BitStream &_reasondata )
{
	std::stringstream strs;
	strs<<"Client: Connection with ID: "<<_id<<" has been closed, reason is: "<<_reasondata.getStringStatic();
	LogHandler::getInstance()->LogToFile(strs.str(), 2);   

	m_connected = false;
}


/***********************************************************************
 * zoidlevel transition finished
 ***********************************************************************/
void ChatClient::ZCom_cbZoidResult(ZCom_ConnID _id, eZCom_ZoidResult _result, zU8 _new_level, 
								   ZCom_BitStream &_reason)
{
	// disconnect on failure
	if (_result != eZCom_ZoidEnabled)
	{
		std::stringstream strs;
		strs<<"Client: Connection with ID: "<<_id<<" channel subscription failed: "
				<<(int)_new_level<<" - disconnecting";
		LogHandler::getInstance()->LogToFile(strs.str(), 2);   

		ZCom_Disconnect(_id, NULL);
	}
	else
	{
		std::stringstream strs;
		strs<<"Client: Connection with ID: "<<_id<<" channel subscription successfull: "<<(int)_new_level;
		LogHandler::getInstance()->LogToFile(strs.str(), 2);   
	}
}


/***********************************************************************
 * server wants to tell us about new node
 ***********************************************************************/
void ChatClient::ZCom_cbNodeRequest_Dynamic(ZCom_ConnID _id, ZCom_ClassID _requested_class, 
											ZCom_BitStream *_announcedata, eZCom_NodeRole _role, 
											ZCom_NodeID _net_id)
{
  //// check the requested class
  //if (_requested_class == ZCom_getClassID("NObject"))
  //{
  //  sys_print("Client: Server requested creation of a new 'NObject' node. Network ID is: [%d].", _net_id);
  //  sys_print("Client: Role for requested node will be %s", (_role == eZCom_RoleProxy) ? "'eZCom_RoleProxy'" : "'eZCom_RoleOwner' <- This is the node we can control now!");

  //  // create the object
  //  NObject *no = new NObject();

  //  // this will create the object's node and register it with us
  //  no->init(this);

  //  no->data.slot = -1;

  //  // search for a free slot
  //  for (int i = 0; i < OBJ_MAX; i++)
  //  {
  //    if (objs[i] == NULL)
  //    {
  //      no->data.slot = i;
  //      objs[i] = no;
  //      ZCom_setUserData(_id, (void*) no->data.slot);

  //      // check if object is 'our' object
  //      if (_role == eZCom_RoleOwner)
  //      {
  //        no->data.local = true;
  //        m_localnode = i;
  //      }

  //      break;
  //    }
  //  }

  //  // if no free slot was found disconnect
  //  if (no->data.slot == -1)
  //  {
  //    delete no;
  //    sys_print("Client: unable to handle more objects - diconnecting");
  //    ZCom_Disconnect(m_id, NULL);
  //  }
  //}
  //else
  //  sys_print("Client: invalid class requested");
}
