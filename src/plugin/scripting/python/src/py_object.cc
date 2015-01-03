/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <python/stdafx.h>
#include    <python/pythonlib.hh>
#include    <py_object.hh>

namespace BugEngine { namespace Python
{

static PyTypeObject s_bugengineValueType =
{
    { { 0, 0 }, 0 },
    "py_bugengine.Value",
    sizeof(PyBugObject),
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
    "BugEngine::RTTI::Value",
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

void PyBugObject::registerType(weak<PythonLibrary> library, PyObject* module)
{
    int result = library->m_PyType_Ready(&s_bugengineValueType);
    be_assert(result >= 0, "unable to register type");
    be_forceuse(result);
    be_forceuse(module);
}

}}
