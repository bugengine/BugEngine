/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA                                                                  *
* 02110-1301  USA                                                             *
\*****************************************************************************/

#include    <core/stdafx.h>
#include    <core/threads/event.hh>
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
        Assert(false);
        return Abandoned;
    }
}


}
