/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <python/stdafx.h>
#include    <python/pythonlib.hh>
#include    <py_plugin.hh>

namespace BugEngine { namespace Python
{

static PyTypeObject s_bugenginePluginType =
{
    { { 0, 0 }, 0 },
    "py_bugengine.Plugin",
    sizeof(PyBugPlugin),
    0,
    0, /* dealloc */
    0,
    0, /* getattr */
    0, /* setattr */
    0,
    0,
    0,
    0,
    0,
    0,
    0, /* call */
    0,
    0,
    0,
    0,
    0, /* flags */
    "BugEngine::Plugin::Plugin",
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
    
};

void PyBugPlugin::registerType(weak<PythonLibrary> library, PyObject* module)
{
    int result = library->m_PyType_Ready(&s_bugenginePluginType);
    be_assert(result >= 0, "unable to register type");
    be_forceuse(result);
    be_forceuse(module);
}

}}
