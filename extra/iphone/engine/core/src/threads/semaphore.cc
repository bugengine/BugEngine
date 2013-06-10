/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/threads/semaphore.hh>
#include    <objc/objc.h>

#define USE_DISPATCH_SEMAPHORE __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ >= 50000
#if USE_DISPATCH_SEMAPHORE
# include   <dispatch/dispatch.h>
#else
# include   <pthread.h>
#endif
#include    <stdio.h>
#include    <errno.h>

namespace BugEngine
{

Semaphore::Semaphore(int initialCount)
#if USE_DISPATCH_SEMAPHORE
:   m_data(dispatch_semaphore_create(initialCount))
#else
:   m_data(new sem_t)
#endif
{
#if USE_DISPATCH_SEMAPHORE
    if (!m_data)
#else
    if (!sem_init((sem_t*)m_data, initialCount, 65535))
#endif
    {
        be_error("Could not initialize semaphore: %s" | strerror(errno));
    }
}

Semaphore::~Semaphore()
{
#if USE_DISPATCH_SEMAPHORE
    dispatch_release(reinterpret_cast<dispatch_semaphore_t>(m_data));
#else
    sem_destroy((sem_t*)m_data);
#endif
}

void Semaphore::release(int count)
{
    for (int i = 0; i < count; ++i)
    {
#if USE_DISPATCH_SEMAPHORE
        dispatch_semaphore_signal(reinterpret_cast<dispatch_semaphore_t>(m_data));
#else
        sem_post((sem_t*)m_data);
#endif
    }
}

Threads::Waitable::WaitResult Semaphore::wait()
{
#if USE_DISPATCH_SEMAPHORE
    int result = dispatch_semaphore_wait(reinterpret_cast<dispatch_semaphore_t>(m_data), DISPATCH_TIME_FOREVER);
#else
    int result = sem_wait((sem_t*)m_data);
#endif
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


