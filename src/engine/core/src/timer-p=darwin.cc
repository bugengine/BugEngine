/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/timer.hh>
#include    <mach/mach_time.h>

namespace BugEngine
{

u64 Timer::tick()
{
#if defined(_X86)
    u64 x = 0;
    __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
    return x;
#elif defined(_AMD64)
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((u64)lo) | (((u64)hi)<<32);
#elif defined(_PPC)
    u64 result=0;
    unsigned long int upper, lower,tmp;
    __asm__ volatile(
                "0:                  \n"
                "\tmftbu   %0           \n"
                "\tmftb    %1           \n"
                "\tmftbu   %2           \n"
                "\tcmpw    %2,%0        \n"
                "\tbne     0b         \n"
                : "=r"(upper),"=r"(lower),"=r"(tmp)
                );
    result = upper;
    result = result<<32;
    result = result|lower;

    return(result);
#elif defined(_ARM)
    return 0;
#else
# error "Architecture not supported"
    return 0;
#endif
}

float Timer::now()
{
    static mach_timebase_info_data_t info = { 0, 0 };
    static uint64_t s_base = mach_absolute_time();
    if (info.denom == 0)
    {
        mach_timebase_info(&info);
        info.denom *= 1000000;
    }
    return ((float)(mach_absolute_time()-s_base))*info.numer/info.denom;
}

}
