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

#include <MapObserver.h>
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

static const ::std::string __LbaNet__MapObserver__ActivateActor_name = "ActivateActor";

static const ::std::string __LbaNet__MapObserver__DesactivateActor_name = "DesactivateActor";

::Ice::Object* IceInternal::upCast(::LbaNet::MapObserver* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::LbaNet::MapObserver* p) { return p; }

void
LbaNet::__read(::IceInternal::BasicStream* __is, ::LbaNet::MapObserverPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::LbaNet::MapObserver;
        v->__copyFrom(proxy);
    }
}

bool
LbaNet::ActorActivationInfo::operator==(const ActorActivationInfo& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(ActorId != __rhs.ActorId)
    {
        return false;
    }
    if(PlayerId != __rhs.PlayerId)
    {
        return false;
    }
    if(X != __rhs.X)
    {
        return false;
    }
    if(Y != __rhs.Y)
    {
        return false;
    }
    if(Z != __rhs.Z)
    {
        return false;
    }
    if(Rotation != __rhs.Rotation)
    {
        return false;
    }
    if(NeedDesactivation != __rhs.NeedDesactivation)
    {
        return false;
    }
    return true;
}

bool
LbaNet::ActorActivationInfo::operator<(const ActorActivationInfo& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(ActorId < __rhs.ActorId)
    {
        return true;
    }
    else if(__rhs.ActorId < ActorId)
    {
        return false;
    }
    if(PlayerId < __rhs.PlayerId)
    {
        return true;
    }
    else if(__rhs.PlayerId < PlayerId)
    {
        return false;
    }
    if(X < __rhs.X)
    {
        return true;
    }
    else if(__rhs.X < X)
    {
        return false;
    }
    if(Y < __rhs.Y)
    {
        return true;
    }
    else if(__rhs.Y < Y)
    {
        return false;
    }
    if(Z < __rhs.Z)
    {
        return true;
    }
    else if(__rhs.Z < Z)
    {
        return false;
    }
    if(Rotation < __rhs.Rotation)
    {
        return true;
    }
    else if(__rhs.Rotation < Rotation)
    {
        return false;
    }
    if(NeedDesactivation < __rhs.NeedDesactivation)
    {
        return true;
    }
    else if(__rhs.NeedDesactivation < NeedDesactivation)
    {
        return false;
    }
    return false;
}

void
LbaNet::ActorActivationInfo::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(ActorId);
    __os->write(PlayerId);
    __os->write(X);
    __os->write(Y);
    __os->write(Z);
    __os->write(Rotation);
    __os->write(NeedDesactivation);
}

void
LbaNet::ActorActivationInfo::__read(::IceInternal::BasicStream* __is)
{
    __is->read(ActorId);
    __is->read(PlayerId);
    __is->read(X);
    __is->read(Y);
    __is->read(Z);
    __is->read(Rotation);
    __is->read(NeedDesactivation);
}

void
IceProxy::LbaNet::MapObserver::ActivateActor(const ::LbaNet::ActorActivationInfo& ai, const ::Ice::Context* __ctx)
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
            ::IceDelegate::LbaNet::MapObserver* __del = dynamic_cast< ::IceDelegate::LbaNet::MapObserver*>(__delBase.get());
            __del->ActivateActor(ai, __ctx);
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

void
IceProxy::LbaNet::MapObserver::DesactivateActor(const ::LbaNet::ActorActivationInfo& ai, const ::Ice::Context* __ctx)
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
            ::IceDelegate::LbaNet::MapObserver* __del = dynamic_cast< ::IceDelegate::LbaNet::MapObserver*>(__delBase.get());
            __del->DesactivateActor(ai, __ctx);
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
IceProxy::LbaNet::MapObserver::ice_staticId()
{
    return ::LbaNet::MapObserver::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::LbaNet::MapObserver::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::LbaNet::MapObserver);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::LbaNet::MapObserver::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::LbaNet::MapObserver);
}

::IceProxy::Ice::Object*
IceProxy::LbaNet::MapObserver::__newInstance() const
{
    return new MapObserver;
}

