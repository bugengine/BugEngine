/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_PYTHONLIB_PY_ENUM_HH_
#define BE_PYTHONLIB_PY_ENUM_HH_
/**************************************************************************************************/
#include <bugengine/plugin.scripting.pythonlib/stdafx.h>
#include <py_object.hh>

namespace BugEngine { namespace Python {

struct PyBugEnum : public PyBugObject
{
    static void registerType(PyObject* module);

    static PyObject* stealValue(PyObject* owner, Meta::Value& value);
    static PyObject* repr(PyObject* self);
    static PyObject* str(PyObject* self);
    static PyObject* toint(PyObject* self);
    static PyObject* tolong(PyObject* self);
    static PyObject* tofloat(PyObject* self);
    static int       nonZero(PyObject* self);

    static PyTypeObject                   s_pyType;
    static PyTypeObject::Py2NumberMethods s_py2EnumNumber;
    static PyTypeObject::Py3NumberMethods s_py3EnumNumber;
};

}}  // namespace BugEngine::Python

/**************************************************************************************************/
#endif
