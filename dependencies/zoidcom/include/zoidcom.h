/****************************************
* zoidcom.h
* ZoidCom main header
*
* This file is part of the "Zoidcom Automated Networking System" application library.
* Copyright (C)2002-2007 by Joerg Rueppel. See documentation for copyright and licensing details.
*****************************************/

#ifndef _ZOIDCOM_H_
#define _ZOIDCOM_H_

#include "zoidcom_prereq.h"

/** \file zoidcom.h
*/

class ZoidCom_Private;

/** @brief Available socket types.

    Zoidcom uses UDP and internal sockets for communication.
*/
enum eZCom_SocketType
{
  /// local socket (ZoidCom internal), can be used to connect two ZCom_Controls within the same process
  eZCom_SocketLocal,
  /// TCP socket, currently not implemented
  eZCom_SocketTCP,
  /// UDP socket
  eZCom_SocketUDP,
  eZCom_SocketCount
};

/** @brief Blocking mode for ZCom_Control::ZCom_processInput()
*/
enum eZCom_BlockMode
{
  /// block until data arrived
  eZCom_Block,
  /// return if no data is present
  eZCom_NoBlock
};

/** @brief Set or remove dependency option for ZCom_Node::dependsOn().
*/
enum eZCom_DependencyOpt
{
  eZCom_AddDependency,
  eZCom_RemoveDependency
};

/** @brief Discover options.
*/
enum eZCom_DiscoverOpt
{
  /// start listening for discover requests(open socket or reuse previous socket)
  eZCom_DiscoverEnable,
  /// stop listening to discover events but keep socket open
  eZCom_DiscoverDisableAndKeep,
  /// stop listening to discover events and close socket
  eZCom_DiscoverDisable
};

/** @brief Address type as in \ref eZCom_SocketType
*/
enum eZCom_AddressType
{
  /// address refers to a \ref eZCom_SocketLocal
  eZCom_AddressLocal,
  /// address refers to a \ref eZCom_SocketTCP
  eZCom_AddressTCP,
  /// address refers to a \ref eZCom_SocketUDP
  eZCom_AddressUDP,
  /// data sent to this address will be sent to all connections with all socket types
  eZCom_AddressBroadcast
};

/** @brief Address type as in \ref eZCom_SocketType
*/
enum eZCom_GetIPAddressOption
{
  /// retrieve address as string with :port
  eZCom_AddressWithPort,
  /// retrieve address as string without a port
  eZCom_AddressWithoutPort
};

/** @brief Result values for ZCom_Control::ZCom_cbConnectResult() */
enum eZCom_ConnectResult
{
  /// connection has been accepted and is established now
  eZCom_ConnAccepted,
  /// connection has been denied
  eZCom_ConnDenied,
  /// connection request ran into timeout
  eZCom_ConnTimeout,
  /// name resolution failed
  eZCom_ConnHostnameFailed,
  /// destination uses different Zoidcom version
  eZCom_ConnWrongVersion,
  /// problem mapping registered classes between local and peer
  eZCom_ConnClassMapError
  
};

/** @brief Reason value for ZCom_Control::ZCom_cbConnectionClosed */
enum eZCom_CloseReason
{
  /// closed on request
  eZCom_ClosedDisconnect,
  /// closed due to timeout
  eZCom_ClosedTimeout,
  /// closed due to reconnect from same client
  eZCom_ClosedReconnect
};

/** @brief Result values for ZCom_Control::ZCom_cbChannelSubscriptionChangeResult() */
enum eZCom_SubscriptionResult
{
  /// requested channel successfully subscribed
  eZCom_RequestedChannelSubscribed,
  /// peer subscribed this channel for us
  eZCom_AutomaticChannelSubscribed,
  /// requested channel successfully unsubscribed
  eZCom_RequestedChannelUnsubscribed,
  /// peer unsubscribed this channel for us
  eZCom_AutomaticChannelUnsubscribed,
  /// peer denied request
  eZCom_ChannelSubscriptionDenied,
  /// a node with setNodeMustSync(true) failed to sync
  eZCom_ChannelSubscriptionFailed_Node
};

/** @brief Result values for ZCom_Address::checkHostname() */
enum eZCom_HostnameResult
{
  /// no hostname lookup in progress
  eZCom_HostnameIdle,
  /// failed to resolve
  eZCom_HostnameFailed,
  /// successfully resolved
  eZCom_HostnameSuccess,
  /// still in progress
  eZCom_HostnameInProgress
};

/** @brief Channel Subscription Options */
enum eZCom_ObjChannelSubscription
{
  /// subscribe to channel
  eZCom_Subscribe,
  /// unsubscribe from channel
  eZCom_Unsubscribe
};

/** @brief API operation mode */
enum eZCom_OperationMode
{
  /// perform the operation and return with the result
  eZCom_Synchronous,
  /// start the operation and return immediately. the result comes through
  /// a callback or need to be polled
  eZCom_Asynchronous
};

