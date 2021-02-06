/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_PYTHONLIB_PY_OBJECT_HH_
#define BE_PYTHONLIB_PY_OBJECT_HH_
/**************************************************************************************************/
#include <bugengine/plugin.scripting.pythonlib/stdafx.h>
#include <bugengine/plugin.scripting.pythonlib/pythontypes.hh>
#include <bugengine/rtti/value.hh>

namespace BugEngine { namespace Python {

struct PyBugObject : public PyObject
{
    PyObject*   owner;
    RTTI::Value value;

    static void registerType(PyObject* module);

    static PyObject*            stealValue(PyObject* owner, RTTI::Value& value);
    static PyObject*            newinst(PyTypeObject* type, PyObject* args, PyObject* kwargs);
    static PyObject*            dir(raw< const RTTI::Class > metaclass);
    static PyObject*            dir(PyObject* self, PyObject* args);
    static RTTI::ConversionCost distance(PyObject* object, const RTTI::Type& desiredType);
    static void      unpack(PyObject* object, const RTTI::Type& desiredType, void* buffer);
    static void      unpackAny(PyObject* object, void* buffer);
    static PyObject* getattr(PyObject* self, const char* name);
    static int       setattr(PyObject* self, const char* name, PyObject* value);
    static PyObject* repr(PyObject* self);
    static void      dealloc(PyObject* self);
    static PyObject* call(PyObject* self, PyObject* args, PyObject* kwds);
    static int       nonZero(PyObject* self);

    static PyTypeObject s_pyType;
    static PyMethodDef  s_methods[];
};

}}  // namespace BugEngine::Python

/**************************************************************************************************/
#endif
