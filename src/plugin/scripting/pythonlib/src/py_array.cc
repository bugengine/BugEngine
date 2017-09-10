/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <pythonlib/stdafx.h>
#include    <pythonlib/pythonlib.hh>
#include    <rtti/engine/call.hh>
#include    <py_array.hh>

namespace BugEngine { namespace Python
{

static PyTypeObject::Py2NumberMethods s_py2ArrayNumber =
{
    { 0, 0, 0 },
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    &PyBugArray::nonZero,
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

static PyTypeObject::Py3NumberMethods s_py3ArrayNumber =
{
    { 0, 0, 0 },
    0,
    0,
    0,
    0,
    0,
    0,
    &PyBugArray::nonZero,
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

static PyTypeObject::PySequenceMethods s_pyArraySequence =
{
    &PyBugArray::length,
    0,
    0,
    &PyBugArray::item,
    0,
    &PyBugArray::setItem,
    0,
    0,
    0,
    0
};

PyTypeObject PyBugArray::s_pyType =
{
    { { 0, 0 }, 0 },
    "py_bugengine.Array",
    sizeof(PyBugArray),
    0,
    &PyBugArray::dealloc,
    0,
    0,
    0,
    0,
    &PyBugArray::repr,
    0,
    &s_pyArraySequence,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_IS_ABSTRACT,
    "Wrapper class for the C++ BugEngine array types",
    0,
    0,
    0,
    0,
    0,
    0,
    PyBugObject::s_methods,
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

PyObject* PyBugArray::create(PyObject* owner, const RTTI::Value& value)
{
    be_assert(value.type().metaclass->type() == RTTI::ClassType_Array,
              "PyBugArray only accepts Array types");
    PyBugArray* result = static_cast<PyBugArray*>(s_pyType.tp_alloc(&s_pyType, 0));
    (result)->owner = owner;
    if (owner)
    {
        Py_INCREF(owner);
    }
    new(&(result)->value) RTTI::Value(value);
    {
        raw<const RTTI::Method> m = value.type().metaclass->getMethod("size");
        if (m)
        {
            RTTI::ArgInfo<RTTI::Type> t(value.type());
            RTTI::CallInfo c = RTTI::resolve(m, &t, 1);
            result->m_size = c.overload;
        }
    }
    {
        raw<const RTTI::Method> m = value.type().metaclass->getMethod(RTTI::Class::nameOperatorIndex());
        if (m)
        {
            RTTI::ArgInfo<RTTI::Type> t[2] = {
                RTTI::ArgInfo<RTTI::Type>(value.type()),
                RTTI::ArgInfo<RTTI::Type>(be_typeid<u32>::type())
            };
            RTTI::CallInfo c = RTTI::resolve(m, t, 2);
            result->m_get = c.overload;
        }
    }
    if (!result->m_get)
        be_warning("%s: could not retrieve array index method" | value.type().name());
    if (!result->m_size)
        be_warning("%s: could not retrieve size() method" | value.type().name());
    return result;
}

PyObject* PyBugArray::repr(PyObject *self)
{
    PyBugObject* self_ = static_cast<PyBugObject*>(self);
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

int PyBugArray::nonZero(PyObject *self)
{
    PyBugArray* self_ = static_cast<PyBugArray*>(self);
    const RTTI::Type t = self_->value.type();
    be_assert(t.metaclass->type() == RTTI::ClassType_Array,
              "PyBugArray expected array value");
    if (t.indirection == RTTI::Type::Value)
    {
        be_unimplemented();
        return 1;
    }
    else
    {
        return self_->value.as<const void* const>() != 0;
    }
}


Py_ssize_t PyBugArray::length(PyObject *self)
{
    PyBugArray* self_ = static_cast<PyBugArray*>(self);
    if (self_->m_size)
    {
        return be_checked_numcast<Py_ssize_t>(self_->m_size->call(&self_->value, 1).as<u32>());
    }
    else
    {
        s_library->m_PyErr_Format(*s_library->m_PyExc_TypeError,
                                  "Type %s does not implement method size()",
                                  self_->value.type().name().c_str());
        return 0;
    }
}

PyObject* PyBugArray::item(PyObject *self, Py_ssize_t index)
{
    PyBugArray* self_ = static_cast<PyBugArray*>(self);
    if (self_->m_get)
    {
        if (index < length(self))
        {
            RTTI::Value params[2] = {
                RTTI::Value(RTTI::Value::ByRef(self_->value)),
                RTTI::Value(be_checked_numcast<u32>(index))
            };
            RTTI::Value result = self_->m_get->call(params, 2);
            return PyBugObject::create(0, result);
        }
        else
        {
            s_library->m_PyErr_Format(*s_library->m_PyExc_IndexError,
                                      "list index out of range",
                                      self_->value.type().name().c_str());
            return 0;
        }
    }
    else
    {
        s_library->m_PyErr_Format(*s_library->m_PyExc_TypeError,
                                  "Type %s does not implement operator[]",
                                  self_->value.type().name().c_str());
        return 0;
    }
}

int PyBugArray::setItem(PyObject *self, Py_ssize_t index, PyObject *value)
{
    be_forceuse(self);
    be_forceuse(index);
    be_forceuse(value);
    return 0;
}


void PyBugArray::registerType(PyObject* module)
{
    if (s_library->getVersion() >= 30)
        s_pyType.tp_as_number = &s_py3ArrayNumber.nb_common;
    else
        s_pyType.tp_as_number = &s_py2ArrayNumber.nb_common;
    int result = s_library->m_PyType_Ready(&s_pyType);
    be_assert(result >= 0, "unable to register type");
    Py_INCREF(&s_pyType);
    result = (*s_library->m_PyModule_AddObject)(module, "Array", (PyObject*)&s_pyType);
    be_assert(result >= 0, "unable to register type");
    be_forceuse(result);
}

}}
