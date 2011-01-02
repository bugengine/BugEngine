/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/threads/semaphore.hh>
#include    <core/timer.hh>
#include    <CoreServices/CoreServices.h>

namespace BugEngine
{

Semaphore::Semaphore(int initialCount, int maxCount)
:   m_data(new MPSemaphoreID)
{
    MPCreateSemaphore(maxCount, initialCount, (MPSemaphoreID*)m_data);
}

Semaphore::~Semaphore()
{
    MPDeleteSemaphore(*(MPSemaphoreID*)m_data);
    delete (MPSemaphoreID*)m_data;
}

void Semaphore::release(int count)
{
    for(int i = 0; i < count; ++i)
    {
        MPSignalSemaphore(*(MPSemaphoreID*)m_data);
    }
}

Threads::Waitable::WaitResult Semaphore::wait()
{
    int result = MPWaitOnSemaphore(*(MPSemaphoreID*)m_data, kDurationForever);
    if(result == 0)
    {
        return Finished;
    }
    else
    {
        be_error("MPWaitOnSemaphore returned %d" | result);
        be_notreached();
        return Abandoned;
    }
}


}
