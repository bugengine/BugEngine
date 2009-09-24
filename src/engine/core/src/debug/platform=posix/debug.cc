/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/debug/assert.hh>
#include    <core/debug/callstack.hh>
#include    <core/debug/symbols.hh>
#include    <cstdio>
#include    <cstdarg>

namespace BugEngine { namespace Debug
{

AssertionResult defaultAssertionCallback( const char *file,
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
    Callstack::Address buffer[1024];
    size_t backtraceSize = Callstack::backtrace(buffer, 1024, 1);
    Symbols::Symbol s;
    const Symbols& symbols = Symbols::runningSymbols();

    for(size_t i = 0; i < backtraceSize; ++i)
    {
        symbols.resolve(buffer[i], s);
        fprintf(stderr, "%s:%d - %s\n", s.filename(), s.line(), s.function());
    }
    return Break;
}

static AssertionCallback_t g_callback = defaultAssertionCallback;

AssertionCallback_t be_api(CORE) setAssertionCallback(AssertionCallback_t callback)
{
    AssertionCallback_t previous = g_callback;
    g_callback = callback;
    return previous;
}

AssertionCallback_t getAssertionCallback()
{
    return g_callback;
}

}}

