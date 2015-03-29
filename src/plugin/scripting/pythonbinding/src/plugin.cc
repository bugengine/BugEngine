/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <stdafx.h>

#include    <python/context.hh>
#include    <pythonlib/pythonlib.hh>
#include    <plugin/plugin.hh>
#include    <rtti/engine/namespace.hh>

static minitl::ref<BugEngine::Python::Context> create(const BugEngine::Plugin::Context &context)
{
    using namespace BugEngine::Python;
    ref<PythonLibrary> library = loadPython(PYTHON_LIBRARY);
    if (!library)
    {
        return minitl::ref<Context>();
    }
    else
    {
        return minitl::ref<Context>::create(BugEngine::Arena::general(),
                                            context,
                                            library);
    }
}

BE_PLUGIN_REGISTER_CREATE(&create);
