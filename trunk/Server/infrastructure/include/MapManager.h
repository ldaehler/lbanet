// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `MapManager.ice'

#ifndef _____cpp_MapManager_h__
#define _____cpp_MapManager_h__

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

class MapManager;

}

}

namespace LbaNet
{

class MapManager;
bool operator==(const MapManager&, const MapManager&);
bool operator<(const MapManager&, const MapManager&);

}

namespace IceInternal
{

::Ice::Object* upCast(::LbaNet::MapManager*);
::IceProxy::Ice::Object* upCast(::IceProxy::LbaNet::MapManager*);

}

namespace LbaNet
{

typedef ::IceInternal::Handle< ::LbaNet::MapManager> MapManagerPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::LbaNet::MapManager> MapManagerPrx;

void __read(::IceInternal::BasicStream*, MapManagerPrx&);
void __patch__MapManagerPtr(void*, ::Ice::ObjectPtr&);

}

namespace IceProxy
{

namespace LbaNet
{

class MapManager : virtual public ::IceProxy::Ice::Object
{
public:

    ::LbaNet::MapObserverPrx JoinMap(const ::std::string& mapName, ::Ice::Long PlayerId, const ::LbaNet::ActorLifeInfo& lifeinfo)
    {
        return JoinMap(mapName, PlayerId, lifeinfo, 0);
    }
    ::LbaNet::MapObserverPrx JoinMap(const ::std::string& mapName, ::Ice::Long PlayerId, const ::LbaNet::ActorLifeInfo& lifeinfo, const ::Ice::Context& __ctx)
    {
        return JoinMap(mapName, PlayerId, lifeinfo, &__ctx);
    }
    
private:

    ::LbaNet::MapObserverPrx JoinMap(const ::std::string&, ::Ice::Long, const ::LbaNet::ActorLifeInfo&, const ::Ice::Context*);
    
public:

    ::LbaNet::ActorLifeInfo LeaveMap(const ::std::string& mapName, ::Ice::Long PlayerId)
    {
        return LeaveMap(mapName, PlayerId, 0);
    }
    ::LbaNet::ActorLifeInfo LeaveMap(const ::std::string& mapName, ::Ice::Long PlayerId, const ::Ice::Context& __ctx)
    {
        return LeaveMap(mapName, PlayerId, &__ctx);
    }
    
private:

    ::LbaNet::ActorLifeInfo LeaveMap(const ::std::string&, ::Ice::Long, const ::Ice::Context*);
    
public:
    
    ::IceInternal::ProxyHandle<MapManager> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapManager*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<MapManager*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapManager> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapManager*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<MapManager*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapManager> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapManager*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<MapManager*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapManager> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapManager*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<MapManager*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapManager> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapManager*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<MapManager*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapManager> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapManager*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<MapManager*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapManager> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapManager*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<MapManager*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapManager> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapManager*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<MapManager*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapManager> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapManager*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<MapManager*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapManager> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapManager*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<MapManager*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapManager> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapManager*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<MapManager*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapManager> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapManager*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<MapManager*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapManager> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapManager*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<MapManager*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapManager> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapManager*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<MapManager*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapManager> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapManager*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<MapManager*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapManager> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapManager*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<MapManager*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapManager> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapManager*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<MapManager*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapManager> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapManager*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<MapManager*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapManager> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapManager*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<MapManager*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
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

class MapManager : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual ::LbaNet::MapObserverPrx JoinMap(const ::std::string&, ::Ice::Long, const ::LbaNet::ActorLifeInfo&, const ::Ice::Context*) = 0;

    virtual ::LbaNet::ActorLifeInfo LeaveMap(const ::std::string&, ::Ice::Long, const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace LbaNet
{

class MapManager : virtual public ::IceDelegate::LbaNet::MapManager,
                   virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual ::LbaNet::MapObserverPrx JoinMap(const ::std::string&, ::Ice::Long, const ::LbaNet::ActorLifeInfo&, const ::Ice::Context*);

    virtual ::LbaNet::ActorLifeInfo LeaveMap(const ::std::string&, ::Ice::Long, const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace LbaNet
{

class MapManager : virtual public ::IceDelegate::LbaNet::MapManager,
                   virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual ::LbaNet::MapObserverPrx JoinMap(const ::std::string&, ::Ice::Long, const ::LbaNet::ActorLifeInfo&, const ::Ice::Context*);

    virtual ::LbaNet::ActorLifeInfo LeaveMap(const ::std::string&, ::Ice::Long, const ::Ice::Context*);
};

}

}

namespace LbaNet
{

class MapManager : virtual public ::Ice::Object
{
public:

    typedef MapManagerPrx ProxyType;
    typedef MapManagerPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::LbaNet::MapObserverPrx JoinMap(const ::std::string&, ::Ice::Long, const ::LbaNet::ActorLifeInfo&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___JoinMap(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::LbaNet::ActorLifeInfo LeaveMap(const ::std::string&, ::Ice::Long, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___LeaveMap(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

}

#endif
