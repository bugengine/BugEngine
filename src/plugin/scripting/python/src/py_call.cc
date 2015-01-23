/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <python/stdafx.h>
#include    <python/pythonlib.hh>
#include    <py_call.hh>
#include    <py_object.hh>
#include    <rtti/value.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/engine/methodinfo.script.hh>

namespace BugEngine { namespace Python
{

struct ArgInfo
{
    istring const       name;
    PyObject* const     arg;
    PyTypeObject* const pythonType;
    RTTI::Type const    bugengineType;

    ArgInfo(PyObject* object);
    ArgInfo(const istring name, PyObject* object);
};

static RTTI::Type getTypeFromPyObject(PyObject* object)
{
    if (object->py_type == &PyBugObject::s_pyType)
    {
        PyBugObject* o = reinterpret_cast<PyBugObject*>(object);
        return o->value.type();
    }
    else
    {
        return be_typeid<void>::type();
    }
}

static PyTypeObject* getPyTypeFromPyObject(PyObject* object)
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

ArgInfo::ArgInfo(PyObject* object)
    :   name("")
    ,   arg(object)
    ,   pythonType(getPyTypeFromPyObject(object))
    ,   bugengineType(getTypeFromPyObject(object))
{
}

ArgInfo::ArgInfo(const istring name, PyObject* object)
    :   name(name)
    ,   arg(object)
    ,   pythonType(getPyTypeFromPyObject(object))
    ,   bugengineType(getTypeFromPyObject(object))
{
}

static const u32 s_maxDistance = (u32)RTTI::Type::MaxTypeDistance;

static raw<const RTTI::Method::Overload::Parameter> findParameter(raw<const RTTI::Method::Overload::Parameter> parameters,
                                                                  const istring name)
{
    raw<const RTTI::Method::Overload::Parameter> result = { 0 };
    for (result = parameters; result && result->name != name; result = result->next)
    {
        /* nothing */
    }
    return result;
}

static inline void unpackArray(PyObject* arg, const RTTI::Type& type, RTTI::Value* buffer)
{
    be_forceuse(arg);
    be_forceuse(type);
    be_forceuse(buffer);
}

static inline void unpackInteger(PyObject* arg, const RTTI::Type& type, RTTI::Value* buffer)
{
    
    be_forceuse(arg);
    be_forceuse(type);
    be_forceuse(buffer);
}

static inline void unpackString(PyObject* arg, const RTTI::Type& type, RTTI::Value* buffer)
{
    be_forceuse(arg);
    be_forceuse(type);
    be_forceuse(buffer);
}

static inline void unpackObject(PyObject* arg, const RTTI::Type& type, RTTI::Value* buffer)
{
    be_forceuse(arg);
    be_forceuse(type);
    be_forceuse(buffer);
}

static void unpackValue(const ArgInfo& arg, const RTTI::Type& type, RTTI::Value* buffer)
{
    if (arg.pythonType)
    {
        switch(type.metaclass->type())
        {
        case RTTI::ClassType_Array:
            unpackArray(arg.arg, type, buffer);
            break;
        case RTTI::ClassType_Enum:
        case RTTI::ClassType_Integer:
            unpackInteger(arg.arg, type, buffer);
            break;
        case RTTI::ClassType_String:
            unpackString(arg.arg, type, buffer);
            break;
        case RTTI::ClassType_Object:
        case RTTI::ClassType_Struct:
        case RTTI::ClassType_Pod:
            unpackObject(arg.arg, type, buffer);
            break;
        case RTTI::ClassType_Variant:
        default:
            be_notreached();
            new (buffer) RTTI::Value();
        }
    }
    else
    {
        PyBugObject* object = reinterpret_cast<PyBugObject*>(arg.arg);
        be_assert(type <= object->value.type(),
                  "incompatible types: %s is not compatible with %s"
                    | object->value.type().name().c_str()
                    | type.name().c_str());
        new (buffer) RTTI::Value(object->value);
    }
}

static void unpackValues(raw<const RTTI::Method::Overload> overload, const ArgInfo args[],
                         u32 unnamedArgCount, RTTI::Value* buffer)
{
    raw<const RTTI::Method::Overload::Parameter> p = overload->params;
    for (u32 i = 0; i < unnamedArgCount; ++i, p = p->next)
    {
         unpackValue(args[i], p->type, &buffer[i]);
    }
    for (u32 i = unnamedArgCount; i < overload->parameterCount; ++i)
    {
        raw<const RTTI::Method::Overload::Parameter> namedParameter = findParameter(p, args[i].name);
        unpackValue(args[i], namedParameter->type, &buffer[i]);
    }
}

static u32 distance(PyTypeObject* pythonType, const RTTI::Type& bugengineType,
                    const RTTI::Type& desiredType)
{
    if (pythonType)
    {
        return s_maxDistance;
    }
    else
    {
        return bugengineType.distance(desiredType);
    }
}

static u32 distance(raw<const RTTI::Method::Overload> overload, ArgInfo* args, u32 unnamedArgCount)
{
    raw<const RTTI::Method::Overload::Parameter> param = overload->params;
    u32 result = 0;
    for (u32 i = 0; i < unnamedArgCount; ++i)
    {
        u32 argDistance = distance(args[i].pythonType, args[i].bugengineType, param->type);
        result += argDistance;
        if (argDistance == s_maxDistance)
        {
            return s_maxDistance;
        }
        param = param->next;
    }
    for (u32 i = unnamedArgCount; i < overload->parameterCount; ++i)
    {
        raw<const RTTI::Method::Overload::Parameter> namedParam = findParameter(param, args[i].name);
        if (namedParam)
        {
            u32 argDistance = distance(args[i].pythonType, args[i].bugengineType, param->type);
            result += argDistance;
            if (argDistance == s_maxDistance)
            {
                return s_maxDistance;
            }
        }
        else
        {
            return s_maxDistance;
        }
    }
    return result;
}

PyObject* call(raw<const RTTI::Method> method, PyObject* args, PyObject* kwargs)
{
    const u32 unnamedArgCount = s_library->m_PyTuple_Size(args);
    const u32 namedArgCount = kwargs ? s_library->m_PyDict_Size(kwargs) : 0;
    const u32 argCount = unnamedArgCount + namedArgCount;
    ArgInfo* argInfos = (ArgInfo*)malloca(argCount * sizeof(ArgInfo*));
    for (u32 i = 0; i < unnamedArgCount; ++i)
    {
        new (&argInfos[i]) ArgInfo(s_library->m_PyTuple_GetItem(args, i));
    }
    if (kwargs)
    {
        Py_ssize_t pos = 0;
        PyObject* key = 0;
        PyObject* item = 0;
        u32 i = 0;
        while (s_library->m_PyDict_Next(kwargs, &pos, &key, &item))
        {
            int version = s_library->getVersion();
            if (version >= 33)
            {
                new (&argInfos[unnamedArgCount+i]) ArgInfo(s_library->m_PyUnicode_AsUTF8(key), item);
            }
            else if (version >= 30)
            {
                PyObject* bytes = s_library->m_PyUnicode_AsASCIIString(key);
                if (!bytes)
                {
                    return 0;
                }
                const char* name = s_library->m_PyBytes_AsString(bytes);
                new (&argInfos[unnamedArgCount+i]) ArgInfo(name, item);
                Py_DECREF(bytes);
            }
            else
            {
                new (&argInfos[unnamedArgCount+i]) ArgInfo(s_library->m_PyString_AsString(key), item);
            }
            ++i;
        }
    }
    raw<const RTTI::Method::Overload> o = method->overloads;
    u32 bestDistance = s_maxDistance;
    raw<const RTTI::Method::Overload> bestOverload = { 0 };
    while (o)
    {
        if (o->parameterCount == argCount)
        {
            u32 d = distance(o, argInfos, unnamedArgCount);
            if (d < bestDistance)
            {
                bestDistance = d;
                bestOverload = o;
            }
        }
        o = o->next;
    }
    if (bestOverload)
    {
        RTTI::Value* values = (RTTI::Value*)malloca(argCount * sizeof(RTTI::Value));
        unpackValues(bestOverload, argInfos, unnamedArgCount, values);
        RTTI::Value v = bestOverload->call(values, bestOverload->parameterCount);
        for (int i = argCount-1; i >= 0; --i)
        {
            values[i].~Value();
        }
        freea(values);
        for (u32 i = argCount; i > 0; --i)
        {
            argInfos[i-1].~ArgInfo();
        }
        freea(argInfos);
        return PyBugObject::create(v);
    }
    else
    {
        for (u32 i = argCount; i > 0; --i)
        {
            argInfos[i-1].~ArgInfo();
        }
        freea(argInfos);
        s_library->m_PyErr_BadArgument();
        return 0;
    }
}

}}
