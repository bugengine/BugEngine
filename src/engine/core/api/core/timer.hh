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

#ifndef BE_CORE_TIMER_HH_
#define BE_CORE_TIMER_HH_
/*****************************************************************************/

namespace BugEngine
{

#if defined(_MSC_VER)
typedef u64 tick_type;
inline tick_type tick(){ u64 result; QueryPerformanceCounter((LARGE_INTEGER*)&result); return result; }
#elif defined __GNUC__
# if defined(_X86_)
typedef unsigned long long tick_type;
static inline tick_type tick(void)
{
    unsigned long long int x;
    __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
    return x;
}
# elif defined(_AMD64_)
typedef unsigned long long tick_type;
static inline tick_type tick(void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((unsigned long long)lo) | (((unsigned long long)hi)<<32);
}
# elif defined(__powerpc__)
typedef unsigned long long tick_type;
static inline tick_type tick(void)
{
    tick_type result=0;
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
}
# else
#  error platform not supported...
# endif
#else
# error compiler not supported...
#endif

class COREEXPORT Timer
{
private:
    u64         m_total;
    u64         m_start;
    mutable u64 m_tick;
public:
    Timer();
    ~Timer();

    void start();
    void stop();
    void reset();
    float tick() const;
    float elapsed() const;
};

}

/*****************************************************************************/
#endif
