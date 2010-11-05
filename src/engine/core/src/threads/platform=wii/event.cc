/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/threads/event.hh>
#include    <ogc/cond.h>
#include    <ogc/mutex.h>
#include    <cerrno>


namespace BugEngine
{

Event::Event()
:   m_data(new cond_t)
,   m_lock(new mutex_t)
{
    LWP_CondInit(reinterpret_cast<cond_t*>(m_data));
    LWP_MutexInit(reinterpret_cast<mutex_t*>(m_lock), false);
}

Event::~Event()
{
    LWP_MutexDestroy(*reinterpret_cast<mutex_t*>(m_lock));
    LWP_CondDestroy(*reinterpret_cast<cond_t*>(m_data));
    delete reinterpret_cast<mutex_t*>(m_lock);
    delete reinterpret_cast<cond_t*>(m_data);
}

void Event::set()
{
    LWP_CondSignal(*reinterpret_cast<cond_t*>(m_data));
}

void Event::pulse()
{
    LWP_CondBroadcast(*reinterpret_cast<cond_t*>(m_data));
}

void Event::lock()
{
    LWP_MutexLock(*reinterpret_cast<mutex_t*>(m_lock));
}


void Event::unlock()
{
    LWP_MutexUnlock(*reinterpret_cast<mutex_t*>(m_lock));
}

Threads::Waitable::WaitResult Event::wait()
{
    int result = LWP_CondWait( *reinterpret_cast<cond_t*>(m_data),
                               *reinterpret_cast<mutex_t*>(m_lock));
    switch(result)
    {
    case 0:
        return Finished;
    default:
        be_notreached();
        return Abandoned;
    }
}


}
