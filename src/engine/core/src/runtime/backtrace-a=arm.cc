/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>

#include    <core/runtime/callstack.hh>

namespace BugEngine { namespace Runtime
{

static inline void** st_next(void** stack_pointer)
{
    void** nextStackPointer = reinterpret_cast<void**>(*stack_pointer);
    if (nextStackPointer <= stack_pointer)
    {
        return 0;
    }
    if (reinterpret_cast<char*>(stack_pointer) - reinterpret_cast<char*>(nextStackPointer) > 512*1024)
    {
        return 0;
    }
    return nextStackPointer;
}

BE_NOINLINE size_t Callstack::backtrace(Address* buffer, size_t count, size_t skip)
{
    void** stackPointer;
    __asm__ volatile ("mov %0,r13" : "=r" (stackPointer));
    size_t result = 0;
    while (stackPointer && result < count)
    {
        if (skip > 0)
        {
            skip--;
        }
        else
        {
            buffer[result].m_address = (u64)*(stackPointer+1);
            result++;
        }
        stackPointer = st_next(stackPointer);
    }
    return result;
}

}}
