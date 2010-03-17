/****************************************
* server.cpp
* part of example source code
*
* This file is part of the "Zoidcom Automated Networking System" application library.
* Copyright (C)2002-2006 by Joerg Rueppel. See documentation for copyright and licensing details.
*****************************************/

#include "common.h"
#include "server.h"

Server::Server( int _internalport, int _udpport )
{
	// this will allocate the sockets and create local bindings
	if ( !ZCom_initSockets( true, _udpport, _internalport, CONTROL_ID_SIZE ) )
	{
	sys_print( "Server: unable to initialize the sockets" );
	is_ok = false;
	}
	else
	is_ok = true;

	ZCom_setControlID(CONTROL_ID);

	// string shown in zcoms debug output
	ZCom_setDebugName("ZCOM_SRV");


	// register the only object class we are going to use
	ZCom_registerClass("NObject", ZCOM_CLASSFLAG_ANNOUNCEDATA);


	// maximum 8k/sec upstream and 2k/sec/connection
	ZCom_setUpstreamLimit(8000, 2000);

}

Server::~Server()
{

}

// update the four random moving objects
void Server::UpdateInput(unsigned int elpasedTime)
{

	// all callbacks are generated from within the processInput calls
	ZCom_processInput( eZCom_NoBlock );


	// update replicators and tell zoidcom how much ingame time has passed since the last
	// time this was called
	ZCom_processReplicators(elpasedTime);


	// process client inputs

	// update the objects and their physics
}



//! update network output
void UpdateOutput()
{
	// outstanding data will be packed up and sent from here
	Server::ZCom_processOutput();
}




// called on incoming connections
bool Server::ZCom_cbConnectionRequest( ZCom_ConnID _id, ZCom_BitStream &_request, ZCom_BitStream &_reply )
{
  // retrieve request, we work with strings now, could as well be anything else the ZCom_BitStream class can handle
  const char * req = _request.getStringStatic();

  // address information
  const ZCom_Address* addr = ZCom_getPeer( _id );
  if ( addr )
  {
    if ( addr->getType() == eZCom_AddressLocal )
      sys_print( "Server: Incoming connection from localport: %d", addr->getPort() );
    else if ( addr->getType() == eZCom_AddressUDP )
      sys_print( "Server: Incoming connection from UDP: %d.%d.%d.%d:%d", addr->getIP( 0 ), addr->getIP( 1 ), addr->getIP( 2 ), addr->getIP( 3 ), addr->getPort() );
  }

  // check what the client is requesting
  if ( req && strlen( req ) > 0 && strcmp( req, "letmein2" ) == 0 )
  {
    sys_print( "Server: Incoming connection with ID: %d requesting: '%s'... accepted", _id, req );
    // accept the connection request
    _reply.addString("come_in");
    return true;
  }
  else
  {
    sys_print( "Server: Incoming connection with ID: %d requesting: '%s'... denied", _id, req );
    // deny connection request and send reason back to requester
    _reply.addString( "invalid request" );
    return false;
  }
}

// called when incoming connection has been established
void Server::ZCom_cbConnectionSpawned( ZCom_ConnID _id )
{
  sys_print( "Server: Incoming connection with ID: %d has been established.", _id );

  // request 20 packets/second and 200 bytes per packet from client (maximum values of course)
  ZCom_requestDownstreamLimit(_id, 20, 200);

  // initialize userdata to -1, otherwise we delete object 0 when connection closes again
  // and the connection has not entered zoidmode before
  ZCom_setUserData(_id, (void*) -1);

  conncount++;
}

// called when a connection closed
void Server::ZCom_cbConnectionClosed( ZCom_ConnID _id, eZCom_CloseReason _reason, ZCom_BitStream &_reasondata )
{
  sys_print( "Server: Connection with ID: %d has been closed", _id);

  // retrieve object belonging to client
  int slot = (int) ZCom_getUserData(_id);
  if (slot >= 0 && slot < OBJ_MAX)
  {
    // and delete the object
    if (objs[slot]) delete objs[slot];
    objs[slot] = NULL;
  }

  conncount--;
}

// a client wants to enter a zoidlevel
bool Server::ZCom_cbZoidRequest( ZCom_ConnID _id, zU8 _requested_level, ZCom_BitStream &_reason)
{
  // check level and accept
  if (_requested_level == 2)
  {
    sys_print("Server: accepted Zoidrequest for level [%d] from %d", _requested_level, _id);
    return true;
  }
  // or deny
  else
    return false;
}

// client entered a zoidlevel or failed
void Server::ZCom_cbZoidResult(ZCom_ConnID _id, eZCom_ZoidResult _result, zU8 _new_level, ZCom_BitStream &_reason)
{
  // failed
  if (_result != eZCom_ZoidEnabled)
  {
    sys_print("Server: %d failed to enter zoidmode", _id);
    return;
  }

  sys_print("Server: Zoidlevel transition successful for level [%d] from connection %d", _new_level, _id);
  sys_print("Server: Creating node for client to play with now...", _new_level, _id);

  // spawn new playerobject
  NObject *no = new NObject;

  // this will create the ZCom_Node and register it with us (we are a ZCom_Control)
  no->init(this);
  // make connection owner of object so connection may change x and y of object (see NObject::init())
  no->node->setOwner(_id, true);

  // search free slot
  for (int i = 0; i < OBJ_MAX; i++)
  {
    if (objs[i] == NULL)
    {
      no->data.slot = i;
      objs[i] = no;
      // remember slot in connection userdata
      ZCom_setUserData(_id, (void*) no->data.slot);
      sys_print("Server: inserted node in slot: %d", i);
      break;
    }
  }
}

