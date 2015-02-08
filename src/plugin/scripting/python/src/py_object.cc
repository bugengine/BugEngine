/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <python/stdafx.h>
#include    <python/pythonlib.hh>
#include    <py_object.hh>
#include    <py_number.hh>
#include    <py_string.hh>
#include    <py_array.hh>
#include    <py_namespace.hh>
#include    <py_call.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/engine/propertyinfo.script.hh>

namespace BugEngine { namespace Python
{

static PyTypeObject::Py2NumberMethods s_py2ObjectNumber =
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
    0,
    &PyBugObject::nonZero,
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

static PyTypeObject::Py3NumberMethods s_py3ObjectNumber =
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
    &PyBugObject::nonZero,
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

PyTypeObject PyBugObject::s_pyType =
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
    if (!value)
    {
        PyObject* result = s_library->m__Py_NoneStruct;
        Py_INCREF(result);
        return result;
    }
    else switch(t.metaclass->type())
    {
    case RTTI::ClassType_Number:
        return PyBugNumber::create(value);
    case RTTI::ClassType_String:
        return PyBugString::create(value);
    case RTTI::ClassType_Array:
        return PyBugArray::create(value);
    case RTTI::ClassType_Namespace:
        return PyBugNamespace::create(value);
    default:
        {
            PyObject* result = s_pyType.tp_alloc(&s_pyType, 0);
            new(&((PyBugObject*)result)->value) RTTI::Value(value);
            return result;
        }
    }
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
                                  "%s object has no attribute %s",
                                  self_->value.type().name().c_str(), name);
        return NULL;
    }
}

