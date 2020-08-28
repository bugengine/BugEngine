/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/core/stdafx.h>
#include <bugengine/core/threads/thread.hh>

#include <bugengine/core/timer.hh>

#include <cerrno>
#include <pthread.h>

#ifdef BE_PLATFORM_FREEBSD
#    include <pthread_np.h>
#endif
#ifdef BE_PLATFORM_SUN
#    include <thread.h>
#endif
#include <sched.h>

namespace BugEngine {

class Thread::ThreadParams
{
    friend class Thread;

private:
    istring                m_name;
    Thread::ThreadFunction m_function;
    intptr_t               m_param1;
    intptr_t               m_param2;
    intptr_t               m_result;

public:
    ThreadParams(const istring& name, ThreadFunction f, intptr_t p1, intptr_t p2);
    ~ThreadParams();

    static void* threadWrapper(void* params);
};

struct ThreadData
{
    pthread_key_t key;
    istring       name;
    ThreadData() : name("main")
    {
        pthread_key_create(&key, 0);
        pthread_setspecific(key, &name);
#if BE_PLATFORM_LINUX && !BE_COMPILER_SUNCC
        pthread_setname_np(pthread_self(), name.c_str());
#elif BE_PLATFORM_FREEBSD
        pthread_set_name_np(pthread_self(), name.c_str());
#elif BE_PLATFORM_MACOSX
        pthread_setname_np(name.c_str());
#endif
    }
    ~ThreadData()
    {
        pthread_key_delete(key);
    }
};

static ThreadData s_data;

Thread::ThreadParams::ThreadParams(const istring& name, ThreadFunction f, intptr_t p1, intptr_t p2)
    : m_name(name)
    , m_function(f)
    , m_param1(p1)
    , m_param2(p2)
    , m_result(0)
{
    be_info("starting thread %s" | name);
}

Thread::ThreadParams::~ThreadParams()
{
}

void* Thread::ThreadParams::threadWrapper(void* params)
{
    ThreadParams* p = reinterpret_cast< ThreadParams* >(params);
    pthread_setspecific(s_data.key, &p->m_name);
#if BE_PLATFORM_LINUX && !BE_COMPILER_SUNCC
    pthread_setname_np(pthread_self(), p->m_name.c_str());
#elif BE_PLATFORM_FREEBSD
    pthread_set_name_np(pthread_self(), p->m_name.c_str());
#elif BE_PLATFORM_MACOSX
    pthread_setname_np(p->m_name.c_str());
#endif
    be_debug("started thread %s" | p->m_name);
    p->m_result = (*p->m_function)(p->m_param1, p->m_param2);
    be_info("stopped thread %s" | p->m_name);
    return 0;
}

Thread::Thread(const istring& name, ThreadFunction f, intptr_t p1, intptr_t p2, Priority p)
    : m_params(new ThreadParams(name, f, p1, p2))
    , m_data(new pthread_t)
{
    pthread_create(reinterpret_cast< pthread_t* >(m_data), 0, &ThreadParams::threadWrapper, m_params);
    m_id = u64(ptrdiff_t(*reinterpret_cast< pthread_t* >(m_data)));
    setPriority(p);
}

Thread::~Thread()
{
    void* rvalue;
#ifdef __GNU_SOURCE
    timespec abstime;
    clock_gettime(CLOCK_REALTIME, &abstime);
    abstime.tv_sec += 2;
    int result = pthread_timedjoin_np(*reinterpret_cast< pthread_t* >(m_data), &rvalue, &abstime);
    be_assert(result != ETIMEDOUT, "timed out when waiting for thread %s" | m_params->m_name.c_str());
#else
    int result = pthread_join(*reinterpret_cast< pthread_t* >(m_data), &rvalue);
#endif
    (void)result;
    delete m_params;
    delete reinterpret_cast< pthread_t* >(m_data);
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
    istring* name = static_cast< istring* >(pthread_getspecific(s_data.key));
    if(name)
    {
        return *name;
    }
    else
    {
        return istring(minitl::format< 128u >("%d") | currentId());
    }
}

void Thread::wait() const
{
    void* result;
    pthread_join(*reinterpret_cast< pthread_t* >(m_data), &result);
}

void Thread::setPriority(Priority p)
{
    sched_param param;
    param.sched_priority = sched_get_priority_min(SCHED_RR) + (int)p;
    pthread_setschedparam(*reinterpret_cast< pthread_t* >(m_data), SCHED_RR, &param);
}

}  // namespace BugEngine
