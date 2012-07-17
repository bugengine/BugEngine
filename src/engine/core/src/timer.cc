/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/timer.hh>

namespace BugEngine
{

Timer::Timer() 
:   m_total(0)
,   m_start(0)
{
    start();
}

Timer::~Timer()
{
}

void Timer::start()
{
    m_start = tick();
}

u64 Timer::stop()
{
    u64 stop = tick();
    m_total += (stop - m_start);
    return m_total;
}

void Timer::reset()
{
    m_total = 0;
}

u64 Timer::elapsed() const
{
    return tick() - m_start;
}

u64 Timer::total() const
{
    return m_total;
}

}
