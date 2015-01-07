/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <python/stdafx.h>
#include    <python/pythonlib.hh>
#include    <py_object.hh>
#include    <rtti/classinfo.script.hh>

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
            result = s_library->m_PyInt_FromLong(value.as<u8>());
            break;
        case 2:
            be_assert(be_typeid<u16>::klass() == t.metaclass,
                      "mismatching index for class %s: mistaken for %s" | t.metaclass->fullname() | be_typeid<u16>::klass()->fullname());
            result = s_library->m_PyInt_FromLong(value.as<u16>());
            break;
        case 3:
            be_assert(be_typeid<u32>::klass() == t.metaclass,
                      "mismatching index for class %s: mistaken for %s" | t.metaclass->fullname() | be_typeid<u32>::klass()->fullname());
            result = s_library->m_PyInt_FromLong(value.as<u32>());
            break;
        case 4:
            be_assert(be_typeid<u64>::klass() == t.metaclass,
                      "mismatching index for class %s: mistaken for %s" | t.metaclass->fullname() | be_typeid<u64>::klass()->fullname());
            result = s_library->m_PyInt_FromLong(be_checked_numcast<long>(value.as<u64>()));
            break;
        case 5:
            be_assert(be_typeid<i8>::klass() == t.metaclass,
                      "mismatching index for class %s: mistaken for %s" | t.metaclass->fullname() | be_typeid<i8>::klass()->fullname());
            result = s_library->m_PyInt_FromLong(value.as<i8>());
            break;
        case 6:
            be_assert(be_typeid<i16>::klass() == t.metaclass,
                      "mismatching index for class %s: mistaken for %s" | t.metaclass->fullname() | be_typeid<i16>::klass()->fullname());
            result = s_library->m_PyInt_FromLong(value.as<i16>());
            break;
        case 7:
            be_assert(be_typeid<i32>::klass() == t.metaclass,
                      "mismatching index for class %s: mistaken for %s" | t.metaclass->fullname() | be_typeid<i32>::klass()->fullname());
            result = s_library->m_PyInt_FromLong(value.as<i32>());
            break;
        case 8:
            be_assert(be_typeid<i64>::klass() == t.metaclass,
                      "mismatching index for class %s: mistaken for %s" | t.metaclass->fullname() | be_typeid<i64>::klass()->fullname());
            result = s_library->m_PyInt_FromLong(be_checked_numcast<long>(value.as<i64>()));
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
        result = s_bugengineValueType.tp_alloc(&s_bugengineValueType, 0);
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
    RTTI::Value v = self_->value[name];
    if (v)
    {
        return create(v);
    }
    else
    {
        be_assert(false, "TODO");
        return create(v);
    }
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
