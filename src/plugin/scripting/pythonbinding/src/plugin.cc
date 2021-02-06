/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <stdafx.h>

#include <bugengine/plugin.scripting.python/context.hh>
#include <bugengine/plugin.scripting.pythonlib/pythonlib.hh>
#include <bugengine/plugin/plugin.hh>
#include <bugengine/rtti/engine/namespace.hh>

#ifndef PYTHON_LIBRARY
#    error PYTHON_LIBRARY must be defined to the library name for this module
#endif

static minitl::ref< BugEngine::Python::Context > create(const BugEngine::Plugin::Context& context)
{
    using namespace BugEngine::Python;
    ref< PythonLibrary > library = loadPython(BE_STRINGIZE(PYTHON_LIBRARY));
    if(!library)
    {
        return minitl::ref< Context >();
    }
    else
    {
        return minitl::ref< Context >::create(BugEngine::Arena::general(), context, library);
    }
}

BE_PLUGIN_REGISTER_CREATE(&create);
