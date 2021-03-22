/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.scripting.pythonlib/stdafx.h>
#include <bugengine/meta/classinfo.script.hh>
#include <bugengine/plugin.scripting.pythonlib/pythonlib.hh>
#include <py_namespace.hh>

namespace BugEngine { namespace Python {

PyMethodDef PyBugNamespace::s_methods[]
    = {{"__dir__", &PyBugNamespace::dir, METH_NOARGS, NULL}, {NULL, NULL, 0, NULL}};

PyTypeObject PyBugNamespace::s_pyType = {{{0, 0}, 0},
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

PyObject* PyBugNamespace::stealValue(PyObject* owner, Meta::Value& value)
{
    be_assert(value.type().metaclass->type() == Meta::ClassType_Namespace,
              "PyBugNamespace only accepts Namespace types");
    PyObject* result                              = s_pyType.tp_alloc(&s_pyType, 0);
    static_cast< PyBugNamespace* >(result)->owner = owner;

    if(owner)
    {
        Py_INCREF(owner);
    }
    new(&(static_cast< PyBugNamespace* >(result))->value) Meta::Value();
    (static_cast< PyBugNamespace* >(result))->value.swap(value);
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
    PyBugNamespace*    self_ = static_cast< PyBugNamespace* >(self);
    const Meta::Class& klass = self_->value.as< const Meta::Class& >();
    istring            name_(name);
    for(raw< const Meta::ObjectInfo > o = klass.objects; o; o = o->next)
    {
        if(o->name == name_)
        {
            Meta::Value v = o->value;
            return PyBugObject::stealValue(self, v);
        }
    }
    return PyBugObject::getattr(self, name);
}

int PyBugNamespace::setattr(PyObject* self, const char* name, PyObject* value)
{
    PyBugObject*       self_ = static_cast< PyBugObject* >(self);
    istring            name_(name);
    const Meta::Class& klass = self_->value.as< const Meta::Class& >();
    for(raw< const Meta::ObjectInfo > ob = klass.objects; ob; ob = ob->next)
    {
        if(ob->name == name_)
        {
            if(ob->value.type().access != Meta::Type::Const)
            {
                Meta::ConversionCost c = distance(value, ob->value.type());
                if(c < Meta::ConversionCost::s_incompatible)
                {
                    Meta::Value* v = (Meta::Value*)malloca(sizeof(Meta::Value));
                    unpack(value, ob->value.type(), v);
                    ob->value = *v;
                    v->~Value();
                    freea(v);
                    return 0;
                }
                else
                {
                    s_library->m_PyErr_Format(*s_library->m_PyExc_TypeError, "%s.%s is of type %s",
                                              self_->value.type().name().c_str(), name,
                                              ob->value.type().name().c_str());
                    return -1;
                }
            }
            else
            {
                s_library->m_PyErr_Format(*s_library->m_PyExc_TypeError, "%s.%s is const",
                                          self_->value.type().name().c_str(), name);
                return -1;
            }
        }
    }
    s_library->m_PyErr_Format(*s_library->m_PyExc_AttributeError, "%s object has no attribute %s",
                              self_->value.type().name().c_str(), name);
    return -1;
}

PyObject* PyBugNamespace::dir(PyObject* self, PyObject* args)
{
    PyBugObject* self_ = static_cast< PyBugObject* >(self);
    be_forceuse(args);
    PyObject* result = s_library->m_PyList_New(0);
    if(!result) return NULL;
    const Meta::Class&             klass      = self_->value.as< const Meta::Class& >();
    PyString_FromStringAndSizeType fromString = s_library->getVersion() >= 30
                                                    ? s_library->m_PyUnicode_FromStringAndSize
                                                    : s_library->m_PyString_FromStringAndSize;

    for(raw< const Meta::ObjectInfo > o = klass.objects; o; o = o->next)
    {
        PyObject* str = fromString(o->name.c_str(), o->name.size());
        if(!str)
        {
            Py_DECREF(result);
            return NULL;
        }
        if(s_library->m_PyList_Append(result, str) == -1)
        {
            Py_DECREF(str);
            Py_DECREF(result);
            return NULL;
        }
        Py_DECREF(str);
    }
    return result;
}

PyObject* PyBugNamespace::repr(PyObject* self)
{
    PyBugObject*       self_ = static_cast< PyBugObject* >(self);
    const Meta::Value& v     = self_->value;
    const Meta::Class& ns    = v.as< const Meta::Class& >();

    if(s_library->getVersion() >= 30)
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
    result = (*s_library->m_PyModule_AddObject)(module, "Namespace", (PyObject*)&s_pyType);
    be_assert(result >= 0, "unable to register type");
    Meta::Value v = Meta::Value(be_bugengine_Namespace());
    result        = (*s_library->m_PyModule_AddObject)(module, "BugEngine", stealValue(0, v));
    be_assert(result >= 0, "unable to register type");
    be_forceuse(result);
}

}}  // namespace BugEngine::Python
