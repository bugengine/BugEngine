/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <python/stdafx.h>
#include    <python/pythonlib.hh>
#include    <py_object.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/engine/methodinfo.script.hh>

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

struct ArgInfo
{
    istring const       name;
    PyObject* const     arg;
    PyTypeObject* const pythonType;
    RTTI::Type const    bugengineType;

    ArgInfo(PyObject* object);
    ArgInfo(const istring name, PyObject* object);
};

ArgInfo::ArgInfo(PyObject* object)
    :   name("")
    ,   arg(object)
    ,   pythonType(object->py_type)
    ,   bugengineType()
{
}

ArgInfo::ArgInfo(const istring name, PyObject* object)
    :   name(name)
    ,   arg(object)
    ,   pythonType(object->py_type)
    ,   bugengineType()
{
}

static const u32 s_maxDistance = (u32)-1;

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

static void unpackValue(const ArgInfo& arg, const RTTI::Type& type, RTTI::Value* buffer)
{
    be_forceuse(arg);
    new (buffer) RTTI::Value(type, RTTI::Value::Reserve);
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
    be_forceuse(pythonType);
    be_forceuse(bugengineType);
    be_forceuse(desiredType);
    return s_maxDistance;
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

static PyObject* call(raw<const RTTI::Method> method, PyObject* args, PyObject* kwargs)
{
    const u32 unnamedArgCount = s_library->m_PyTuple_Size(args);
    const u32 namedArgCount = s_library->m_PyDict_Size(kwargs);
    const u32 argCount = unnamedArgCount + namedArgCount;
    ArgInfo* argInfos = (ArgInfo*)malloca(argCount * sizeof(ArgInfo*));
    for (u32 i = 0; i < unnamedArgCount; ++i)
    {
        new (&argInfos[i]) ArgInfo(s_library->m_PyList_GetItem(args, i));
    }
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
    raw<const RTTI::Method::Overload> o = method->overloads;
    u32 bestDistance = (u32)-1;
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
    for (u32 i = argCount; i > 0; --i)
    {
        argInfos[i-1].~ArgInfo();
    }
    freea(args);
    if (o)
    {
        RTTI::Value* values = (RTTI::Value*)malloca(argCount * sizeof(RTTI::Value));
        unpackValues(o, argInfos, unnamedArgCount, values);
        RTTI::Value v = o->call(values, o->parameterCount);
        for (int i = argCount-1; i >= 0; --i)
        {
            values[i].~Value();
        }
        freea(values);
        return PyBugObject::create(v);
    }
    else
    {
        // TODO: bad arg exception
        return 0;
    }
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
    int result = s_library->m_PyType_Ready(&s_bugengineValueType);
    be_assert(result >= 0, "unable to register type");
    be_forceuse(result);
    Py_INCREF(&s_bugengineValueType);
    result = (*s_library->m_PyModule_AddObject)(module, "Value", (PyObject*)&s_bugengineValueType);
    be_assert(result >= 0, "unable to register type");
    be_forceuse(result);
    result = (*s_library->m_PyModule_AddObject)(module, "BugEngine",
                                                create(RTTI::Value(be_game_Namespace())));
    be_assert(result >= 0, "unable to register type");
    be_forceuse(result);

}

}}
