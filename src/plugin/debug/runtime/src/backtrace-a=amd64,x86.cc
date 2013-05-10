/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <runtime/stdafx.h>

#include    <runtime/callstack.hh>

namespace BugEngine { namespace Runtime
{

static inline void** st_next(void** stack_pointer)
{
    void** nextStackPointer = reinterpret_cast<void**>(*stack_pointer);
    if (nextStackPointer <= stack_pointer)
    {
        return 0;
    }
    if (reinterpret_cast<char*>(nextStackPointer) >= reinterpret_cast<char*>(stack_pointer) + 2*1024*1024)
    {
        return 0;
    }
    return nextStackPointer;
}

BE_NOINLINE size_t Callstack::backtrace(Address* buffer, size_t count, size_t skip)
{
    void** stackPointer;
#ifdef __llvm__
    stackPointer = (void**)__builtin_frame_address(0);
#elif defined(__GNUC__)
# ifdef _AMD64
    __asm__ volatile ("mov %%rbp, %0" : "=r" (stackPointer));
# else
    __asm__ volatile ("mov %%ebp, %0" : "=r" (stackPointer));
# endif
#else
    stackPointer = (void**)(&buffer)-2;
#endif
    size_t result = 0;
    while (stackPointer && result < count)
    {
        if (skip > 0)
        {
            skip--;
        }
        else
        {
#ifdef _X64
            // stack frame is aligned on a 16 bytes boundary in x64
            buffer[result].m_address = reinterpret_cast<u64>(*(minitl::align(stackPointer, 16)+1));
#else
            buffer[result].m_address = reinterpret_cast<u64>(*(stackPointer+1));
#endif
            result++;
        }
        stackPointer = st_next(stackPointer);
    }
    return result;
}

}}
