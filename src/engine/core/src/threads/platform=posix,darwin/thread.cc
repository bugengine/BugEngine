/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/threads/thread.hh>

#include    <pthread.h>
#include    <cerrno>
#include    <core/timer.hh>

#ifdef BE_PLATFORM_SUN
# include   <thread.h>
#endif
#include    <sched.h>

namespace BugEngine
{

class Thread::ThreadParams
{
    friend class Thread;
private:
    istring                 m_name;
    Thread::ThreadFunction  m_function;
    intptr_t                m_param1;
    intptr_t                m_param2;
    intptr_t                m_result;
public:
    ThreadParams(const istring& name, ThreadFunction f, intptr_t p1, intptr_t p2);
    ~ThreadParams();

    static void* threadWrapper(void* params);
};

Thread::ThreadParams::ThreadParams(const istring& name, ThreadFunction f, intptr_t p1, intptr_t p2)
:   m_name(name)
,   m_function(f)
,   m_param1(p1)
,   m_param2(p2)
,   m_result(0)
{
    be_info("starting thread %s" | name);
}

Thread::ThreadParams::~ThreadParams()
{
}

void* Thread::ThreadParams::threadWrapper(void* params)
{
    ThreadParams* p = reinterpret_cast<ThreadParams*>(params);
    be_debug("started thread %s" | p->m_name);
    p->m_result = (*p->m_function)(p->m_param1, p->m_param2);
    be_info("stopped thread %s" | p->m_name);
    return 0;
}



Thread::Thread(const istring& name, ThreadFunction f, intptr_t p1, intptr_t p2, Priority p)
:   m_params(new ThreadParams(name, f, p1, p2))
,   m_data(new pthread_t)
{
    pthread_create(reinterpret_cast<pthread_t*>(m_data), 0, &ThreadParams::threadWrapper, m_params);
    m_id = u64(ptrdiff_t(*reinterpret_cast<pthread_t*>(m_data)));
    setPriority(p);
}

Thread::~Thread()
{
    void* rvalue;
#ifdef __GNU_SOURCE
    timespec abstime;
    clock_gettime(CLOCK_REALTIME, &abstime);
    abstime.tv_sec += 2;
    int result = pthread_timedjoin_np(*reinterpret_cast<pthread_t*>(m_data), &rvalue, &abstime);
    be_assert(result != ETIMEDOUT, "timed out when waiting for thread %s" | m_params->m_name.c_str());
#else
    int result = pthread_join(*reinterpret_cast<pthread_t*>(m_data), &rvalue);
#endif
    (void)result;
    delete reinterpret_cast<ThreadParams*>(m_params);
    delete reinterpret_cast<pthread_t*>(m_data);
}

void Thread::yield()
{
#ifdef BE_PLATFORM_SUN
    thr_yield();
#elif defined(BE_PLATFORM_LINUX) || defined(BE_PLATFORM_MACOS)
    sched_yield();
#else
    pthread_yield();
#endif
}

u64 Thread::id() const
{
    return m_id;
}

u64 Thread::currentId()
{
    return u64(ptrdiff_t(pthread_self()));
}

istring Thread::name()
{
    return istring(minitl::format<128u>("thread %d") | currentId());
}

void Thread::wait() const
{
    void* result;
    pthread_join(*reinterpret_cast<pthread_t*>(m_data), &result);
}

void Thread::setPriority(Priority p)
{
    sched_param param;
    param.sched_priority = sched_get_priority_min(SCHED_RR)+(int)p;
    pthread_setschedparam(*reinterpret_cast<pthread_t*>(m_data), SCHED_RR, &param);
}

}
