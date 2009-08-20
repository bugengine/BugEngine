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
# define    Assert(cond) ((void)0)
# define    AssertMsg(cond,message) ((void)0)
# define    AssertNotReached() ((void)0)
#else
# ifdef      assert
#  undef     assert
# endif      /*assert*/
# define    Assert(cond)                                                                        \
    {                                                                                           \
        static bool ignore = false;                                                             \
        if(!ignore && !(cond))                                                                  \
        {                                                                                       \
            BugEngine::Debug::AssertionResult r;                                                \
            r = BugEngine::Debug::getAssertionCallback()(__FILE__,__LINE__,#cond);              \
            switch(r)                                                                           \
            {                                                                                   \
                case BugEngine::Debug::Abort:         std::abort(); break;                      \
            case BugEngine::Debug::IgnoreAll:     ignore = true; break;                         \
            case BugEngine::Debug::Break:         BREAKPOINT; break;                            \
            default:;                                                                           \
            }                                                                                   \
        }                                                                                       \
    }
# define    AssertMsg(cond,message)                                                             \
    {                                                                                           \
        static bool ignore = false;                                                             \
        if(!ignore && !(cond))                                                                  \
        {                                                                                       \
            BugEngine::Debug::AssertionResult r;                                                \
            r = BugEngine::Debug::getAssertionCallback()(__FILE__,__LINE__,message);            \
            switch(r)                                                                           \
            {                                                                                   \
            case BugEngine::Debug::Abort:         std::abort(); break;                          \
            case BugEngine::Debug::IgnoreAll:     ignore = true; break;                         \
            case BugEngine::Debug::Break:         BREAKPOINT; break;                            \
            default:;                                                                           \
            }                                                                                   \
        }                                                                                       \
    }
# define    AssertNotReached()                                                                  \
    {                                                                                           \
        static bool ignore = false;                                                             \
        if(!ignore)                                                                             \
        {                                                                                       \
            BugEngine::Debug::AssertionResult r;                                                \
            r = BugEngine::Debug::getAssertionCallback()(__FILE__,__LINE__,"Unreachable code"); \
            switch(r)                                                                           \
            {                                                                                   \
            case BugEngine::Debug::Abort:         std::abort(); break;                          \
            case BugEngine::Debug::IgnoreAll:     ignore = true; break;                         \
            case BugEngine::Debug::Break:         BREAKPOINT; break;                            \
            default:;                                                                           \
            }                                                                                   \
        }                                                                                       \
    }
#endif

}}

/*****************************************************************************/
#endif
