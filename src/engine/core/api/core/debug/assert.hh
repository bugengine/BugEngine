/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

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


typedef AssertionResult(*AssertionCallback_t)(const char *filename, int line, const char *expr, const char *message, ...);
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
            r = BugEngine::Debug::getAssertionCallback()(__FILE__,__LINE__,#cond,msg);          \
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
