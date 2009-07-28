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

