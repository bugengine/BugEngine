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
#include    <core/log/debug.hh>
#include <iostream>

#include    <stdio.h>
#include    <execinfo.h>
#if defined(_MSC_VER)
# include <intrin.h>
# define BREAKPOINT __debugbreak()
#elif defined(__GNUC__)
# define BREAKPOINT __asm("int3")
#endif


namespace BugEngine
{

bool defaultAssertionCallback( const char *file,
                               int        line,
                               const char *message,
                               ...)
{
    UNUSED(file);
    UNUSED(line);
    va_list l;
    va_start(l,message);
    (void)vfprintf(stderr,message,l);
    va_end(l);

    const int max_adresses = 196;
    void* adresses[max_adresses];
    int size = backtrace(adresses, max_adresses);
    backtrace_symbols_fd(adresses, size, 0);
    return true;
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

}
