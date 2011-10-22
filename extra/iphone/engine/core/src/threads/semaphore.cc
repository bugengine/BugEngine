/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/threads/semaphore.hh>
#include    <dispatch/dispatch.h>
#include    <stdio.h>
#include    <errno.h>

namespace BugEngine
{

Semaphore::Semaphore(int initialCount, int maxCount)
:   m_data(dispatch_semaphore_create(initialCount))
{
    if (!m_data)
    {
        be_error("Could not initialize semaphore: %s" | strerror(errno));
    }
}

Semaphore::~Semaphore()
{
    dispatch_release(reinterpret_cast<dispatch_semaphore_t>(m_data));
}

void Semaphore::release(int count)
{
    for (int i = 0; i < count; ++i)
    {
        dispatch_semaphore_signal(reinterpret_cast<dispatch_semaphore_t>(m_data));
    }
}

Threads::Waitable::WaitResult Semaphore::wait()
{
    int result = dispatch_semaphore_wait(reinterpret_cast<dispatch_semaphore_t>(m_data), DISPATCH_TIME_FOREVER);
    if (result == 0)
    {
        return Finished;
    }
    else
    {
        be_error("Could not wait on semaphore: %s" | strerror(errno));
        be_notreached();
        return Abandoned;
    }
}


}


