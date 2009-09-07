// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `ActorInfo.ice'

#ifndef _____cpp_ActorInfo_h__
#define _____cpp_ActorInfo_h__

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

class ActorsObserver;

class ActorsParticipant;

}

}

namespace LbaNet
{

class ActorsObserver;
bool operator==(const ActorsObserver&, const ActorsObserver&);
bool operator<(const ActorsObserver&, const ActorsObserver&);

class ActorsParticipant;
bool operator==(const ActorsParticipant&, const ActorsParticipant&);
bool operator<(const ActorsParticipant&, const ActorsParticipant&);

}

namespace IceInternal
{

::Ice::Object* upCast(::LbaNet::ActorsObserver*);
::IceProxy::Ice::Object* upCast(::IceProxy::LbaNet::ActorsObserver*);

::Ice::Object* upCast(::LbaNet::ActorsParticipant*);
::IceProxy::Ice::Object* upCast(::IceProxy::LbaNet::ActorsParticipant*);

}

namespace LbaNet
{

typedef ::IceInternal::Handle< ::LbaNet::ActorsObserver> ActorsObserverPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::LbaNet::ActorsObserver> ActorsObserverPrx;

void __read(::IceInternal::BasicStream*, ActorsObserverPrx&);
void __patch__ActorsObserverPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::LbaNet::ActorsParticipant> ActorsParticipantPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::LbaNet::ActorsParticipant> ActorsParticipantPrx;

void __read(::IceInternal::BasicStream*, ActorsParticipantPrx&);
void __patch__ActorsParticipantPtr(void*, ::Ice::ObjectPtr&);

}

namespace LbaNet
{

struct ActorInfo
{
    ::Ice::Double Time;
    ::Ice::Long ActorId;
    ::std::string MapName;
    ::std::string Name;
    bool DisplayName;
    ::Ice::Float X;
    ::Ice::Float Y;
    ::Ice::Float Z;
    ::Ice::Float Rotation;
    ::Ice::Int Model;
    ::Ice::Int Body;
    ::Ice::Int Animation;
    ::Ice::Short BodyColor;
    ::Ice::Float vX;
    ::Ice::Float vY;
    ::Ice::Float vZ;
    ::Ice::Float vRotation;

