// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `ConnectedTracker.ice'

#ifndef _____cpp_ConnectedTracker_h__
#define _____cpp_ConnectedTracker_h__

#include <Ice/LocalObjectF.h>
#include <Ice/ProxyF.h>
#include <Ice/ObjectF.h>
#include <Ice/Exception.h>
#include <Ice/LocalObject.h>
#include <Ice/Proxy.h>
#include <Ice/Object.h>
#include <Ice/Outgoing.h>
#include <Ice/Incoming.h>
#include <Ice/Direct.h>
#include <Ice/StreamF.h>
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

class ConnectedTracker;

}

}

namespace LbaNet
{

class ConnectedTracker;
bool operator==(const ConnectedTracker&, const ConnectedTracker&);
bool operator<(const ConnectedTracker&, const ConnectedTracker&);

}

namespace IceInternal
{

::Ice::Object* upCast(::LbaNet::ConnectedTracker*);
::IceProxy::Ice::Object* upCast(::IceProxy::LbaNet::ConnectedTracker*);

}

namespace LbaNet
{

typedef ::IceInternal::Handle< ::LbaNet::ConnectedTracker> ConnectedTrackerPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::LbaNet::ConnectedTracker> ConnectedTrackerPrx;

void __read(::IceInternal::BasicStream*, ConnectedTrackerPrx&);
void __patch__ConnectedTrackerPtr(void*, ::Ice::ObjectPtr&);

}

namespace LbaNet
{

struct PlayerInfo
{
    ::Ice::Long Id;
    ::std::string Status;

    bool operator==(const PlayerInfo&) const;
    bool operator<(const PlayerInfo&) const;
    bool operator!=(const PlayerInfo& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const PlayerInfo& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const PlayerInfo& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const PlayerInfo& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

typedef ::std::map< ::std::string, ::LbaNet::PlayerInfo> ConnectedL;
void __writeConnectedL(::IceInternal::BasicStream*, const ConnectedL&);
void __readConnectedL(::IceInternal::BasicStream*, ConnectedL&);

}

namespace IceProxy
{

namespace LbaNet
{

class ConnectedTracker : virtual public ::IceProxy::Ice::Object
{
public:

    ::Ice::Long Connect(const ::std::string& Nickname)
    {
        return Connect(Nickname, 0);
    }
    ::Ice::Long Connect(const ::std::string& Nickname, const ::Ice::Context& __ctx)
    {
        return Connect(Nickname, &__ctx);
    }
    
private:

    ::Ice::Long Connect(const ::std::string&, const ::Ice::Context*);
    
public:

    void Disconnect(::Ice::Long PlayerId)
    {
        Disconnect(PlayerId, 0);
    }
    void Disconnect(::Ice::Long PlayerId, const ::Ice::Context& __ctx)
    {
        Disconnect(PlayerId, &__ctx);
    }
    
private:

    void Disconnect(::Ice::Long, const ::Ice::Context*);
    
public:

    void ChangeStatus(const ::std::string& Nickname, const ::std::string& NewStatus)
    {
        ChangeStatus(Nickname, NewStatus, 0);
    }
    void ChangeStatus(const ::std::string& Nickname, const ::std::string& NewStatus, const ::Ice::Context& __ctx)
    {
        ChangeStatus(Nickname, NewStatus, &__ctx);
    }
    
private:

    void ChangeStatus(const ::std::string&, const ::std::string&, const ::Ice::Context*);
    
public:

    ::LbaNet::ConnectedL GetConnected()
    {
        return GetConnected(0);
    }
    ::LbaNet::ConnectedL GetConnected(const ::Ice::Context& __ctx)
    {
        return GetConnected(&__ctx);
    }
    
private:

    ::LbaNet::ConnectedL GetConnected(const ::Ice::Context*);
    
public:
    
    ::IceInternal::ProxyHandle<ConnectedTracker> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ConnectedTracker*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<ConnectedTracker*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ConnectedTracker> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ConnectedTracker*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<ConnectedTracker*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ConnectedTracker> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ConnectedTracker*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<ConnectedTracker*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ConnectedTracker> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ConnectedTracker*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<ConnectedTracker*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ConnectedTracker> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ConnectedTracker*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<ConnectedTracker*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ConnectedTracker> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ConnectedTracker*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<ConnectedTracker*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ConnectedTracker> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ConnectedTracker*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<ConnectedTracker*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ConnectedTracker> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ConnectedTracker*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<ConnectedTracker*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ConnectedTracker> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ConnectedTracker*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<ConnectedTracker*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ConnectedTracker> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ConnectedTracker*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<ConnectedTracker*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ConnectedTracker> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ConnectedTracker*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<ConnectedTracker*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ConnectedTracker> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ConnectedTracker*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<ConnectedTracker*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ConnectedTracker> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ConnectedTracker*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<ConnectedTracker*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ConnectedTracker> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ConnectedTracker*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<ConnectedTracker*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ConnectedTracker> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ConnectedTracker*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<ConnectedTracker*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ConnectedTracker> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ConnectedTracker*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<ConnectedTracker*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ConnectedTracker> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ConnectedTracker*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<ConnectedTracker*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ConnectedTracker> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ConnectedTracker*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<ConnectedTracker*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ConnectedTracker> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ConnectedTracker*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<ConnectedTracker*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
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

class ConnectedTracker : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual ::Ice::Long Connect(const ::std::string&, const ::Ice::Context*) = 0;

    virtual void Disconnect(::Ice::Long, const ::Ice::Context*) = 0;

    virtual void ChangeStatus(const ::std::string&, const ::std::string&, const ::Ice::Context*) = 0;

    virtual ::LbaNet::ConnectedL GetConnected(const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace LbaNet
{

class ConnectedTracker : virtual public ::IceDelegate::LbaNet::ConnectedTracker,
                         virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual ::Ice::Long Connect(const ::std::string&, const ::Ice::Context*);

    virtual void Disconnect(::Ice::Long, const ::Ice::Context*);

    virtual void ChangeStatus(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual ::LbaNet::ConnectedL GetConnected(const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace LbaNet
{

class ConnectedTracker : virtual public ::IceDelegate::LbaNet::ConnectedTracker,
                         virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual ::Ice::Long Connect(const ::std::string&, const ::Ice::Context*);

    virtual void Disconnect(::Ice::Long, const ::Ice::Context*);

    virtual void ChangeStatus(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual ::LbaNet::ConnectedL GetConnected(const ::Ice::Context*);
};

}

}

namespace LbaNet
{

class ConnectedTracker : virtual public ::Ice::Object
{
public:

    typedef ConnectedTrackerPrx ProxyType;
    typedef ConnectedTrackerPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::Ice::Long Connect(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___Connect(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void Disconnect(::Ice::Long, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___Disconnect(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void ChangeStatus(const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___ChangeStatus(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::LbaNet::ConnectedL GetConnected(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___GetConnected(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

}

#endif