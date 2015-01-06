/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PYTHON_PY_OBJECT_HH_
#define BE_PYTHON_PY_OBJECT_HH_
/**************************************************************************************************/
#include    <python/stdafx.h>
#include    <python/pythontypes.hh>
#include    <rtti/value.hh>

namespace BugEngine { namespace Python
{

struct PyBugObject
{
    PyObject    py_object;
    RTTI::Value value;

    static void registerType(PyObject* module);

    static PyObject* create(const RTTI::Value& value);
    static int init(PyObject* self, PyObject* args, PyObject* kwds);
    static PyObject* getattr(PyObject* self, const char* name);
    static int setattr(PyObject* self, const char* name, PyObject* value);
    static void dealloc(PyObject* self);
    static PyObject* call(PyObject* self, PyObject* args, PyObject* kwds);
};

}}

/**************************************************************************************************/
#endif
