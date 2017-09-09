/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <pythonlib/stdafx.h>
#include    <pythonlib/pythonlib.hh>
#include    <py_call.hh>
#include    <py_object.hh>
#include    <rtti/value.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/engine/call.hh>

namespace BugEngine { namespace Python
{

namespace
{

struct PythonTypeInfo
{
    PyObject*       arg;
    PyTypeObject*   pythonType;
    RTTI::Type      bugengineType;

    static RTTI::Type getTypeFromPyObject(PyObject* object);
    static PyTypeObject* getPyTypeFromPyObject(PyObject* object);

    PythonTypeInfo(PyObject* object);
};

static RTTI::ConversionCost calculateConversion(const PythonTypeInfo& typeInfo,
                                                const RTTI::Type& other)
{
    return PyBugObject::distance(typeInfo.arg, other);
}

static void convert(const PythonTypeInfo& typeInfo, void* buffer, RTTI::Type type)
{
    PyBugObject::unpack(typeInfo.arg, type, buffer);
}

PythonTypeInfo::PythonTypeInfo(PyObject* object)
    :   arg(object)
    ,   pythonType(getPyTypeFromPyObject(object))
    ,   bugengineType(getTypeFromPyObject(object))
{
}

RTTI::Type PythonTypeInfo::getTypeFromPyObject(PyObject* object)
{
    if (object->py_type == &PyBugObject::s_pyType)
    {
        PyBugObject* o = static_cast<PyBugObject*>(object);
        return o->value.type();
    }
    else
    {
        return be_typeid<void>::type();
    }
}

PyTypeObject* PythonTypeInfo::getPyTypeFromPyObject(PyObject* object)
{
    if (object->py_type == &PyBugObject::s_pyType)
    {
        return 0;
    }
    else
    {
        return object->py_type;
    }
}

}

typedef RTTI::ArgInfo<PythonTypeInfo> PythonArgInfo;

PyObject* call(raw<const RTTI::Method> method, PyObject* self, PyObject* args, PyObject* kwargs)
{
    const u32 selfArgCount = self ? 1 : 0;
    const u32 unnamedArgCount =  args
                                   ? be_checked_numcast<u32>(s_library->m_PyTuple_Size(args))
                                   : 0;
    const u32 namedArgCount = kwargs
                               ? be_checked_numcast<u32>(s_library->m_PyDict_Size(kwargs))
                               : 0;
    const u32 argCount = selfArgCount + unnamedArgCount + namedArgCount;
    PythonArgInfo* argInfos = reinterpret_cast<PythonArgInfo*>(malloca(argCount * sizeof(PythonArgInfo)));

    {
        u32 argIndex = 0;
        if (self)
        {
             new (&argInfos[argIndex]) PythonArgInfo(PythonTypeInfo(self));
             argIndex++;
        }
        for (u32 i = 0; i < unnamedArgCount; ++argIndex, ++i)
        {
            new (&argInfos[argIndex]) PythonArgInfo(PythonTypeInfo(s_library->m_PyTuple_GetItem(args, i)));
        }
        if (kwargs)
        {
            Py_ssize_t pos = 0;
            PyObject* key = 0;
            PyObject* item = 0;
            int version = s_library->getVersion();
            while (s_library->m_PyDict_Next(kwargs, &pos, &key, &item))
            {
                if (version >= 33)
                {
                    new (&argInfos[argIndex]) PythonArgInfo(s_library->m_PyUnicode_AsUTF8(key), item);
                }
                else if (version >= 30)
                {
                    PyObject* bytes = s_library->m_PyUnicode_AsASCIIString(key);
                    if (!bytes)
                    {
                        return 0;
                    }
                    const char* name = s_library->m_PyBytes_AsString(bytes);
                    new (&argInfos[argIndex]) PythonArgInfo(name, PythonTypeInfo(item));
                    Py_DECREF(bytes);
                }
                else
                {
                    new (&argInfos[argIndex]) PythonArgInfo(s_library->m_PyString_AsString(key),
                                                            PythonTypeInfo(item));
                }
                ++argIndex;
            }
        }
    }

    RTTI::CallInfo info = RTTI::resolve(method, argInfos, selfArgCount + unnamedArgCount,
                                        argInfos + selfArgCount + unnamedArgCount, namedArgCount);
    if (info.conversion < RTTI::ConversionCost::s_incompatible)
    {
        RTTI::Value result = RTTI::call(info, argInfos, selfArgCount + unnamedArgCount,
                                        argInfos + selfArgCount + unnamedArgCount, namedArgCount);
        for (u32 i = argCount; i > 0; --i)
        {
            argInfos[i-1].~PythonArgInfo();
        }
        freea(argInfos);
        return PyBugObject::create(0, result);
    }
    else
    {
        for (u32 i = argCount; i > 0; --i)
        {
            argInfos[i-1].~PythonArgInfo();
        }
        freea(argInfos);
        s_library->m_PyErr_Format(*s_library->m_PyExc_TypeError,
                                  "Could not call method %s: "
                                  "no overload could convert all parameters",
                                  method->name.c_str());
        return 0;
    }
}

}}
