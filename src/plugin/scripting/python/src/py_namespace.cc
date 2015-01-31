/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <python/stdafx.h>
#include    <python/pythonlib.hh>
#include    <py_namespace.hh>

namespace BugEngine { namespace Python
{

PyTypeObject PyBugNamespace::s_pyType =
{
    { { 0, 0 }, 0 },
    "py_bugengine.Namespace",
    sizeof(PyBugNamespace),
    0,
    &PyBugNamespace::dealloc,
    0,
    &PyBugNamespace::getattr,
    0,
    0,
    &PyBugNamespace::repr,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_IS_ABSTRACT,
    "Wrapper class for the C++ BugEngine namespaces",
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
    &PyBugNamespace::init,
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

PyObject* PyBugNamespace::create(const RTTI::Value& value)
{
    const RTTI::Type& t = value.type();
    be_assert(t.metaclass->type() == RTTI::ClassType_Namespace,
              "PyBugString only accepts Namespace types");
    PyObject* result = s_pyType.tp_alloc(&s_pyType, 0);
    new(&((PyBugNamespace*)result)->value) RTTI::Value(value);
    return result;
}

int PyBugNamespace::init(PyObject* self, PyObject* args, PyObject* kwds)
{
    /* todo */
    be_forceuse(self);
    be_forceuse(args);
    be_forceuse(kwds);
    return 0;
}

PyObject* PyBugNamespace::repr(PyObject *self)
{
    PyBugObject* self_ = reinterpret_cast<PyBugObject*>(self);
    const RTTI::Value& v = self_->value;
    const RTTI::Class& ns = v.as<const RTTI::Class&>();
    
    if (s_library->getVersion() >= 30)
    {
        return s_library->m_PyUnicode_FromFormat("[%s]", ns.fullname().str().name);
    }
    else
    {
        return s_library->m_PyString_FromFormat("[%s]", ns.fullname().str().name);
    }
}

void PyBugNamespace::registerType(PyObject* module)
{
    int result = s_library->m_PyType_Ready(&s_pyType);
    be_assert(result >= 0, "unable to register type");
    Py_INCREF(&s_pyType);
    result = (*s_library->m_PyModule_AddObject)(module, "Array", (PyObject*)&s_pyType);
    be_assert(result >= 0, "unable to register type");
    result = (*s_library->m_PyModule_AddObject)(module, "BugEngine",
                                                create(RTTI::Value(be_game_Namespace())));
    be_assert(result >= 0, "unable to register type");
    be_forceuse(result);
}

}}
