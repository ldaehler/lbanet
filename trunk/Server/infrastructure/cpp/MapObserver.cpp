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

static const ::std::string __LbaNet__MapObserver__SignalActor_name = "SignalActor";

static const ::std::string __LbaNet__MapObserver__GetUpdatedInfo_name = "GetUpdatedInfo";

static const ::std::string __LbaNet__MapObserver__GetPlayersInfo_name = "GetPlayersInfo";

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
LbaNet::ActorInfo::operator==(const ActorInfo& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(Time != __rhs.Time)
    {
        return false;
    }
    if(ActorId != __rhs.ActorId)
    {
        return false;
    }
    if(MapName != __rhs.MapName)
    {
        return false;
    }
    if(Name != __rhs.Name)
    {
        return false;
    }
    if(DisplayName != __rhs.DisplayName)
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
    if(Model != __rhs.Model)
    {
        return false;
    }
    if(Body != __rhs.Body)
    {
        return false;
    }
    if(Animation != __rhs.Animation)
    {
        return false;
    }
    if(BodyColor != __rhs.BodyColor)
    {
        return false;
    }
    if(vX != __rhs.vX)
    {
        return false;
    }
    if(vY != __rhs.vY)
    {
        return false;
    }
    if(vZ != __rhs.vZ)
    {
        return false;
    }
    if(vRotation != __rhs.vRotation)
    {
        return false;
    }
    return true;
}

bool
LbaNet::ActorInfo::operator<(const ActorInfo& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(Time < __rhs.Time)
    {
        return true;
    }
    else if(__rhs.Time < Time)
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
    if(MapName < __rhs.MapName)
    {
        return true;
    }
    else if(__rhs.MapName < MapName)
    {
        return false;
    }
    if(Name < __rhs.Name)
    {
        return true;
    }
    else if(__rhs.Name < Name)
    {
        return false;
    }
    if(DisplayName < __rhs.DisplayName)
    {
        return true;
    }
    else if(__rhs.DisplayName < DisplayName)
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
    if(Model < __rhs.Model)
    {
        return true;
    }
    else if(__rhs.Model < Model)
    {
        return false;
    }
    if(Body < __rhs.Body)
    {
        return true;
    }
    else if(__rhs.Body < Body)
    {
        return false;
    }
    if(Animation < __rhs.Animation)
    {
        return true;
    }
    else if(__rhs.Animation < Animation)
    {
        return false;
    }
    if(BodyColor < __rhs.BodyColor)
    {
        return true;
    }
    else if(__rhs.BodyColor < BodyColor)
    {
        return false;
    }
    if(vX < __rhs.vX)
    {
        return true;
    }
    else if(__rhs.vX < vX)
    {
        return false;
    }
    if(vY < __rhs.vY)
    {
        return true;
    }
    else if(__rhs.vY < vY)
    {
        return false;
    }
    if(vZ < __rhs.vZ)
    {
        return true;
    }
    else if(__rhs.vZ < vZ)
    {
        return false;
    }
    if(vRotation < __rhs.vRotation)
    {
        return true;
    }
    else if(__rhs.vRotation < vRotation)
    {
        return false;
    }
    return false;
}

void
LbaNet::ActorInfo::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(Time);
    __os->write(ActorId);
    __os->write(MapName);
    __os->write(Name);
    __os->write(DisplayName);
    __os->write(X);
    __os->write(Y);
    __os->write(Z);
    __os->write(Rotation);
    __os->write(Model);
    __os->write(Body);
    __os->write(Animation);
    __os->write(BodyColor);
    __os->write(vX);
    __os->write(vY);
    __os->write(vZ);
    __os->write(vRotation);
}

void
LbaNet::ActorInfo::__read(::IceInternal::BasicStream* __is)
{
    __is->read(Time);
    __is->read(ActorId);
    __is->read(MapName);
    __is->read(Name);
    __is->read(DisplayName);
    __is->read(X);
    __is->read(Y);
    __is->read(Z);
    __is->read(Rotation);
    __is->read(Model);
    __is->read(Body);
    __is->read(Animation);
    __is->read(BodyColor);
    __is->read(vX);
    __is->read(vY);
    __is->read(vZ);
    __is->read(vRotation);
}

bool
LbaNet::ActorActivationInfo::operator==(const ActorActivationInfo& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(ActivatedId != __rhs.ActivatedId)
    {
        return false;
    }
    if(Activate != __rhs.Activate)
    {
        return false;
    }
    if(ActorId != __rhs.ActorId)
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
    return true;
}

