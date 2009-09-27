// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `ClientSession.ice'

#include <ClientSession.h>
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

static const ::std::string __LbaNet__ClientSession__JoinChat_name = "JoinChat";

static const ::std::string __LbaNet__ClientSession__LeaveChat_name = "LeaveChat";

static const ::std::string __LbaNet__ClientSession__ChangeRoom_name = "ChangeRoom";

static const ::std::string __LbaNet__ClientSession__GetConnected_name = "GetConnected";

static const ::std::string __LbaNet__ClientSession__ActivateActor_name = "ActivateActor";

static const ::std::string __LbaNet__ClientSession__SignalActor_name = "SignalActor";

static const ::std::string __LbaNet__ClientSession__GetUpdatedInfo_name = "GetUpdatedInfo";

static const ::std::string __LbaNet__ClientSession__GetPlayersInfo_name = "GetPlayersInfo";

static const ::std::string __LbaNet__ClientSession__ChangeStatus_name = "ChangeStatus";

static const ::std::string __LbaNet__ClientSession__GetTime_name = "GetTime";

static const ::std::string __LbaNet__ClientSession__GetVersion_name = "GetVersion";

static const ::std::string __LbaNet__ClientSession__GetLifeInfo_name = "GetLifeInfo";

static const ::std::string __LbaNet__ClientSession__GotHurtByActor_name = "GotHurtByActor";

static const ::std::string __LbaNet__ClientSession__GotHurtByFalling_name = "GotHurtByFalling";

static const ::std::string __LbaNet__ClientSession__PlayerRaisedFromDead_name = "PlayerRaisedFromDead";

::Ice::Object* IceInternal::upCast(::LbaNet::ClientSession* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::LbaNet::ClientSession* p) { return p; }

void
LbaNet::__read(::IceInternal::BasicStream* __is, ::LbaNet::ClientSessionPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::LbaNet::ClientSession;
        v->__copyFrom(proxy);
    }
}

