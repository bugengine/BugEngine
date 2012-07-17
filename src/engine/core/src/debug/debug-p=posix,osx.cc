/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
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
    static ref<const Runtime::Module> executable = Runtime::Module::self();
    static weak<const Runtime::Module> last = executable;
    static ref<const Runtime::SymbolResolver> s_symbols = executable ? Runtime::SymbolResolver::loadSymbols(executable->getSymbolInformation(), ref<const Runtime::SymbolResolver>()) : ref<const Runtime::SymbolResolver>();
    while (last && last->next())
    {
        last = last->next();
        Runtime::SymbolResolver::SymbolInformations infos = last->getSymbolInformation();
        s_symbols = Runtime::SymbolResolver::loadSymbols(infos, s_symbols);
    }
    if (s_symbols)
    {
        fprintf(stderr, "Callstack:\n");
        for (Runtime::Callstack::Address* a = address; a < address+result; ++a)
        {
            s_symbols->resolve(*a, s);
            be_info("[%d] %s - %s:%d - %s\n" | s.address() | s.module() | s.filename() | s.line() | s.function());
        }
    }

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

}}

