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

#include <MapManager.h>
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

static const ::std::string __LbaNet__MapManager__JoinMap_name = "JoinMap";

static const ::std::string __LbaNet__MapManager__LeaveMap_name = "LeaveMap";

::Ice::Object* IceInternal::upCast(::LbaNet::MapManager* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::LbaNet::MapManager* p) { return p; }

void
LbaNet::__read(::IceInternal::BasicStream* __is, ::LbaNet::MapManagerPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::LbaNet::MapManager;
        v->__copyFrom(proxy);
    }
}

::LbaNet::MapObserverPrx
IceProxy::LbaNet::MapManager::JoinMap(const ::std::string& mapName, ::Ice::Long PlayerId, const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__LbaNet__MapManager__JoinMap_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::LbaNet::MapManager* __del = dynamic_cast< ::IceDelegate::LbaNet::MapManager*>(__delBase.get());
            return __del->JoinMap(mapName, PlayerId, __ctx);
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
IceProxy::LbaNet::MapManager::LeaveMap(const ::std::string& mapName, ::Ice::Long PlayerId, const ::Ice::Context* __ctx)
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
            ::IceDelegate::LbaNet::MapManager* __del = dynamic_cast< ::IceDelegate::LbaNet::MapManager*>(__delBase.get());
            __del->LeaveMap(mapName, PlayerId, __ctx);
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
IceProxy::LbaNet::MapManager::ice_staticId()
{
    return ::LbaNet::MapManager::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::LbaNet::MapManager::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::LbaNet::MapManager);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::LbaNet::MapManager::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::LbaNet::MapManager);
}

::IceProxy::Ice::Object*
IceProxy::LbaNet::MapManager::__newInstance() const
{
    return new MapManager;
}

::LbaNet::MapObserverPrx
IceDelegateM::LbaNet::MapManager::JoinMap(const ::std::string& mapName, ::Ice::Long PlayerId, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__MapManager__JoinMap_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(mapName);
        __os->write(PlayerId);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    ::LbaNet::MapObserverPrx __ret;
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
        ::LbaNet::__read(__is, __ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::LbaNet::MapManager::LeaveMap(const ::std::string& mapName, ::Ice::Long PlayerId, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__MapManager__LeaveMap_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(mapName);
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

::LbaNet::MapObserverPrx
IceDelegateD::LbaNet::MapManager::JoinMap(const ::std::string& mapName, ::Ice::Long PlayerId, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::LbaNet::MapObserverPrx& __result, const ::std::string& mapName, ::Ice::Long PlayerId, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_mapName(mapName),
            _m_PlayerId(PlayerId)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::LbaNet::MapManager* servant = dynamic_cast< ::LbaNet::MapManager*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->JoinMap(_m_mapName, _m_PlayerId, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::LbaNet::MapObserverPrx& _result;
        const ::std::string& _m_mapName;
        ::Ice::Long _m_PlayerId;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __LbaNet__MapManager__JoinMap_name, ::Ice::Normal, __context);
    ::LbaNet::MapObserverPrx __result;
    try
    {
        _DirectI __direct(__result, mapName, PlayerId, __current);
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
IceDelegateD::LbaNet::MapManager::LeaveMap(const ::std::string& mapName, ::Ice::Long PlayerId, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::std::string& mapName, ::Ice::Long PlayerId, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_mapName(mapName),
            _m_PlayerId(PlayerId)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::LbaNet::MapManager* servant = dynamic_cast< ::LbaNet::MapManager*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->LeaveMap(_m_mapName, _m_PlayerId, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::std::string& _m_mapName;
        ::Ice::Long _m_PlayerId;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __LbaNet__MapManager__LeaveMap_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(mapName, PlayerId, __current);
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
LbaNet::MapManager::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __LbaNet__MapManager_ids[2] =
{
    "::Ice::Object",
    "::LbaNet::MapManager"
};

bool
LbaNet::MapManager::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__LbaNet__MapManager_ids, __LbaNet__MapManager_ids + 2, _s);
}

::std::vector< ::std::string>
LbaNet::MapManager::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__LbaNet__MapManager_ids[0], &__LbaNet__MapManager_ids[2]);
}

const ::std::string&
LbaNet::MapManager::ice_id(const ::Ice::Current&) const
{
    return __LbaNet__MapManager_ids[1];
}

const ::std::string&
LbaNet::MapManager::ice_staticId()
{
    return __LbaNet__MapManager_ids[1];
}

::Ice::DispatchStatus
LbaNet::MapManager::___JoinMap(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string mapName;
    ::Ice::Long PlayerId;
    __is->read(mapName);
    __is->read(PlayerId);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::LbaNet::MapObserverPrx __ret = JoinMap(mapName, PlayerId, __current);
    __os->write(::Ice::ObjectPrx(::IceInternal::upCast(__ret.get())));
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
LbaNet::MapManager::___LeaveMap(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string mapName;
    ::Ice::Long PlayerId;
    __is->read(mapName);
    __is->read(PlayerId);
    __is->endReadEncaps();
    LeaveMap(mapName, PlayerId, __current);
    return ::Ice::DispatchOK;
}

static ::std::string __LbaNet__MapManager_all[] =
{
    "JoinMap",
    "LeaveMap",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping"
};

::Ice::DispatchStatus
LbaNet::MapManager::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__LbaNet__MapManager_all, __LbaNet__MapManager_all + 6, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __LbaNet__MapManager_all)
    {
        case 0:
        {
            return ___JoinMap(in, current);
        }
        case 1:
        {
            return ___LeaveMap(in, current);
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
LbaNet::MapManager::__write(::IceInternal::BasicStream* __os) const
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
LbaNet::MapManager::__read(::IceInternal::BasicStream* __is, bool __rid)
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
LbaNet::MapManager::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type LbaNet::MapManager was not generated with stream support";
    throw ex;
}

void
LbaNet::MapManager::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type LbaNet::MapManager was not generated with stream support";
    throw ex;
}

void 
LbaNet::__patch__MapManagerPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::LbaNet::MapManagerPtr* p = static_cast< ::LbaNet::MapManagerPtr*>(__addr);
    assert(p);
    *p = ::LbaNet::MapManagerPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::LbaNet::MapManager::ice_staticId(), v->ice_id());
    }
}

bool
LbaNet::operator==(const ::LbaNet::MapManager& l, const ::LbaNet::MapManager& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
LbaNet::operator<(const ::LbaNet::MapManager& l, const ::LbaNet::MapManager& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}
