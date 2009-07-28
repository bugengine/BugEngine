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
#include    <core/threads/semaphore.hh>
#include    <semaphore.h>
#include    <cerrno>

namespace BugEngine
{

Semaphore::Semaphore(int initialCount, int maxCount)
:   m_data(new sem_t)
{
    sem_init(reinterpret_cast<sem_t*>(m_data), 0, initialCount);
}

Semaphore::~Semaphore()
{
    sem_destroy(reinterpret_cast<sem_t*>(m_data));
    delete reinterpret_cast<sem_t*>(m_data);
}

void Semaphore::release(int count)
{
    for(int i = 0; i < count; ++i)
        sem_post(reinterpret_cast<sem_t*>(m_data));
}

Threads::Waitable::WaitResult Semaphore::wait(unsigned int timeout)
{
    timespec abstime;
    clock_gettime(CLOCK_REALTIME, &abstime);
    abstime.tv_nsec += timeout % 1000;
    abstime.tv_sec += timeout / 1000;
    abstime.tv_sec += abstime.tv_nsec % 1000000000;
    abstime.tv_nsec = abstime.tv_nsec % 1000000000;
    int result = sem_timedwait( reinterpret_cast<sem_t*>(m_data),
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
