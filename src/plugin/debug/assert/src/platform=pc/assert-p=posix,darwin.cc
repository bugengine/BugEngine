/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <minitl/assert.hh>
#include    <runtime/callstack.hh>
#include    <runtime/module.hh>
#include    <runtime/symbols.hh>
#include    <cstdio>
#include    <cstdarg>

namespace BugEngine { namespace Debug
{

minitl::AssertionResult AssertionCallback(const char *file,
                                          int        line,
                                          const char *expr,
                                          const char *message)
{
    fprintf(stderr, "%s:%d Assertion failed: %s\n\t", file, line, expr);
    fprintf(stderr, "%s\n", message);

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

    return minitl::Break;
}

}}

