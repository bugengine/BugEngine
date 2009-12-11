/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

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
    abstime.tv_nsec += (timeout * 1000000);
    abstime.tv_sec += abstime.tv_nsec / 1000000000;
    abstime.tv_nsec = abstime.tv_nsec % 1000000000;
    int result = sem_timedwait( reinterpret_cast<sem_t*>(m_data),
                                &abstime);
    if(result == 0)
    {
        return Finished;
    }
    else
    {
        switch(errno)
        {
        case ETIMEDOUT:
            return TimeOut;
        default:
            be_notreached();
            return Abandoned;
        }
    }
}


}
