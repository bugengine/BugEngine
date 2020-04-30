/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/minitl/stdafx.h>
#include <bugengine/minitl/assert.hh>
#include <stdarg.h>
#include <stdio.h>

namespace minitl {

AssertionResult defaultAssertionCallback(const char* /*file*/, int /*line*/, const char* /*expr*/,
                                         const char* /*message*/)
{
    return Break;
}

static AssertionCallback_t g_callback = defaultAssertionCallback;
AssertionCallback_t        setAssertionCallback(AssertionCallback_t callback)
{
    AssertionCallback_t previous = g_callback;
    g_callback                   = callback;
    return previous;
}

AssertionCallback_t getAssertionCallback()
{
    return g_callback;
}

}  // namespace minitl
