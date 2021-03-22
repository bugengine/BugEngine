/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.scripting.pythonlib/stdafx.h>
#include <bugengine/plugin.scripting.pythonlib/pythonlib.hh>
#include <py_number.hh>

namespace BugEngine { namespace Python {

template < typename T >
PyTypeObject::Py2NumberMethods PyBugNumber< T >::s_py2NumberNumber = {{0, 0, 0},
                                                                      0,
                                                                      0,
                                                                      0,
                                                                      0,
                                                                      0,
                                                                      0,
                                                                      0,
                                                                      &PyBugNumber< T >::nonZero,
                                                                      0,
                                                                      0,
                                                                      0,
                                                                      0,
                                                                      0,
                                                                      0,
                                                                      0,
                                                                      &PyBugNumber< T >::toint,
                                                                      &PyBugNumber< T >::tolong,
                                                                      &PyBugNumber< T >::tofloat,
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
                                                                      0};

template < typename T >
PyTypeObject::Py3NumberMethods PyBugNumber< T >::s_py3NumberNumber = {{0, 0, 0},
                                                                      0,
                                                                      0,
                                                                      0,
                                                                      0,
                                                                      0,
                                                                      0,
                                                                      &PyBugNumber< T >::nonZero,
                                                                      0,
                                                                      0,
                                                                      0,
                                                                      0,
                                                                      0,
                                                                      0,
                                                                      &PyBugNumber< T >::tolong,
                                                                      0,
                                                                      &PyBugNumber< T >::tofloat,
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
                                                                      0};

template < typename T >
PyTypeObject PyBugNumber< T >::s_pyType
    = {{{0, 0}, 0},
       istring(minitl::format< 128u >("py_bugengine.%s") | be_type< T >().metaclass->name).c_str(),
       sizeof(PyBugNumber< T >),
       0,
       &PyBugNumber< T >::dealloc,
       0,
       0,
       0,
       0,
       &PyBugNumber< T >::repr,
       0,
       0,
       0,
       0,
       0,
       &PyBugNumber< T >::str,
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
       PyBugObject::s_methods,
       0,
       0,
       &PyBugObject::s_pyType,
       0,
       0,
       0,
       0,
       &PyBugNumber< T >::init,
       0,
       &PyBugNumber< T >::newinst,
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

template < typename T >
PyObject* PyBugNumber< T >::stealValue(PyObject* owner, Meta::Value& value)
{
    be_assert(value.type().metaclass->type() == Meta::ClassType_Number,
              "PyBugNumber only accepts Number types");
    be_assert(value.type().metaclass->index() == be_type< T >().metaclass->index(),
              "expected %s; got %s" | be_type< T >().metaclass->name
                  | value.type().metaclass->name);
    PyObject* result                           = s_pyType.tp_alloc(&s_pyType, 0);
    static_cast< PyBugNumber* >(result)->owner = owner;

    if(owner)
    {
        Py_INCREF(owner);
    }
    new(&(static_cast< PyBugNumber* >(result))->value) Meta::Value();
    (static_cast< PyBugNumber* >(result))->value.swap(value);
    return result;
}

template < typename T >
int PyBugNumber< T >::init(PyObject* self, PyObject* args, PyObject* kwds)
{
    be_forceuse(kwds);
    PyBugObject* self_    = static_cast< PyBugObject* >(self);
    Py_ssize_t   argCount = s_library->m_PyTuple_Size(args);
    if(argCount == 0)
    {
        self_->value = Meta::Value(T());
    }
    else if(argCount == 1)
    {
        PyObject* arg = s_library->m_PyTuple_GetItem(args, 0);
        if(arg->py_type == &s_pyType)
        {
            self_->value = static_cast< PyBugNumber* >(arg)->value;
        }
        else if(arg->py_type->tp_flags & Py_TPFLAGS_INT_SUBCLASS)
        {
            unsigned long value = s_library->m_PyInt_AsUnsignedLongMask(arg);
            self_->value        = Meta::Value(T(value));
        }
        else if(arg->py_type->tp_flags & Py_TPFLAGS_LONG_SUBCLASS)
        {
            unsigned long long value = s_library->m_PyLong_AsUnsignedLongLongMask(arg);
            self_->value             = Meta::Value(T(value));
        }
        else
        {
            s_library->m_PyErr_Format(*s_library->m_PyExc_TypeError, "Cannot convert from %s to %s",
                                      arg->py_type->tp_name,
                                      be_type< T >().metaclass->name.c_str());
            return -1;
        }
    }
    return 0;
}

template < typename T >
PyObject* PyBugNumber< T >::repr(PyObject* self)
{
    PyBugObject*            self_ = static_cast< PyBugObject* >(self);
    const Meta::Value&      v     = self_->value;
    minitl::format< 1024u > format
        = minitl::format< 1024u >("[%s %d]") | v.type().name().c_str() | v.as< const T >();
    if(s_library->getVersion() >= 30)
    {
        return s_library->m_PyUnicode_FromFormat(format);
    }
    else
    {
        return s_library->m_PyString_FromFormat(format);
    }
}

template < typename T >
PyObject* PyBugNumber< T >::toint(PyObject* self)
{
    PyBugObject* self_ = static_cast< PyBugObject* >(self);
    long         value = (long)self_->value.as< const T >();
    return s_library->m_PyInt_FromLong(value);
}

template < typename T >
PyObject* PyBugNumber< T >::tolong(PyObject* self)
{
    PyBugObject*       self_ = static_cast< PyBugObject* >(self);
    unsigned long long value = (unsigned long long)self_->value.as< const T >();
    return s_library->m_PyLong_FromUnsignedLongLong(value);
}

template < typename T >
PyObject* PyBugNumber< T >::tofloat(PyObject* self)
{
    PyBugObject* self_ = static_cast< PyBugObject* >(self);
    double       value = (double)self_->value.as< const T >();
    return s_library->m_PyFloat_FromDouble(value);
}

template < typename T >
PyObject* PyBugNumber< T >::str(PyObject* self)
{
    PyBugObject*       self_                       = static_cast< PyBugObject* >(self);
    const Meta::Value& v                           = self_->value;
    PyObject* (*tostring)(const char* format, ...) = s_library->getVersion() >= 30
                                                         ? s_library->m_PyUnicode_FromFormat
                                                         : s_library->m_PyString_FromFormat;
    return tostring(minitl::format< 1024u >("%d") | v.as< const T >());
}

template < typename T >
int PyBugNumber< T >::nonZero(PyObject* self)
{
    PyBugNumber*     self_ = static_cast< PyBugNumber* >(self);
    const Meta::Type t     = self_->value.type();
    be_assert(t.metaclass->type() == Meta::ClassType_Number, "PyBugNumber expected number value");
    if(t.indirection == Meta::Type::Value)
    {
        return self_->value.template as< const T >() != 0;
    }
    else
    {
        return self_->value.template as< const void* const >() != 0;
    }
}

template < typename T >
void PyBugNumber< T >::registerType(PyObject* module)
{
    if(s_library->getVersion() >= 30)
        s_pyType.tp_as_number = &s_py3NumberNumber.nb_common;
    else
        s_pyType.tp_as_number = &s_py2NumberNumber.nb_common;
    s_pyType.tp_alloc = s_library->m_PyType_GenericAlloc;
    int result        = s_library->m_PyType_Ready(&s_pyType);
    be_assert(result >= 0, "unable to register type");
    Py_INCREF(&s_pyType);
    result = (*s_library->m_PyModule_AddObject)(module, be_type< T >().metaclass->name.c_str(),
                                                (PyObject*)&s_pyType);
    be_assert(result >= 0, "unable to register type");
    be_forceuse(result);
}

template struct PyBugNumber< bool >;
template struct PyBugNumber< u8 >;
template struct PyBugNumber< u16 >;
template struct PyBugNumber< u32 >;
template struct PyBugNumber< u64 >;
template struct PyBugNumber< i8 >;
template struct PyBugNumber< i16 >;
template struct PyBugNumber< i32 >;
template struct PyBugNumber< i64 >;
template struct PyBugNumber< float >;
template struct PyBugNumber< double >;

}}  // namespace BugEngine::Python
