/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_PYTHONLIB_PY_CLASS_HH_
#define BE_PYTHONLIB_PY_CLASS_HH_
/**************************************************************************************************/
#include <bugengine/plugin.scripting.pythonlib/stdafx.h>
#include <py_namespace.hh>

namespace BugEngine { namespace Python {

struct PyBugClass : public PyBugNamespace
{
    static PyObject* stealValue(PyObject* owner, RTTI::Value& value);

    static void         registerType(PyObject* module);
    static PyTypeObject s_pyType;
};

}}  // namespace BugEngine::Python

/**************************************************************************************************/
#endif
