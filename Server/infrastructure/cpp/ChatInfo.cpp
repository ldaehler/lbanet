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

#include <ChatInfo.h>
#include <Ice/LocalException.h>
#include <Ice/ObjectFactory.h>
#include <Ice/BasicStream.h>
#include <IceUtil/Iterator.h>
#include <IceUtil/ScopedArray.h>

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

static const ::std::string __LbaNet__ChatRoomObserver__Message_name = "Message";

static const ::std::string __LbaNet__ChatRoomParticipant__Say_name = "Say";

::Ice::Object* IceInternal::upCast(::LbaNet::ChatRoomObserver* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::LbaNet::ChatRoomObserver* p) { return p; }

::Ice::Object* IceInternal::upCast(::LbaNet::ChatRoomParticipant* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::LbaNet::ChatRoomParticipant* p) { return p; }

void
LbaNet::__read(::IceInternal::BasicStream* __is, ::LbaNet::ChatRoomObserverPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::LbaNet::ChatRoomObserver;
        v->__copyFrom(proxy);
    }
}

void
LbaNet::__read(::IceInternal::BasicStream* __is, ::LbaNet::ChatRoomParticipantPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::LbaNet::ChatRoomParticipant;
        v->__copyFrom(proxy);
    }
}

void
IceProxy::LbaNet::ChatRoomObserver::Message(const ::std::string& Sender, const ::std::string& data, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
#if defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ >= 0x0600) // C++Builder 2009 compiler bug
            IceUtil::DummyBCC dummy;
#endif
            __delBase = __getDelegate(false);
            ::IceDelegate::LbaNet::ChatRoomObserver* __del = dynamic_cast< ::IceDelegate::LbaNet::ChatRoomObserver*>(__delBase.get());
            __del->Message(Sender, data, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

const ::std::string&
IceProxy::LbaNet::ChatRoomObserver::ice_staticId()
{
    return ::LbaNet::ChatRoomObserver::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::LbaNet::ChatRoomObserver::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::LbaNet::ChatRoomObserver);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::LbaNet::ChatRoomObserver::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::LbaNet::ChatRoomObserver);
}

::IceProxy::Ice::Object*
IceProxy::LbaNet::ChatRoomObserver::__newInstance() const
{
    return new ChatRoomObserver;
}

void
IceProxy::LbaNet::ChatRoomParticipant::Say(const ::std::string& Sender, const ::std::string& data, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
#if defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ >= 0x0600) // C++Builder 2009 compiler bug
            IceUtil::DummyBCC dummy;
#endif
            __delBase = __getDelegate(false);
            ::IceDelegate::LbaNet::ChatRoomParticipant* __del = dynamic_cast< ::IceDelegate::LbaNet::ChatRoomParticipant*>(__delBase.get());
            __del->Say(Sender, data, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

const ::std::string&
IceProxy::LbaNet::ChatRoomParticipant::ice_staticId()
{
    return ::LbaNet::ChatRoomParticipant::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::LbaNet::ChatRoomParticipant::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::LbaNet::ChatRoomParticipant);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::LbaNet::ChatRoomParticipant::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::LbaNet::ChatRoomParticipant);
}

::IceProxy::Ice::Object*
IceProxy::LbaNet::ChatRoomParticipant::__newInstance() const
{
    return new ChatRoomParticipant;
}

