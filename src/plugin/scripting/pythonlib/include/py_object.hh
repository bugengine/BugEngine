/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PYTHONLIB_PY_OBJECT_HH_
#define BE_PYTHONLIB_PY_OBJECT_HH_
/**************************************************************************************************/
#include    <pythonlib/stdafx.h>
#include    <pythonlib/pythontypes.hh>
#include    <rtti/value.hh>

namespace BugEngine { namespace Python
{

struct PyBugObject : public PyObject
{
    PyObject* owner;
    RTTI::Value value;

    static void registerType(PyObject* module);

    static PyObject* create(PyObject* owner, const RTTI::Value& value);
    static PyObject* newinst(PyTypeObject* type, PyObject* args, PyObject* kwargs);
    static u32 distance(PyObject* object, const RTTI::Type& desiredType);
    static void unpack(PyObject* object, const RTTI::Type& desiredType, RTTI::Value* buffer);
    static void unpackAny(PyObject* object, RTTI::Value* buffer);
    static PyObject* getattr(PyObject* self, const char* name);
    static int setattr(PyObject* self, const char* name, PyObject* value);
    static PyObject* repr(PyObject* self);
    static void dealloc(PyObject* self);
    static PyObject* call(PyObject* self, PyObject* args, PyObject* kwds);
    static int nonZero(PyObject* self);

    static PyTypeObject s_pyType;
};

}}

/**************************************************************************************************/
#endif