/** @brief UDP toggle */
enum eZCom_UDPOption
{
  /// enable UDP socket
  eZCom_EnableUDP,
  /// disable udp socket
  eZCom_DisableUDP
};

/** @brief Accept or deny a request. */
enum eZCom_RequestResult
{
   eZCom_AcceptRequest,
   eZCom_DenyRequest
};

/** @brief Node Roles

   See also ZCom_Node::setOwner() and ZCom_Node::getRole()
*/
enum eZCom_NodeRole
{
  /// no role assigned yet
  eZCom_RoleUndefined,
  /// node is a proxy
  eZCom_RoleProxy,
  /// node is owner
  eZCom_RoleOwner,
  /// node is authority
  eZCom_RoleAuthority
};

/** @brief Send modes.
*/
enum eZCom_SendMode
{
  /// no ordering, but reliable
  eZCom_ReliableUnordered,
  /// reliable and ordered with other ordered events from this node
  eZCom_ReliableOrdered,
  /// unreliable, sequenced. i.e. 1 2 4 might happen but 1 2 4 3 not
  eZCom_Unreliable,
  /// only for ZCom_ReplicatorAdvanced::sendData() and ZCom_ReplicatorAdvanced::sendDataDirect()
  /// send event unreliable, but notify replicator about loss or successful transmission
  eZCom_UnreliableNotify
};

/** @brief ZCom_Node event types, all events except eZCom_EventUser are generated by Zoidcom
 */
enum eZCom_Event
{
  eZCom_EventNoEvent,

  /* system events */

  /// A remote node has connected to this node, use this to send any necessary 
  /// events to it. This event will be received only if notification of it is enabled 
  /// with setEventNotification(). After it has been received it is safe to send events to the 
  /// new node directly, by using the ZCom_ConnID and sendEventDirect().
  eZCom_EventInit,
  /// This node has eZCom_RoleAuthority and setMustSync(true). A new proxy/owner has become 
  /// relevant to this node. It is now possible to send events back and forth to the new proxy 
  /// node and so exchange data with it. Whenever ready, call setSyncResult() and notify Zoidcom 
  /// that the data exchange was successful or that it failed. If failure is reported, the client's
  /// Zoidlevel transition will fail and the connection will fall back to the previous Zoidlevel. 
  /// This event will only be received if a client connection is entering one of the node's Zoidlevels. 
  /// eZCom_EventInit will be received in addition to that on both nodes, if enabled.
  eZCom_EventSyncRequest,
  /// If this node is eZCom_RoleAuthority and a proxy or owner disconnected, you will receive this event.
  /// (If enabled with setEventNotification())
  ///
  /// If this node is eZCom_RoleOwner or eZCom_RoleProxy and the authority on the other end disconnected,
  /// you will receive this event. It can not be disabled since it is an implicit request to delete 
  /// this node.
  eZCom_EventRemoved,

  /// The remote node is trying to send a file. The data in the BitStream is:\n
  ///   - ZCom_FileTransID fid = (ZCom_FileTransID) stream->getInt(ZCOM_FTRANS_ID_BITS);\n
  /// PLUS additional data passed to the sendFile() method
  eZCom_EventFile_Incoming,
  /// New file data has arrived, data is:
  ///  - ZCom_FileTransID fid = (ZCom_FileTransID) stream->getInt(ZCOM_FTRANS_ID_BITS);\n\n
  /// The actual data is not passed around in the stream here, Zoidcom appends the incoming 
  /// data to the file automatically. This is just a notification to let the app know when to 
  /// look for new progress information with ZCom_Node::getFileInfo().
  eZCom_EventFile_Data,
  /// File transfer aborted:
  ///  - ZCom_FileTransID fid = (ZCom_FileTransID) stream->getInt(ZCOM_FTRANS_ID_BITS);
  eZCom_EventFile_Aborted,
  /// File transfer complete:
  ///  - ZCom_FileTransID fid = (ZCom_FileTransID) stream->getInt(ZCOM_FTRANS_ID_BITS);
  eZCom_EventFile_Complete,

  /* replicator events */

  // replicator is sending some data through node event system (internal)
  eZCom_EventReplicator,

  /* normal events */

  /// Application generated event from remote node received.
  eZCom_EventUser
};

/// connection ID
typedef zU32 ZCom_ConnID;
/// class ID
typedef zU32 ZCom_ClassID;
/// node ID
typedef zU32 ZCom_NodeID;
/// file transfer ID
typedef zU32 ZCom_FileTransID;
/// sendgroup ID
typedef zU32 ZCom_GroupID;
/// replication item ID
typedef zU8  ZCom_InterceptID;

const ZCom_ClassID ZCom_Invalid_ID = 0;