bool
LbaNet::ActorActivationInfo::operator<(const ActorActivationInfo& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(ActivatedId < __rhs.ActivatedId)
    {
        return true;
    }
    else if(__rhs.ActivatedId < ActivatedId)
    {
        return false;
    }
    if(Activate < __rhs.Activate)
    {
        return true;
    }
    else if(__rhs.Activate < Activate)
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
    return false;
}

void
LbaNet::ActorActivationInfo::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(ActivatedId);
    __os->write(Activate);
    __os->write(ActorId);
    __os->write(X);
    __os->write(Y);
    __os->write(Z);
    __os->write(Rotation);
}

void
LbaNet::ActorActivationInfo::__read(::IceInternal::BasicStream* __is)
{
    __is->read(ActivatedId);
    __is->read(Activate);
    __is->read(ActorId);
    __is->read(X);
    __is->read(Y);
    __is->read(Z);
    __is->read(Rotation);
}

bool
LbaNet::ActorSignalInfo::operator==(const ActorSignalInfo& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(Targets != __rhs.Targets)
    {
        return false;
    }
    if(SignalId != __rhs.SignalId)
    {
        return false;
    }
    if(ActorId != __rhs.ActorId)
    {
        return false;
    }
    return true;
}

bool
LbaNet::ActorSignalInfo::operator<(const ActorSignalInfo& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(Targets < __rhs.Targets)
    {
        return true;
    }
    else if(__rhs.Targets < Targets)
    {
        return false;
    }
    if(SignalId < __rhs.SignalId)
    {
        return true;
    }
    else if(__rhs.SignalId < SignalId)
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
    return false;
}

void
LbaNet::ActorSignalInfo::__write(::IceInternal::BasicStream* __os) const
{
    if(Targets.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        __os->write(&Targets[0], &Targets[0] + Targets.size());
    }
    __os->write(SignalId);
    __os->write(ActorId);
}

void
LbaNet::ActorSignalInfo::__read(::IceInternal::BasicStream* __is)
{
    __is->read(Targets);
    __is->read(SignalId);
    __is->read(ActorId);
}

bool
LbaNet::ActorUpdateInfo::operator==(const ActorUpdateInfo& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(ActorId != __rhs.ActorId)
    {
        return false;
    }
    if(On != __rhs.On)
    {
        return false;
    }
    if(Open != __rhs.Open)
    {
        return false;
    }
    if(Counter != __rhs.Counter)
    {
        return false;
    }
    if(SignalOn != __rhs.SignalOn)
    {
        return false;
    }
    if(CurrentScript != __rhs.CurrentScript)
    {
        return false;
    }
    if(CurrentSignals != __rhs.CurrentSignals)
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
    return true;
}

bool
LbaNet::ActorUpdateInfo::operator<(const ActorUpdateInfo& __rhs) const
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
    if(On < __rhs.On)
    {
        return true;
    }
    else if(__rhs.On < On)
    {
        return false;
    }
    if(Open < __rhs.Open)
    {
        return true;
    }
    else if(__rhs.Open < Open)
    {
        return false;
    }
    if(Counter < __rhs.Counter)
    {
        return true;
    }
    else if(__rhs.Counter < Counter)
    {
        return false;
    }
    if(SignalOn < __rhs.SignalOn)
    {
        return true;
    }
    else if(__rhs.SignalOn < SignalOn)
    {
        return false;
    }
    if(CurrentScript < __rhs.CurrentScript)
    {
        return true;
    }
    else if(__rhs.CurrentScript < CurrentScript)
    {
        return false;
    }
    if(CurrentSignals < __rhs.CurrentSignals)
    {
        return true;
    }
    else if(__rhs.CurrentSignals < CurrentSignals)
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
    return false;
}

void
LbaNet::ActorUpdateInfo::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(ActorId);
    __os->write(On);
    __os->write(Open);
    __os->write(Counter);
    __os->write(SignalOn);
    __os->write(CurrentScript);
    if(CurrentSignals.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        __os->write(&CurrentSignals[0], &CurrentSignals[0] + CurrentSignals.size());
    }
    __os->write(X);
    __os->write(Y);
    __os->write(Z);
    __os->write(Rotation);
}

void
LbaNet::ActorUpdateInfo::__read(::IceInternal::BasicStream* __is)
{
    __is->read(ActorId);
    __is->read(On);
    __is->read(Open);
    __is->read(Counter);
    __is->read(SignalOn);
    __is->read(CurrentScript);
    __is->read(CurrentSignals);
    __is->read(X);
    __is->read(Y);
    __is->read(Z);
    __is->read(Rotation);
}

