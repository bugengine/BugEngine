/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PYTHON_PY_NUMBER_HH_
#define BE_PYTHON_PY_NUMBER_HH_
/**************************************************************************************************/
#include    <python/stdafx.h>
#include    <py_object.hh>

namespace BugEngine { namespace Python
{

template< typename T >
struct PyBugNumber : public PyBugObject
{
    static void registerType(PyObject* module);

    static PyObject* create(const RTTI::Value &value);
    static int init(PyObject* self, PyObject* args, PyObject* kwds);
    static PyObject* repr(PyObject* self);
    static PyObject* str(PyObject* self);
    static PyObject* toint(PyObject* self);
    static PyObject* tolong(PyObject* self);
    static PyObject* tofloat(PyObject* self);
    static int nonZero(PyObject* self);

    static PyTypeObject s_pyType;
    static PyTypeObject::Py2NumberMethods s_py2NumberNumber;
    static PyTypeObject::Py3NumberMethods s_py3NumberNumber;
};

}}

/**************************************************************************************************/
#endif
