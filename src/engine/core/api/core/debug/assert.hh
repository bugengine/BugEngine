/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_DEBUG_ASSERT_HH_
#define BE_CORE_DEBUG_ASSERT_HH_
/*****************************************************************************/
#include <minitl/string/format.hh>
#include <cstdlib>

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
be_api(CORE) AssertionCallback_t setAssertionCallback(AssertionCallback_t callback);
be_api(CORE) AssertionCallback_t getAssertionCallback();

#if !defined(BE_ENABLE_ASSERT)
# define    be_assert_impl_(cond,message,code) ((void)0)
#else
# ifdef      assert
#  undef     assert
# endif
# define    be_assert_impl_(cond,message,code)                                                  \
    do {                                                                                        \
        static bool be_ignore_ = false;                                                         \
        if(!be_ignore_ && !(cond))                                                              \
        {                                                                                       \
            BugEngine::Debug::AssertionResult be_r_;                                            \
            minitl::format<4096> be_msg_ = (minitl::format<4096>)message;                       \
            be_r_ = BugEngine::Debug::getAssertionCallback()(__FILE__,__LINE__,#cond,be_msg_);  \
            switch(be_r_)                                                                       \
            {                                                                                   \
            case BugEngine::Debug::Abort:         std::abort(); break;                          \
            case BugEngine::Debug::IgnoreAll:     be_ignore_ = true; break;                     \
            case BugEngine::Debug::Break:         be_break(); break;                            \
            default:;                                                                           \
            }                                                                                   \
            code;                                                                               \
        }                                                                                       \
    } while (0)
#endif

#define be_assert(cond,message)                 be_assert_impl_(cond,message,)
#define be_assert_recover(cond,message,code)    be_assert_impl_(cond,message,code)
#define be_unimplemented()                      be_assert_impl_(!"implemented", "not implemented",)
#define be_notreached()                         be_assert_impl_(!"reached", "should not reach code",)

}}

/*****************************************************************************/
#endif
