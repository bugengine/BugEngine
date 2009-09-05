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

#ifndef BE_CORE_DEBUG_HH_
#define BE_CORE_DEBUG_HH_
/*****************************************************************************/

#include <cstdlib>
#if defined(BE_COMPILER_MSVC)
# include <intrin.h>
# define BREAKPOINT __debugbreak()
#elif defined(BE_COMPILER_GCC)
# define BREAKPOINT __asm("int3")
#endif

namespace BugEngine { namespace Debug
{

enum AssertionResult
{
    Abort,
    Ignore,
    IgnoreAll,
    Break
};


typedef AssertionResult(*AssertionCallback_t)(const char *filename, int line, const char *message, ...);
COREEXPORT AssertionCallback_t setAssertionCallback(AssertionCallback_t callback);
COREEXPORT AssertionCallback_t getAssertionCallback();

#if !defined(BE_ENABLE_ASSERT)
# define    be_assert(cond,message) ((void)0)
# define    be_unimplemented() ((void)0)
# define    be_notreached() ((void)0)
#else
# ifdef      assert
#  undef     assert
# endif
# define    be_assert(cond,message)                                                             \
    do {                                                                                        \
        static bool ignore = false;                                                             \
        if(!ignore && !(cond))                                                                  \
        {                                                                                       \
            BugEngine::Debug::AssertionResult r;                                                \
            minitl::format<4096> msg = (minitl::format<4096>)message;                           \
            r = BugEngine::Debug::getAssertionCallback()(__FILE__,__LINE__,msg);                \
            switch(r)                                                                           \
            {                                                                                   \
            case BugEngine::Debug::Abort:         std::abort(); break;                          \
            case BugEngine::Debug::IgnoreAll:     ignore = true; break;                         \
            case BugEngine::Debug::Break:         BREAKPOINT; break;                            \
            default:;                                                                           \
            }                                                                                   \
        }                                                                                       \
    } while (0)
#endif

#define be_unimplemented()  be_assert(false, "not implemented")
#define be_notreached()     be_assert(false, "should not reach code")

}}

/*****************************************************************************/
#endif
