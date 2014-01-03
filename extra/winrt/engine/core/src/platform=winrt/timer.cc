/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/timer.hh>

namespace BugEngine
{

u64 Timer::tick()
{
    LARGE_INTEGER t;
    QueryPerformanceCounter(&t);
    return t.QuadPart;
}

float Timer::now()
{
    LARGE_INTEGER t, freq;
    QueryPerformanceCounter(&t);
    QueryPerformanceFrequency(&freq);
    return (float)t.QuadPart*1000.0f/freq.QuadPart;
}

}
