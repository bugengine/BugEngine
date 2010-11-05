/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/threads/semaphore.hh>
#include    <semaphore.h>
#include    <cerrno>
#include    <core/timer.hh>

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

Threads::Waitable::WaitResult Semaphore::wait()
{
    int result = sem_wait(reinterpret_cast<sem_t*>(m_data));
    if(result == 0)
    {
        return Finished;
    }
    else
    {
        be_notreached();
        return Abandoned;
    }
}


}
