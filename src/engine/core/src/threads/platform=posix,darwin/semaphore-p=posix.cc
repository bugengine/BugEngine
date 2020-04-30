/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/core/stdafx.h>
#include <bugengine/core/threads/semaphore.hh>

#include <bugengine/core/timer.hh>

#include <cerrno>
#include <semaphore.h>
#include <stdio.h>

namespace BugEngine {

Semaphore::Semaphore(int initialCount) : m_data(new sem_t)
{
    if(sem_init(reinterpret_cast< sem_t* >(m_data), 0, initialCount) != 0)
    {
        be_error("Could not initialize semaphore: %s" | strerror(errno));
    }
}

Semaphore::~Semaphore()
{
    if(sem_destroy(reinterpret_cast< sem_t* >(m_data)) != 0)
    {
        be_error("Could not initialize semaphore: %s" | strerror(errno));
    }
    delete reinterpret_cast< sem_t* >(m_data);
}

void Semaphore::release(int count)
{
    for(int i = 0; i < count; ++i)
    {
        if(sem_post(reinterpret_cast< sem_t* >(m_data)) != 0)
        {
            be_error("Could not release semaphore: %s" | strerror(errno));
        }
    }
}

Threads::Waitable::WaitResult Semaphore::wait()
{
    int result;
    do
    {
        result = sem_wait(reinterpret_cast< sem_t* >(m_data));
    } while(result != 0 || errno == EINTR);
    if(result == 0)
    {
        return Finished;
    }
    else
    {
        be_info("Semaphore error: %s" | strerror(errno));
        be_notreached();
        return Abandoned;
    }
}

}  // namespace BugEngine