void
IceDelegateM::LbaNet::ChatRoomObserver::Message(const ::std::string& Sender, const ::std::string& data, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__ChatRoomObserver__Message_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(Sender);
        __os->write(data);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

void
IceDelegateM::LbaNet::ChatRoomParticipant::Say(const ::std::string& Sender, const ::std::string& data, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__ChatRoomParticipant__Say_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(Sender);
        __os->write(data);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

void
IceDelegateD::LbaNet::ChatRoomObserver::Message(const ::std::string& Sender, const ::std::string& data, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::std::string& Sender, const ::std::string& data, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_Sender(Sender),
            _m_data(data)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::LbaNet::ChatRoomObserver* servant = dynamic_cast< ::LbaNet::ChatRoomObserver*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->Message(_m_Sender, _m_data, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::std::string& _m_Sender;
        const ::std::string& _m_data;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __LbaNet__ChatRoomObserver__Message_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(Sender, data, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

void
IceDelegateD::LbaNet::ChatRoomParticipant::Say(const ::std::string& Sender, const ::std::string& data, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::std::string& Sender, const ::std::string& data, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_Sender(Sender),
            _m_data(data)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::LbaNet::ChatRoomParticipant* servant = dynamic_cast< ::LbaNet::ChatRoomParticipant*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->Say(_m_Sender, _m_data, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::std::string& _m_Sender;
        const ::std::string& _m_data;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __LbaNet__ChatRoomParticipant__Say_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(Sender, data, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

::Ice::ObjectPtr
LbaNet::ChatRoomObserver::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __LbaNet__ChatRoomObserver_ids[2] =
{
    "::Ice::Object",
    "::LbaNet::ChatRoomObserver"
};

bool
LbaNet::ChatRoomObserver::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__LbaNet__ChatRoomObserver_ids, __LbaNet__ChatRoomObserver_ids + 2, _s);
}

::std::vector< ::std::string>
LbaNet::ChatRoomObserver::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__LbaNet__ChatRoomObserver_ids[0], &__LbaNet__ChatRoomObserver_ids[2]);
}

const ::std::string&
LbaNet::ChatRoomObserver::ice_id(const ::Ice::Current&) const
{
    return __LbaNet__ChatRoomObserver_ids[1];
}

const ::std::string&
LbaNet::ChatRoomObserver::ice_staticId()
{
    return __LbaNet__ChatRoomObserver_ids[1];
}

::Ice::DispatchStatus
LbaNet::ChatRoomObserver::___Message(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string Sender;
    ::std::string data;
    __is->read(Sender);
    __is->read(data);
    __is->endReadEncaps();
    Message(Sender, data, __current);
    return ::Ice::DispatchOK;
}

static ::std::string __LbaNet__ChatRoomObserver_all[] =
{
    "Message",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping"
};

::Ice::DispatchStatus
LbaNet::ChatRoomObserver::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__LbaNet__ChatRoomObserver_all, __LbaNet__ChatRoomObserver_all + 5, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __LbaNet__ChatRoomObserver_all)
    {
        case 0:
        {
            return ___Message(in, current);
        }
        case 1:
        {
            return ___ice_id(in, current);
        }
        case 2:
        {
            return ___ice_ids(in, current);
        }
        case 3:
        {
            return ___ice_isA(in, current);
        }
        case 4:
        {
            return ___ice_ping(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
LbaNet::ChatRoomObserver::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
LbaNet::ChatRoomObserver::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
LbaNet::ChatRoomObserver::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type LbaNet::ChatRoomObserver was not generated with stream support";
    throw ex;
}

void
LbaNet::ChatRoomObserver::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type LbaNet::ChatRoomObserver was not generated with stream support";
    throw ex;
}

void 
LbaNet::__patch__ChatRoomObserverPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::LbaNet::ChatRoomObserverPtr* p = static_cast< ::LbaNet::ChatRoomObserverPtr*>(__addr);
    assert(p);
    *p = ::LbaNet::ChatRoomObserverPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::LbaNet::ChatRoomObserver::ice_staticId(), v->ice_id());
    }
}

bool
LbaNet::operator==(const ::LbaNet::ChatRoomObserver& l, const ::LbaNet::ChatRoomObserver& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
LbaNet::operator<(const ::LbaNet::ChatRoomObserver& l, const ::LbaNet::ChatRoomObserver& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

::Ice::ObjectPtr
LbaNet::ChatRoomParticipant::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __LbaNet__ChatRoomParticipant_ids[2] =
{
    "::Ice::Object",
    "::LbaNet::ChatRoomParticipant"
};

bool
LbaNet::ChatRoomParticipant::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__LbaNet__ChatRoomParticipant_ids, __LbaNet__ChatRoomParticipant_ids + 2, _s);
}

::std::vector< ::std::string>
LbaNet::ChatRoomParticipant::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__LbaNet__ChatRoomParticipant_ids[0], &__LbaNet__ChatRoomParticipant_ids[2]);
}

const ::std::string&
LbaNet::ChatRoomParticipant::ice_id(const ::Ice::Current&) const
{
    return __LbaNet__ChatRoomParticipant_ids[1];
}

const ::std::string&
LbaNet::ChatRoomParticipant::ice_staticId()
{
    return __LbaNet__ChatRoomParticipant_ids[1];
}

::Ice::DispatchStatus
LbaNet::ChatRoomParticipant::___Say(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string Sender;
    ::std::string data;
    __is->read(Sender);
    __is->read(data);
    __is->endReadEncaps();
    Say(Sender, data, __current);
    return ::Ice::DispatchOK;
}

static ::std::string __LbaNet__ChatRoomParticipant_all[] =
{
    "Say",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping"
};

::Ice::DispatchStatus
LbaNet::ChatRoomParticipant::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__LbaNet__ChatRoomParticipant_all, __LbaNet__ChatRoomParticipant_all + 5, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __LbaNet__ChatRoomParticipant_all)
    {
        case 0:
        {
            return ___Say(in, current);
        }
        case 1:
        {
            return ___ice_id(in, current);
        }
        case 2:
        {
            return ___ice_ids(in, current);
        }
        case 3:
        {
            return ___ice_isA(in, current);
        }
        case 4:
        {
            return ___ice_ping(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
LbaNet::ChatRoomParticipant::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
LbaNet::ChatRoomParticipant::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
LbaNet::ChatRoomParticipant::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type LbaNet::ChatRoomParticipant was not generated with stream support";
    throw ex;
}

void
LbaNet::ChatRoomParticipant::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type LbaNet::ChatRoomParticipant was not generated with stream support";
    throw ex;
}

void 
LbaNet::__patch__ChatRoomParticipantPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::LbaNet::ChatRoomParticipantPtr* p = static_cast< ::LbaNet::ChatRoomParticipantPtr*>(__addr);
    assert(p);
    *p = ::LbaNet::ChatRoomParticipantPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::LbaNet::ChatRoomParticipant::ice_staticId(), v->ice_id());
    }
}

bool
LbaNet::operator==(const ::LbaNet::ChatRoomParticipant& l, const ::LbaNet::ChatRoomParticipant& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
LbaNet::operator<(const ::LbaNet::ChatRoomParticipant& l, const ::LbaNet::ChatRoomParticipant& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}
