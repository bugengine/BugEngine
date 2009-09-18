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
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_start));
    m_tick = m_start;
}

void Timer::stop()
{
    u64 stop;
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&stop));
    m_total += (stop - m_start);
}

void Timer::reset()
{
    m_total = 0;
}

float Timer::tick() const
{
    u64 tick;
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&tick));
    u64 freq;
    QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&freq));
    float total = (float)(tick - m_tick)/(float)freq;
    m_tick = tick;
    return total;
}

float Timer::elapsed() const
{
    u64 freq;
    u64 now;
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&now));
    QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&freq));
    return (float)(now-m_start)/(float)freq;
}

}
