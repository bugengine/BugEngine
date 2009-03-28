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
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

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
