/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <python/stdafx.h>
#include    <python/pythonlib.hh>
#include    <py_namespace.hh>
#include    <rtti/classinfo.script.hh>

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
    &PyBugNamespace::setattr,
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
    be_assert(value.type().metaclass->type() == RTTI::ClassType_Namespace,
              "PyBugNamespace only accepts Namespace types");
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

PyObject* PyBugNamespace::getattr(PyObject* self, const char* name)
{
    PyBugNamespace* self_ = reinterpret_cast<PyBugNamespace*>(self);
    const RTTI::Class& klass = self_->value.as<const RTTI::Class&>();
    istring name_(name);
    for (raw<const RTTI::ObjectInfo> o = klass.objects; o; o = o->next)
    {
        if (o->name == name_)
        {
            return PyBugObject::create(o->value);
        }
    }
    return PyBugObject::getattr(self, name);
}

int PyBugNamespace::setattr(PyObject* self, const char* name, PyObject* value)
{
    PyBugObject* self_ = reinterpret_cast<PyBugObject*>(self);
    istring name_(name);
    const RTTI::Class& klass = self_->value.as<const RTTI::Class&>();
    for (raw<const RTTI::ObjectInfo> ob = klass.objects; ob; ob = ob->next)
    {
        if (ob->name == name_)
        {
            if (ob->value.type().access != RTTI::Type::Const)
            {
                u32 d = distance(value, ob->value.type());
                if (d < RTTI::Type::MaxTypeDistance)
                {
                    RTTI::Value* v = (RTTI::Value*)malloca(sizeof(RTTI::Value));
                    unpack(value, ob->value.type(), v);
                    ob->value = *v;
                    v->~Value();
                    freea(v);
                    return 0;
                }
                else
                {
                    s_library->m_PyErr_Format(*s_library->m_PyExc_TypeError,
                                              "%s.%s is of type %s",
                                              self_->value.type().name().c_str(),
                                              name, ob->value.type().name().c_str());
                    return -1;
                }
            }
            else
            {
                s_library->m_PyErr_Format(*s_library->m_PyExc_TypeError,
                                          "%s.%s is const",
                                          self_->value.type().name().c_str(),
                                          name);
                return -1;
            }
        }
    }
    s_library->m_PyErr_Format(*s_library->m_PyExc_AttributeError,
                              "%s object has no attribute %s",
                              self_->value.type().name().c_str(), name);
    return -1;
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
