/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <minitl/stdafx.h>
#include    <minitl/assert.hh>
#include    <stdio.h>
#include    <stdarg.h>

namespace minitl
{

AssertionResult defaultAssertionCallback(const char */*file*/, int /*line*/, const char */*expr*/, const char */*message*/, ...)
{
    return Break;
}

static AssertionCallback_t g_callback = defaultAssertionCallback;
AssertionCallback_t setAssertionCallback(AssertionCallback_t callback)
{
    AssertionCallback_t previous = g_callback;
    g_callback = callback;
    return previous;
}

AssertionCallback_t getAssertionCallback()
{
    return g_callback;
}

}

