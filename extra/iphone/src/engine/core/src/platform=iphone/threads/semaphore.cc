/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/threads/semaphore.hh>
#include    <objc/objc.h>

#define USE_DISPATCH_SEMAPHORE (__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ >= 50000)
#if USE_DISPATCH_SEMAPHORE
# include   <dispatch/dispatch.h>
#else
# include   <semaphore.h>
#endif
#include    <stdio.h>
#include    <errno.h>

namespace BugEngine
{

#if !USE_DISPATCH_SEMAPHORE
static int x;
#endif

Semaphore::Semaphore(int initialCount)
#if USE_DISPATCH_SEMAPHORE
:   m_data(dispatch_semaphore_create(initialCount))
#else
:   m_data(sem_open(minitl::format<1024u>("/be_%s") | x++, O_CREAT, 0644, 65535))
#endif
{
#if USE_DISPATCH_SEMAPHORE
    if (!m_data)
#else
    if ((sem_t*)m_data == SEM_FAILED)
#endif
    {
        be_error("Could not initialize semaphore: %s" | strerror(errno));
    }
#if !USE_DISPATCH_SEMAPHORE
    release(initialCount);
#endif
}

Semaphore::~Semaphore()
{
#if USE_DISPATCH_SEMAPHORE
    dispatch_release(reinterpret_cast<dispatch_semaphore_t>(m_data));
#else
    sem_close((sem_t*)m_data);
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


