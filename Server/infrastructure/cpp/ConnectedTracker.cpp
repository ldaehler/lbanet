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

#include <ConnectedTracker.h>
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

static const ::std::string __LbaNet__ConnectedTracker__Connect_name = "Connect";

static const ::std::string __LbaNet__ConnectedTracker__Disconnect_name = "Disconnect";

static const ::std::string __LbaNet__ConnectedTracker__ChangeStatus_name = "ChangeStatus";

static const ::std::string __LbaNet__ConnectedTracker__GetConnected_name = "GetConnected";

::Ice::Object* IceInternal::upCast(::LbaNet::ConnectedTracker* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::LbaNet::ConnectedTracker* p) { return p; }

void
LbaNet::__read(::IceInternal::BasicStream* __is, ::LbaNet::ConnectedTrackerPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::LbaNet::ConnectedTracker;
        v->__copyFrom(proxy);
    }
}

bool
LbaNet::PlayerInfo::operator==(const PlayerInfo& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(Id != __rhs.Id)
    {
        return false;
    }
    if(Status != __rhs.Status)
    {
        return false;
    }
    return true;
}

bool
LbaNet::PlayerInfo::operator<(const PlayerInfo& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(Id < __rhs.Id)
    {
        return true;
    }
    else if(__rhs.Id < Id)
    {
        return false;
    }
    if(Status < __rhs.Status)
    {
        return true;
    }
    else if(__rhs.Status < Status)
    {
        return false;
    }
    return false;
}

void
LbaNet::PlayerInfo::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(Id);
    __os->write(Status);
}

void
LbaNet::PlayerInfo::__read(::IceInternal::BasicStream* __is)
{
    __is->read(Id);
    __is->read(Status);
}

void
LbaNet::__writeConnectedL(::IceInternal::BasicStream* __os, const ::LbaNet::ConnectedL& v)
{
    __os->writeSize(::Ice::Int(v.size()));
    ::LbaNet::ConnectedL::const_iterator p;
    for(p = v.begin(); p != v.end(); ++p)
    {
        __os->write(p->first);
        p->second.__write(__os);
    }
}

void
LbaNet::__readConnectedL(::IceInternal::BasicStream* __is, ::LbaNet::ConnectedL& v)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    while(sz--)
    {
        ::std::pair<const  ::std::string, ::LbaNet::PlayerInfo> pair;
        __is->read(const_cast< ::std::string&>(pair.first));
        ::LbaNet::ConnectedL::iterator __i = v.insert(v.end(), pair);
        __i->second.__read(__is);
    }
}

::Ice::Long
IceProxy::LbaNet::ConnectedTracker::Connect(const ::std::string& Nickname, const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__LbaNet__ConnectedTracker__Connect_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::LbaNet::ConnectedTracker* __del = dynamic_cast< ::IceDelegate::LbaNet::ConnectedTracker*>(__delBase.get());
            return __del->Connect(Nickname, __ctx);
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
IceProxy::LbaNet::ConnectedTracker::Disconnect(::Ice::Long PlayerId, const ::Ice::Context* __ctx)
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
            ::IceDelegate::LbaNet::ConnectedTracker* __del = dynamic_cast< ::IceDelegate::LbaNet::ConnectedTracker*>(__delBase.get());
            __del->Disconnect(PlayerId, __ctx);
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
IceProxy::LbaNet::ConnectedTracker::ChangeStatus(const ::std::string& Nickname, const ::std::string& NewStatus, const ::Ice::Context* __ctx)
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
            ::IceDelegate::LbaNet::ConnectedTracker* __del = dynamic_cast< ::IceDelegate::LbaNet::ConnectedTracker*>(__delBase.get());
            __del->ChangeStatus(Nickname, NewStatus, __ctx);
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

::LbaNet::ConnectedL
IceProxy::LbaNet::ConnectedTracker::GetConnected(const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__LbaNet__ConnectedTracker__GetConnected_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::LbaNet::ConnectedTracker* __del = dynamic_cast< ::IceDelegate::LbaNet::ConnectedTracker*>(__delBase.get());
            return __del->GetConnected(__ctx);
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
IceProxy::LbaNet::ConnectedTracker::ice_staticId()
{
    return ::LbaNet::ConnectedTracker::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::LbaNet::ConnectedTracker::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::LbaNet::ConnectedTracker);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::LbaNet::ConnectedTracker::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::LbaNet::ConnectedTracker);
}

::IceProxy::Ice::Object*
IceProxy::LbaNet::ConnectedTracker::__newInstance() const
{
    return new ConnectedTracker;
}

