/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/threads/semaphore.hh>
#include    <ogc/semaphore.h>
#include    <cerrno>

namespace BugEngine
{

Semaphore::Semaphore(int initialCount, int maxCount)
:   m_data(new sem_t)
{
    LWP_SemInit(reinterpret_cast<sem_t*>(m_data), initialCount, maxCount);
}

Semaphore::~Semaphore()
{
    LWP_SemDestroy(*reinterpret_cast<sem_t*>(m_data));
    delete reinterpret_cast<sem_t*>(m_data);
}

void Semaphore::release(int count)
{
    for(int i = 0; i < count; ++i)
        LWP_SemPost(*reinterpret_cast<sem_t*>(m_data));
}

Threads::Waitable::WaitResult Semaphore::wait(unsigned int timeout)
{
    LWP_SemWait(*reinterpret_cast<sem_t*>(m_data));
    return Finished;
}


}
