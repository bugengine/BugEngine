/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <stdafx.h>

#include    <python/context.hh>
#include    <python/pythonlib.hh>
#include    <plugin/plugin.hh>
#include    <rtti/engine/namespace.hh>

static minitl::ref<BugEngine::Python::Context> create(const BugEngine::Plugin::Context &context)
{
    static ref<BugEngine::Python::PythonLibrary> library =
                ref<BugEngine::Python::PythonLibrary>::create(BugEngine::Arena::general(), PYTHON_LIBRARY);
    if (library->operator !())
    {
        return minitl::ref<BugEngine::Python::Context>();
    }
    else
    {
        return minitl::ref<BugEngine::Python::Context>::create(BugEngine::Arena::general(),
                                                               context,
                                                               library);
    }
}

BE_PLUGIN_REGISTER_CREATE(&create);
