/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PYTHONLIB_PY_ARRAY_HH_
#define BE_PYTHONLIB_PY_ARRAY_HH_
/**************************************************************************************************/
#include    <pythonlib/stdafx.h>
#include    <py_object.hh>

namespace BugEngine { namespace Python
{

struct PyBugArray : public PyBugObject
{
    static void registerType(PyObject* module);
    
    static PyObject* create(PyObject* owner, const RTTI::Value &value);
    static PyObject* repr(PyObject* self);
    static int nonZero(PyObject* self);

    static PyTypeObject s_pyType;
};

}}

/**************************************************************************************************/
#endif
