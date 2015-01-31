/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <python/stdafx.h>
#include    <python/pythonlib.hh>
#include    <py_string.hh>

namespace BugEngine { namespace Python
{

static PyTypeObject::PyNumberMethods s_pyTypeStringNumber =
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
    &PyBugString::nonZero,
    &PyBugString::nonZero,
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

PyTypeObject PyBugString::s_pyType =
{
    { { 0, 0 }, 0 },
    "py_bugengine.String",
    sizeof(PyBugString),
    0,
    &PyBugString::dealloc,
    0,
    0,
    0,
    0,
    &PyBugString::repr,
    &s_pyTypeStringNumber,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    Py_TPFLAGS_DEFAULT,
    "Wrapper class for the C++ BugEngine string types",
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
    &PyBugString::init,
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

PyObject* PyBugString::create(const RTTI::Value& value)
{
    const RTTI::Type& t = value.type();
    be_assert(t.metaclass->type() == RTTI::ClassType_String,
              "PyBugString only accepts String types");
    PyObject* result = s_pyType.tp_alloc(&s_pyType, 0);
    new(&((PyBugString*)result)->value) RTTI::Value(value);
    return result;
}

int PyBugString::init(PyObject* self, PyObject* args, PyObject* kwds)
{
    /* todo */
    be_forceuse(self);
    be_forceuse(args);
    be_forceuse(kwds);
    return 0;
}

PyObject* PyBugString::repr(PyObject *self)
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

int PyBugString::nonZero(PyObject *self)
{
    PyBugNumber* self_ = reinterpret_cast<PyBugNumber*>(self);
    const RTTI::Type t = self_->value.type();
    be_assert(t.metaclass->type() == RTTI::ClassType_String,
              "PyBugString expected string value");
    if (t.indirection == RTTI::Type::Value)
    {
        switch(t.metaclass->index())
        {
        case 0:
            be_assert(be_typeid<istring>::klass() == t.metaclass,
                      "mismatching index for class %s: mistaken for %s" | t.metaclass->fullname()
                    | be_typeid<istring>::klass()->fullname());
            return self_->value.as<istring>() != istring("");
        case 1:
            be_assert(be_typeid<inamespace>::klass() == t.metaclass,
                      "mismatching index for class %s: mistaken for %s" | t.metaclass->fullname()
                    | be_typeid<inamespace>::klass()->fullname());
            return self_->value.as<const inamespace&>().size() > 0;
        case 2:
            be_assert(be_typeid<ifilename>::klass() == t.metaclass,
                      "mismatching index for class %s: mistaken for %s" | t.metaclass->fullname()
                    | be_typeid<ifilename>::klass()->fullname());
            return self_->value.as<const ifilename&>().size() > 0;
        case 3:
            be_assert(be_typeid<ipath>::klass() == t.metaclass,
                      "mismatching index for class %s: mistaken for %s" | t.metaclass->fullname()
                    | be_typeid<ipath>::klass()->fullname());
            return self_->value.as<const ipath&>().size() > 0;
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

void PyBugString::registerType(PyObject* module)
{
    int result = s_library->m_PyType_Ready(&s_pyType);
    be_assert(result >= 0, "unable to register type");
    Py_INCREF(&s_pyType);
    result = (*s_library->m_PyModule_AddObject)(module, "String", (PyObject*)&s_pyType);
    be_assert(result >= 0, "unable to register type");
    be_forceuse(result);
}

}}
