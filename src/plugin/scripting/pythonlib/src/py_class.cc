/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.scripting.pythonlib/stdafx.h>
#include <bugengine/plugin.scripting.pythonlib/pythonlib.hh>
#include <py_class.hh>

namespace BugEngine { namespace Python {

PyTypeObject PyBugClass::s_pyType = {{{0, 0}, 0},
                                     "py_bugengine.Class",
                                     sizeof(PyBugClass),
                                     0,
                                     &PyBugNamespace::dealloc,
                                     0,
                                     &PyBugNamespace::getattr,
                                     &PyBugNamespace::setattr,
                                     0,
                                     &PyBugNamespace::repr,
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
                                     "Wrapper class for the C++ BugEngine namespaces",
                                     0,
                                     0,
                                     0,
                                     0,
                                     0,
                                     0,
                                     PyBugNamespace::s_methods,
                                     0,
                                     0,
                                     &PyBugObject::s_pyType,
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
                                     0,
                                     0};

PyObject* PyBugClass::stealValue(PyObject* owner, Meta::Value& value)
{
    be_assert(value.type().metaclass->type() == Meta::ClassType_Namespace,
              "PyBugClass only accepts Namespace types");
    PyObject* result                          = s_pyType.tp_alloc(&s_pyType, 0);
    static_cast< PyBugClass* >(result)->owner = owner;

    if(owner)
    {
        Py_INCREF(owner);
    }
    new(&(static_cast< PyBugClass* >(result))->value) Meta::Value();
    (static_cast< PyBugClass* >(result))->value.swap(value);
    return result;
}

void PyBugClass::registerType(PyObject* module)
{
    int result = s_library->m_PyType_Ready(&s_pyType);
    be_assert(result >= 0, "unable to register type");
    Py_INCREF(&s_pyType);
    result = (*s_library->m_PyModule_AddObject)(module, "Class", (PyObject*)&s_pyType);
    be_assert(result >= 0, "unable to register type");
    be_forceuse(result);
}

}}  // namespace BugEngine::Python