void
IceDelegateM::LbaNet::MapObserver::ActivateActor(const ::LbaNet::ActorActivationInfo& ai, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__MapObserver__ActivateActor_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        ai.__write(__os);
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
IceDelegateM::LbaNet::MapObserver::DesactivateActor(const ::LbaNet::ActorActivationInfo& ai, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__MapObserver__DesactivateActor_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        ai.__write(__os);
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
IceDelegateD::LbaNet::MapObserver::ActivateActor(const ::LbaNet::ActorActivationInfo& ai, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::LbaNet::ActorActivationInfo& ai, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_ai(ai)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::LbaNet::MapObserver* servant = dynamic_cast< ::LbaNet::MapObserver*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->ActivateActor(_m_ai, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::LbaNet::ActorActivationInfo& _m_ai;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __LbaNet__MapObserver__ActivateActor_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(ai, __current);
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
IceDelegateD::LbaNet::MapObserver::DesactivateActor(const ::LbaNet::ActorActivationInfo& ai, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::LbaNet::ActorActivationInfo& ai, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_ai(ai)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::LbaNet::MapObserver* servant = dynamic_cast< ::LbaNet::MapObserver*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->DesactivateActor(_m_ai, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::LbaNet::ActorActivationInfo& _m_ai;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __LbaNet__MapObserver__DesactivateActor_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(ai, __current);
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
LbaNet::MapObserver::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __LbaNet__MapObserver_ids[2] =
{
    "::Ice::Object",
    "::LbaNet::MapObserver"
};

bool
LbaNet::MapObserver::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__LbaNet__MapObserver_ids, __LbaNet__MapObserver_ids + 2, _s);
}

::std::vector< ::std::string>
LbaNet::MapObserver::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__LbaNet__MapObserver_ids[0], &__LbaNet__MapObserver_ids[2]);
}

const ::std::string&
LbaNet::MapObserver::ice_id(const ::Ice::Current&) const
{
    return __LbaNet__MapObserver_ids[1];
}

const ::std::string&
LbaNet::MapObserver::ice_staticId()
{
    return __LbaNet__MapObserver_ids[1];
}

::Ice::DispatchStatus
LbaNet::MapObserver::___ActivateActor(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::LbaNet::ActorActivationInfo ai;
    ai.__read(__is);
    __is->endReadEncaps();
    ActivateActor(ai, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
LbaNet::MapObserver::___DesactivateActor(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::LbaNet::ActorActivationInfo ai;
    ai.__read(__is);
    __is->endReadEncaps();
    DesactivateActor(ai, __current);
    return ::Ice::DispatchOK;
}

static ::std::string __LbaNet__MapObserver_all[] =
{
    "ActivateActor",
    "DesactivateActor",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping"
};

::Ice::DispatchStatus
LbaNet::MapObserver::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__LbaNet__MapObserver_all, __LbaNet__MapObserver_all + 6, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __LbaNet__MapObserver_all)
    {
        case 0:
        {
            return ___ActivateActor(in, current);
        }
        case 1:
        {
            return ___DesactivateActor(in, current);
        }
        case 2:
        {
            return ___ice_id(in, current);
        }
        case 3:
        {
            return ___ice_ids(in, current);
        }
        case 4:
        {
            return ___ice_isA(in, current);
        }
        case 5:
        {
            return ___ice_ping(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
LbaNet::MapObserver::__write(::IceInternal::BasicStream* __os) const
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
LbaNet::MapObserver::__read(::IceInternal::BasicStream* __is, bool __rid)
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
LbaNet::MapObserver::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type LbaNet::MapObserver was not generated with stream support";
    throw ex;
}

void
LbaNet::MapObserver::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type LbaNet::MapObserver was not generated with stream support";
    throw ex;
}

void 
LbaNet::__patch__MapObserverPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::LbaNet::MapObserverPtr* p = static_cast< ::LbaNet::MapObserverPtr*>(__addr);
    assert(p);
    *p = ::LbaNet::MapObserverPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::LbaNet::MapObserver::ice_staticId(), v->ice_id());
    }
}

bool
LbaNet::operator==(const ::LbaNet::MapObserver& l, const ::LbaNet::MapObserver& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
LbaNet::operator<(const ::LbaNet::MapObserver& l, const ::LbaNet::MapObserver& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}
