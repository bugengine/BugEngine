/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_PYTHONLIB_PY_ARRAY_HH_
#define BE_PYTHONLIB_PY_ARRAY_HH_
/**************************************************************************************************/
#include <bugengine/plugin.scripting.pythonlib/stdafx.h>
#include <bugengine/meta/engine/methodinfo.script.hh>
#include <py_object.hh>

namespace BugEngine { namespace Python {

struct PyBugArray : public PyBugObject
{
    static void registerType(PyObject* module);

    static PyObject*    stealValue(PyObject* owner, Meta::Value& value);
    static PyObject*    repr(PyObject* self);
    static int          nonZero(PyObject* self);
    static Py_ssize_t   length(PyObject* self);
    static PyObject*    item(PyObject* self, Py_ssize_t index);
    static int          setItem(PyObject* self, Py_ssize_t index, PyObject* value);
    static PyTypeObject s_pyType;
};

}}  // namespace BugEngine::Python

/**************************************************************************************************/
#endif
