/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <pythonlib/stdafx.h>
#include    <pythonlib/pythonlib.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <py_enum.hh>

namespace BugEngine { namespace Python
{

PyTypeObject::Py2NumberMethods PyBugEnum::s_py2EnumNumber =
{
    { 0, 0, 0 },
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    &PyBugEnum::nonZero,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    &PyBugEnum::toint,
    &PyBugEnum::tolong,
    &PyBugEnum::tofloat,
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

PyTypeObject::Py3NumberMethods PyBugEnum::s_py3EnumNumber =
{
    { 0, 0, 0 },
    0,
    0,
    0,
    0,
    0,
    0,
    &PyBugEnum::nonZero,
    0,
    0,
    0,
    0,
    0,
    0,
    &PyBugEnum::tolong,
    0,
    &PyBugEnum::tofloat,
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

PyTypeObject PyBugEnum::s_pyType =
{
    { { 0, 0 }, 0 },
    "py_bugengine.Enum",
    sizeof(PyBugEnum),
    0,
    PyBugEnum::dealloc,
    0,
    0,
    0,
    0,
    &PyBugEnum::str,
    0,
    0,
    0,
    0,
    0,
    &PyBugEnum::str,
    0,
    0,
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_IS_ABSTRACT,
    "Wrapper class for the C++ BugEngine Enum types",
    0,
    0,
    0,
    0,
    0,
    0,
    PyBugObject::s_methods,
    0,
    0,
    &PyBugObject::s_pyType,
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

PyObject* PyBugEnum::create(PyObject* owner, const RTTI::Value& value)
{
    be_assert(value.type().metaclass->type() == RTTI::ClassType_Enum,
              "PyBugNumber only accepts Enum types");
    PyObject* result = s_pyType.tp_alloc(&s_pyType, 0);
    static_cast<PyBugEnum*>(result)->owner = owner;
    if (owner)
    {
        Py_INCREF(owner);
    }
    new(&(static_cast<PyBugEnum*>(result))->value) RTTI::Value(value);
    return result;
}

static istring s_toString = istring("toString");
static istring s_toInt = istring("toInt");

PyObject* PyBugEnum::str(PyObject *self)
{
    PyBugObject* self_ = static_cast<PyBugObject*>(self);
    const RTTI::Value& v = self_->value;
    raw<const RTTI::Method> toString = self_->value[s_toString].as< raw<const RTTI::Method> >();
    minitl::format<1024u> format = minitl::format<1024u>("%s.%s")
            |   v.type().name().c_str()
            |   toString->doCall(&self_->value, 1).as<const istring>();
    if (s_library->getVersion() >= 30)
    {
        return s_library->m_PyUnicode_FromFormat(format);
    }
    else
    {
        return s_library->m_PyString_FromFormat(format);
    }
}

PyObject* PyBugEnum::toint(PyObject *self)
{
    PyBugObject* self_ = static_cast<PyBugObject*>(self);
    raw<const RTTI::Method> toInt = self_->value[s_toInt].as< raw<const RTTI::Method> >();
    long value = (long)toInt->doCall(&self_->value, 1).as<u32>();
    return s_library->m_PyInt_FromLong(value);
}

PyObject* PyBugEnum::tolong(PyObject *self)
{
    PyBugObject* self_ = static_cast<PyBugObject*>(self);
    raw<const RTTI::Method> toInt = self_->value[s_toInt].as< raw<const RTTI::Method> >();
    unsigned long long value = (unsigned long long)toInt->doCall(&self_->value, 1).as<u32>();
    return s_library->m_PyLong_FromUnsignedLongLong(value);
}

PyObject* PyBugEnum::tofloat(PyObject *self)
{
    PyBugObject* self_ = static_cast<PyBugObject*>(self);
    raw<const RTTI::Method> toInt = self_->value[s_toInt].as< raw<const RTTI::Method> >();
    double value = (double)toInt->doCall(&self_->value, 1).as<u32>();
    return s_library->m_PyFloat_FromDouble(value);
}

int PyBugEnum::nonZero(PyObject *self)
{
    PyBugObject* self_ = static_cast<PyBugObject*>(self);
    raw<const RTTI::Method> toInt = self_->value[s_toInt].as< raw<const RTTI::Method> >();
    return toInt->doCall(&self_->value, 1).as<u32>() != 0;
}

void PyBugEnum::registerType(PyObject* module)
{
    if (s_library->getVersion() >= 30)
        s_pyType.tp_as_number = &s_py3EnumNumber.nb_common;
    else
        s_pyType.tp_as_number = &s_py2EnumNumber.nb_common;
    s_pyType.tp_alloc = s_library->m_PyType_GenericAlloc;
    int result = s_library->m_PyType_Ready(&s_pyType);
    be_assert(result >= 0, "unable to register type");
    be_forceuse(module);
    be_forceuse(result);
    Py_INCREF(&s_pyType);
}

}}
