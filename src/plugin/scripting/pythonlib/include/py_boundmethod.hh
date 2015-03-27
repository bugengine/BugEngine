/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PYTHONLIB_PY_BOUNDMETHOD_HH_
#define BE_PYTHONLIB_PY_BOUNDMETHOD_HH_
/**************************************************************************************************/
#include    <pythonlib/stdafx.h>
#include    <pythonlib/pythontypes.hh>

namespace BugEngine { namespace Python
{

struct PyBugObject;

struct PyBoundMethod
{
    PyObject                py_object;
    raw<const RTTI::Method> method;
    PyObject*               value;

    static void registerType(PyObject* module);

    static PyObject* create(raw<const RTTI::Method> method, PyBugObject* value);
    static PyObject* repr(PyObject* self);
    static void dealloc(PyObject* self);
    static PyObject* call(PyObject* self, PyObject* args, PyObject* kwds);

    static PyTypeObject s_pyType;
};

}}

/**************************************************************************************************/
#endif
