/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/debug/assert.hh>
#include    <core/runtime/callstack.hh>
#include    <core/runtime/module.hh>
#include    <core/runtime/symbols.hh>
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

    Runtime::Callstack::Address address[128];
    size_t result = Runtime::Callstack::backtrace(address, 128, 1);
    Runtime::Symbol s;

    static weak<const Runtime::Module> executable;
    static ref<const Runtime::SymbolResolver> s_symbols;
    if(!executable)
    {
        executable = Runtime::Module::self();
        Runtime::SymbolResolver::SymbolInformations infos = executable->getSymbolInformation();
        s_symbols = Runtime::SymbolResolver::loadSymbols(infos, s_symbols);
    }
    while(executable->next())
    {
        executable = executable->next();
        Runtime::SymbolResolver::SymbolInformations infos = executable->getSymbolInformation();
        s_symbols = Runtime::SymbolResolver::loadSymbols(infos, s_symbols);
    }
    if(s_symbols)
    {
        fprintf(stderr, "Callstack:\n");
        for(Runtime::Callstack::Address* a = address; a < address+result; ++a)
        {
            s_symbols->resolve(*a, s);
            fprintf(stderr, "[%p] %s - %s:%d - %s\n", (void*)s.address(), s.module(), s.filename(), s.line(), s.function());
        }
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

