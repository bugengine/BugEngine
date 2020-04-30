/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <stdafx.h>
#include <bugengine/minitl/assert.hh>
#include <bugengine/plugin.debug.runtime/callstack.hh>
#include <bugengine/plugin.debug.runtime/module.hh>
#include <bugengine/plugin.debug.runtime/symbols.hh>
#include <cstdarg>
#include <cstdio>

namespace BugEngine { namespace Debug {

minitl::AssertionResult AssertionCallback(const char* file, int line, const char* expr,
                                          const char* message)
{
    fprintf(stderr, "%s:%d Assertion failed: %s\n\t", file, line, expr);
    fprintf(stderr, "%s\n", message);

    be_fatal("%s:%d Assertion failed: %s\n\t%s" | file | line | expr | message);

    Runtime::Callstack::Address          address[128];
    size_t                               result = Runtime::Callstack::backtrace(address, 128, 1);
    Runtime::Symbol                      s;
    static ref< const Runtime::Module >  executable = Runtime::Module::self();
    static weak< const Runtime::Module > last       = executable;
    static ref< const Runtime::SymbolResolver > s_symbols
       = executable ? Runtime::SymbolResolver::loadSymbols(executable->getSymbolInformation(),
                                                           ref< const Runtime::SymbolResolver >())
                    : ref< const Runtime::SymbolResolver >();
    while(last && last->next())
    {
        last                                              = last->next();
        Runtime::SymbolResolver::SymbolInformations infos = last->getSymbolInformation();
        s_symbols = Runtime::SymbolResolver::loadSymbols(infos, s_symbols);
    }
    if(s_symbols)
    {
        fprintf(stderr, "Callstack:\n");
        for(Runtime::Callstack::Address* a = address; a < address + result; ++a)
        {
            s_symbols->resolve(*a, s);
            be_info("[%d] %s - %s:%d - %s\n" | s.address() | s.module() | s.filename() | s.line()
                    | s.function());
        }
    }

    return minitl::Break;
}

}}  // namespace BugEngine::Debug
