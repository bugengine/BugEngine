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

Threads::Waitable::WaitResult Event::wait(unsigned int timeout)
{
    timespec time;
    time.tv_nsec = timeout % 1000;
    time.tv_sec = timeout / 1000;
    time.tv_sec += time.tv_nsec % 1000000000;
    time.tv_nsec = time.tv_nsec % 1000000000;
    int result = LWP_CondTimedWait( *reinterpret_cast<cond_t*>(m_data),
                                    *reinterpret_cast<mutex_t*>(m_lock),
                                    &time);
    switch(result)
    {
    case 0:
        return Finished;
    case ETIMEDOUT:
        return TimeOut;
    default:
        be_notreached();
        return Abandoned;
    }
}


}
