/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <minitl/assert.hh>
#include    <cstdio>
#include    <cstdarg>

namespace BugEngine { namespace Debug
{

minitl::AssertionResult defaultAssertionCallback(const char *file,
                                                 int        line,
                                                 const char *expr,
                                                 const char *message,
                                                 ...)
{
    fprintf(stderr, "%s:%d Assertion failed: %s\n\t", file, line, expr);
    va_list l;
    va_start(l, message);
    vfprintf(stderr, message, l);
    va_end(l);
    fprintf(stderr, "\n");

    return minitl::Ignore;
}

}}

