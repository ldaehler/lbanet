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

#include <ActorInfo.h>
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

static const ::std::string __LbaNet__ActorsObserver__UpdatedInfo_name = "UpdatedInfo";

static const ::std::string __LbaNet__ActorsObserver__Quitted_name = "Quitted";

static const ::std::string __LbaNet__ActorsObserver__ActivatedActor_name = "ActivatedActor";

static const ::std::string __LbaNet__ActorsObserver__SignaledActor_name = "SignaledActor";

static const ::std::string __LbaNet__ActorsParticipant__Update_name = "Update";

::Ice::Object* IceInternal::upCast(::LbaNet::ActorsObserver* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::LbaNet::ActorsObserver* p) { return p; }

::Ice::Object* IceInternal::upCast(::LbaNet::ActorsParticipant* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::LbaNet::ActorsParticipant* p) { return p; }

void
LbaNet::__read(::IceInternal::BasicStream* __is, ::LbaNet::ActorsObserverPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::LbaNet::ActorsObserver;
        v->__copyFrom(proxy);
    }
}

void
LbaNet::__read(::IceInternal::BasicStream* __is, ::LbaNet::ActorsParticipantPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::LbaNet::ActorsParticipant;
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
LbaNet::CurrentAndFutureInfo::operator==(const CurrentAndFutureInfo& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(CurrentState != __rhs.CurrentState)
    {
        return false;
    }
    if(FutureState != __rhs.FutureState)
    {
        return false;
    }
    return true;
}

bool
LbaNet::CurrentAndFutureInfo::operator<(const CurrentAndFutureInfo& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(CurrentState < __rhs.CurrentState)
    {
        return true;
    }
    else if(__rhs.CurrentState < CurrentState)
    {
        return false;
    }
    if(FutureState < __rhs.FutureState)
    {
        return true;
    }
    else if(__rhs.FutureState < FutureState)
    {
        return false;
    }
    return false;
}

void
LbaNet::CurrentAndFutureInfo::__write(::IceInternal::BasicStream* __os) const
{
    CurrentState.__write(__os);
    FutureState.__write(__os);
}

void
LbaNet::CurrentAndFutureInfo::__read(::IceInternal::BasicStream* __is)
{
    CurrentState.__read(__is);
    FutureState.__read(__is);
}

void
IceProxy::LbaNet::ActorsObserver::UpdatedInfo(const ::LbaNet::ActorInfo& asi, const ::Ice::Context* __ctx)
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
            ::IceDelegate::LbaNet::ActorsObserver* __del = dynamic_cast< ::IceDelegate::LbaNet::ActorsObserver*>(__delBase.get());
            __del->UpdatedInfo(asi, __ctx);
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
IceProxy::LbaNet::ActorsObserver::Quitted(const ::std::string& ActorName, const ::Ice::Context* __ctx)
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
            ::IceDelegate::LbaNet::ActorsObserver* __del = dynamic_cast< ::IceDelegate::LbaNet::ActorsObserver*>(__delBase.get());
            __del->Quitted(ActorName, __ctx);
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
IceProxy::LbaNet::ActorsObserver::ActivatedActor(const ::LbaNet::ActorActivationInfo& ai, const ::Ice::Context* __ctx)
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
            ::IceDelegate::LbaNet::ActorsObserver* __del = dynamic_cast< ::IceDelegate::LbaNet::ActorsObserver*>(__delBase.get());
            __del->ActivatedActor(ai, __ctx);
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
IceProxy::LbaNet::ActorsObserver::SignaledActor(const ::LbaNet::ActorSignalInfo& ai, const ::Ice::Context* __ctx)
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
            ::IceDelegate::LbaNet::ActorsObserver* __del = dynamic_cast< ::IceDelegate::LbaNet::ActorsObserver*>(__delBase.get());
            __del->SignaledActor(ai, __ctx);
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
IceProxy::LbaNet::ActorsObserver::ice_staticId()
{
    return ::LbaNet::ActorsObserver::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::LbaNet::ActorsObserver::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::LbaNet::ActorsObserver);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::LbaNet::ActorsObserver::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::LbaNet::ActorsObserver);
}

::IceProxy::Ice::Object*
IceProxy::LbaNet::ActorsObserver::__newInstance() const
{
    return new ActorsObserver;
}

void
IceProxy::LbaNet::ActorsParticipant::Update(const ::LbaNet::ActorInfo& asi, const ::Ice::Context* __ctx)
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
            ::IceDelegate::LbaNet::ActorsParticipant* __del = dynamic_cast< ::IceDelegate::LbaNet::ActorsParticipant*>(__delBase.get());
            __del->Update(asi, __ctx);
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
IceProxy::LbaNet::ActorsParticipant::ice_staticId()
{
    return ::LbaNet::ActorsParticipant::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::LbaNet::ActorsParticipant::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::LbaNet::ActorsParticipant);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::LbaNet::ActorsParticipant::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::LbaNet::ActorsParticipant);
}

