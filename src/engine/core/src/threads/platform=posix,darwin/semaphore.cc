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
:   m_data(0)
{
    static unsigned int s_semId = 0;
    minitl::format<> name = minitl::format<>("/bugengine/%d/%d") | 0 | s_semId++; // TODO: process ID
    m_data = sem_open(name, O_CREAT, 0770, initialCount);
    if (!m_data)
    {
        be_error("sem_open returned 0");
    }
}

Semaphore::~Semaphore()
{
    int result = sem_close(reinterpret_cast<sem_t*>(m_data));
    if (result)
    {
        be_error("sem_close returned %d" | result);
    }
    delete reinterpret_cast<sem_t*>(m_data);
}

void Semaphore::release(int count)
{
    for(int i = 0; i < count; ++i)
    {
        int result = sem_post(reinterpret_cast<sem_t*>(m_data));
        if (result)
        {
            be_error("sem_post returned %d" | result);
        }
    }
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
        be_error("sem_wait returned %d" | result);
        perror("sem_wait");
        be_notreached();
        return Abandoned;
    }
}


}
