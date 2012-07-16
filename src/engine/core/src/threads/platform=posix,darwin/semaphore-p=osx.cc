/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/threads/semaphore.hh>
#include    <core/timer.hh>
#include    <cerrno>
#if __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ >= 1060
# include   <dispatch/dispatch.h>
#else
# include   <CoreServices/CoreServices.h>
#endif

namespace BugEngine
{

Semaphore::Semaphore(int initialCount)
#if __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ >= 1060
:   m_data(dispatch_semaphore_create(initialCount))
#else
:   m_data(new MPSemaphoreID)
#endif
{
#if __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ >= 1060
    if (!m_data)
    {
        be_error("Could not initialize semaphore: %s" | strerror(errno));
    }
#else
    MPCreateSemaphore(65000, initialCount, (MPSemaphoreID*)m_data);
#endif
}

Semaphore::~Semaphore()
{
#if __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ >= 1060
    dispatch_release(reinterpret_cast<dispatch_semaphore_t>(m_data));
#else
    MPDeleteSemaphore(*(MPSemaphoreID*)m_data);
    delete (MPSemaphoreID*)m_data;
#endif
}

void Semaphore::release(int count)
{
    for (int i = 0; i < count; ++i)
    {
#if __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ >= 1060
        dispatch_semaphore_signal(reinterpret_cast<dispatch_semaphore_t>(m_data));
#else
        MPSignalSemaphore(*(MPSemaphoreID*)m_data);
#endif
    }
}

Threads::Waitable::WaitResult Semaphore::wait()
{
#if __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ >= 1060
    int result = dispatch_semaphore_wait(reinterpret_cast<dispatch_semaphore_t>(m_data), DISPATCH_TIME_FOREVER);
#else
    int result = MPWaitOnSemaphore(*(MPSemaphoreID*)m_data, kDurationForever);
#endif
    if (result == 0)
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
