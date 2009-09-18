/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/timer.hh>

namespace BugEngine
{

Timer::Timer() 
:   m_total(0)
,   m_start(0)
{
}

Timer::~Timer()
{
}

void Timer::start()
{
    timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    m_total = t.tv_nsec+t.tv_sec*1000000000;
    m_tick = m_start;
}

void Timer::stop()
{
    timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    m_total += t.tv_nsec+t.tv_sec*1000000000 - m_start;
}

void Timer::reset()
{
    m_total = 0;
}

float Timer::tick() const
{
    timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    u64 tick = t.tv_nsec+t.tv_sec*1000000000;
    float total = (float)(tick - m_tick)*1000000000.0f;
    m_tick = t.tv_nsec+t.tv_sec*1000000000;
    return total;
}

float Timer::elapsed() const
{
    timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    u64 tick = t.tv_nsec+t.tv_sec*1000000000;
    return (float)(tick - m_start)*1000000000.0f;
}

}

