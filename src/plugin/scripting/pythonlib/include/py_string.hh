/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_PYTHONLIB_PY_STRING_HH_
#define BE_PYTHONLIB_PY_STRING_HH_
/**************************************************************************************************/
#include <bugengine/plugin.scripting.pythonlib/stdafx.h>
#include <py_object.hh>

namespace BugEngine { namespace Python {

template < typename T >
struct PyBugString : public PyBugObject
{
    static void registerType(PyObject* module);

    static PyObject* stealValue(PyObject* owner, Meta::Value& value);
    static int       init(PyObject* self, PyObject* args, PyObject* kwds);
    static PyObject* repr(PyObject* self);
    static PyObject* str(PyObject* self);
    static int       nonZero(PyObject* self);

    static PyTypeObject                   s_pyType;
    static PyTypeObject::Py2NumberMethods s_py2StringNumber;
    static PyTypeObject::Py3NumberMethods s_py3StringNumber;
};

}}  // namespace BugEngine::Python

/**************************************************************************************************/
#endif
