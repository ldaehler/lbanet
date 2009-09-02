// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `RoomManager.ice'

#include <RoomManager.h>
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

static const ::std::string __LbaNet__RoomManager__JoinChat_name = "JoinChat";

static const ::std::string __LbaNet__RoomManager__LeaveChat_name = "LeaveChat";

static const ::std::string __LbaNet__RoomManager__JoinActorRoom_name = "JoinActorRoom";

static const ::std::string __LbaNet__RoomManager__LeaveActorRoom_name = "LeaveActorRoom";

::Ice::Object* IceInternal::upCast(::LbaNet::RoomManager* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::LbaNet::RoomManager* p) { return p; }

void
LbaNet::__read(::IceInternal::BasicStream* __is, ::LbaNet::RoomManagerPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::LbaNet::RoomManager;
        v->__copyFrom(proxy);
    }
}

::LbaNet::ChatRoomObserverPrx
IceProxy::LbaNet::RoomManager::JoinChat(const ::std::string& room, const ::LbaNet::ChatRoomObserverPrx& view, const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__LbaNet__RoomManager__JoinChat_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::LbaNet::RoomManager* __del = dynamic_cast< ::IceDelegate::LbaNet::RoomManager*>(__delBase.get());
            return __del->JoinChat(room, view, __ctx);
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
IceProxy::LbaNet::RoomManager::LeaveChat(const ::std::string& room, const ::LbaNet::ChatRoomObserverPrx& view, const ::Ice::Context* __ctx)
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
            ::IceDelegate::LbaNet::RoomManager* __del = dynamic_cast< ::IceDelegate::LbaNet::RoomManager*>(__delBase.get());
            __del->LeaveChat(room, view, __ctx);
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

::LbaNet::ActorsObserverPrx
IceProxy::LbaNet::RoomManager::JoinActorRoom(const ::std::string& room, const ::std::string& actorName, const ::LbaNet::ActorsObserverPrx& view, const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__LbaNet__RoomManager__JoinActorRoom_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::LbaNet::RoomManager* __del = dynamic_cast< ::IceDelegate::LbaNet::RoomManager*>(__delBase.get());
            return __del->JoinActorRoom(room, actorName, view, __ctx);
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
IceProxy::LbaNet::RoomManager::LeaveActorRoom(const ::std::string& room, const ::LbaNet::ActorsObserverPrx& view, const ::Ice::Context* __ctx)
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
            ::IceDelegate::LbaNet::RoomManager* __del = dynamic_cast< ::IceDelegate::LbaNet::RoomManager*>(__delBase.get());
            __del->LeaveActorRoom(room, view, __ctx);
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
IceProxy::LbaNet::RoomManager::ice_staticId()
{
    return ::LbaNet::RoomManager::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::LbaNet::RoomManager::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::LbaNet::RoomManager);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::LbaNet::RoomManager::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::LbaNet::RoomManager);
}

::IceProxy::Ice::Object*
IceProxy::LbaNet::RoomManager::__newInstance() const
{
    return new RoomManager;
}

