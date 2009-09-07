// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `ClientSession.ice'

#ifndef _____cpp_ClientSession_h__
#define _____cpp_ClientSession_h__

#include <Ice/LocalObjectF.h>
#include <Ice/ProxyF.h>
#include <Ice/ObjectF.h>
#include <Ice/Exception.h>
#include <Ice/LocalObject.h>
#include <Ice/Proxy.h>
#include <Ice/Object.h>
#include <Ice/Outgoing.h>
#include <Ice/OutgoingAsync.h>
#include <Ice/Incoming.h>
#include <Ice/Direct.h>
#include <Ice/UserExceptionFactory.h>
#include <Ice/FactoryTable.h>
#include <Ice/StreamF.h>
#include <Glacier2/Session.h>
#include <ActorInfo.h>
#include <ChatInfo.h>
#include <MapObserver.h>
#include <Ice/UndefSysMacros.h>

#ifndef ICE_IGNORE_VERSION
#   if ICE_INT_VERSION / 100 != 303
#       error Ice version mismatch!
#   endif
#   if ICE_INT_VERSION % 100 > 50
#       error Beta header file detected
#   endif
#   if ICE_INT_VERSION % 100 < 1
#       error Ice patch level mismatch!
#   endif
#endif

namespace IceProxy
{

namespace LbaNet
{

class ClientSession;

}

}

namespace LbaNet
{

class ClientSession;
bool operator==(const ClientSession&, const ClientSession&);
bool operator<(const ClientSession&, const ClientSession&);

}

namespace IceInternal
{

::Ice::Object* upCast(::LbaNet::ClientSession*);
::IceProxy::Ice::Object* upCast(::IceProxy::LbaNet::ClientSession*);

}

namespace LbaNet
{

typedef ::IceInternal::Handle< ::LbaNet::ClientSession> ClientSessionPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::LbaNet::ClientSession> ClientSessionPrx;

void __read(::IceInternal::BasicStream*, ClientSessionPrx&);
void __patch__ClientSessionPtr(void*, ::Ice::ObjectPtr&);

}

namespace LbaNet
{

typedef ::std::map< ::std::string, ::Ice::Long> ConnectedList;
void __writeConnectedList(::IceInternal::BasicStream*, const ConnectedList&);
void __readConnectedList(::IceInternal::BasicStream*, ConnectedList&);

}

namespace IceProxy
{

namespace LbaNet
{

class ClientSession : virtual public ::IceProxy::Glacier2::Session
{
public:

    ::LbaNet::ChatRoomParticipantPrx JoinChat(const ::std::string& room, const ::LbaNet::ChatRoomObserverPrx& view)
    {
        return JoinChat(room, view, 0);
    }
    ::LbaNet::ChatRoomParticipantPrx JoinChat(const ::std::string& room, const ::LbaNet::ChatRoomObserverPrx& view, const ::Ice::Context& __ctx)
    {
        return JoinChat(room, view, &__ctx);
    }
    
private:

    ::LbaNet::ChatRoomParticipantPrx JoinChat(const ::std::string&, const ::LbaNet::ChatRoomObserverPrx&, const ::Ice::Context*);
    
public:

    void LeaveChat(const ::std::string& room, const ::LbaNet::ChatRoomObserverPrx& view)
    {
        LeaveChat(room, view, 0);
    }
    void LeaveChat(const ::std::string& room, const ::LbaNet::ChatRoomObserverPrx& view, const ::Ice::Context& __ctx)
    {
        LeaveChat(room, view, &__ctx);
    }
    
private:

    void LeaveChat(const ::std::string&, const ::LbaNet::ChatRoomObserverPrx&, const ::Ice::Context*);
    
public:

    ::LbaNet::ActorsParticipantPrx ChangeRoom(const ::std::string& newroom, const ::std::string& actorname, const ::LbaNet::ActorsObserverPrx& view)
    {
        return ChangeRoom(newroom, actorname, view, 0);
    }
    ::LbaNet::ActorsParticipantPrx ChangeRoom(const ::std::string& newroom, const ::std::string& actorname, const ::LbaNet::ActorsObserverPrx& view, const ::Ice::Context& __ctx)
    {
        return ChangeRoom(newroom, actorname, view, &__ctx);
    }
    
private:

