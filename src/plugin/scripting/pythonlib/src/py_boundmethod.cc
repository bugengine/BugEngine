/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <pythonlib/stdafx.h>
#include    <pythonlib/pythonlib.hh>
#include    <py_boundmethod.hh>
#include    <py_call.hh>
#include    <py_object.hh>
#include    <rtti/engine/methodinfo.script.hh>

namespace BugEngine { namespace Python
{

PyTypeObject PyBoundMethod::s_pyType =
{
    { { 0, 0 }, 0 },
    "py_bugengine.BoundMethod",
    sizeof(PyBoundMethod),
    0,
    &PyBoundMethod::dealloc,
    0,
    0,
    0,
    0,
    &PyBoundMethod::repr,
    0,
    0,
    0,
    0,
    &PyBoundMethod::call,
    0,
    0,
    0,
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_IS_ABSTRACT,
    "Wrapper class for bound methods to C++ methods",
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
    0
};

PyObject* PyBoundMethod::create(raw<const RTTI::Method> method, PyBugObject* value)
{
    PyBoundMethod* result = reinterpret_cast<PyBoundMethod*>(s_pyType.tp_alloc(&s_pyType, 0));
    result->method = method;
    result->value = static_cast<PyObject*>(value);
    Py_INCREF(result->value);
    return reinterpret_cast<PyObject*>(result);
}

PyObject* PyBoundMethod::repr(PyObject *self)
{
    PyBoundMethod* self_ = reinterpret_cast<PyBoundMethod*>(self);
    if (s_library->getVersion() >= 30)
    {
        return s_library->m_PyUnicode_FromFormat("[BoundMethod %p.%s]",
                                                 self_, self_->method->name.c_str());
    }
    else
    {
        return s_library->m_PyString_FromFormat("[BoundMethod %p.%s]",
                                                self_, self_->method->name.c_str());
    }
}

void PyBoundMethod::dealloc(PyObject* self)
{
    PyBoundMethod* self_ = reinterpret_cast<PyBoundMethod*>(self);
    Py_DECREF(self_->value);
    self->py_type->tp_free(self);
}

PyObject* PyBoundMethod::call(PyObject* self, PyObject* args, PyObject* kwds)
{
    PyBoundMethod* self_ = reinterpret_cast<PyBoundMethod*>(self);
    return Python::call(self_->method, self_->value, args, kwds);
}

void PyBoundMethod::registerType(PyObject* module)
{
    int result = s_library->m_PyType_Ready(&s_pyType);
    be_assert(result >= 0, "unable to register type");
    Py_INCREF(&s_pyType);
    result = (*s_library->m_PyModule_AddObject)(module, "BoundMethod", (PyObject*)&s_pyType);
    be_assert(result >= 0, "unable to register type");
    be_forceuse(result);
}


}}