::LbaNet::ChatRoomObserverPrx
IceDelegateM::LbaNet::RoomManager::JoinChat(const ::std::string& room, const ::LbaNet::ChatRoomObserverPrx& view, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__RoomManager__JoinChat_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(room);
        __os->write(::Ice::ObjectPrx(::IceInternal::upCast(view.get())));
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    ::LbaNet::ChatRoomObserverPrx __ret;
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
IceDelegateM::LbaNet::RoomManager::LeaveChat(const ::std::string& room, const ::LbaNet::ChatRoomObserverPrx& view, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__RoomManager__LeaveChat_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(room);
        __os->write(::Ice::ObjectPrx(::IceInternal::upCast(view.get())));
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

::LbaNet::ActorsObserverPrx
IceDelegateM::LbaNet::RoomManager::JoinActorRoom(const ::std::string& room, const ::std::string& actorName, const ::LbaNet::ActorsObserverPrx& view, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__RoomManager__JoinActorRoom_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(room);
        __os->write(actorName);
        __os->write(::Ice::ObjectPrx(::IceInternal::upCast(view.get())));
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    ::LbaNet::ActorsObserverPrx __ret;
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
IceDelegateM::LbaNet::RoomManager::LeaveActorRoom(const ::std::string& room, const ::LbaNet::ActorsObserverPrx& view, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__RoomManager__LeaveActorRoom_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(room);
        __os->write(::Ice::ObjectPrx(::IceInternal::upCast(view.get())));
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

::LbaNet::ChatRoomObserverPrx
IceDelegateD::LbaNet::RoomManager::JoinChat(const ::std::string& room, const ::LbaNet::ChatRoomObserverPrx& view, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::LbaNet::ChatRoomObserverPrx& __result, const ::std::string& room, const ::LbaNet::ChatRoomObserverPrx& view, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_room(room),
            _m_view(view)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::LbaNet::RoomManager* servant = dynamic_cast< ::LbaNet::RoomManager*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->JoinChat(_m_room, _m_view, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::LbaNet::ChatRoomObserverPrx& _result;
        const ::std::string& _m_room;
        const ::LbaNet::ChatRoomObserverPrx& _m_view;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __LbaNet__RoomManager__JoinChat_name, ::Ice::Normal, __context);
    ::LbaNet::ChatRoomObserverPrx __result;
    try
    {
        _DirectI __direct(__result, room, view, __current);
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
IceDelegateD::LbaNet::RoomManager::LeaveChat(const ::std::string& room, const ::LbaNet::ChatRoomObserverPrx& view, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::std::string& room, const ::LbaNet::ChatRoomObserverPrx& view, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_room(room),
            _m_view(view)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::LbaNet::RoomManager* servant = dynamic_cast< ::LbaNet::RoomManager*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->LeaveChat(_m_room, _m_view, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::std::string& _m_room;
        const ::LbaNet::ChatRoomObserverPrx& _m_view;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __LbaNet__RoomManager__LeaveChat_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(room, view, __current);
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

::LbaNet::ActorsObserverPrx
IceDelegateD::LbaNet::RoomManager::JoinActorRoom(const ::std::string& room, const ::std::string& actorName, const ::LbaNet::ActorsObserverPrx& view, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::LbaNet::ActorsObserverPrx& __result, const ::std::string& room, const ::std::string& actorName, const ::LbaNet::ActorsObserverPrx& view, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_room(room),
            _m_actorName(actorName),
            _m_view(view)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::LbaNet::RoomManager* servant = dynamic_cast< ::LbaNet::RoomManager*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->JoinActorRoom(_m_room, _m_actorName, _m_view, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::LbaNet::ActorsObserverPrx& _result;
        const ::std::string& _m_room;
        const ::std::string& _m_actorName;
        const ::LbaNet::ActorsObserverPrx& _m_view;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __LbaNet__RoomManager__JoinActorRoom_name, ::Ice::Normal, __context);
    ::LbaNet::ActorsObserverPrx __result;
    try
    {
        _DirectI __direct(__result, room, actorName, view, __current);
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
IceDelegateD::LbaNet::RoomManager::LeaveActorRoom(const ::std::string& room, const ::LbaNet::ActorsObserverPrx& view, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::std::string& room, const ::LbaNet::ActorsObserverPrx& view, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_room(room),
            _m_view(view)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::LbaNet::RoomManager* servant = dynamic_cast< ::LbaNet::RoomManager*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->LeaveActorRoom(_m_room, _m_view, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::std::string& _m_room;
        const ::LbaNet::ActorsObserverPrx& _m_view;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __LbaNet__RoomManager__LeaveActorRoom_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(room, view, __current);
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
LbaNet::RoomManager::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __LbaNet__RoomManager_ids[2] =
{
    "::Ice::Object",
    "::LbaNet::RoomManager"
};

bool
LbaNet::RoomManager::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__LbaNet__RoomManager_ids, __LbaNet__RoomManager_ids + 2, _s);
}

::std::vector< ::std::string>
LbaNet::RoomManager::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__LbaNet__RoomManager_ids[0], &__LbaNet__RoomManager_ids[2]);
}

const ::std::string&
LbaNet::RoomManager::ice_id(const ::Ice::Current&) const
{
    return __LbaNet__RoomManager_ids[1];
}

const ::std::string&
LbaNet::RoomManager::ice_staticId()
{
    return __LbaNet__RoomManager_ids[1];
}

::Ice::DispatchStatus
LbaNet::RoomManager::___JoinChat(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string room;
    ::LbaNet::ChatRoomObserverPrx view;
    __is->read(room);
    ::LbaNet::__read(__is, view);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::LbaNet::ChatRoomObserverPrx __ret = JoinChat(room, view, __current);
    __os->write(::Ice::ObjectPrx(::IceInternal::upCast(__ret.get())));
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
LbaNet::RoomManager::___LeaveChat(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string room;
    ::LbaNet::ChatRoomObserverPrx view;
    __is->read(room);
    ::LbaNet::__read(__is, view);
    __is->endReadEncaps();
    LeaveChat(room, view, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
LbaNet::RoomManager::___JoinActorRoom(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string room;
    ::std::string actorName;
    ::LbaNet::ActorsObserverPrx view;
    __is->read(room);
    __is->read(actorName);
    ::LbaNet::__read(__is, view);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::LbaNet::ActorsObserverPrx __ret = JoinActorRoom(room, actorName, view, __current);
    __os->write(::Ice::ObjectPrx(::IceInternal::upCast(__ret.get())));
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
LbaNet::RoomManager::___LeaveActorRoom(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string room;
    ::LbaNet::ActorsObserverPrx view;
    __is->read(room);
    ::LbaNet::__read(__is, view);
    __is->endReadEncaps();
    LeaveActorRoom(room, view, __current);
    return ::Ice::DispatchOK;
}

static ::std::string __LbaNet__RoomManager_all[] =
{
    "JoinActorRoom",
    "JoinChat",
    "LeaveActorRoom",
    "LeaveChat",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping"
};

::Ice::DispatchStatus
LbaNet::RoomManager::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__LbaNet__RoomManager_all, __LbaNet__RoomManager_all + 8, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __LbaNet__RoomManager_all)
    {
        case 0:
        {
            return ___JoinActorRoom(in, current);
        }
        case 1:
        {
            return ___JoinChat(in, current);
        }
        case 2:
        {
            return ___LeaveActorRoom(in, current);
        }
        case 3:
        {
            return ___LeaveChat(in, current);
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
LbaNet::RoomManager::__write(::IceInternal::BasicStream* __os) const
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
LbaNet::RoomManager::__read(::IceInternal::BasicStream* __is, bool __rid)
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
LbaNet::RoomManager::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type LbaNet::RoomManager was not generated with stream support";
    throw ex;
}

void
LbaNet::RoomManager::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type LbaNet::RoomManager was not generated with stream support";
    throw ex;
}

void 
LbaNet::__patch__RoomManagerPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::LbaNet::RoomManagerPtr* p = static_cast< ::LbaNet::RoomManagerPtr*>(__addr);
    assert(p);
    *p = ::LbaNet::RoomManagerPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::LbaNet::RoomManager::ice_staticId(), v->ice_id());
    }
}

bool
LbaNet::operator==(const ::LbaNet::RoomManager& l, const ::LbaNet::RoomManager& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
LbaNet::operator<(const ::LbaNet::RoomManager& l, const ::LbaNet::RoomManager& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}