::Ice::Long
IceDelegateM::LbaNet::ConnectedTracker::Connect(const ::std::string& Nickname, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__ConnectedTracker__Connect_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(Nickname);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    ::Ice::Long __ret;
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
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::LbaNet::ConnectedTracker::Disconnect(::Ice::Long PlayerId, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__ConnectedTracker__Disconnect_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(PlayerId);
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
IceDelegateM::LbaNet::ConnectedTracker::ChangeStatus(const ::std::string& Nickname, const ::std::string& NewStatus, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__ConnectedTracker__ChangeStatus_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(Nickname);
        __os->write(NewStatus);
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

::LbaNet::ConnectedL
IceDelegateM::LbaNet::ConnectedTracker::GetConnected(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__ConnectedTracker__GetConnected_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    ::LbaNet::ConnectedL __ret;
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
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        ::LbaNet::__readConnectedL(__is, __ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::Ice::Long
IceDelegateD::LbaNet::ConnectedTracker::Connect(const ::std::string& Nickname, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::Long& __result, const ::std::string& Nickname, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_Nickname(Nickname)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::LbaNet::ConnectedTracker* servant = dynamic_cast< ::LbaNet::ConnectedTracker*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->Connect(_m_Nickname, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::Ice::Long& _result;
        const ::std::string& _m_Nickname;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __LbaNet__ConnectedTracker__Connect_name, ::Ice::Normal, __context);
    ::Ice::Long __result;
    try
    {
        _DirectI __direct(__result, Nickname, __current);
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
    return __result;
}

void
IceDelegateD::LbaNet::ConnectedTracker::Disconnect(::Ice::Long PlayerId, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::Long PlayerId, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_PlayerId(PlayerId)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::LbaNet::ConnectedTracker* servant = dynamic_cast< ::LbaNet::ConnectedTracker*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->Disconnect(_m_PlayerId, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::Ice::Long _m_PlayerId;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __LbaNet__ConnectedTracker__Disconnect_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(PlayerId, __current);
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
IceDelegateD::LbaNet::ConnectedTracker::ChangeStatus(const ::std::string& Nickname, const ::std::string& NewStatus, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::std::string& Nickname, const ::std::string& NewStatus, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_Nickname(Nickname),
            _m_NewStatus(NewStatus)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::LbaNet::ConnectedTracker* servant = dynamic_cast< ::LbaNet::ConnectedTracker*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->ChangeStatus(_m_Nickname, _m_NewStatus, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::std::string& _m_Nickname;
        const ::std::string& _m_NewStatus;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __LbaNet__ConnectedTracker__ChangeStatus_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(Nickname, NewStatus, __current);
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

::LbaNet::ConnectedL
IceDelegateD::LbaNet::ConnectedTracker::GetConnected(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::LbaNet::ConnectedL& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::LbaNet::ConnectedTracker* servant = dynamic_cast< ::LbaNet::ConnectedTracker*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->GetConnected(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::LbaNet::ConnectedL& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __LbaNet__ConnectedTracker__GetConnected_name, ::Ice::Normal, __context);
    ::LbaNet::ConnectedL __result;
    try
    {
        _DirectI __direct(__result, __current);
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
    return __result;
}

::Ice::ObjectPtr
LbaNet::ConnectedTracker::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __LbaNet__ConnectedTracker_ids[2] =
{
    "::Ice::Object",
    "::LbaNet::ConnectedTracker"
};

bool
LbaNet::ConnectedTracker::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__LbaNet__ConnectedTracker_ids, __LbaNet__ConnectedTracker_ids + 2, _s);
}

::std::vector< ::std::string>
LbaNet::ConnectedTracker::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__LbaNet__ConnectedTracker_ids[0], &__LbaNet__ConnectedTracker_ids[2]);
}

const ::std::string&
LbaNet::ConnectedTracker::ice_id(const ::Ice::Current&) const
{
    return __LbaNet__ConnectedTracker_ids[1];
}

const ::std::string&
LbaNet::ConnectedTracker::ice_staticId()
{
    return __LbaNet__ConnectedTracker_ids[1];
}

::Ice::DispatchStatus
LbaNet::ConnectedTracker::___Connect(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string Nickname;
    __is->read(Nickname);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::Ice::Long __ret = Connect(Nickname, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
LbaNet::ConnectedTracker::___Disconnect(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::Ice::Long PlayerId;
    __is->read(PlayerId);
    __is->endReadEncaps();
    Disconnect(PlayerId, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
LbaNet::ConnectedTracker::___ChangeStatus(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string Nickname;
    ::std::string NewStatus;
    __is->read(Nickname);
    __is->read(NewStatus);
    __is->endReadEncaps();
    ChangeStatus(Nickname, NewStatus, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
LbaNet::ConnectedTracker::___GetConnected(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::LbaNet::ConnectedL __ret = GetConnected(__current);
    ::LbaNet::__writeConnectedL(__os, __ret);
    return ::Ice::DispatchOK;
}

static ::std::string __LbaNet__ConnectedTracker_all[] =
{
    "ChangeStatus",
    "Connect",
    "Disconnect",
    "GetConnected",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping"
};

::Ice::DispatchStatus
LbaNet::ConnectedTracker::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__LbaNet__ConnectedTracker_all, __LbaNet__ConnectedTracker_all + 8, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __LbaNet__ConnectedTracker_all)
    {
        case 0:
        {
            return ___ChangeStatus(in, current);
        }
        case 1:
        {
            return ___Connect(in, current);
        }
        case 2:
        {
            return ___Disconnect(in, current);
        }
        case 3:
        {
            return ___GetConnected(in, current);
        }
        case 4:
        {
            return ___ice_id(in, current);
        }
        case 5:
        {
            return ___ice_ids(in, current);
        }
        case 6:
        {
            return ___ice_isA(in, current);
        }
        case 7:
        {
            return ___ice_ping(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
LbaNet::ConnectedTracker::__write(::IceInternal::BasicStream* __os) const
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
LbaNet::ConnectedTracker::__read(::IceInternal::BasicStream* __is, bool __rid)
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
LbaNet::ConnectedTracker::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type LbaNet::ConnectedTracker was not generated with stream support";
    throw ex;
}

void
LbaNet::ConnectedTracker::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type LbaNet::ConnectedTracker was not generated with stream support";
    throw ex;
}

void 
LbaNet::__patch__ConnectedTrackerPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::LbaNet::ConnectedTrackerPtr* p = static_cast< ::LbaNet::ConnectedTrackerPtr*>(__addr);
    assert(p);
    *p = ::LbaNet::ConnectedTrackerPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::LbaNet::ConnectedTracker::ice_staticId(), v->ice_id());
    }
}

bool
LbaNet::operator==(const ::LbaNet::ConnectedTracker& l, const ::LbaNet::ConnectedTracker& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
LbaNet::operator<(const ::LbaNet::ConnectedTracker& l, const ::LbaNet::ConnectedTracker& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}