void
LbaNet::__writeUpdateSeq(::IceInternal::BasicStream* __os, const ::LbaNet::ActorUpdateInfo* begin, const ::LbaNet::ActorUpdateInfo* end)
{
    ::Ice::Int size = static_cast< ::Ice::Int>(end - begin);
    __os->writeSize(size);
    for(int i = 0; i < size; ++i)
    {
        begin[i].__write(__os);
    }
}

void
LbaNet::__readUpdateSeq(::IceInternal::BasicStream* __is, ::LbaNet::UpdateSeq& v)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    __is->startSeq(sz, 40);
    v.resize(sz);
    for(int i = 0; i < sz; ++i)
    {
        v[i].__read(__is);
        __is->checkSeq();
        __is->endElement();
    }
    __is->endSeq(sz);
}

void
LbaNet::__writePlayerSeq(::IceInternal::BasicStream* __os, const ::LbaNet::ActorInfo* begin, const ::LbaNet::ActorInfo* end)
{
    ::Ice::Int size = static_cast< ::Ice::Int>(end - begin);
    __os->writeSize(size);
    for(int i = 0; i < size; ++i)
    {
        begin[i].__write(__os);
    }
}

void
LbaNet::__readPlayerSeq(::IceInternal::BasicStream* __is, ::LbaNet::PlayerSeq& v)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    __is->startSeq(sz, 65);
    v.resize(sz);
    for(int i = 0; i < sz; ++i)
    {
        v[i].__read(__is);
        __is->checkSeq();
        __is->endElement();
    }
    __is->endSeq(sz);
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
IceProxy::LbaNet::MapObserver::SignalActor(const ::LbaNet::ActorSignalInfo& ai, const ::Ice::Context* __ctx)
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
IceProxy::LbaNet::MapObserver::GetUpdatedInfo(const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__LbaNet__MapObserver__GetUpdatedInfo_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::LbaNet::MapObserver* __del = dynamic_cast< ::IceDelegate::LbaNet::MapObserver*>(__delBase.get());
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
IceProxy::LbaNet::MapObserver::GetPlayersInfo(const ::Ice::Context* __ctx)
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
            __checkTwowayOnly(__LbaNet__MapObserver__GetPlayersInfo_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::LbaNet::MapObserver* __del = dynamic_cast< ::IceDelegate::LbaNet::MapObserver*>(__delBase.get());
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
IceDelegateM::LbaNet::MapObserver::SignalActor(const ::LbaNet::ActorSignalInfo& ai, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__MapObserver__SignalActor_name, ::Ice::Normal, __context);
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
IceDelegateM::LbaNet::MapObserver::GetUpdatedInfo(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__MapObserver__GetUpdatedInfo_name, ::Ice::Normal, __context);
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
IceDelegateM::LbaNet::MapObserver::GetPlayersInfo(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__MapObserver__GetPlayersInfo_name, ::Ice::Normal, __context);
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
IceDelegateD::LbaNet::MapObserver::SignalActor(const ::LbaNet::ActorSignalInfo& ai, const ::Ice::Context* __context)
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
            ::LbaNet::MapObserver* servant = dynamic_cast< ::LbaNet::MapObserver*>(object);
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
    __initCurrent(__current, __LbaNet__MapObserver__SignalActor_name, ::Ice::Normal, __context);
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
IceDelegateD::LbaNet::MapObserver::GetUpdatedInfo(const ::Ice::Context* __context)
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
            ::LbaNet::MapObserver* servant = dynamic_cast< ::LbaNet::MapObserver*>(object);
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
    __initCurrent(__current, __LbaNet__MapObserver__GetUpdatedInfo_name, ::Ice::Normal, __context);
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
IceDelegateD::LbaNet::MapObserver::GetPlayersInfo(const ::Ice::Context* __context)
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
            ::LbaNet::MapObserver* servant = dynamic_cast< ::LbaNet::MapObserver*>(object);
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
    __initCurrent(__current, __LbaNet__MapObserver__GetPlayersInfo_name, ::Ice::Normal, __context);
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
LbaNet::MapObserver::___SignalActor(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
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
LbaNet::MapObserver::___GetUpdatedInfo(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
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
LbaNet::MapObserver::___GetPlayersInfo(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
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

static ::std::string __LbaNet__MapObserver_all[] =
{
    "ActivateActor",
    "GetPlayersInfo",
    "GetUpdatedInfo",
    "SignalActor",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping"
};

::Ice::DispatchStatus
LbaNet::MapObserver::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__LbaNet__MapObserver_all, __LbaNet__MapObserver_all + 8, current.operation);
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
            return ___GetPlayersInfo(in, current);
        }
        case 2:
        {
            return ___GetUpdatedInfo(in, current);
        }
        case 3:
        {
            return ___SignalActor(in, current);
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