int PyBugObject::setattr(PyObject* self, const char* name, PyObject* value)
{
    PyBugObject* self_ = reinterpret_cast<PyBugObject*>(self);
    istring name_(name);
    raw<const RTTI::Class> metaclass = self_->value.type().metaclass;
    if (metaclass == be_typeid<const RTTI::Class>::type().metaclass)
    {
        const RTTI::Class& klass = self_->value.as<const RTTI::Class&>();
        for (raw<const RTTI::ObjectInfo> ob = klass.objects; ob; ob = ob->next)
        {
            if (ob->name == name_)
            {
                u32 d = distance(value, ob->value.type());
                if (d < RTTI::Type::MaxTypeDistance)
                {
                    RTTI::Value* v = (RTTI::Value*)malloca(sizeof(RTTI::Value));
                    unpack(value, ob->value.type(), v);
                    ob->value = v;
                    v->~Value();
                    freea(v);
                    return 1;
                }
                else
                {
                    s_library->m_PyErr_Format(*s_library->m_PyExc_TypeError,
                                              "%s.%s is of type %s",
                                              self_->value.type().name().c_str(),
                                              name, ob->value.type().name().c_str());
                    return 0;
                }
            }
        }
        s_library->m_PyErr_Format(*s_library->m_PyExc_AttributeError,
                                  "%s object has no attribute %s",
                                  self_->value.type().name().c_str(), name);
        return 0;
    }
    else
    {
        raw<const RTTI::Property> prop = metaclass->getProperty(name_);
        if (!prop)
        {
            s_library->m_PyErr_Format(*s_library->m_PyExc_AttributeError,
                                      "%s object has no attribute %s",
                                      self_->value.type().name().c_str(), name);
            return 0;
        }
        else
        {
            u32 d = distance(value, prop->type);
            if (d < RTTI::Type::MaxTypeDistance)
            {
                RTTI::Value* v = (RTTI::Value*)malloca(sizeof(RTTI::Value));
                unpack(value, prop->type, v);
                prop->set(self_->value, *v);
                v->~Value();
                freea(v);
                return 1;
            }
            else
            {
                s_library->m_PyErr_Format(*s_library->m_PyExc_TypeError,
                                          "%s.%s is of type %s",
                                          self_->value.type().name().c_str(),
                                          name, prop->type.name().c_str());
                return 0;
            }
        }
    }
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

int PyBugObject::nonZero(PyObject *self)
{
    PyBugObject* self_ = reinterpret_cast<PyBugObject*>(self);
    const RTTI::Type t = self_->value.type();
    if (t.indirection == RTTI::Type::Value)
    {
        return 1;
    }
    else
    {
        return self_->value.as<const void* const>() != 0;
    }
}

void PyBugObject::registerType(PyObject* module)
{
    if (s_library->getVersion() >= 30)
        s_pyType.tp_as_number = (PyTypeObject::PyNumberMethods*)&s_py3ObjectNumber;
    else
        s_pyType.tp_as_number = (PyTypeObject::PyNumberMethods*)&s_py2ObjectNumber;
    int result = s_library->m_PyType_Ready(&s_pyType);
    be_assert(result >= 0, "unable to register type");
    Py_INCREF(&s_pyType);
    result = (*s_library->m_PyModule_AddObject)(module, "Value", (PyObject*)&s_pyType);
    be_assert(result >= 0, "unable to register type");
    be_forceuse(result);
}

static inline void unpackArray(PyObject* arg, const RTTI::Type& type, RTTI::Value* buffer)
{
    be_assert(type.metaclass->type() == RTTI::ClassType_Array,
              "expected to unpack Python Array into RTTI::ClassType_Array, got %s"
              | type.metaclass->name);
    be_unimplemented();
    be_forceuse(arg);
    be_forceuse(buffer);
}

static inline void unpackNumber(PyObject* arg, const RTTI::Type& type, RTTI::Value* buffer)
{
    be_assert(type.metaclass->type() == RTTI::ClassType_Number,
              "expected to unpack Python Number into RTTI::ClassType_Number, got %s"
              | type.metaclass->name);
    unsigned long long value = arg->py_type->tp_flags & Py_TPFLAGS_INT_SUBCLASS
            ?   (unsigned long long) s_library->m_PyInt_AsUnsignedLongMask(arg)
            :   (unsigned long long) s_library->m_PyLong_AsUnsignedLongLongMask(arg);
    switch(type.metaclass->index())
    {
    case 0:
        {
            bool v = value ? true : false;
            new (buffer) RTTI::Value(type, &v);
            break;
        }
    case 1:
        {
            u8 v = (u8)value;
            new (buffer) RTTI::Value(type, &v);
            break;
        }
    case 2:
        {
            u16 v = (u16)value;
            new (buffer) RTTI::Value(type, &v);
            break;
        }
    case 3:
        {
            u32 v = (u32)value;
            new (buffer) RTTI::Value(type, &v);
            break;
        }
    case 4:
        {
            u64 v = (u64)value;
            new (buffer) RTTI::Value(type, &v);
            break;
        }
    case 5:
        {
            i8 v = (i8)value;
            new (buffer) RTTI::Value(type, &v);
            break;
        }
    case 6:
        {
            i16 v = (i16)value;
            new (buffer) RTTI::Value(type, &v);
            break;
        }
    case 7:
        {
            i32 v = (i32)value;
            new (buffer) RTTI::Value(type, &v);
            break;
        }
    case 8:
        {
            i64 v = (i64)value;
            new (buffer) RTTI::Value(type, &v);
            break;
        }
    case 9:
        {
            float v = (float)value;
            new (buffer) RTTI::Value(type, &v);
            break;
        }
    case 10:
        {
            double v = (double)value;
            new (buffer) RTTI::Value(type, &v);
            break;
        }
    default:
        be_notreached();
        new (buffer) RTTI::Value(0);
    }
}

static inline void unpackFloat(PyObject* arg, const RTTI::Type& type, RTTI::Value* buffer)
{
    be_assert(type.metaclass->type() == RTTI::ClassType_Number,
              "expected to unpack Python Float into RTTI::ClassType_Number, got %s"
              | type.metaclass->name);
    double value = s_library->m_PyFloat_AsDouble(arg);
    switch(type.metaclass->index())
    {
    case 0:
        {
            bool v = value ? true : false;
            new (buffer) RTTI::Value(type, &v);
            break;
        }
    case 1:
        {
            u8 v = (u8)value;
            new (buffer) RTTI::Value(type, &v);
            break;
        }
    case 2:
        {
            u16 v = (u16)value;
            new (buffer) RTTI::Value(type, &v);
            break;
        }
    case 3:
        {
            u32 v = (u32)value;
            new (buffer) RTTI::Value(type, &v);
            break;
        }
    case 4:
        {
            u64 v = (u64)value;
            new (buffer) RTTI::Value(type, &v);
            break;
        }
    case 5:
        {
            i8 v = (i8)value;
            new (buffer) RTTI::Value(type, &v);
            break;
        }
    case 6:
        {
            i16 v = (i16)value;
            new (buffer) RTTI::Value(type, &v);
            break;
        }
    case 7:
        {
            i32 v = (i32)value;
            new (buffer) RTTI::Value(type, &v);
            break;
        }
    case 8:
        {
            i64 v = (i64)value;
            new (buffer) RTTI::Value(type, &v);
            break;
        }
    case 9:
        {
            float v = (float)value;
            new (buffer) RTTI::Value(type, &v);
            break;
        }
    case 10:
        {
            double v = (double)value;
            new (buffer) RTTI::Value(type, &v);
            break;
        }
    default:
        be_notreached();
        new (buffer) RTTI::Value(0);
    }
}
static inline void unpackString(PyObject* arg, const RTTI::Type& type, RTTI::Value* buffer)
{
    be_assert(type.metaclass->type() == RTTI::ClassType_String,
              "expected to unpack Python String into RTTI::ClassType_String, got %s"
              | type.metaclass->name);
    char* string;
    PyObject* decodedUnicode = 0;
    if (arg->py_type->tp_flags & Py_TPFLAGS_UNICODE_SUBCLASS)
    {
        if (s_library->getVersion() >= 33)
        {
            string = s_library->m_PyUnicode_AsUTF8(arg);
        }
        else
        {
            decodedUnicode = s_library->m_PyUnicode_AsASCIIString(arg);
            string = s_library->m_PyBytes_AsString(decodedUnicode);
        }
    }
    else
    {
        string = s_library->m_PyString_AsString(arg);
    }
    switch(type.metaclass->index())
    {
    case 0:
        new (buffer) RTTI::Value(istring(string));
        break;
    case 1:
        new (buffer) RTTI::Value(inamespace(string));
        break;
    case 2:
        new (buffer) RTTI::Value(ifilename(string));
        break;
    case 3:
        new (buffer) RTTI::Value(ipath(string));
        break;
    default:
        be_notreached();
        new (buffer) RTTI::Value();
    }

    if (decodedUnicode)
    {
        Py_DECREF(decodedUnicode);
    }
}

static inline void unpackPod(PyObject* arg, const RTTI::Type& type, RTTI::Value* buffer)
{
    be_assert(type.metaclass->type() == RTTI::ClassType_Pod,
              "expected to unpack Python Dict into RTTI::ClassType_Pod, got %s"
              | type.metaclass->name);

    new (buffer) RTTI::Value(type, RTTI::Value::Reserve);
    for (raw<const RTTI::Property> p = type.metaclass->properties; p; p = p->next)
    {
        PyObject* value = s_library->m_PyDict_GetItemString(arg, p->name.c_str());

        RTTI::Value* v = (RTTI::Value*)malloca(sizeof(RTTI::Value));
        PyBugObject::unpack(value, p->type, v);
        p->set(*buffer, *v);
        v->~Value();
        freea(v);
    }
}

u32 PyBugObject::distance(PyObject* object, const RTTI::Type& desiredType)
{
    if (object->py_type == &PyBugObject::s_pyType
     || object->py_type == &PyBugString::s_pyType
     || object->py_type == &PyBugNumber::s_pyType
     || object->py_type == &PyBugArray::s_pyType
     || object->py_type == &PyBugNamespace::s_pyType)
    {
        PyBugObject* object_ = reinterpret_cast<PyBugObject*>(object);
        return object_->value.type().distance(desiredType);
    }
    else if (object->py_type->tp_flags & (Py_TPFLAGS_INT_SUBCLASS|Py_TPFLAGS_LONG_SUBCLASS))
    {
        if (desiredType.metaclass->type() == RTTI::ClassType_Number)
        {
            switch (desiredType.metaclass->index())
            {
            case 0: //bool
            case 1: //u8
            case 5: //i8
            case 2: //u16
            case 6: //i16
            case 3: //u32
            case 7: //i32
                return 1;
            case 4: //u64
            case 8: //i64
                return 0;
            case 9:
            case 10:
                return 2;
            default:
                return (u32)RTTI::Type::MaxTypeDistance;
            }
        }
        else
        {
            return (u32)RTTI::Type::MaxTypeDistance;
        }
    }
    else if (object->py_type->tp_flags & (Py_TPFLAGS_LIST_SUBCLASS|Py_TPFLAGS_TUPLE_SUBCLASS))
    {
        return desiredType.metaclass->type() == RTTI::ClassType_Array
                ?   0
                :   (u32)RTTI::Type::MaxTypeDistance;
    }
    else if (object->py_type->tp_flags & (Py_TPFLAGS_STRING_SUBCLASS|Py_TPFLAGS_UNICODE_SUBCLASS))
    {
        return desiredType.metaclass->type() == RTTI::ClassType_String
                ?   0
                :   (u32)RTTI::Type::MaxTypeDistance;
    }
    else if (object->py_type->tp_flags & Py_TPFLAGS_DICT_SUBCLASS)
    {
        if (desiredType.metaclass->type() == RTTI::ClassType_Pod)
        {
            u32 i = 0;
            for (raw<const RTTI::Property> p = desiredType.metaclass->properties;
                 p;
                 p = p->next, ++i)
            {
                PyObject* value = s_library->m_PyDict_GetItemString(object, p->name.c_str());
                if (!value)
                {
                    return (u32)RTTI::Type::MaxTypeDistance;
                }
                if (distance(value, p->type) == RTTI::Type::MaxTypeDistance)
                {
                    return (u32)RTTI::Type::MaxTypeDistance;
                }
            }
            return i == s_library->m_PyDict_Size(object)
                    ?   0
                    :   (u32)RTTI::Type::MaxTypeDistance;
        }
        else //todo
        {
            return (u32)RTTI::Type::MaxTypeDistance;
        }
    }
    else if (object->py_type == s_library->m_PyFloat_Type)
    {
        if (desiredType.metaclass->type() == RTTI::ClassType_Number)
        {
            switch (desiredType.metaclass->index())
            {
            case 0: //bool
                return 3;
            case 1: //u8
            case 5: //i8
            case 2: //u16
            case 6: //i16
            case 3: //u32
            case 7: //i32
            case 4: //u64
            case 8: //i64
                return 2;
            case 9:
                return 1;
            case 10:
                return 0;
            default:
                return (u32)RTTI::Type::MaxTypeDistance;
            }
        }
        else
        {
            return (u32)RTTI::Type::MaxTypeDistance;
        }
    }
    else
    {
        return (u32)RTTI::Type::MaxTypeDistance;
    }
}

void PyBugObject::unpack(PyObject* object, const RTTI::Type& desiredType, RTTI::Value* buffer)
{
    if (object->py_type == &PyBugObject::s_pyType
     || object->py_type == &PyBugString::s_pyType
     || object->py_type == &PyBugNumber::s_pyType
     || object->py_type == &PyBugArray::s_pyType
     || object->py_type == &PyBugNamespace::s_pyType)
    {
        PyBugObject* object_ = reinterpret_cast<PyBugObject*>(object);
        be_assert(desiredType <= object_->value.type(),
                  "incompatible types: %s is not compatible with %s"
                    | object_->value.type().name().c_str()
                    | desiredType.name().c_str());
        new (buffer) RTTI::Value(object_->value);
    }
    else if (object->py_type->tp_flags & (Py_TPFLAGS_INT_SUBCLASS|Py_TPFLAGS_LONG_SUBCLASS))
    {
        unpackNumber(object, desiredType, buffer);
    }
    else if (object->py_type->tp_flags & (Py_TPFLAGS_LIST_SUBCLASS|Py_TPFLAGS_TUPLE_SUBCLASS))
    {
        unpackArray(object, desiredType, buffer);
    }
    else if (object->py_type->tp_flags & (Py_TPFLAGS_STRING_SUBCLASS|Py_TPFLAGS_UNICODE_SUBCLASS))
    {
        unpackString(object, desiredType, buffer);
    }
    else if (object->py_type->tp_flags & (Py_TPFLAGS_DICT_SUBCLASS))
    {
        if (desiredType.metaclass->type() == RTTI::ClassType_Pod)
        {
            unpackPod(object, desiredType, buffer);
        }
    }
    else if (object->py_type == s_library->m_PyFloat_Type)
    {
        unpackFloat(object, desiredType, buffer);
    }
    else
    {
        be_notreached();
        new (buffer) RTTI::Value();
    }
}


}}