::LbaNet::ChatRoomParticipantPrx
IceProxy::LbaNet::ClientSession::JoinChat(const ::std::string& room, const ::LbaNet::ChatRoomObserverPrx& view, const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__LbaNet__ClientSession__JoinChat_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::LbaNet::ClientSession* __del = dynamic_cast< ::IceDelegate::LbaNet::ClientSession*>(__delBase.get());
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
IceProxy::LbaNet::ClientSession::LeaveChat(const ::std::string& room, const ::LbaNet::ChatRoomObserverPrx& view, const ::Ice::Context* __ctx)
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
            ::IceDelegate::LbaNet::ClientSession* __del = dynamic_cast< ::IceDelegate::LbaNet::ClientSession*>(__delBase.get());
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

::LbaNet::ActorsParticipantPrx
IceProxy::LbaNet::ClientSession::ChangeRoom(const ::std::string& newroom, const ::std::string& actorname, const ::LbaNet::ActorsObserverPrx& view, const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__LbaNet__ClientSession__ChangeRoom_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::LbaNet::ClientSession* __del = dynamic_cast< ::IceDelegate::LbaNet::ClientSession*>(__delBase.get());
            return __del->ChangeRoom(newroom, actorname, view, __ctx);
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
IceProxy::LbaNet::ClientSession::GetConnected(::Ice::Long& ownid, const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__LbaNet__ClientSession__GetConnected_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::LbaNet::ClientSession* __del = dynamic_cast< ::IceDelegate::LbaNet::ClientSession*>(__delBase.get());
            return __del->GetConnected(ownid, __ctx);
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
IceProxy::LbaNet::ClientSession::ActivateActor(const ::LbaNet::ActorActivationInfo& ai, const ::Ice::Context* __ctx)
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
            ::IceDelegate::LbaNet::ClientSession* __del = dynamic_cast< ::IceDelegate::LbaNet::ClientSession*>(__delBase.get());
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
IceProxy::LbaNet::ClientSession::SignalActor(const ::LbaNet::ActorSignalInfo& ai, const ::Ice::Context* __ctx)
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
            ::IceDelegate::LbaNet::ClientSession* __del = dynamic_cast< ::IceDelegate::LbaNet::ClientSession*>(__delBase.get());
            __del->SignalActor(ai, __ctx);
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

::LbaNet::UpdateSeq
IceProxy::LbaNet::ClientSession::GetUpdatedInfo(const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__LbaNet__ClientSession__GetUpdatedInfo_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::LbaNet::ClientSession* __del = dynamic_cast< ::IceDelegate::LbaNet::ClientSession*>(__delBase.get());
            return __del->GetUpdatedInfo(__ctx);
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

::LbaNet::PlayerSeq
IceProxy::LbaNet::ClientSession::GetPlayersInfo(const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__LbaNet__ClientSession__GetPlayersInfo_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::LbaNet::ClientSession* __del = dynamic_cast< ::IceDelegate::LbaNet::ClientSession*>(__delBase.get());
            return __del->GetPlayersInfo(__ctx);
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
IceProxy::LbaNet::ClientSession::ChangeStatus(const ::std::string& Status, const ::Ice::Context* __ctx)
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
            ::IceDelegate::LbaNet::ClientSession* __del = dynamic_cast< ::IceDelegate::LbaNet::ClientSession*>(__delBase.get());
            __del->ChangeStatus(Status, __ctx);
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

::Ice::Long
IceProxy::LbaNet::ClientSession::GetTime(const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__LbaNet__ClientSession__GetTime_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::LbaNet::ClientSession* __del = dynamic_cast< ::IceDelegate::LbaNet::ClientSession*>(__delBase.get());
            return __del->GetTime(__ctx);
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

::std::string
IceProxy::LbaNet::ClientSession::GetVersion(const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__LbaNet__ClientSession__GetVersion_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::LbaNet::ClientSession* __del = dynamic_cast< ::IceDelegate::LbaNet::ClientSession*>(__delBase.get());
            return __del->GetVersion(__ctx);
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

::LbaNet::ActorLifeInfo
IceProxy::LbaNet::ClientSession::GetLifeInfo(const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__LbaNet__ClientSession__GetLifeInfo_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::LbaNet::ClientSession* __del = dynamic_cast< ::IceDelegate::LbaNet::ClientSession*>(__delBase.get());
            return __del->GetLifeInfo(__ctx);
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
IceProxy::LbaNet::ClientSession::GotHurtByActor(::Ice::Long HurtingActorId, const ::Ice::Context* __ctx)
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
            ::IceDelegate::LbaNet::ClientSession* __del = dynamic_cast< ::IceDelegate::LbaNet::ClientSession*>(__delBase.get());
            __del->GotHurtByActor(HurtingActorId, __ctx);
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
IceProxy::LbaNet::ClientSession::GotHurtByFalling(::Ice::Float FallingDistance, const ::Ice::Context* __ctx)
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
            ::IceDelegate::LbaNet::ClientSession* __del = dynamic_cast< ::IceDelegate::LbaNet::ClientSession*>(__delBase.get());
            __del->GotHurtByFalling(FallingDistance, __ctx);
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
IceProxy::LbaNet::ClientSession::PlayerRaisedFromDead(const ::Ice::Context* __ctx)
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
            ::IceDelegate::LbaNet::ClientSession* __del = dynamic_cast< ::IceDelegate::LbaNet::ClientSession*>(__delBase.get());
            __del->PlayerRaisedFromDead(__ctx);
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
IceProxy::LbaNet::ClientSession::ice_staticId()
{
    return ::LbaNet::ClientSession::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::LbaNet::ClientSession::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::LbaNet::ClientSession);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::LbaNet::ClientSession::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::LbaNet::ClientSession);
}

::IceProxy::Ice::Object*
IceProxy::LbaNet::ClientSession::__newInstance() const
{
    return new ClientSession;
}

::LbaNet::ChatRoomParticipantPrx
IceDelegateM::LbaNet::ClientSession::JoinChat(const ::std::string& room, const ::LbaNet::ChatRoomObserverPrx& view, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__ClientSession__JoinChat_name, ::Ice::Normal, __context);
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
    ::LbaNet::ChatRoomParticipantPrx __ret;
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
IceDelegateM::LbaNet::ClientSession::LeaveChat(const ::std::string& room, const ::LbaNet::ChatRoomObserverPrx& view, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__ClientSession__LeaveChat_name, ::Ice::Normal, __context);
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

::LbaNet::ActorsParticipantPrx
IceDelegateM::LbaNet::ClientSession::ChangeRoom(const ::std::string& newroom, const ::std::string& actorname, const ::LbaNet::ActorsObserverPrx& view, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__ClientSession__ChangeRoom_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(newroom);
        __os->write(actorname);
        __os->write(::Ice::ObjectPrx(::IceInternal::upCast(view.get())));
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    ::LbaNet::ActorsParticipantPrx __ret;
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

::LbaNet::ConnectedL
IceDelegateM::LbaNet::ClientSession::GetConnected(::Ice::Long& ownid, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__ClientSession__GetConnected_name, ::Ice::Normal, __context);
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
        __is->read(ownid);
        ::LbaNet::__readConnectedL(__is, __ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::LbaNet::ClientSession::ActivateActor(const ::LbaNet::ActorActivationInfo& ai, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__ClientSession__ActivateActor_name, ::Ice::Normal, __context);
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
IceDelegateM::LbaNet::ClientSession::SignalActor(const ::LbaNet::ActorSignalInfo& ai, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__ClientSession__SignalActor_name, ::Ice::Normal, __context);
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

::LbaNet::UpdateSeq
IceDelegateM::LbaNet::ClientSession::GetUpdatedInfo(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__ClientSession__GetUpdatedInfo_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    ::LbaNet::UpdateSeq __ret;
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
        ::LbaNet::__readUpdateSeq(__is, __ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::LbaNet::PlayerSeq
IceDelegateM::LbaNet::ClientSession::GetPlayersInfo(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__ClientSession__GetPlayersInfo_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    ::LbaNet::PlayerSeq __ret;
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
        ::LbaNet::__readPlayerSeq(__is, __ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::LbaNet::ClientSession::ChangeStatus(const ::std::string& Status, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__ClientSession__ChangeStatus_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(Status);
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

::Ice::Long
IceDelegateM::LbaNet::ClientSession::GetTime(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__ClientSession__GetTime_name, ::Ice::Normal, __context);
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

::std::string
IceDelegateM::LbaNet::ClientSession::GetVersion(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__ClientSession__GetVersion_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    ::std::string __ret;
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

::LbaNet::ActorLifeInfo
IceDelegateM::LbaNet::ClientSession::GetLifeInfo(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__ClientSession__GetLifeInfo_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    ::LbaNet::ActorLifeInfo __ret;
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
        __ret.__read(__is);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::LbaNet::ClientSession::GotHurtByActor(::Ice::Long HurtingActorId, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__ClientSession__GotHurtByActor_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(HurtingActorId);
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
IceDelegateM::LbaNet::ClientSession::GotHurtByFalling(::Ice::Float FallingDistance, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__ClientSession__GotHurtByFalling_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(FallingDistance);
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
IceDelegateM::LbaNet::ClientSession::PlayerRaisedFromDead(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__ClientSession__PlayerRaisedFromDead_name, ::Ice::Normal, __context);
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

::LbaNet::ChatRoomParticipantPrx
IceDelegateD::LbaNet::ClientSession::JoinChat(const ::std::string& room, const ::LbaNet::ChatRoomObserverPrx& view, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::LbaNet::ChatRoomParticipantPrx& __result, const ::std::string& room, const ::LbaNet::ChatRoomObserverPrx& view, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_room(room),
            _m_view(view)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::LbaNet::ClientSession* servant = dynamic_cast< ::LbaNet::ClientSession*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->JoinChat(_m_room, _m_view, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::LbaNet::ChatRoomParticipantPrx& _result;
        const ::std::string& _m_room;
        const ::LbaNet::ChatRoomObserverPrx& _m_view;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __LbaNet__ClientSession__JoinChat_name, ::Ice::Normal, __context);
    ::LbaNet::ChatRoomParticipantPrx __result;
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
IceDelegateD::LbaNet::ClientSession::LeaveChat(const ::std::string& room, const ::LbaNet::ChatRoomObserverPrx& view, const ::Ice::Context* __context)
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
            ::LbaNet::ClientSession* servant = dynamic_cast< ::LbaNet::ClientSession*>(object);
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
    __initCurrent(__current, __LbaNet__ClientSession__LeaveChat_name, ::Ice::Normal, __context);
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

::LbaNet::ActorsParticipantPrx
IceDelegateD::LbaNet::ClientSession::ChangeRoom(const ::std::string& newroom, const ::std::string& actorname, const ::LbaNet::ActorsObserverPrx& view, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::LbaNet::ActorsParticipantPrx& __result, const ::std::string& newroom, const ::std::string& actorname, const ::LbaNet::ActorsObserverPrx& view, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_newroom(newroom),
            _m_actorname(actorname),
            _m_view(view)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::LbaNet::ClientSession* servant = dynamic_cast< ::LbaNet::ClientSession*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->ChangeRoom(_m_newroom, _m_actorname, _m_view, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::LbaNet::ActorsParticipantPrx& _result;
        const ::std::string& _m_newroom;
        const ::std::string& _m_actorname;
        const ::LbaNet::ActorsObserverPrx& _m_view;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __LbaNet__ClientSession__ChangeRoom_name, ::Ice::Normal, __context);
    ::LbaNet::ActorsParticipantPrx __result;
    try
    {
        _DirectI __direct(__result, newroom, actorname, view, __current);
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

::LbaNet::ConnectedL
IceDelegateD::LbaNet::ClientSession::GetConnected(::Ice::Long& ownid, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::LbaNet::ConnectedL& __result, ::Ice::Long& ownid, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_ownid(ownid)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::LbaNet::ClientSession* servant = dynamic_cast< ::LbaNet::ClientSession*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->GetConnected(_m_ownid, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::LbaNet::ConnectedL& _result;
        ::Ice::Long& _m_ownid;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __LbaNet__ClientSession__GetConnected_name, ::Ice::Normal, __context);
    ::LbaNet::ConnectedL __result;
    try
    {
        _DirectI __direct(__result, ownid, __current);
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
IceDelegateD::LbaNet::ClientSession::ActivateActor(const ::LbaNet::ActorActivationInfo& ai, const ::Ice::Context* __context)
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
            ::LbaNet::ClientSession* servant = dynamic_cast< ::LbaNet::ClientSession*>(object);
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
    __initCurrent(__current, __LbaNet__ClientSession__ActivateActor_name, ::Ice::Normal, __context);
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
IceDelegateD::LbaNet::ClientSession::SignalActor(const ::LbaNet::ActorSignalInfo& ai, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::LbaNet::ActorSignalInfo& ai, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_ai(ai)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::LbaNet::ClientSession* servant = dynamic_cast< ::LbaNet::ClientSession*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->SignalActor(_m_ai, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::LbaNet::ActorSignalInfo& _m_ai;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __LbaNet__ClientSession__SignalActor_name, ::Ice::Normal, __context);
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

::LbaNet::UpdateSeq
IceDelegateD::LbaNet::ClientSession::GetUpdatedInfo(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::LbaNet::UpdateSeq& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::LbaNet::ClientSession* servant = dynamic_cast< ::LbaNet::ClientSession*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->GetUpdatedInfo(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::LbaNet::UpdateSeq& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __LbaNet__ClientSession__GetUpdatedInfo_name, ::Ice::Normal, __context);
    ::LbaNet::UpdateSeq __result;
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

::LbaNet::PlayerSeq
IceDelegateD::LbaNet::ClientSession::GetPlayersInfo(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::LbaNet::PlayerSeq& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::LbaNet::ClientSession* servant = dynamic_cast< ::LbaNet::ClientSession*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->GetPlayersInfo(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::LbaNet::PlayerSeq& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __LbaNet__ClientSession__GetPlayersInfo_name, ::Ice::Normal, __context);
    ::LbaNet::PlayerSeq __result;
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

void
IceDelegateD::LbaNet::ClientSession::ChangeStatus(const ::std::string& Status, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::std::string& Status, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_Status(Status)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::LbaNet::ClientSession* servant = dynamic_cast< ::LbaNet::ClientSession*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->ChangeStatus(_m_Status, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::std::string& _m_Status;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __LbaNet__ClientSession__ChangeStatus_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(Status, __current);
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

::Ice::Long
IceDelegateD::LbaNet::ClientSession::GetTime(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::Long& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::LbaNet::ClientSession* servant = dynamic_cast< ::LbaNet::ClientSession*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->GetTime(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::Ice::Long& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __LbaNet__ClientSession__GetTime_name, ::Ice::Normal, __context);
    ::Ice::Long __result;
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

::std::string
IceDelegateD::LbaNet::ClientSession::GetVersion(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::std::string& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::LbaNet::ClientSession* servant = dynamic_cast< ::LbaNet::ClientSession*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->GetVersion(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::std::string& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __LbaNet__ClientSession__GetVersion_name, ::Ice::Normal, __context);
    ::std::string __result;
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

::LbaNet::ActorLifeInfo
IceDelegateD::LbaNet::ClientSession::GetLifeInfo(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::LbaNet::ActorLifeInfo& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::LbaNet::ClientSession* servant = dynamic_cast< ::LbaNet::ClientSession*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->GetLifeInfo(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::LbaNet::ActorLifeInfo& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __LbaNet__ClientSession__GetLifeInfo_name, ::Ice::Normal, __context);
    ::LbaNet::ActorLifeInfo __result;
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

void
IceDelegateD::LbaNet::ClientSession::GotHurtByActor(::Ice::Long HurtingActorId, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::Long HurtingActorId, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_HurtingActorId(HurtingActorId)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::LbaNet::ClientSession* servant = dynamic_cast< ::LbaNet::ClientSession*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->GotHurtByActor(_m_HurtingActorId, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::Ice::Long _m_HurtingActorId;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __LbaNet__ClientSession__GotHurtByActor_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(HurtingActorId, __current);
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
IceDelegateD::LbaNet::ClientSession::GotHurtByFalling(::Ice::Float FallingDistance, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::Float FallingDistance, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_FallingDistance(FallingDistance)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::LbaNet::ClientSession* servant = dynamic_cast< ::LbaNet::ClientSession*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->GotHurtByFalling(_m_FallingDistance, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::Ice::Float _m_FallingDistance;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __LbaNet__ClientSession__GotHurtByFalling_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(FallingDistance, __current);
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
IceDelegateD::LbaNet::ClientSession::PlayerRaisedFromDead(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::LbaNet::ClientSession* servant = dynamic_cast< ::LbaNet::ClientSession*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->PlayerRaisedFromDead(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __LbaNet__ClientSession__PlayerRaisedFromDead_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(__current);
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
LbaNet::ClientSession::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __LbaNet__ClientSession_ids[3] =
{
    "::Glacier2::Session",
    "::Ice::Object",
    "::LbaNet::ClientSession"
};

bool
LbaNet::ClientSession::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__LbaNet__ClientSession_ids, __LbaNet__ClientSession_ids + 3, _s);
}

::std::vector< ::std::string>
LbaNet::ClientSession::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__LbaNet__ClientSession_ids[0], &__LbaNet__ClientSession_ids[3]);
}

const ::std::string&
LbaNet::ClientSession::ice_id(const ::Ice::Current&) const
{
    return __LbaNet__ClientSession_ids[2];
}

const ::std::string&
LbaNet::ClientSession::ice_staticId()
{
    return __LbaNet__ClientSession_ids[2];
}

::Ice::DispatchStatus
LbaNet::ClientSession::___JoinChat(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
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
    ::LbaNet::ChatRoomParticipantPrx __ret = JoinChat(room, view, __current);
    __os->write(::Ice::ObjectPrx(::IceInternal::upCast(__ret.get())));
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
LbaNet::ClientSession::___LeaveChat(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
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
LbaNet::ClientSession::___ChangeRoom(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string newroom;
    ::std::string actorname;
    ::LbaNet::ActorsObserverPrx view;
    __is->read(newroom);
    __is->read(actorname);
    ::LbaNet::__read(__is, view);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::LbaNet::ActorsParticipantPrx __ret = ChangeRoom(newroom, actorname, view, __current);
    __os->write(::Ice::ObjectPrx(::IceInternal::upCast(__ret.get())));
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
LbaNet::ClientSession::___GetConnected(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::Ice::Long ownid;
    ::LbaNet::ConnectedL __ret = GetConnected(ownid, __current);
    __os->write(ownid);
    ::LbaNet::__writeConnectedL(__os, __ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
LbaNet::ClientSession::___ActivateActor(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
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
LbaNet::ClientSession::___SignalActor(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::LbaNet::ActorSignalInfo ai;
    ai.__read(__is);
    __is->endReadEncaps();
    SignalActor(ai, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
LbaNet::ClientSession::___GetUpdatedInfo(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::LbaNet::UpdateSeq __ret = GetUpdatedInfo(__current);
    if(__ret.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        ::LbaNet::__writeUpdateSeq(__os, &__ret[0], &__ret[0] + __ret.size());
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
LbaNet::ClientSession::___GetPlayersInfo(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::LbaNet::PlayerSeq __ret = GetPlayersInfo(__current);
    if(__ret.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        ::LbaNet::__writePlayerSeq(__os, &__ret[0], &__ret[0] + __ret.size());
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
LbaNet::ClientSession::___ChangeStatus(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string Status;
    __is->read(Status);
    __is->endReadEncaps();
    ChangeStatus(Status, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
LbaNet::ClientSession::___GetTime(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::Ice::Long __ret = GetTime(__current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
LbaNet::ClientSession::___GetVersion(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string __ret = GetVersion(__current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
LbaNet::ClientSession::___GetLifeInfo(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::LbaNet::ActorLifeInfo __ret = GetLifeInfo(__current);
    __ret.__write(__os);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
LbaNet::ClientSession::___GotHurtByActor(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::Ice::Long HurtingActorId;
    __is->read(HurtingActorId);
    __is->endReadEncaps();
    GotHurtByActor(HurtingActorId, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
LbaNet::ClientSession::___GotHurtByFalling(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::Ice::Float FallingDistance;
    __is->read(FallingDistance);
    __is->endReadEncaps();
    GotHurtByFalling(FallingDistance, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
LbaNet::ClientSession::___PlayerRaisedFromDead(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    PlayerRaisedFromDead(__current);
    return ::Ice::DispatchOK;
}

static ::std::string __LbaNet__ClientSession_all[] =
{
    "ActivateActor",
    "ChangeRoom",
    "ChangeStatus",
    "GetConnected",
    "GetLifeInfo",
    "GetPlayersInfo",
    "GetTime",
    "GetUpdatedInfo",
    "GetVersion",
    "GotHurtByActor",
    "GotHurtByFalling",
    "JoinChat",
    "LeaveChat",
    "PlayerRaisedFromDead",
    "SignalActor",
    "destroy",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping"
};

::Ice::DispatchStatus
LbaNet::ClientSession::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__LbaNet__ClientSession_all, __LbaNet__ClientSession_all + 20, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __LbaNet__ClientSession_all)
    {
        case 0:
        {
            return ___ActivateActor(in, current);
        }
        case 1:
        {
            return ___ChangeRoom(in, current);
        }
        case 2:
        {
            return ___ChangeStatus(in, current);
        }
        case 3:
        {
            return ___GetConnected(in, current);
        }
        case 4:
        {
            return ___GetLifeInfo(in, current);
        }
        case 5:
        {
            return ___GetPlayersInfo(in, current);
        }
        case 6:
        {
            return ___GetTime(in, current);
        }
        case 7:
        {
            return ___GetUpdatedInfo(in, current);
        }
        case 8:
        {
            return ___GetVersion(in, current);
        }
        case 9:
        {
            return ___GotHurtByActor(in, current);
        }
        case 10:
        {
            return ___GotHurtByFalling(in, current);
        }
        case 11:
        {
            return ___JoinChat(in, current);
        }
        case 12:
        {
            return ___LeaveChat(in, current);
        }
        case 13:
        {
            return ___PlayerRaisedFromDead(in, current);
        }
        case 14:
        {
            return ___SignalActor(in, current);
        }
        case 15:
        {
            return ___destroy(in, current);
        }
        case 16:
        {
            return ___ice_id(in, current);
        }
        case 17:
        {
            return ___ice_ids(in, current);
        }
        case 18:
        {
            return ___ice_isA(in, current);
        }
        case 19:
        {
            return ___ice_ping(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
LbaNet::ClientSession::__write(::IceInternal::BasicStream* __os) const
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
LbaNet::ClientSession::__read(::IceInternal::BasicStream* __is, bool __rid)
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
LbaNet::ClientSession::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type LbaNet::ClientSession was not generated with stream support";
    throw ex;
}

void
LbaNet::ClientSession::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type LbaNet::ClientSession was not generated with stream support";
    throw ex;
}

void 
LbaNet::__patch__ClientSessionPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::LbaNet::ClientSessionPtr* p = static_cast< ::LbaNet::ClientSessionPtr*>(__addr);
    assert(p);
    *p = ::LbaNet::ClientSessionPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::LbaNet::ClientSession::ice_staticId(), v->ice_id());
    }
}

bool
LbaNet::operator==(const ::LbaNet::ClientSession& l, const ::LbaNet::ClientSession& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
LbaNet::operator<(const ::LbaNet::ClientSession& l, const ::LbaNet::ClientSession& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}
