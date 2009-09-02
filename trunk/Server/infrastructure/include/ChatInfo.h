// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `ChatInfo.ice'

#ifndef _____cpp_ChatInfo_h__
#define _____cpp_ChatInfo_h__

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

class ChatRoomObserver;

class ChatRoomParticipant;

}

}

namespace LbaNet
{

class ChatRoomObserver;
bool operator==(const ChatRoomObserver&, const ChatRoomObserver&);
bool operator<(const ChatRoomObserver&, const ChatRoomObserver&);

class ChatRoomParticipant;
bool operator==(const ChatRoomParticipant&, const ChatRoomParticipant&);
bool operator<(const ChatRoomParticipant&, const ChatRoomParticipant&);

}

namespace IceInternal
{

::Ice::Object* upCast(::LbaNet::ChatRoomObserver*);
::IceProxy::Ice::Object* upCast(::IceProxy::LbaNet::ChatRoomObserver*);

::Ice::Object* upCast(::LbaNet::ChatRoomParticipant*);
::IceProxy::Ice::Object* upCast(::IceProxy::LbaNet::ChatRoomParticipant*);

}

namespace LbaNet
{

typedef ::IceInternal::Handle< ::LbaNet::ChatRoomObserver> ChatRoomObserverPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::LbaNet::ChatRoomObserver> ChatRoomObserverPrx;

void __read(::IceInternal::BasicStream*, ChatRoomObserverPrx&);
void __patch__ChatRoomObserverPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::LbaNet::ChatRoomParticipant> ChatRoomParticipantPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::LbaNet::ChatRoomParticipant> ChatRoomParticipantPrx;

void __read(::IceInternal::BasicStream*, ChatRoomParticipantPrx&);
void __patch__ChatRoomParticipantPtr(void*, ::Ice::ObjectPtr&);

}

namespace IceProxy
{

namespace LbaNet
{

class ChatRoomObserver : virtual public ::IceProxy::Ice::Object
{
public:

    void Message(const ::std::string& Sender, const ::std::string& data)
    {
        Message(Sender, data, 0);
    }
    void Message(const ::std::string& Sender, const ::std::string& data, const ::Ice::Context& __ctx)
    {
        Message(Sender, data, &__ctx);
    }
    
private:

    void Message(const ::std::string&, const ::std::string&, const ::Ice::Context*);
    
public:
    
    ::IceInternal::ProxyHandle<ChatRoomObserver> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomObserver*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<ChatRoomObserver*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomObserver> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomObserver*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<ChatRoomObserver*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomObserver> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomObserver*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<ChatRoomObserver*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomObserver> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomObserver*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<ChatRoomObserver*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomObserver> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomObserver*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<ChatRoomObserver*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomObserver> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomObserver*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<ChatRoomObserver*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomObserver> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomObserver*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<ChatRoomObserver*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomObserver> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomObserver*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<ChatRoomObserver*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomObserver> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomObserver*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<ChatRoomObserver*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomObserver> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomObserver*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<ChatRoomObserver*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomObserver> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomObserver*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<ChatRoomObserver*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomObserver> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomObserver*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<ChatRoomObserver*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomObserver> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomObserver*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<ChatRoomObserver*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomObserver> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomObserver*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<ChatRoomObserver*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomObserver> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomObserver*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<ChatRoomObserver*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomObserver> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomObserver*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<ChatRoomObserver*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomObserver> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomObserver*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<ChatRoomObserver*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomObserver> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomObserver*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<ChatRoomObserver*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomObserver> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomObserver*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<ChatRoomObserver*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
    #endif
    }
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

class ChatRoomParticipant : virtual public ::IceProxy::Ice::Object
{
public:

    void Say(const ::std::string& Sender, const ::std::string& data)
    {
        Say(Sender, data, 0);
    }
    void Say(const ::std::string& Sender, const ::std::string& data, const ::Ice::Context& __ctx)
    {
        Say(Sender, data, &__ctx);
    }
    
private:

    void Say(const ::std::string&, const ::std::string&, const ::Ice::Context*);
    
public:
    
    ::IceInternal::ProxyHandle<ChatRoomParticipant> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomParticipant*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<ChatRoomParticipant*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomParticipant> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomParticipant*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<ChatRoomParticipant*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomParticipant> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomParticipant*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<ChatRoomParticipant*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomParticipant> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomParticipant*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<ChatRoomParticipant*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomParticipant> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomParticipant*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<ChatRoomParticipant*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomParticipant> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomParticipant*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<ChatRoomParticipant*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomParticipant> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomParticipant*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<ChatRoomParticipant*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomParticipant> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomParticipant*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<ChatRoomParticipant*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomParticipant> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomParticipant*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<ChatRoomParticipant*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomParticipant> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomParticipant*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<ChatRoomParticipant*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomParticipant> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomParticipant*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<ChatRoomParticipant*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomParticipant> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomParticipant*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<ChatRoomParticipant*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomParticipant> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomParticipant*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<ChatRoomParticipant*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomParticipant> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomParticipant*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<ChatRoomParticipant*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomParticipant> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomParticipant*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<ChatRoomParticipant*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomParticipant> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomParticipant*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<ChatRoomParticipant*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomParticipant> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomParticipant*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<ChatRoomParticipant*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomParticipant> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomParticipant*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<ChatRoomParticipant*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<ChatRoomParticipant> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<ChatRoomParticipant*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<ChatRoomParticipant*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
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

class ChatRoomObserver : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual void Message(const ::std::string&, const ::std::string&, const ::Ice::Context*) = 0;
};

class ChatRoomParticipant : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual void Say(const ::std::string&, const ::std::string&, const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace LbaNet
{

class ChatRoomObserver : virtual public ::IceDelegate::LbaNet::ChatRoomObserver,
                         virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual void Message(const ::std::string&, const ::std::string&, const ::Ice::Context*);
};

class ChatRoomParticipant : virtual public ::IceDelegate::LbaNet::ChatRoomParticipant,
                            virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual void Say(const ::std::string&, const ::std::string&, const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace LbaNet
{

class ChatRoomObserver : virtual public ::IceDelegate::LbaNet::ChatRoomObserver,
                         virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual void Message(const ::std::string&, const ::std::string&, const ::Ice::Context*);
};

class ChatRoomParticipant : virtual public ::IceDelegate::LbaNet::ChatRoomParticipant,
                            virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual void Say(const ::std::string&, const ::std::string&, const ::Ice::Context*);
};

}

}

namespace LbaNet
{

class ChatRoomObserver : virtual public ::Ice::Object
{
public:

    typedef ChatRoomObserverPrx ProxyType;
    typedef ChatRoomObserverPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void Message(const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___Message(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

class ChatRoomParticipant : virtual public ::Ice::Object
{
public:

    typedef ChatRoomParticipantPrx ProxyType;
    typedef ChatRoomParticipantPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void Say(const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___Say(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

}

#endif
