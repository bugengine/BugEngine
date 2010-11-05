/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/timer.hh>
#include    <mach/mach_time.h>

namespace BugEngine
{

u64 Timer::tick()
{
    return mach_absolute_time();
}

float Timer::now()
{
    static mach_timebase_info_data_t info = { 0, 0 };
    if(info.denom == 0)
        mach_timebase_info(&info);
    return ((float)mach_absolute_time())*1000.0f*info.numer/info.denom;
}

}
