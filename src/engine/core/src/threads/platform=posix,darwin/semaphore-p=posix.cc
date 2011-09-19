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
    if (sem_init(reinterpret_cast<sem_t*>(m_data), 0, initialCount) != 0)
    {
        be_error("Could not initialize semaphore: %s" | sys_errlist[errno]);
    }
}

Semaphore::~Semaphore()
{
    if (sem_destroy(reinterpret_cast<sem_t*>(m_data)) != 0)
    {
        be_error("Could not initialize semaphore: %s" | sys_errlist[errno]);
    }
    delete reinterpret_cast<sem_t*>(m_data);
}

void Semaphore::release(int count)
{
    for (int i = 0; i < count; ++i)
    {
        if (sem_post(reinterpret_cast<sem_t*>(m_data)) != 0)
        {
            be_error("Could not release semaphore: %s" | sys_errlist[errno]);
        }
    }
}

Threads::Waitable::WaitResult Semaphore::wait()
{
    int result = sem_wait(reinterpret_cast<sem_t*>(m_data));
    if (result == 0)
    {
        return Finished;
    }
    else
    {
        be_error("Could not wait on semaphore: %s" | sys_errlist[errno]);
        be_notreached();
        return Abandoned;
    }
}


}


