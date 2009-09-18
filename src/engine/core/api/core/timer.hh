/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

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
typedef u64 tick_type;
static inline tick_type tick(void)
{
    u64 x;
    __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
    return x;
}
# elif defined(_AMD64_)
typedef u64 tick_type;
static inline tick_type tick(void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((u64)lo) | (((u64)hi)<<32);
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
