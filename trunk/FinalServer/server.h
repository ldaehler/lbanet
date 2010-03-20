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

#ifndef _LBANET_server_h_
#define _LBANET_server_h_

#include <zoidcom.h>


class Server : public ZCom_Control
{
public:
	//! constructor
	Server( int _internalport, int _udpport );

	//! destructor
	~Server();


protected:
	// called on incoming connections
	bool ZCom_cbConnectionRequest( ZCom_ConnID _id, ZCom_BitStream &_request, 
																		ZCom_BitStream &_reply );

	// called when incoming connection has been established
	void ZCom_cbConnectionSpawned( ZCom_ConnID _id );

	// called when a connection closed
	void ZCom_cbConnectionClosed( ZCom_ConnID _id, eZCom_CloseReason _reason, ZCom_BitStream &_reasondata );

	// called when a connection wants to enter a channel
	bool ZCom_cbZoidRequest( ZCom_ConnID _id, zU8 _requested_level, ZCom_BitStream &_reason);
	
	// called when a connection enters a channel
	void ZCom_cbZoidResult(ZCom_ConnID _id, eZCom_ZoidResult _result, zU8 _new_level, ZCom_BitStream &_reason);



	// called when broadcast has been received
	bool ZCom_cbDiscoverRequest(const ZCom_Address &_addr, ZCom_BitStream &_request, 
												ZCom_BitStream &_reply) {return false;}

	//data received from the clients
	virtual void ZCom_cbDataReceived( ZCom_ConnID _id, ZCom_BitStream &_data ) {}


	// client stuff
	virtual void ZCom_cbConnectResult( ZCom_ConnID _id, eZCom_ConnectResult _result, ZCom_BitStream &_reply ) {}
	virtual void ZCom_cbNodeRequest_Dynamic( ZCom_ConnID _id, ZCom_ClassID _requested_class, 
								ZCom_BitStream *_announcedata, eZCom_NodeRole _role, ZCom_NodeID _net_id ) {}
	virtual void ZCom_cbNodeRequest_Tag( ZCom_ConnID _id, ZCom_ClassID _requested_class, 
											ZCom_BitStream *_announcedata, eZCom_NodeRole _role, zU32 _tag ) {}
	virtual void ZCom_cbDiscovered( const ZCom_Address & _addr, ZCom_BitStream &_reply )  {}


private:
	int m_conncount;
};


#endif
