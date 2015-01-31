/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PYTHON_PY_NAMESPACE_HH_
#define BE_PYTHON_PY_NAMESPACE_HH_
/**************************************************************************************************/
#include    <python/stdafx.h>
#include    <py_object.hh>

namespace BugEngine { namespace Python
{

struct PyBugNamespace : public PyBugObject
{
    static void registerType(PyObject* module);

    static PyObject* create(const RTTI::Value &value);
    static int init(PyObject* self, PyObject* args, PyObject* kwds);
    static PyObject* repr(PyObject* self);

    static PyTypeObject s_pyType;
};

}}

/**************************************************************************************************/
#endif
