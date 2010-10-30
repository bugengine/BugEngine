/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/timer.hh>

#ifdef BE_PLATFORM_MACOS
# include <mach/mach_time.h>
#endif

namespace BugEngine
{

#ifdef BE_PLATFORM_MACOS
static mach_timebase_info_data_t s_info = { 0, 0 };
#endif

Timer::Timer()
:   m_total(0)
,   m_start(0)
{
#ifdef BE_PLATFORM_MACOS
    mach_timebase_info(&s_info);
#endif
}

Timer::~Timer()
{
}

void Timer::start()
{
#ifdef BE_PLATFORM_MACOS
    m_start = mach_absolute_time() * (s_info.numer / s_info.denom);
#else
    timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    m_start = t.tv_nsec+t.tv_sec*1000000000;
#endif
    m_tick = m_start;
}

void Timer::stop()
{
#ifdef BE_PLATFORM_MACOS
    m_total += mach_absolute_time() * (s_info.numer / s_info.denom) - m_start;
#else
    timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    m_total += t.tv_nsec+t.tv_sec*1000000000 - m_start;
#endif
}

void Timer::reset()
{
    m_total = 0;
}

float Timer::tick() const
{
#ifdef BE_PLATFORM_MACOS
    u64 tick = mach_absolute_time() * (s_info.numer / s_info.denom);
#else
    timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    u64 tick = t.tv_nsec+t.tv_sec*1000000000;
#endif
    float total = (float)(tick - m_tick)*1000000000.0f;
    m_tick = tick;
    return total;
}

float Timer::elapsed() const
{
#ifdef BE_PLATFORM_MACOS
    u64 tick = mach_absolute_time() * (s_info.numer / s_info.denom);
#else
    timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    u64 tick = t.tv_nsec+t.tv_sec*1000000000;
#endif
    return (float)(tick - m_start)*1000000000.0f;
}

}

