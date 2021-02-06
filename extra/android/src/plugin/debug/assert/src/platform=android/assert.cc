/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <stdafx.h>
#include <bugengine/minitl/assert.hh>
#include <stdarg.h>
#include <stdio.h>

namespace BugEngine { namespace Debug {

minitl::AssertionResult AssertionCallback(const char* file, int line, const char* expr,
                                          const char* message)
{
    fprintf(stderr, "%s:%d Assertion failed: %s\n\t", file, line, expr);
    fprintf(stderr, "%s\n", message);

    return minitl::Break;
}

}}  // namespace BugEngine::Debug