    bool operator==(const ActorInfo&) const;
    bool operator<(const ActorInfo&) const;
    bool operator!=(const ActorInfo& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const ActorInfo& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const ActorInfo& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const ActorInfo& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct CurrentAndFutureInfo
{
    ::LbaNet::ActorInfo CurrentState;
    ::LbaNet::ActorInfo FutureState;

    bool operator==(const CurrentAndFutureInfo&) const;
    bool operator<(const CurrentAndFutureInfo&) const;
    bool operator!=(const CurrentAndFutureInfo& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const CurrentAndFutureInfo& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const CurrentAndFutureInfo& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const CurrentAndFutureInfo& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

}

namespace IceProxy
{

namespace LbaNet
{

class ActorsObserver : virtual public ::IceProxy::Ice::Object
{
public:

    void UpdatedInfo(const ::LbaNet::ActorInfo& asi)
    {
        UpdatedInfo(asi, 0);
    }
    void UpdatedInfo(const ::LbaNet::ActorInfo& asi, const ::Ice::Context& __ctx)
    {
        UpdatedInfo(asi, &__ctx);
    }
    
private:

    void UpdatedInfo(const ::LbaNet::ActorInfo&, const ::Ice::Context*);
    
public:

    void Quitted(const ::std::string& ActorName)
    {
        Quitted(ActorName, 0);
    }
    void Quitted(const ::std::string& ActorName, const ::Ice::Context& __ctx)
    {
        Quitted(ActorName, &__ctx);
    }
    
private:

    void Quitted(const ::std::string&, const ::Ice::Context*);
    
public:

    void ActivatedActor(const ::LbaNet::ActorActivationInfo& ai)
    {
        ActivatedActor(ai, 0);
    }
    void ActivatedActor(const ::LbaNet::ActorActivationInfo& ai, const ::Ice::Context& __ctx)
    {
        ActivatedActor(ai, &__ctx);
    }
    
private:

    void ActivatedActor(const ::LbaNet::ActorActivationInfo&, const ::Ice::Context*);
    
public:

    void SignaledActor(const ::LbaNet::ActorSignalInfo& ai)
    {
        SignaledActor(ai, 0);
    }
    void SignaledActor(const ::LbaNet::ActorSignalInfo& ai, const ::Ice::Context& __ctx)
    {
        SignaledActor(ai, &__ctx);
    }
    
private:

    void SignaledActor(const ::LbaNet::ActorSignalInfo&, const ::Ice::Context*);
    
public:
    
    ::IceInternal::ProxyHandle<ActorsObserver> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsObserver*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<ActorsObserver*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsObserver> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsObserver*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<ActorsObserver*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsObserver> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsObserver*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<ActorsObserver*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsObserver> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsObserver*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<ActorsObserver*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsObserver> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsObserver*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<ActorsObserver*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsObserver> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsObserver*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<ActorsObserver*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsObserver> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsObserver*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<ActorsObserver*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsObserver> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsObserver*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<ActorsObserver*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsObserver> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsObserver*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<ActorsObserver*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsObserver> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsObserver*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<ActorsObserver*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsObserver> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsObserver*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<ActorsObserver*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsObserver> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsObserver*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<ActorsObserver*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsObserver> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsObserver*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<ActorsObserver*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsObserver> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsObserver*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<ActorsObserver*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsObserver> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsObserver*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<ActorsObserver*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsObserver> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsObserver*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<ActorsObserver*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsObserver> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsObserver*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<ActorsObserver*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsObserver> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsObserver*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<ActorsObserver*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsObserver> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsObserver*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<ActorsObserver*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
    #endif
    }
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

class ActorsParticipant : virtual public ::IceProxy::Ice::Object
{
public:

    void Update(const ::LbaNet::ActorInfo& asi)
    {
        Update(asi, 0);
    }
    void Update(const ::LbaNet::ActorInfo& asi, const ::Ice::Context& __ctx)
    {
        Update(asi, &__ctx);
    }
    
private:

    void Update(const ::LbaNet::ActorInfo&, const ::Ice::Context*);
    
public:
    
    ::IceInternal::ProxyHandle<ActorsParticipant> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsParticipant*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<ActorsParticipant*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsParticipant> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsParticipant*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<ActorsParticipant*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsParticipant> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsParticipant*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<ActorsParticipant*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsParticipant> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsParticipant*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<ActorsParticipant*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsParticipant> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsParticipant*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<ActorsParticipant*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsParticipant> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsParticipant*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<ActorsParticipant*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsParticipant> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsParticipant*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<ActorsParticipant*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsParticipant> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsParticipant*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<ActorsParticipant*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsParticipant> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsParticipant*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<ActorsParticipant*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsParticipant> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsParticipant*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<ActorsParticipant*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsParticipant> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsParticipant*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<ActorsParticipant*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsParticipant> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsParticipant*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<ActorsParticipant*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsParticipant> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsParticipant*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<ActorsParticipant*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsParticipant> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsParticipant*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<ActorsParticipant*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsParticipant> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsParticipant*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<ActorsParticipant*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsParticipant> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsParticipant*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<ActorsParticipant*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsParticipant> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsParticipant*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<ActorsParticipant*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsParticipant> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsParticipant*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<ActorsParticipant*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ActorsParticipant> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ActorsParticipant*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<ActorsParticipant*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
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

class ActorsObserver : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual void UpdatedInfo(const ::LbaNet::ActorInfo&, const ::Ice::Context*) = 0;

    virtual void Quitted(const ::std::string&, const ::Ice::Context*) = 0;

    virtual void ActivatedActor(const ::LbaNet::ActorActivationInfo&, const ::Ice::Context*) = 0;

    virtual void SignaledActor(const ::LbaNet::ActorSignalInfo&, const ::Ice::Context*) = 0;
};

class ActorsParticipant : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual void Update(const ::LbaNet::ActorInfo&, const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace LbaNet
{

class ActorsObserver : virtual public ::IceDelegate::LbaNet::ActorsObserver,
                       virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual void UpdatedInfo(const ::LbaNet::ActorInfo&, const ::Ice::Context*);

    virtual void Quitted(const ::std::string&, const ::Ice::Context*);

    virtual void ActivatedActor(const ::LbaNet::ActorActivationInfo&, const ::Ice::Context*);

    virtual void SignaledActor(const ::LbaNet::ActorSignalInfo&, const ::Ice::Context*);
};

class ActorsParticipant : virtual public ::IceDelegate::LbaNet::ActorsParticipant,
                          virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual void Update(const ::LbaNet::ActorInfo&, const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace LbaNet
{

class ActorsObserver : virtual public ::IceDelegate::LbaNet::ActorsObserver,
                       virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual void UpdatedInfo(const ::LbaNet::ActorInfo&, const ::Ice::Context*);

    virtual void Quitted(const ::std::string&, const ::Ice::Context*);

    virtual void ActivatedActor(const ::LbaNet::ActorActivationInfo&, const ::Ice::Context*);

    virtual void SignaledActor(const ::LbaNet::ActorSignalInfo&, const ::Ice::Context*);
};

class ActorsParticipant : virtual public ::IceDelegate::LbaNet::ActorsParticipant,
                          virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual void Update(const ::LbaNet::ActorInfo&, const ::Ice::Context*);
};

}

}

namespace LbaNet
{

class ActorsObserver : virtual public ::Ice::Object
{
public:

    typedef ActorsObserverPrx ProxyType;
    typedef ActorsObserverPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void UpdatedInfo(const ::LbaNet::ActorInfo&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___UpdatedInfo(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void Quitted(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___Quitted(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void ActivatedActor(const ::LbaNet::ActorActivationInfo&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___ActivatedActor(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void SignaledActor(const ::LbaNet::ActorSignalInfo&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___SignaledActor(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

class ActorsParticipant : virtual public ::Ice::Object
{
public:

    typedef ActorsParticipantPrx ProxyType;
    typedef ActorsParticipantPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void Update(const ::LbaNet::ActorInfo&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___Update(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

}

#endif
