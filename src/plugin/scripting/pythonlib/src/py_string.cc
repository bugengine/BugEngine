/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/plugin.scripting.pythonlib/stdafx.h>
#include <bugengine/plugin.scripting.pythonlib/pythonlib.hh>
#include <py_string.hh>

namespace BugEngine { namespace Python {

template < typename T >
PyTypeObject::Py2NumberMethods PyBugString< T >::s_py2StringNumber
   = {{0, 0, 0}, 0, 0, 0, 0, 0, 0, 0, &PyBugString< T >::nonZero,
      0,         0, 0, 0, 0, 0, 0, 0, 0,
      0,         0, 0, 0, 0, 0, 0, 0, 0,
      0,         0, 0, 0, 0, 0, 0, 0, 0,
      0};

template < typename T >
PyTypeObject::Py3NumberMethods PyBugString< T >::s_py3StringNumber
   = {{0, 0, 0}, 0, 0, 0, 0, 0, 0, &PyBugString< T >::nonZero,
      0,         0, 0, 0, 0, 0, 0, 0,
      0,         0, 0, 0, 0, 0, 0, 0,
      0,         0, 0, 0, 0, 0, 0, 0,
      0,         0};

template < typename T >
PyTypeObject PyBugString< T >::s_pyType
   = {{{0, 0}, 0},
      istring(minitl::format< 128u >("py_bugengine.%s") | be_type< T >().metaclass->name).c_str(),
      sizeof(PyBugString< T >),
      0,
      &PyBugString< T >::dealloc,
      0,
      &PyBugString< T >::getattr,
      &PyBugString< T >::setattr,
      0,
      &PyBugString< T >::repr,
      0,
      0,
      0,
      0,
      0,
      &PyBugString< T >::str,
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
      &PyBugObject::s_pyType,
      0,
      0,
      0,
      0,
      &PyBugString< T >::init,
      0,
      &PyBugString< T >::newinst,
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
PyObject* PyBugString< T >::stealValue(PyObject* owner, RTTI::Value& value)
{
    const T& t = value.as< const T& >();
    be_forceuse(t);
    be_assert(value.type().metaclass->type() == RTTI::ClassType_String,
              "PyBugString only accepts String types");
    be_assert(value.type().metaclass->index() == be_type< T >().metaclass->index(),
              "expected %s; got %s" | be_type< T >().metaclass->name
                 | value.type().metaclass->name);
    PyObject* result                           = s_pyType.tp_alloc(&s_pyType, 0);
    static_cast< PyBugString* >(result)->owner = owner;

    if(owner)
    {
        Py_INCREF(owner);
    }
    new(&(static_cast< PyBugString* >(result))->value) RTTI::Value();
    (static_cast< PyBugString* >(result))->value.swap(value);
    return result;
}

template < typename T >
int PyBugString< T >::init(PyObject* self, PyObject* args, PyObject* kwds)
{
    be_forceuse(kwds);
    PyBugObject* self_    = static_cast< PyBugObject* >(self);
    Py_ssize_t   argCount = s_library->m_PyTuple_Size(args);
    if(argCount == 0)
    {
        self_->value = RTTI::Value(T(""));
    }
    else if(argCount == 1)
    {
        PyObject* arg = s_library->m_PyTuple_GetItem(args, 0);
        if(arg->py_type == &s_pyType)
        {
            self_->value = static_cast< PyBugString* >(arg)->value;
        }
        else if(arg->py_type->tp_flags & Py_TPFLAGS_STRING_SUBCLASS)
        {
            const char* value = s_library->m_PyString_AsString(arg);
            self_->value      = RTTI::Value(T(value));
        }
        else if(arg->py_type->tp_flags & Py_TPFLAGS_UNICODE_SUBCLASS)
        {
            if(s_library->getVersion() >= 33)
            {
                const char* value = s_library->m_PyUnicode_AsUTF8(arg);
                self_->value      = RTTI::Value(T(value));
            }
            else
            {
                PyObject*   decodedUnicode = s_library->m_PyUnicode_AsUTF8String(arg);
                const char* value          = s_library->m_PyBytes_AsString(decodedUnicode);
                self_->value               = RTTI::Value(T(value));
                Py_DECREF(decodedUnicode);
            }
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
PyObject* PyBugString< T >::repr(PyObject* self)
{
    PyBugObject*       self_ = static_cast< PyBugObject* >(self);
    const RTTI::Value& v     = self_->value;
    typedef PyObject* (*toStringType)(const char* format, ...);
    toStringType toString = s_library->getVersion() >= 30 ? s_library->m_PyUnicode_FromFormat
                                                          : s_library->m_PyString_FromFormat;
    return toString(minitl::format< 1024u >("[%s \"%s\"]") | v.type().name() | v.as< const T& >());
}

const char* toCharPtr(const istring& t)
{
    return t.c_str();
}

const inamespace::Path toCharPtr(const inamespace& t)
{
    return t.str();
}

const ipath::Filename toCharPtr(const ipath& t)
{
    return t.str();
}

const ifilename::Filename toCharPtr(const ifilename& t)
{
    return t.str();
}

const char* toCharPtr(const text& t)
{
    return t.begin();
}

template < typename T >
PyObject* PyBugString< T >::str(PyObject* self)
{
    PyBugObject*       self_ = static_cast< PyBugObject* >(self);
    const RTTI::Value& v     = self_->value;
    typedef PyObject* (*toStringType)(const char* format);
    toStringType toString = s_library->getVersion() >= 30 ? s_library->m_PyUnicode_FromString
                                                          : s_library->m_PyString_FromString;
    return toString(static_cast< const char* >(toCharPtr(v.as< const T& >())));
}

template < typename T >
bool nonZeroString(const T& t)
{
    return t.size() != 0;
}

template <>
bool nonZeroString< istring >(const istring& t)
{
    return t != istring("");
}

template < typename T >
int PyBugString< T >::nonZero(PyObject* self)
{
    PyBugObject*     self_ = static_cast< PyBugObject* >(self);
    const RTTI::Type t     = self_->value.type();
    be_assert(t.metaclass->type() == RTTI::ClassType_String, "PyBugString expected string value");
    if(t.indirection == RTTI::Type::Value)
    {
        return nonZeroString(self_->value.as< const T& >());
    }
    else
    {
        return self_->value.as< const void* const >() != 0;
    }
}

template < typename T >
void PyBugString< T >::registerType(PyObject* module)
{
    if(s_library->getVersion() >= 30)
        s_pyType.tp_as_number = &s_py3StringNumber.nb_common;
    else
        s_pyType.tp_as_number = &s_py2StringNumber.nb_common;
    s_pyType.tp_alloc = s_library->m_PyType_GenericAlloc;
    int result        = s_library->m_PyType_Ready(&s_pyType);
    be_assert(result >= 0, "unable to register type");
    Py_INCREF(&s_pyType);
    result = (*s_library->m_PyModule_AddObject)(module, be_type< T >().metaclass->name.c_str(),
                                                (PyObject*)&s_pyType);
    be_assert(result >= 0, "unable to register type");
    be_forceuse(result);
}

template struct PyBugString< istring >;
template struct PyBugString< inamespace >;
template struct PyBugString< ifilename >;
template struct PyBugString< ipath >;
template struct PyBugString< text >;

}}  // namespace BugEngine::Python
