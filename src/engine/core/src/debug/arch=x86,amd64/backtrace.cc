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

#include    <core/debug/callstack.hh>
#ifdef BE_COMPILER_MSVC
# include <intrin.h>
#endif

namespace BugEngine { namespace Debug
{

static inline void** st_next(void** stack_pointer)
{
    void** nextStackPointer = reinterpret_cast<void**>(*stack_pointer);
    if(nextStackPointer <= stack_pointer)
    {
        return 0;
    }
    if(reinterpret_cast<char*>(nextStackPointer) >= reinterpret_cast<char*>(stack_pointer) + 2*1024*1024)
    {
        return 0;
    }
    return nextStackPointer;
}

BE_NOINLINE size_t Callstack::backtrace(void** buffer, size_t count, size_t skip)
{
    void** stackPointer;
#ifdef __llvm__
    stackPointer = __builtin_frame_address(0);
#elif defined(__GNUC__)
# ifdef _X64
    __asm__ volatile ("mov %%rbp, %0" : "=r" (stackPointer));
# else
    __asm__ volatile ("mov %%ebp, %0" : "=r" (stackPointer));
# endif
#else
    stackPointer = (void**)(&buffer)-2;
#endif
    size_t result = 0;
    while(stackPointer && result < count)
    {
        if(skip > 0)
        {
            skip--;
        }
        else
        {
#ifdef _X64
            // stack frame is aligned on a 16 bytes boundary in x64
            buffer[result] = *(be_align(stackPointer, 16)+1);
#else
            buffer[result] = *(stackPointer+1);
#endif
            result++;
        }
        stackPointer = st_next(stackPointer);
    }
    return result;
}

}}
