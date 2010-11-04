/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/threads/event.hh>
#include    <core/timer.hh>
#include    <cerrno>


namespace BugEngine
{

Event::Event()
:   m_data(new pthread_cond_t)
,   m_lock(new pthread_mutex_t)
{
    pthread_cond_init(reinterpret_cast<pthread_cond_t*>(m_data), 0);
    pthread_mutex_init(reinterpret_cast<pthread_mutex_t*>(m_lock), 0);
}

Event::~Event()
{
    pthread_mutex_destroy(reinterpret_cast<pthread_mutex_t*>(m_lock));
    pthread_cond_destroy(reinterpret_cast<pthread_cond_t*>(m_data));
    delete reinterpret_cast<pthread_mutex_t*>(m_lock);
    delete reinterpret_cast<pthread_cond_t*>(m_data);
}

void Event::set()
{
    pthread_cond_signal(reinterpret_cast<pthread_cond_t*>(m_data));
}

void Event::pulse()
{
    pthread_cond_broadcast(reinterpret_cast<pthread_cond_t*>(m_data));
}

void Event::lock()
{
    pthread_mutex_lock(reinterpret_cast<pthread_mutex_t*>(m_lock));
}


void Event::unlock()
{
    pthread_mutex_unlock(reinterpret_cast<pthread_mutex_t*>(m_lock));
}

Threads::Waitable::WaitResult Event::wait(unsigned int timeout)
{
    timespec abstime;
    clock_gettime(CLOCK_REALTIME, &abstime);
    abstime.tv_nsec += timeout % 1000;
    abstime.tv_sec += timeout / 1000;
    abstime.tv_sec += abstime.tv_nsec % 1000000000;
    abstime.tv_nsec = abstime.tv_nsec % 1000000000;
    int result = pthread_cond_timedwait( reinterpret_cast<pthread_cond_t*>(m_data),
                                         reinterpret_cast<pthread_mutex_t*>(m_lock),
                                         &abstime);
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
