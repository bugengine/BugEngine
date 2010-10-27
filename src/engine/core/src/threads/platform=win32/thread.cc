/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>

#include    <core/threads/thread.hh>



namespace BugEngine
{

#define MS_VC_EXCEPTION 0x406D1388
#pragma pack(push,8)
struct THREADNAME_INFO
{
    DWORD dwType;
    LPCSTR szName;
    DWORD dwThreadID;
    DWORD dwFlags;
};
#pragma pack(pop)


static BE_THREAD_LOCAL const istring* st_name;


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

    static unsigned long WINAPI threadWrapper(void* params);
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

unsigned long WINAPI Thread::ThreadParams::threadWrapper(void* params)
{
    ThreadParams* p = static_cast<ThreadParams*>(params);
    st_name = &(p->m_name);
#ifdef _MSC_VER
    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = p->m_name.c_str();
    info.dwThreadID = GetCurrentThreadId();
    info.dwFlags = 0;
    __try
    {
        RaiseException(MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info);
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
    }
#endif
    p->m_result = (*p->m_function)(p->m_param1, p->m_param2);

    return 0;
}



Thread::Thread(const istring& name, ThreadFunction f, intptr_t p1, intptr_t p2, Priority p, bool isSuspended)
:   m_params(new ThreadParams(name, f, p1, p2))
,   m_data((void*)CreateThread(0, 0, &ThreadParams::threadWrapper, m_params, isSuspended?CREATE_SUSPENDED:0, &m_id))
{
    setPriority(p);
}

Thread::~Thread()
{
    DWORD result = WaitForSingleObject((HANDLE)m_data, 2000);
    be_assert(result != WAIT_TIMEOUT, "timed out when waiting for thread %s" | m_params->m_name.c_str());
    be_forceuse(result);
    CloseHandle((HANDLE)m_data);
    delete static_cast<ThreadParams*>(m_params);
}

void Thread::resume()
{
    ResumeThread((HANDLE)m_data);
}

void Thread::sleep(int millisecond)
{
    Sleep(millisecond);
}

void Thread::yield()
{
    Sleep(0);
}

unsigned long Thread::id() const
{
    return m_id;
}

unsigned long Thread::currentId()
{
    return GetCurrentThreadId();
}

istring Thread::name()
{
    return *st_name;
}

void Thread::wait() const
{
    WaitForSingleObject((HANDLE)m_data, INFINITE);
}

void Thread::setPriority(Priority p)
{
    switch(p)
    {
    case Idle:
        SetThreadPriority((HANDLE)m_data, THREAD_PRIORITY_IDLE);
        return;
    case Lowest:
        SetThreadPriority((HANDLE)m_data, THREAD_PRIORITY_LOWEST);
        return;
    case BelowNormal:
        SetThreadPriority((HANDLE)m_data, THREAD_PRIORITY_BELOW_NORMAL);
        return;
    case Normal:
        SetThreadPriority((HANDLE)m_data, THREAD_PRIORITY_NORMAL);
        return;
    case AboveNormal:
        SetThreadPriority((HANDLE)m_data, THREAD_PRIORITY_ABOVE_NORMAL);
        return;
    case Highest:
        SetThreadPriority((HANDLE)m_data, THREAD_PRIORITY_HIGHEST);
        return;
    case Critical:
        SetThreadPriority((HANDLE)m_data, THREAD_PRIORITY_TIME_CRITICAL);
        return;
    }
}

}
