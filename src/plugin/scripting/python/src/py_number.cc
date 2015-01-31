/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <python/stdafx.h>
#include    <python/pythonlib.hh>
#include    <py_number.hh>

namespace BugEngine { namespace Python
{

static PyTypeObject::PyNumberMethods s_pyTypeNumberNumber =
{
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    &PyBugNumber::nonZero,
    &PyBugNumber::nonZero,
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
    0,
    0
};

PyTypeObject PyBugNumber::s_pyType =
{
    { { 0, 0 }, 0 },
    "py_bugengine.Number",
    sizeof(PyBugNumber),
    0,
    &PyBugNumber::dealloc,
    0,
    0,
    0,
    0,
    &PyBugNumber::repr,
    &s_pyTypeNumberNumber,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    Py_TPFLAGS_DEFAULT,
    "Wrapper class for the C++ BugEngine number types",
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
    &PyBugNumber::init,
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

PyObject* PyBugNumber::create(const RTTI::Value& value)
{
    const RTTI::Type& t = value.type();
    be_assert(t.metaclass->type() == RTTI::ClassType_Number,
              "PyBugNumber only accepts Number types");
    PyObject* result = s_pyType.tp_alloc(&s_pyType, 0);
    new(&((PyBugNumber*)result)->value) RTTI::Value(value);
    return result;
}

int PyBugNumber::init(PyObject* self, PyObject* args, PyObject* kwds)
{
    /* todo */
    be_forceuse(self);
    be_forceuse(args);
    be_forceuse(kwds);
    return 0;
}

PyObject* PyBugNumber::repr(PyObject *self)
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
        return s_library->m_PyUnicode_FromFormat("[%s%s%s%s%s ]",
                                                 constness, reference, access,
                                                 v.type().metaclass->name.c_str(),
                                                 closing, &v);
    }
    else
    {
        return s_library->m_PyString_FromFormat("[%s%s%s%s%s ]",
                                                constness, reference, access,
                                                v.type().metaclass->name.c_str(),
                                                closing, &v);
    }
}

int PyBugNumber::nonZero(PyObject *self)
{
    PyBugNumber* self_ = reinterpret_cast<PyBugNumber*>(self);
    const RTTI::Type t = self_->value.type();
    be_assert(t.metaclass->type() == RTTI::ClassType_Number,
              "PyBugNumber expected number value");
    if (t.indirection == RTTI::Type::Value)
    {
        switch(t.metaclass->index())
        {
        case 0:
            be_assert(be_typeid<bool>::klass() == t.metaclass,
                      "mismatching index for class %s: mistaken for %s" | t.metaclass->fullname()
                    | be_typeid<bool>::klass()->fullname());
            return self_->value.as<bool>() != 0;
        case 1:
            be_assert(be_typeid<u8>::klass() == t.metaclass,
                      "mismatching index for class %s: mistaken for %s" | t.metaclass->fullname()
                    | be_typeid<u8>::klass()->fullname());
            return self_->value.as<u8>() != 0;
        case 2:
            be_assert(be_typeid<u16>::klass() == t.metaclass,
                      "mismatching index for class %s: mistaken for %s" | t.metaclass->fullname()
                    | be_typeid<u16>::klass()->fullname());
            return self_->value.as<u16>() != 0;
        case 3:
            be_assert(be_typeid<u32>::klass() == t.metaclass,
                      "mismatching index for class %s: mistaken for %s" | t.metaclass->fullname()
                    | be_typeid<u32>::klass()->fullname());
            return self_->value.as<u32>() != 0;
        case 4:
            be_assert(be_typeid<u64>::klass() == t.metaclass,
                      "mismatching index for class %s: mistaken for %s" | t.metaclass->fullname()
                    | be_typeid<u64>::klass()->fullname());
            return self_->value.as<u64>() != 0;
        case 5:
            be_assert(be_typeid<i8>::klass() == t.metaclass,
                      "mismatching index for class %s: mistaken for %s" | t.metaclass->fullname()
                    | be_typeid<i8>::klass()->fullname());
            return self_->value.as<i8>() != 0;
        case 6:
            be_assert(be_typeid<i16>::klass() == t.metaclass,
                      "mismatching index for class %s: mistaken for %s" | t.metaclass->fullname()
                    | be_typeid<i16>::klass()->fullname());
            return self_->value.as<i16>() != 0;
        case 7:
            be_assert(be_typeid<i32>::klass() == t.metaclass,
                      "mismatching index for class %s: mistaken for %s" | t.metaclass->fullname()
                    | be_typeid<i32>::klass()->fullname());
            return self_->value.as<i32>() != 0;
        case 8:
            be_assert(be_typeid<i64>::klass() == t.metaclass,
                      "mismatching index for class %s: mistaken for %s" | t.metaclass->fullname()
                    | be_typeid<i64>::klass()->fullname());
            return self_->value.as<i64>() != 0;
        case 9:
            be_assert(be_typeid<float>::klass() == t.metaclass,
                      "mismatching index for class %s: mistaken for %s" | t.metaclass->fullname()
                    | be_typeid<float>::klass()->fullname());
            return self_->value.as<float>() != 0.0f;
        case 10:
            be_assert(be_typeid<double>::klass() == t.metaclass,
                      "mismatching index for class %s: mistaken for %s" | t.metaclass->fullname()
                    | be_typeid<double>::klass()->fullname());
            return self_->value.as<double>() != 0.0;
        default:
            be_unimplemented();
            return 1;
        }
    }
    else
    {
        return self_->value.as<const void* const>() != 0;
    }
}

void PyBugNumber::registerType(PyObject* module)
{
    int result = s_library->m_PyType_Ready(&s_pyType);
    be_assert(result >= 0, "unable to register type");
    Py_INCREF(&s_pyType);
    result = (*s_library->m_PyModule_AddObject)(module, "Number", (PyObject*)&s_pyType);
    be_assert(result >= 0, "unable to register type");
    be_forceuse(result);
}

}}