/***********************/
/* Connection Stats    */
/***********************/

/// Connection statistics.
struct ZCom_ConnStats
{
  /// time of last packet received
  zU32  last_in;
  /// time of last packet sent
  zU32  last_out;
  /// data throughput over last second in bytes
  zU32  last_sec_out, last_sec_in;
  /// data throughput over last second in packets
  zU32  last_sec_outp, last_sec_inp;
  /// data throughput for current second in bytes
  zU32  current_out, current_in;
  /// data throughput for current second in packets
  zU32  current_outp, current_inp;
  /// total data throughput in bytes
  zU32  total_out, total_in;
  /// time of last update of interval data ( last_sec_..., avg_ping )
  zU32 last_interval_update;
  /// ping of last packet
  zU16  ping;
  /// avg ping over last second
  zU16 avg_ping;
  /// percentage of lost outgoing packets in last second (0-100)
  zU8 last_sec_loss_percent;
  /// number of lost outgoing packets in last second
  zU8 last_sec_loss_count;
  /// how many of our outgoing packets got lost last second
  zU8 current_loss_count;
};

/***********************/
/* File transfer info  */
/***********************/

/// File transfer information.
struct ZCom_FileTransInfo
{
  /// If id == ZCom_Invalid_ID, there was a problem retrieving the data and the contents of all other fields is invalid.
  ZCom_FileTransID id;
  /// total size of file in bytes
  zU32 size;
  /// amount of bytes already transferred
  zU32 transferred;
  /** @brief On senderside, this contains the actual path to the file being sent. On receiverside this contains either the
             received path(parameter _pathtosend in ZCom_Node::sendFile()), or the newly assigned path(parameter _path in
             ZCom_Node::acceptFile())

  */
  const char  *path;
  /// current bytes/second
  zU32 bps;
};

/* ===================================== */

/**
 * @brief Main control class. May be instantiated only once.
 */
class ZCOM_API ZoidCom
{
private:
  ZoidCom_Private *m_priv;
public:
  ZoidCom();
  ZoidCom( const char *_logfile );
  ZoidCom( void ( *_logfunc ) ( const char* ) );
  ~ZoidCom();

  /* init & exit */

  bool
    Init();

  void
    Clear( void );

  /* global setup */

  /** @brief Set timeout after which a connection automatically closes.
      @param _timeout Value in msecs.

      A connection will close after not receiving anything from the peer for _timeout msecs.

      Default is 20 seconds.
  */
  void
    setConnectionTimeout(zU32 _timeout);

  /** @brief Set timeout after which a packet should be resent.
      @param _timeout Value in msecs.

      If Zoidcom is waiting for too many packet acks it won't send any more packets until such an 
      ack arrives. When this timeout passes, Zoidcom will resend the oldest unacked packet. This
      timeout should be lower than the setConnectionTimeout(), enabling Zoidcom to resend connection
      requests several times before giving up.

      Default is 4 seconds.
  */
  void
    setResendTimeout(zU32 _timeout);


  /** @brief Set custom memory handling functions.
      @param _allocator Pointer to a function which allocates memory blocks of size _size and returns a pointer to them.
      @param _deallocator Pointer to a function which takes a memory pointer _ptr and deallocates it.

      Set NULL on one or both parameters to default back to malloc() and/or free(). You should call this method
      before instantiating the ZoidCom object.

      You should also make sure, that, when your application exits, all allocated ZCom_* objects are
      deleted again.
  */
  static void
    overrideMemoryHandlers(void*(*_allocator)(size_t _size), void(*_deallocator)(void *_ptr));

  /** @brief Set Loglevel.
      @param _level 0 = only warnings & errors, 1 = informative, 2 = verbose

      Default is 1.
  */
  void
    setLogLevel(zU8 _level);

  /** @brief Get current time.
      @returns A time value in milliseconds.

      This value is used internally for determining timeouts and computing extrapolations
  */
  static zU32
    getTime();

  /** @brief Let process sleep.
      @param _msecs Amount of milliseconds the process should sleep.
                    0 will just give up our timeslice, so we are nice
                    to other processes.
  */
  static void
    Sleep(zU32 _msecs);
};

#include "zoidcom_bitstream.h"
#include "zoidcom_address.h"
#include "zoidcom_control.h"
#include "zoidcom_node.h"
#include "zoidcom_node_interceptors.h"
#include "zoidcom_conngroup.h"
#include "zoidcom_replicator.h"
#include "zoidcom_replicator_basic.h"
#include "zoidcom_replicator_advanced.h"
#include "zoidcom_replicator_numeric.h"
#include "zoidcom_replicator_bool.h"
#include "zoidcom_replicator_memblock.h"
#include "zoidcom_replicator_string.h"
#include "zoidcom_replicator_interpolate.h"
#include "zoidcom_replicator_movement.h"

#endif
