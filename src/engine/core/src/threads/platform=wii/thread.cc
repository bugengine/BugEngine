/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/threads/thread.hh>

#include    <cerrno>
#include    <ogc/lwp.h>
#include    <ogcsys.h>
#include    <time.h>


namespace BugEngine
{

static BE_THREAD_LOCAL const istring* st_name;

static inline u8 wiiThreadPriority(Thread::Priority p)
{
    return LWP_PRIO_IDLE+p*(LWP_PRIO_HIGHEST-LWP_PRIO_IDLE)/(Thread::Critical-Thread::Idle);
}

static inline Thread::Priority bugengineThreadPriority(u8 p)
{
    return Thread::Priority(p*(Thread::Critical-Thread::Idle)/(LWP_PRIO_HIGHEST-LWP_PRIO_IDLE));
}


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
}

Thread::ThreadParams::~ThreadParams()
{
}

void* Thread::ThreadParams::threadWrapper(void* params)
{
    ThreadParams* p = reinterpret_cast<ThreadParams*>(params);
    st_name = &(p->m_name);
    p->m_result = (*p->m_function)(p->m_param1, p->m_param2);
    return 0;
}



Thread::Thread(const istring& name, ThreadFunction f, intptr_t p1, intptr_t p2, Priority p, bool isSuspended)
:   m_params(new ThreadParams(name, f, p1, p2))
,   m_data(new lwp_t)
{
    LWP_CreateThread(reinterpret_cast<lwp_t*>(m_data), &ThreadParams::threadWrapper, m_params, 0, 0, wiiThreadPriority(p));
    m_id = *reinterpret_cast<lwp_t*>(m_data);
}

Thread::~Thread()
{
    void* rvalue;
    int result = LWP_JoinThread(*reinterpret_cast<lwp_t*>(m_data), &rvalue);
    be_assert(result == 0, "JoinThread did not succeed");
    (void)result;
    delete reinterpret_cast<ThreadParams*>(m_params);
    delete reinterpret_cast<lwp_t*>(m_data);
}

void Thread::resume()
{
}

void Thread::sleep(int milliseconds)
{
    timespec time;
    time.tv_nsec = milliseconds * 10000000;
    time.tv_sec = time.tv_nsec % 1000000000;
    time.tv_nsec = time.tv_nsec % 1000000000;
    nanosleep(&time);
}

void Thread::yield()
{
    LWP_YieldThread();
}

unsigned long Thread::id() const
{
    return m_id;
}

unsigned long Thread::currentId()
{
    return LWP_GetSelf();
}

istring Thread::name()
{
    return *st_name;
}

void Thread::wait() const
{
    void* result;
    LWP_JoinThread(*reinterpret_cast<lwp_t*>(m_data), &result);
}

void Thread::setPriority(Priority p)
{
    LWP_SetThreadPriority(*reinterpret_cast<lwp_t*>(m_data), wiiThreadPriority(p));
}

}
