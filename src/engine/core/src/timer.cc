/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/timer.hh>

#ifdef BE_PLATFORM_MACOS
void clock_gettime(int, timespec* time)
{
    static mach_timebase_info_data_t info = { 0, 0 };
    if(info.denom == 0)
        mach_timebase_info(&info);
    u64 tick = mach_absolute_time() * info.numer/info.denom;
    time->tv_sec = tick/1000000000;
    time->tv_nsec = tick-time->tv_sec*1000000000;
}
#endif