::IceProxy::Ice::Object*
IceProxy::LbaNet::ActorsParticipant::__newInstance() const
{
    return new ActorsParticipant;
}

void
IceDelegateM::LbaNet::ActorsObserver::UpdatedInfo(const ::LbaNet::ActorInfo& asi, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__ActorsObserver__UpdatedInfo_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        asi.__write(__os);
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
IceDelegateM::LbaNet::ActorsObserver::Quitted(const ::std::string& ActorName, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__ActorsObserver__Quitted_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(ActorName);
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
IceDelegateM::LbaNet::ActorsObserver::ActivatedActor(const ::LbaNet::ActorActivationInfo& ai, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__ActorsObserver__ActivatedActor_name, ::Ice::Normal, __context);
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
IceDelegateM::LbaNet::ActorsObserver::SignaledActor(const ::LbaNet::ActorSignalInfo& ai, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__ActorsObserver__SignaledActor_name, ::Ice::Normal, __context);
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
IceDelegateM::LbaNet::ActorsParticipant::Update(const ::LbaNet::ActorInfo& asi, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __LbaNet__ActorsParticipant__Update_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        asi.__write(__os);
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
IceDelegateD::LbaNet::ActorsObserver::UpdatedInfo(const ::LbaNet::ActorInfo& asi, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::LbaNet::ActorInfo& asi, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_asi(asi)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::LbaNet::ActorsObserver* servant = dynamic_cast< ::LbaNet::ActorsObserver*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->UpdatedInfo(_m_asi, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::LbaNet::ActorInfo& _m_asi;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __LbaNet__ActorsObserver__UpdatedInfo_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(asi, __current);
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
IceDelegateD::LbaNet::ActorsObserver::Quitted(const ::std::string& ActorName, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::std::string& ActorName, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_ActorName(ActorName)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::LbaNet::ActorsObserver* servant = dynamic_cast< ::LbaNet::ActorsObserver*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->Quitted(_m_ActorName, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::std::string& _m_ActorName;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __LbaNet__ActorsObserver__Quitted_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(ActorName, __current);
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
IceDelegateD::LbaNet::ActorsObserver::ActivatedActor(const ::LbaNet::ActorActivationInfo& ai, const ::Ice::Context* __context)
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
            ::LbaNet::ActorsObserver* servant = dynamic_cast< ::LbaNet::ActorsObserver*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->ActivatedActor(_m_ai, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::LbaNet::ActorActivationInfo& _m_ai;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __LbaNet__ActorsObserver__ActivatedActor_name, ::Ice::Normal, __context);
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
IceDelegateD::LbaNet::ActorsObserver::SignaledActor(const ::LbaNet::ActorSignalInfo& ai, const ::Ice::Context* __context)
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
            ::LbaNet::ActorsObserver* servant = dynamic_cast< ::LbaNet::ActorsObserver*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->SignaledActor(_m_ai, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::LbaNet::ActorSignalInfo& _m_ai;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __LbaNet__ActorsObserver__SignaledActor_name, ::Ice::Normal, __context);
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
IceDelegateD::LbaNet::ActorsParticipant::Update(const ::LbaNet::ActorInfo& asi, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::LbaNet::ActorInfo& asi, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_asi(asi)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::LbaNet::ActorsParticipant* servant = dynamic_cast< ::LbaNet::ActorsParticipant*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->Update(_m_asi, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::LbaNet::ActorInfo& _m_asi;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __LbaNet__ActorsParticipant__Update_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(asi, __current);
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
LbaNet::ActorsObserver::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __LbaNet__ActorsObserver_ids[2] =
{
    "::Ice::Object",
    "::LbaNet::ActorsObserver"
};

bool
LbaNet::ActorsObserver::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__LbaNet__ActorsObserver_ids, __LbaNet__ActorsObserver_ids + 2, _s);
}

::std::vector< ::std::string>
LbaNet::ActorsObserver::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__LbaNet__ActorsObserver_ids[0], &__LbaNet__ActorsObserver_ids[2]);
}

const ::std::string&
LbaNet::ActorsObserver::ice_id(const ::Ice::Current&) const
{
    return __LbaNet__ActorsObserver_ids[1];
}

const ::std::string&
LbaNet::ActorsObserver::ice_staticId()
{
    return __LbaNet__ActorsObserver_ids[1];
}

::Ice::DispatchStatus
LbaNet::ActorsObserver::___UpdatedInfo(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::LbaNet::ActorInfo asi;
    asi.__read(__is);
    __is->endReadEncaps();
    UpdatedInfo(asi, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
LbaNet::ActorsObserver::___Quitted(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string ActorName;
    __is->read(ActorName);
    __is->endReadEncaps();
    Quitted(ActorName, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
LbaNet::ActorsObserver::___ActivatedActor(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::LbaNet::ActorActivationInfo ai;
    ai.__read(__is);
    __is->endReadEncaps();
    ActivatedActor(ai, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
LbaNet::ActorsObserver::___SignaledActor(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::LbaNet::ActorSignalInfo ai;
    ai.__read(__is);
    __is->endReadEncaps();
    SignaledActor(ai, __current);
    return ::Ice::DispatchOK;
}

static ::std::string __LbaNet__ActorsObserver_all[] =
{
    "ActivatedActor",
    "Quitted",
    "SignaledActor",
    "UpdatedInfo",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping"
};

::Ice::DispatchStatus
LbaNet::ActorsObserver::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__LbaNet__ActorsObserver_all, __LbaNet__ActorsObserver_all + 8, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __LbaNet__ActorsObserver_all)
    {
        case 0:
        {
            return ___ActivatedActor(in, current);
        }
        case 1:
        {
            return ___Quitted(in, current);
        }
        case 2:
        {
            return ___SignaledActor(in, current);
        }
        case 3:
        {
            return ___UpdatedInfo(in, current);
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
LbaNet::ActorsObserver::__write(::IceInternal::BasicStream* __os) const
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
LbaNet::ActorsObserver::__read(::IceInternal::BasicStream* __is, bool __rid)
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
LbaNet::ActorsObserver::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type LbaNet::ActorsObserver was not generated with stream support";
    throw ex;
}

void
LbaNet::ActorsObserver::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type LbaNet::ActorsObserver was not generated with stream support";
    throw ex;
}

void 
LbaNet::__patch__ActorsObserverPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::LbaNet::ActorsObserverPtr* p = static_cast< ::LbaNet::ActorsObserverPtr*>(__addr);
    assert(p);
    *p = ::LbaNet::ActorsObserverPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::LbaNet::ActorsObserver::ice_staticId(), v->ice_id());
    }
}

bool
LbaNet::operator==(const ::LbaNet::ActorsObserver& l, const ::LbaNet::ActorsObserver& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
LbaNet::operator<(const ::LbaNet::ActorsObserver& l, const ::LbaNet::ActorsObserver& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

::Ice::ObjectPtr
LbaNet::ActorsParticipant::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __LbaNet__ActorsParticipant_ids[2] =
{
    "::Ice::Object",
    "::LbaNet::ActorsParticipant"
};

bool
LbaNet::ActorsParticipant::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__LbaNet__ActorsParticipant_ids, __LbaNet__ActorsParticipant_ids + 2, _s);
}

::std::vector< ::std::string>
LbaNet::ActorsParticipant::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__LbaNet__ActorsParticipant_ids[0], &__LbaNet__ActorsParticipant_ids[2]);
}

const ::std::string&
LbaNet::ActorsParticipant::ice_id(const ::Ice::Current&) const
{
    return __LbaNet__ActorsParticipant_ids[1];
}

const ::std::string&
LbaNet::ActorsParticipant::ice_staticId()
{
    return __LbaNet__ActorsParticipant_ids[1];
}

::Ice::DispatchStatus
LbaNet::ActorsParticipant::___Update(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::LbaNet::ActorInfo asi;
    asi.__read(__is);
    __is->endReadEncaps();
    Update(asi, __current);
    return ::Ice::DispatchOK;
}

static ::std::string __LbaNet__ActorsParticipant_all[] =
{
    "Update",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping"
};

::Ice::DispatchStatus
LbaNet::ActorsParticipant::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__LbaNet__ActorsParticipant_all, __LbaNet__ActorsParticipant_all + 5, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __LbaNet__ActorsParticipant_all)
    {
        case 0:
        {
            return ___Update(in, current);
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
LbaNet::ActorsParticipant::__write(::IceInternal::BasicStream* __os) const
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
LbaNet::ActorsParticipant::__read(::IceInternal::BasicStream* __is, bool __rid)
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
LbaNet::ActorsParticipant::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type LbaNet::ActorsParticipant was not generated with stream support";
    throw ex;
}

void
LbaNet::ActorsParticipant::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type LbaNet::ActorsParticipant was not generated with stream support";
    throw ex;
}

void 
LbaNet::__patch__ActorsParticipantPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::LbaNet::ActorsParticipantPtr* p = static_cast< ::LbaNet::ActorsParticipantPtr*>(__addr);
    assert(p);
    *p = ::LbaNet::ActorsParticipantPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::LbaNet::ActorsParticipant::ice_staticId(), v->ice_id());
    }
}

bool
LbaNet::operator==(const ::LbaNet::ActorsParticipant& l, const ::LbaNet::ActorsParticipant& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
LbaNet::operator<(const ::LbaNet::ActorsParticipant& l, const ::LbaNet::ActorsParticipant& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}