    ::LbaNet::ActorsParticipantPrx ChangeRoom(const ::std::string&, const ::std::string&, const ::LbaNet::ActorsObserverPrx&, const ::Ice::Context*);
    
public:

    ::LbaNet::ConnectedList GetConnected(::Ice::Long& ownid)
    {
        return GetConnected(ownid, 0);
    }
    ::LbaNet::ConnectedList GetConnected(::Ice::Long& ownid, const ::Ice::Context& __ctx)
    {
        return GetConnected(ownid, &__ctx);
    }
    
private:

    ::LbaNet::ConnectedList GetConnected(::Ice::Long&, const ::Ice::Context*);
    
public:

    void ActivateActor(const ::LbaNet::ActorActivationInfo& ai)
    {
        ActivateActor(ai, 0);
    }
    void ActivateActor(const ::LbaNet::ActorActivationInfo& ai, const ::Ice::Context& __ctx)
    {
        ActivateActor(ai, &__ctx);
    }
    
private:

    void ActivateActor(const ::LbaNet::ActorActivationInfo&, const ::Ice::Context*);
    
public:

    void SignalActor(const ::LbaNet::ActorSignalInfo& ai)
    {
        SignalActor(ai, 0);
    }
    void SignalActor(const ::LbaNet::ActorSignalInfo& ai, const ::Ice::Context& __ctx)
    {
        SignalActor(ai, &__ctx);
    }
    
private:

    void SignalActor(const ::LbaNet::ActorSignalInfo&, const ::Ice::Context*);
    
public:

    ::LbaNet::UpdateSeq GetUpdatedInfo()
    {
        return GetUpdatedInfo(0);
    }
    ::LbaNet::UpdateSeq GetUpdatedInfo(const ::Ice::Context& __ctx)
    {
        return GetUpdatedInfo(&__ctx);
    }
    
private:

    ::LbaNet::UpdateSeq GetUpdatedInfo(const ::Ice::Context*);
    
public:

    ::Ice::Long GetTime()
    {
        return GetTime(0);
    }
    ::Ice::Long GetTime(const ::Ice::Context& __ctx)
    {
        return GetTime(&__ctx);
    }
    
private:

    ::Ice::Long GetTime(const ::Ice::Context*);
    
public:
    
    ::IceInternal::ProxyHandle<ClientSession> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ClientSession*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<ClientSession*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ClientSession> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ClientSession*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<ClientSession*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ClientSession> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ClientSession*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<ClientSession*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ClientSession> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ClientSession*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<ClientSession*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ClientSession> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ClientSession*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<ClientSession*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ClientSession> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ClientSession*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<ClientSession*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ClientSession> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ClientSession*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<ClientSession*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ClientSession> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ClientSession*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<ClientSession*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ClientSession> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ClientSession*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<ClientSession*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ClientSession> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ClientSession*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<ClientSession*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ClientSession> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ClientSession*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<ClientSession*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ClientSession> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ClientSession*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<ClientSession*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ClientSession> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ClientSession*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<ClientSession*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ClientSession> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ClientSession*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<ClientSession*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ClientSession> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ClientSession*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<ClientSession*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ClientSession> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ClientSession*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<ClientSession*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ClientSession> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ClientSession*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<ClientSession*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ClientSession> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ClientSession*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<ClientSession*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ClientSession> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ClientSession*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<ClientSession*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
    #endif
    }
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

}

}

namespace IceDelegate
{

namespace LbaNet
{

class ClientSession : virtual public ::IceDelegate::Glacier2::Session
{
public:

    virtual ::LbaNet::ChatRoomParticipantPrx JoinChat(const ::std::string&, const ::LbaNet::ChatRoomObserverPrx&, const ::Ice::Context*) = 0;

    virtual void LeaveChat(const ::std::string&, const ::LbaNet::ChatRoomObserverPrx&, const ::Ice::Context*) = 0;

    virtual ::LbaNet::ActorsParticipantPrx ChangeRoom(const ::std::string&, const ::std::string&, const ::LbaNet::ActorsObserverPrx&, const ::Ice::Context*) = 0;

