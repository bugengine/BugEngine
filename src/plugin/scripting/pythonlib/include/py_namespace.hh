/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PYTHONLIB_PY_NAMESPACE_HH_
#define BE_PYTHONLIB_PY_NAMESPACE_HH_
/**************************************************************************************************/
#include    <pythonlib/stdafx.h>
#include    <py_object.hh>

namespace BugEngine { namespace Python
{

struct PyBugNamespace : public PyBugObject
{
    static void registerType(PyObject* module);

    static PyObject* create(PyObject* owner, const RTTI::Value &value);
    static int init(PyObject* self, PyObject* args, PyObject* kwds);
    static PyObject* getattr(PyObject* self, const char* name);
    static int setattr(PyObject* self, const char* name, PyObject* value);
    static PyObject* dir(PyObject* self, PyObject* args);
    static PyObject* repr(PyObject* self);

    static PyTypeObject s_pyType;
    static PyMethodDef s_methods[];
};

}}

/**************************************************************************************************/
#endif
