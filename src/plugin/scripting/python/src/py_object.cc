/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <python/stdafx.h>
#include    <python/pythonlib.hh>
#include    <py_object.hh>

namespace BugEngine { namespace Python
{

static PyTypeObject s_bugengineValueType =
{
    { { 0, 0 }, 0 },
    "py_bugengine.Value",
    sizeof(PyBugObject),
    0,
    &PyBugObject::dealloc,
    0,
    &PyBugObject::getattr,
    &PyBugObject::setattr,
    0,
    0,
    0,
    0,
    0,
    0,
    &PyBugObject::call,
    0,
    0,
    0,
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_IS_ABSTRACT,
    "Wrapper class for the C++ class BugEngine::RTTI::Value",
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    &PyBugObject::init,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
};

PyObject* PyBugObject::create(const RTTI::Value& value)
{
    PyObject* result = s_bugengineValueType.tp_alloc(&s_bugengineValueType, 0);
    new(&((PyBugObject*)result)->value) RTTI::Value(value);
    return result;
}

int PyBugObject::init(PyObject* self, PyObject* args, PyObject* kwds)
{
    be_forceuse(self);
    be_forceuse(args);
    be_forceuse(kwds);
    return 0;
}

PyObject* PyBugObject::getattr(PyObject* self, const char* name)
{
    be_forceuse(self);
    be_forceuse(name);
    return 0;
}

int PyBugObject::setattr(PyObject* self, const char* name, PyObject* value)
{
    be_forceuse(self);
    be_forceuse(name);
    be_forceuse(value);
    return 0;
}

void PyBugObject::dealloc(PyObject* self)
{
    be_forceuse(self);
    PyBugObject* self_ = reinterpret_cast<PyBugObject*>(self);
    self_->value.~Value();
    self->py_type->tp_free(self);
}

PyObject* PyBugObject::call(PyObject* self, PyObject* args, PyObject* kwds)
{
    be_forceuse(self);
    be_forceuse(args);
    be_forceuse(kwds);
    return 0;
}

void PyBugObject::registerType(PyObject* module)
{
    int result = s_library->m_PyType_Ready(&s_bugengineValueType);
    be_assert(result >= 0, "unable to register type");
    be_forceuse(result);
    Py_INCREF(&s_bugengineValueType);
    result = (*s_library->m_PyModule_AddObject)(module, "Value", (PyObject*)&s_bugengineValueType);
    be_assert(result >= 0, "unable to register type");
    be_forceuse(result);
}

}}