    virtual ::LbaNet::ConnectedList GetConnected(::Ice::Long&, const ::Ice::Context*) = 0;

    virtual void ActivateActor(const ::LbaNet::ActorActivationInfo&, const ::Ice::Context*) = 0;

    virtual void SignalActor(const ::LbaNet::ActorSignalInfo&, const ::Ice::Context*) = 0;

    virtual ::LbaNet::UpdateSeq GetUpdatedInfo(const ::Ice::Context*) = 0;

    virtual ::Ice::Long GetTime(const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace LbaNet
{

class ClientSession : virtual public ::IceDelegate::LbaNet::ClientSession,
                      virtual public ::IceDelegateM::Glacier2::Session
{
public:

    virtual ::LbaNet::ChatRoomParticipantPrx JoinChat(const ::std::string&, const ::LbaNet::ChatRoomObserverPrx&, const ::Ice::Context*);

    virtual void LeaveChat(const ::std::string&, const ::LbaNet::ChatRoomObserverPrx&, const ::Ice::Context*);

    virtual ::LbaNet::ActorsParticipantPrx ChangeRoom(const ::std::string&, const ::std::string&, const ::LbaNet::ActorsObserverPrx&, const ::Ice::Context*);

    virtual ::LbaNet::ConnectedList GetConnected(::Ice::Long&, const ::Ice::Context*);

    virtual void ActivateActor(const ::LbaNet::ActorActivationInfo&, const ::Ice::Context*);

    virtual void SignalActor(const ::LbaNet::ActorSignalInfo&, const ::Ice::Context*);

    virtual ::LbaNet::UpdateSeq GetUpdatedInfo(const ::Ice::Context*);

    virtual ::Ice::Long GetTime(const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace LbaNet
{

class ClientSession : virtual public ::IceDelegate::LbaNet::ClientSession,
                      virtual public ::IceDelegateD::Glacier2::Session
{
public:

    virtual ::LbaNet::ChatRoomParticipantPrx JoinChat(const ::std::string&, const ::LbaNet::ChatRoomObserverPrx&, const ::Ice::Context*);

    virtual void LeaveChat(const ::std::string&, const ::LbaNet::ChatRoomObserverPrx&, const ::Ice::Context*);

    virtual ::LbaNet::ActorsParticipantPrx ChangeRoom(const ::std::string&, const ::std::string&, const ::LbaNet::ActorsObserverPrx&, const ::Ice::Context*);

    virtual ::LbaNet::ConnectedList GetConnected(::Ice::Long&, const ::Ice::Context*);

    virtual void ActivateActor(const ::LbaNet::ActorActivationInfo&, const ::Ice::Context*);

    virtual void SignalActor(const ::LbaNet::ActorSignalInfo&, const ::Ice::Context*);

    virtual ::LbaNet::UpdateSeq GetUpdatedInfo(const ::Ice::Context*);

    virtual ::Ice::Long GetTime(const ::Ice::Context*);
};

}

}

namespace LbaNet
{

class ClientSession : virtual public ::Glacier2::Session
{
public:

    typedef ClientSessionPrx ProxyType;
    typedef ClientSessionPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::LbaNet::ChatRoomParticipantPrx JoinChat(const ::std::string&, const ::LbaNet::ChatRoomObserverPrx&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___JoinChat(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void LeaveChat(const ::std::string&, const ::LbaNet::ChatRoomObserverPrx&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___LeaveChat(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::LbaNet::ActorsParticipantPrx ChangeRoom(const ::std::string&, const ::std::string&, const ::LbaNet::ActorsObserverPrx&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___ChangeRoom(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::LbaNet::ConnectedList GetConnected(::Ice::Long&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___GetConnected(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void ActivateActor(const ::LbaNet::ActorActivationInfo&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___ActivateActor(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void SignalActor(const ::LbaNet::ActorSignalInfo&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___SignalActor(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::LbaNet::UpdateSeq GetUpdatedInfo(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___GetUpdatedInfo(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::Long GetTime(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___GetTime(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

}

#endif
