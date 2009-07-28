/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA                                                                  *
* 02110-1301  USA                                                             *
\*****************************************************************************/

#include    <core/stdafx.h>
#include    <core/threads/thread.hh>

#include    <cerrno>


namespace BugEngine
{

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
,   m_data(new pthread_t)
{
    pthread_create(reinterpret_cast<pthread_t*>(m_data), 0, &ThreadParams::threadWrapper, m_params);
    m_id = *reinterpret_cast<pthread_t*>(m_data);
    setPriority(p);
}

Thread::~Thread()
{
    void* rvalue;
    timespec abstime;
    clock_gettime(CLOCK_REALTIME, &abstime);
    abstime.tv_sec += 2;
    int result = pthread_timedjoin_np(*reinterpret_cast<pthread_t*>(m_data), &rvalue, &abstime);
    Assert(result != ETIMEDOUT);
    delete reinterpret_cast<ThreadParams*>(m_params);
    delete reinterpret_cast<pthread_t*>(m_data);
}

void Thread::resume()
{
}

void Thread::sleep(int milliseconds)
{
    timespec abstime, r;
    clock_gettime(CLOCK_REALTIME, &abstime);
    abstime.tv_nsec += milliseconds % 1000;
    abstime.tv_sec += milliseconds / 1000;
    abstime.tv_sec += abstime.tv_nsec % 1000000000;
    abstime.tv_nsec = abstime.tv_nsec % 1000000000;
    while(clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &abstime, &r) == -1)
        /*again*/;
}

void Thread::yield()
{
    pthread_yield();
}

unsigned long Thread::id() const
{
    return m_id;
}

unsigned long Thread::currentId()
{
    return pthread_self();
}

const istring& Thread::name()
{
    return *st_name;
}

void Thread::wait() const
{
    void* result;
    pthread_join(*reinterpret_cast<pthread_t*>(m_data), &result);
}

Thread::Priority Thread::priority() const
{
    int policy;
    sched_param param;
    pthread_getschedparam(*reinterpret_cast<pthread_t*>(m_data), &policy, &param);
    int p = param.sched_priority;
    return Thread::Priority(param.__sched_priority-sched_get_priority_min(policy));
}

void Thread::setPriority(Priority p)
{
    sched_param param;
    param.__sched_priority = sched_get_priority_min(SCHED_RR)+(int)p;
    pthread_setschedparam(*reinterpret_cast<pthread_t*>(m_data), SCHED_RR, &param);
}

}
