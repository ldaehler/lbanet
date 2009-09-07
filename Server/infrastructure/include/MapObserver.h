// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `MapObserver.ice'

#ifndef _____cpp_MapObserver_h__
#define _____cpp_MapObserver_h__

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

class MapObserver;

}

}

namespace LbaNet
{

class MapObserver;
bool operator==(const MapObserver&, const MapObserver&);
bool operator<(const MapObserver&, const MapObserver&);

}

namespace IceInternal
{

::Ice::Object* upCast(::LbaNet::MapObserver*);
::IceProxy::Ice::Object* upCast(::IceProxy::LbaNet::MapObserver*);

}

namespace LbaNet
{

typedef ::IceInternal::Handle< ::LbaNet::MapObserver> MapObserverPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::LbaNet::MapObserver> MapObserverPrx;

void __read(::IceInternal::BasicStream*, MapObserverPrx&);
void __patch__MapObserverPtr(void*, ::Ice::ObjectPtr&);

}

namespace LbaNet
{

struct ActorActivationInfo
{
    ::Ice::Long ActivatedId;
    bool Activate;
    ::Ice::Long ActorId;
    ::Ice::Float X;
    ::Ice::Float Y;
    ::Ice::Float Z;
    ::Ice::Float Rotation;

    bool operator==(const ActorActivationInfo&) const;
    bool operator<(const ActorActivationInfo&) const;
    bool operator!=(const ActorActivationInfo& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const ActorActivationInfo& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const ActorActivationInfo& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const ActorActivationInfo& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

typedef ::std::vector< ::Ice::Long> TargetSeq;

struct ActorSignalInfo
{
    ::LbaNet::TargetSeq Targets;
    ::Ice::Long SignalId;
    ::Ice::Long ActorId;

    bool operator==(const ActorSignalInfo&) const;
    bool operator<(const ActorSignalInfo&) const;
    bool operator!=(const ActorSignalInfo& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const ActorSignalInfo& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const ActorSignalInfo& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const ActorSignalInfo& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct ActorUpdateInfo
{
    ::Ice::Long ActorId;
    bool On;
    bool Open;
    ::Ice::Int Counter;
    bool SignalOn;
    ::Ice::Long CurrentScript;
    ::LbaNet::TargetSeq CurrentSignals;
    ::Ice::Float X;
    ::Ice::Float Y;
    ::Ice::Float Z;
    ::Ice::Float Rotation;

    bool operator==(const ActorUpdateInfo&) const;
    bool operator<(const ActorUpdateInfo&) const;
    bool operator!=(const ActorUpdateInfo& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const ActorUpdateInfo& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const ActorUpdateInfo& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const ActorUpdateInfo& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

typedef ::std::vector< ::LbaNet::ActorUpdateInfo> UpdateSeq;
void __writeUpdateSeq(::IceInternal::BasicStream*, const ::LbaNet::ActorUpdateInfo*, const ::LbaNet::ActorUpdateInfo*);
void __readUpdateSeq(::IceInternal::BasicStream*, UpdateSeq&);

}

namespace IceProxy
{

namespace LbaNet
{

class MapObserver : virtual public ::IceProxy::Ice::Object
{
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
    
    ::IceInternal::ProxyHandle<MapObserver> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapObserver*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<MapObserver*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapObserver> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapObserver*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<MapObserver*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapObserver> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapObserver*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<MapObserver*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapObserver> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapObserver*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<MapObserver*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapObserver> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapObserver*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<MapObserver*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapObserver> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapObserver*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<MapObserver*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapObserver> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapObserver*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<MapObserver*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapObserver> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapObserver*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<MapObserver*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapObserver> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapObserver*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<MapObserver*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapObserver> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapObserver*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<MapObserver*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapObserver> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapObserver*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<MapObserver*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapObserver> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapObserver*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<MapObserver*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapObserver> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapObserver*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<MapObserver*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapObserver> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapObserver*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<MapObserver*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapObserver> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapObserver*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<MapObserver*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapObserver> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapObserver*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<MapObserver*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapObserver> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapObserver*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<MapObserver*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapObserver> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapObserver*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<MapObserver*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<MapObserver> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<MapObserver*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<MapObserver*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
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

class MapObserver : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual void ActivateActor(const ::LbaNet::ActorActivationInfo&, const ::Ice::Context*) = 0;

    virtual void SignalActor(const ::LbaNet::ActorSignalInfo&, const ::Ice::Context*) = 0;

    virtual ::LbaNet::UpdateSeq GetUpdatedInfo(const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace LbaNet
{

class MapObserver : virtual public ::IceDelegate::LbaNet::MapObserver,
                    virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual void ActivateActor(const ::LbaNet::ActorActivationInfo&, const ::Ice::Context*);

    virtual void SignalActor(const ::LbaNet::ActorSignalInfo&, const ::Ice::Context*);

    virtual ::LbaNet::UpdateSeq GetUpdatedInfo(const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace LbaNet
{

class MapObserver : virtual public ::IceDelegate::LbaNet::MapObserver,
                    virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual void ActivateActor(const ::LbaNet::ActorActivationInfo&, const ::Ice::Context*);

    virtual void SignalActor(const ::LbaNet::ActorSignalInfo&, const ::Ice::Context*);

    virtual ::LbaNet::UpdateSeq GetUpdatedInfo(const ::Ice::Context*);
};

}

}

namespace LbaNet
{

class MapObserver : virtual public ::Ice::Object
{
public:

    typedef MapObserverPrx ProxyType;
    typedef MapObserverPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void ActivateActor(const ::LbaNet::ActorActivationInfo&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___ActivateActor(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void SignalActor(const ::LbaNet::ActorSignalInfo&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___SignalActor(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::LbaNet::UpdateSeq GetUpdatedInfo(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___GetUpdatedInfo(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

}

#endif
