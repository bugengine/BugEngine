/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/timer.hh>
#include    <time.h>

namespace BugEngine
{

u64 Timer::tick()
{
#if defined (BE_COMPILER_GCC) || defined(BE_COMPILER_SUNCC)
# if defined(_X86)
    u64 x = 0;
    __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
    return x;
# elif defined(_AMD64)
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((u64)lo) | (((u64)hi)<<32);
# elif defined(_PPC)
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
# else
    timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return ((u64)t.tv_sec * 1000000000 + (u64)t.tv_nsec);
# endif
#else
    timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return ((u64)t.tv_sec * 1000000000 + (u64)t.tv_nsec);
#endif
}

float Timer::now()
{
    timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return (float)t.tv_sec * 1000.0f + (float)t.tv_nsec / 1000000.0f;
}

}
