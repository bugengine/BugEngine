/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_MINITL_ASSERT_HH_
#define BE_MINITL_ASSERT_HH_
/**************************************************************************************************/
#include    <minitl/format.hh>
#include    <cstdlib>

namespace minitl
{

enum AssertionResult
{
    Abort,
    Ignore,
    IgnoreAll,
    Break
};


typedef AssertionResult(*AssertionCallback_t)(const char *filename, int line, const char *expr, const char *message);
be_api(MINITL) AssertionCallback_t setAssertionCallback(AssertionCallback_t callback);
be_api(MINITL) AssertionCallback_t getAssertionCallback();

#if !(BE_ENABLE_ASSERT)
# define    be_assert_impl_(cond,message,code) ((void)0)
#else
# ifdef      assert
#  undef     assert
# endif
# define    be_assert_impl_(cond,message,code)                                                              \
    do {                                                                                                    \
        static bool be_ignore_ = false;                                                                     \
        if (!be_ignore_ && !(cond))                                                                         \
        {                                                                                                   \
            minitl::AssertionResult be_r_;                                                                  \
            be_r_ = minitl::getAssertionCallback()(__FILE__,__LINE__,#cond,(minitl::format<4096>)message);  \
            switch(be_r_)                                                                                   \
            {                                                                                               \
            case minitl::Abort:         std::abort(); break;                                                \
            case minitl::IgnoreAll:     be_ignore_ = true; break;                                           \
            case minitl::Break:         be_break(); break;                                                  \
            default:;                                                                                       \
            }                                                                                               \
            code;                                                                                           \
        }                                                                                                   \
    } while (0)
#endif

#define be_assert(cond,message)                 be_assert_impl_(cond,message,)
#define be_assert_recover(cond,message,code)    be_assert_impl_(cond,message,code)
#define be_unimplemented()                      be_assert_impl_(!"implemented", "not implemented",)
#define be_notreached()                         be_assert_impl_(!"reached", "should not reach code",)

}

/**************************************************************************************************/
#endif
