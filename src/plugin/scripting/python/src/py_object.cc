/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <python/stdafx.h>
#include    <python/pythonlib.hh>
#include    <py_object.hh>
#include    <py_call.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/engine/methodinfo.script.hh>

namespace BugEngine { namespace Python
{

PyTypeObject PyBugObject::s_pyType =
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
    &PyBugObject::repr,
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
    const RTTI::Type& t = value.type();
    PyObject* result;
    if (!value)
    {
        result = s_library->m__Py_NoneStruct;
        Py_INCREF(result);
    }
    else if (t.metaclass->type() == RTTI::ClassType_Integer)
    {
        switch(t.metaclass->index())
        {
        case 0:
            be_assert(be_typeid<bool>::klass() == t.metaclass,
                      "mismatching index for class %s: mistaken for %s" | t.metaclass->fullname() | be_typeid<bool>::klass()->fullname());
            result = s_library->m_PyBool_FromLong(value.as<bool>());
            break;
        case 1:
            be_assert(be_typeid<u8>::klass() == t.metaclass,
                      "mismatching index for class %s: mistaken for %s" | t.metaclass->fullname() | be_typeid<u8>::klass()->fullname());
            result = s_library->m_PyLong_FromLong(value.as<u8>());
            break;
        case 2:
            be_assert(be_typeid<u16>::klass() == t.metaclass,
                      "mismatching index for class %s: mistaken for %s" | t.metaclass->fullname() | be_typeid<u16>::klass()->fullname());
            result = s_library->m_PyLong_FromLong(value.as<u16>());
            break;
        case 3:
            be_assert(be_typeid<u32>::klass() == t.metaclass,
                      "mismatching index for class %s: mistaken for %s" | t.metaclass->fullname() | be_typeid<u32>::klass()->fullname());
            result = s_library->m_PyLong_FromLong(value.as<u32>());
            break;
        case 4:
            be_assert(be_typeid<u64>::klass() == t.metaclass,
                      "mismatching index for class %s: mistaken for %s" | t.metaclass->fullname() | be_typeid<u64>::klass()->fullname());
            result = s_library->m_PyLong_FromSsize_t(be_checked_numcast<Py_ssize_t>(value.as<u64>()));
            break;
        case 5:
            be_assert(be_typeid<i8>::klass() == t.metaclass,
                      "mismatching index for class %s: mistaken for %s" | t.metaclass->fullname() | be_typeid<i8>::klass()->fullname());
            result = s_library->m_PyLong_FromLong(value.as<i8>());
            break;
        case 6:
            be_assert(be_typeid<i16>::klass() == t.metaclass,
                      "mismatching index for class %s: mistaken for %s" | t.metaclass->fullname() | be_typeid<i16>::klass()->fullname());
            result = s_library->m_PyLong_FromLong(value.as<i16>());
            break;
        case 7:
            be_assert(be_typeid<i32>::klass() == t.metaclass,
                      "mismatching index for class %s: mistaken for %s" | t.metaclass->fullname() | be_typeid<i32>::klass()->fullname());
            result = s_library->m_PyLong_FromLong(value.as<i32>());
            break;
        case 8:
            be_assert(be_typeid<i64>::klass() == t.metaclass,
                      "mismatching index for class %s: mistaken for %s" | t.metaclass->fullname() | be_typeid<i64>::klass()->fullname());
            result = s_library->m_PyLong_FromLong(be_checked_numcast<Py_ssize_t>(value.as<i64>()));
            break;
        case 9:
            be_assert(be_typeid<float>::klass() == t.metaclass,
                      "mismatching index for class %s: mistaken for %s" | t.metaclass->fullname() | be_typeid<float>::klass()->fullname());
            result = s_library->m_PyFloat_FromDouble(value.as<float>());
            break;
        case 10:
            be_assert(be_typeid<double>::klass() == t.metaclass,
                      "mismatching index for class %s: mistaken for %s" | t.metaclass->fullname() | be_typeid<double>::klass()->fullname());
            result = s_library->m_PyFloat_FromDouble(value.as<double>());
            break;
        default:
            be_notreached();
            result = s_library->m__Py_NoneStruct;
            Py_INCREF(result);
        }
    }
    else
    {
        result = s_pyType.tp_alloc(&s_pyType, 0);
        new(&((PyBugObject*)result)->value) RTTI::Value(value);
    }

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
    PyBugObject* self_ = reinterpret_cast<PyBugObject*>(self);
    bool found;
    RTTI::Value v = self_->value.type().metaclass->get(self_->value, name, found);
    if (found)
    {
        return create(v);
    }
    else
    {
        s_library->m_PyErr_Format(*s_library->m_PyExc_AttributeError,
                                  "object of type %s has no attrobute %s",
                                  self_->value.type().name().c_str(), name);
        return NULL;
    }
}

int PyBugObject::setattr(PyObject* self, const char* name, PyObject* value)
{
    be_forceuse(self);
    be_forceuse(name);
    be_forceuse(value);
    return 0;
}

PyObject* PyBugObject::repr(PyObject *self)
{
    PyBugObject* self_ = reinterpret_cast<PyBugObject*>(self);
    const RTTI::Value& v = self_->value;

    const char* constness = (v.type().constness == RTTI::Type::Const) ? "const " : "mutable ";
    const char* reference;
    const char* closing;
    switch(v.type().indirection)
    {
        case RTTI::Type::RefPtr:
            reference = "ref<";
            closing = ">";
            break;
        case RTTI::Type::WeakPtr:
            reference = "weak<";
            closing = ">";
            break;
        case RTTI::Type::RawPtr:
            reference = "raw<";
            closing = ">";
            break;
        case RTTI::Type::Value:
            reference = "";
            constness = "";
            closing = "";
            break;
        default:
            reference = "??? <";
            constness = "??? ";
            closing = ">";
            break;
    }
    const char* access = (v.type().access == RTTI::Type::Const) ? "const " : "";
    if (s_library->getVersion() >= 30)
    {
        return s_library->m_PyUnicode_FromFormat("[%s%s%s%s%s object @0x%p]",
                                                 constness, reference, access,
                                                 v.type().metaclass->name.c_str(),
                                                 closing, &v);
    }
    else
    {
        return s_library->m_PyString_FromFormat("[%s%s%s%s%s object @0x%p]",
                                                constness, reference, access,
                                                v.type().metaclass->name.c_str(),
                                                closing, &v);
    }
}

void PyBugObject::dealloc(PyObject* self)
{
    PyBugObject* self_ = reinterpret_cast<PyBugObject*>(self);
    self_->value.~Value();
    self->py_type->tp_free(self);
}

PyObject* PyBugObject::call(PyObject* self, PyObject* args, PyObject* kwds)
{
    PyBugObject* self_ = reinterpret_cast<PyBugObject*>(self);
    static const istring callName("?call");
    RTTI::Value v(self_->value[callName]);
    if (!v)
    {
        s_library->m_PyErr_Format(*s_library->m_PyExc_Exception,
                                  "%s object is not callable",
                                  v.type().name().c_str());
        return 0;
    }
    else
    {
        raw<const RTTI::Method> method = v.as< raw<const RTTI::Method> >();
        return Python::call(method, args, kwds);
    }
}

void PyBugObject::registerType(PyObject* module)
{
    int result = s_library->m_PyType_Ready(&s_pyType);
    be_assert(result >= 0, "unable to register type");
    be_forceuse(result);
    Py_INCREF(&s_pyType);
    result = (*s_library->m_PyModule_AddObject)(module, "Value", (PyObject*)&s_pyType);
    be_assert(result >= 0, "unable to register type");
    be_forceuse(result);
    result = (*s_library->m_PyModule_AddObject)(module, "BugEngine",
                                                create(RTTI::Value(be_game_Namespace())));
    be_assert(result >= 0, "unable to register type");
    be_forceuse(result);

}

}}
