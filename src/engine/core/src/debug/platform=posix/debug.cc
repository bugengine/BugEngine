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
* Boston, MA                                                                  *
* 02110-1301  USA                                                             *
\*****************************************************************************/

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

AssertionCallback_t COREEXPORT setAssertionCallback(AssertionCallback_t callback)
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

