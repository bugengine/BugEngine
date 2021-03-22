/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.scripting.pythonlib/stdafx.h>
#include <bugengine/meta/classinfo.script.hh>
#include <bugengine/meta/engine/methodinfo.script.hh>
#include <bugengine/meta/engine/propertyinfo.script.hh>
#include <bugengine/meta/engine/scriptingapi.hh>
#include <bugengine/plugin.scripting.pythonlib/pythonlib.hh>
#include <py_array.hh>
#include <py_boundmethod.hh>
#include <py_call.hh>
#include <py_class.hh>
#include <py_enum.hh>
#include <py_namespace.hh>
#include <py_number.hh>
#include <py_object.hh>
#include <py_string.hh>

namespace BugEngine { namespace Python {

PyMethodDef PyBugObject::s_methods[]
    = {{"__dir__", &PyBugObject::dir, METH_NOARGS, NULL}, {NULL, NULL, 0, NULL}};

static PyTypeObject::Py2NumberMethods s_py2ObjectNumber
    = {{0, 0, 0}, 0, 0, 0, 0, 0, 0, 0, &PyBugObject::nonZero,
       0,         0, 0, 0, 0, 0, 0, 0, 0,
       0,         0, 0, 0, 0, 0, 0, 0, 0,
       0,         0, 0, 0, 0, 0, 0, 0, 0,
       0};

static PyTypeObject::Py3NumberMethods s_py3ObjectNumber
    = {{0, 0, 0}, 0, 0, 0, 0, 0, 0, &PyBugObject::nonZero,
       0,         0, 0, 0, 0, 0, 0, 0,
       0,         0, 0, 0, 0, 0, 0, 0,
       0,         0, 0, 0, 0, 0, 0, 0,
       0,         0};

PyTypeObject PyBugObject::s_pyType = {{{0, 0}, 0},
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
                                      "Wrapper class for the C++ class BugEngine::Meta::Value",
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
                                      &PyBugObject::newinst,
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

typedef PyObject* (*CreateMethod)(PyObject* owner, Meta::Value& value);

template < typename T >
PyObject* createPyNumeric(PyObject* owner, Meta::Value& value)
{
    be_forceuse(owner);
    unsigned long long v = static_cast< unsigned long long >(value.as< T >());
    return s_library->m_PyLong_FromUnsignedLongLong(v);
}

template <>
PyObject* createPyNumeric< bool >(PyObject* owner, Meta::Value& value)
{
    be_forceuse(owner);
    long v = static_cast< long >(value.as< bool >());
    return s_library->m_PyBool_FromLong(v);
}

template <>
PyObject* createPyNumeric< float >(PyObject* owner, Meta::Value& value)
{
    be_forceuse(owner);
    double v = static_cast< double >(value.as< float >());
    return s_library->m_PyFloat_FromDouble(v);
}

template <>
PyObject* createPyNumeric< double >(PyObject* owner, Meta::Value& value)
{
    be_forceuse(owner);
    double v = value.as< double >();
    return s_library->m_PyFloat_FromDouble(v);
}

PyObject* createPyString(PyObject* owner, Meta::Value& value)
{
    be_forceuse(owner);
    const text& t = static_cast< const text& >(value.as< const text& >());
    typedef PyObject* (*toStringType)(const char* format);
    toStringType toString = s_library->getVersion() >= 30 ? s_library->m_PyUnicode_FromString
                                                          : s_library->m_PyString_FromString;
    return toString(t.begin());
}

static CreateMethod s_createPyNumber[]
    = {&createPyNumeric< bool >,  &createPyNumeric< u8 >,    &createPyNumeric< u16 >,
       &createPyNumeric< u32 >,   &createPyNumeric< u64 >,   &createPyNumeric< i8 >,
       &createPyNumeric< i16 >,   &createPyNumeric< i32 >,   &createPyNumeric< i64 >,
       &createPyNumeric< float >, &createPyNumeric< double >};

static CreateMethod s_createNumber[]
    = {&PyBugNumber< bool >::stealValue,  &PyBugNumber< u8 >::stealValue,
       &PyBugNumber< u16 >::stealValue,   &PyBugNumber< u32 >::stealValue,
       &PyBugNumber< u64 >::stealValue,   &PyBugNumber< i8 >::stealValue,
       &PyBugNumber< i16 >::stealValue,   &PyBugNumber< i32 >::stealValue,
       &PyBugNumber< i64 >::stealValue,   &PyBugNumber< float >::stealValue,
       &PyBugNumber< double >::stealValue};

static CreateMethod s_createString[]
    = {&PyBugString< istring >::stealValue, &PyBugString< inamespace >::stealValue,
       &PyBugString< ifilename >::stealValue, &PyBugString< ipath >::stealValue,
       &PyBugString< text >::stealValue};

PyObject* PyBugObject::stealValue(PyObject* owner, Meta::Value& value)
{
    const Meta::Type& t = value.type();
    if(!value)
    {
        PyObject* result = s_library->m__Py_NoneStruct;
        Py_INCREF(result);
        return result;
    }
    else if(t.indirection != Meta::Type::Value && value.as< const void* const >() == 0)
    {
        PyObject* result = s_library->m__Py_NoneStruct;
        Py_INCREF(result);
        return result;
    }
    else if(t.indirection == Meta::Type::Value && t.metaclass->type() == Meta::ClassType_Number)
    {
        return s_createPyNumber[t.metaclass->index()](owner, value);
    }
    else if(t.indirection == Meta::Type::Value && t.metaclass->type() == Meta::ClassType_String
            && t.metaclass->index() == Meta::ClassIndex_text)
    {
        return createPyString(owner, value);
    }
    else
        switch(t.metaclass->type())
        {
        case Meta::ClassType_Number: return s_createNumber[t.metaclass->index()](owner, value);
        case Meta::ClassType_Enum: return PyBugEnum::stealValue(owner, value);
        case Meta::ClassType_String: return s_createString[t.metaclass->index()](owner, value);
        case Meta::ClassType_Array: return PyBugArray::stealValue(owner, value);
        case Meta::ClassType_Namespace:
        {
            const Meta::Class& cls = value.as< const Meta::Class& >();
            if(cls.constructor)
            {
                return PyBugClass::stealValue(owner, value);
            }
            else
            {
                return PyBugNamespace::stealValue(owner, value);
            }
        }
        default:
        {
            PyObject* result              = s_pyType.tp_alloc(&s_pyType, 0);
            ((PyBugObject*)result)->owner = owner;
            new(&(static_cast< PyBugObject* >(result))->value) Meta::Value();
            (static_cast< PyBugObject* >(result))->value.swap(value);
            if(owner)
            {
                Py_INCREF(owner);
            }
            return result;
        }
        }
}

PyObject* PyBugObject::newinst(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    be_forceuse(args);
    be_forceuse(kwds);
    PyBugObject* inst = static_cast< PyBugObject* >(type->tp_alloc(type, 0));
    inst->owner       = 0;
    new(&inst->value) Meta::Value();
    return inst;
}

PyObject* PyBugObject::getattr(PyObject* self, const char* name)
{
    PyBugObject*                self_     = static_cast< PyBugObject* >(self);
    raw< const Meta::Class >    metaclass = self_->value.type().metaclass;
    istring                     name_(name);
    raw< const Meta::Property > p = metaclass->getProperty(name_);
    if(p)
    {
        Meta::Value v = p->get(self_->value);
        return stealValue(self, v);
    }
    raw< const Meta::Method > m = metaclass->getMethod(name_);
    if(m)
    {
        PyObject* result = PyBoundMethod::create(m, self_);
        return result;
    }
    s_library->m_PyErr_Format(*s_library->m_PyExc_AttributeError, "%s object has no attribute %s",
                              self_->value.type().name().c_str(), name);
    return NULL;
}

int PyBugObject::setattr(PyObject* self, const char* name, PyObject* value)
{
    PyBugObject* self_ = static_cast< PyBugObject* >(self);
    if(self_->value.type().access == Meta::Type::Const)
    {
        s_library->m_PyErr_Format(*s_library->m_PyExc_TypeError, "instance of %s is const",
                                  self_->value.type().name().c_str());
        return -1;
    }
    raw< const Meta::Property > prop = self_->value.type().metaclass->getProperty(name);
    if(!prop)
    {
        s_library->m_PyErr_Format(*s_library->m_PyExc_AttributeError,
                                  "%s object has no attribute %s",
                                  self_->value.type().name().c_str(), name);
        return -1;
    }
    if(prop->type.access == Meta::Type::Const)
    {
        s_library->m_PyErr_Format(*s_library->m_PyExc_TypeError, "property %s.%s is const",
                                  self_->value.type().name().c_str(), name);
        return -1;
    }
    Meta::ConversionCost c = distance(value, prop->type);
    if(c >= Meta::ConversionCost::s_incompatible)
    {
        s_library->m_PyErr_Format(*s_library->m_PyExc_TypeError, "%s.%s is of type %s",
                                  self_->value.type().name().c_str(), name,
                                  prop->type.name().c_str());
        return -1;
    }
    Meta::Value* v = (Meta::Value*)malloca(sizeof(Meta::Value));
    unpack(value, prop->type, v);
    prop->set(self_->value, *v);
    v->~Value();
    freea(v);
    return 0;
}

PyObject* PyBugObject::repr(PyObject* self)
{
    PyBugObject*       self_ = static_cast< PyBugObject* >(self);
    const Meta::Value& v     = self_->value;
    if(s_library->getVersion() >= 30)
    {
        return s_library->m_PyUnicode_FromFormat("[%s @0x%p]", v.type().name().c_str(), &v);
    }
    else
    {
        return s_library->m_PyString_FromFormat("[%s @0x%p]", v.type().name().c_str(), &v);
    }
}

void PyBugObject::dealloc(PyObject* self)
{
    PyBugObject* self_ = static_cast< PyBugObject* >(self);
    if(self_->owner)
    {
        Py_DECREF(self_->owner);
    }
    self_->value.~Value();
    self->py_type->tp_free(self);
}

PyObject* PyBugObject::call(PyObject* self, PyObject* args, PyObject* kwds)
{
    PyBugObject*         self_ = static_cast< PyBugObject* >(self);
    static const istring callName("?call");
    Meta::Value          v(self_->value[callName]);
    if(!v)
    {
        s_library->m_PyErr_Format(*s_library->m_PyExc_Exception, "%s object is not callable",
                                  self_->value.type().name().c_str());
        return 0;
    }
    else
    {
        raw< const Meta::Method > method = v.as< raw< const Meta::Method > >();
        if(!method)
        {
            s_library->m_PyErr_Format(*s_library->m_PyExc_Exception, "%s object is not callable",
                                      self_->value.type().name().c_str());
            return 0;
        }
        return Python::call(method, NULL, args, kwds);
    }
}

int PyBugObject::nonZero(PyObject* self)
{
    PyBugObject*     self_ = static_cast< PyBugObject* >(self);
    const Meta::Type t     = self_->value.type();
    if(t.indirection == Meta::Type::Value)
    {
        return 1;
    }
    else
    {
        return self_->value.as< const void* const >() != 0;
    }
}

void PyBugObject::registerType(PyObject* module)
{
    if(s_library->getVersion() >= 30)
        s_pyType.tp_as_number = &s_py3ObjectNumber.nb_common;
    else
        s_pyType.tp_as_number = &s_py2ObjectNumber.nb_common;
    int result = s_library->m_PyType_Ready(&s_pyType);
    be_assert(result >= 0, "unable to register type");
    Py_INCREF(&s_pyType);
    result = (*s_library->m_PyModule_AddObject)(module, "Value", (PyObject*)&s_pyType);
    be_assert(result >= 0, "unable to register type");
    be_forceuse(result);

    PyBoundMethod::registerType(module);
}

static inline void unpackArray(PyObject* arg, const Meta::Type& type, void* buffer)
{
    be_assert(type.metaclass->type() == Meta::ClassType_Array
                  || type.metaclass->type() == Meta::ClassType_Variant,
              "expected to unpack Python Array into Meta::ClassType_Array, got %s"
                  | type.metaclass->name);
    be_unimplemented();
    be_forceuse(arg);
    be_forceuse(buffer);
}

static inline void unpackNumber(PyObject* arg, const Meta::Type& type, void* buffer)
{
    be_assert(type.metaclass->type() == Meta::ClassType_Number
                  || type.metaclass->type() == Meta::ClassType_Variant,
              "expected to unpack Python Number into Meta::ClassType_Number, got %s"
                  | type.metaclass->name);
    unsigned long long value
        = arg->py_type->tp_flags & Py_TPFLAGS_INT_SUBCLASS
              ? (unsigned long long)s_library->m_PyInt_AsUnsignedLongMask(arg)
              : (unsigned long long)s_library->m_PyLong_AsUnsignedLongLongMask(arg);
    switch(Meta::ClassIndex_Numeric(type.metaclass->index()))
    {
    case Meta::ClassIndex_bool:
    {
        bool v = value ? true : false;
        new(buffer) Meta::Value(v);
        break;
    }
    case Meta::ClassIndex_u8:
    {
        u8 v = (u8)value;
        new(buffer) Meta::Value(v);
        break;
    }
    case Meta::ClassIndex_u16:
    {
        u16 v = (u16)value;
        new(buffer) Meta::Value(v);
        break;
    }
    case Meta::ClassIndex_u32:
    {
        u32 v = (u32)value;
        new(buffer) Meta::Value(v);
        break;
    }
    case Meta::ClassIndex_u64:
    {
        u64 v = (u64)value;
        new(buffer) Meta::Value(v);
        break;
    }
    case Meta::ClassIndex_i8:
    {
        i8 v = (i8)value;
        new(buffer) Meta::Value(v);
        break;
    }
    case Meta::ClassIndex_i16:
    {
        i16 v = (i16)value;
        new(buffer) Meta::Value(v);
        break;
    }
    case Meta::ClassIndex_i32:
    {
        i32 v = (i32)value;
        new(buffer) Meta::Value(v);
        break;
    }
    case Meta::ClassIndex_i64:
    {
        i64 v = (i64)value;
        new(buffer) Meta::Value(v);
        break;
    }
    case Meta::ClassIndex_float:
    {
        float v = (float)value;
        new(buffer) Meta::Value(v);
        break;
    }
    case Meta::ClassIndex_double:
    {
        double v = (double)value;
        new(buffer) Meta::Value(v);
        break;
    }
    default: be_notreached(); new(buffer) Meta::Value(0);
    }
}

static inline void unpackFloat(PyObject* arg, const Meta::Type& type, void* buffer)
{
    be_assert(type.metaclass->type() == Meta::ClassType_Number
                  || type.metaclass->type() == Meta::ClassType_Variant,
              "expected to unpack Python Float into Meta::ClassType_Number, got %s"
                  | type.metaclass->name);
    double value = s_library->m_PyFloat_AsDouble(arg);
    switch(type.metaclass->index())
    {
    case 0:
    {
        bool v = value ? true : false;
        new(buffer) Meta::Value(v);
        break;
    }
    case 1:
    {
        u8 v = (u8)value;
        new(buffer) Meta::Value(v);
        break;
    }
    case 2:
    {
        u16 v = (u16)value;
        new(buffer) Meta::Value(v);
        break;
    }
    case 3:
    {
        u32 v = (u32)value;
        new(buffer) Meta::Value(v);
        break;
    }
    case 4:
    {
        u64 v = (u64)value;
        new(buffer) Meta::Value(v);
        break;
    }
    case 5:
    {
        i8 v = (i8)value;
        new(buffer) Meta::Value(v);
        break;
    }
    case 6:
    {
        i16 v = (i16)value;
        new(buffer) Meta::Value(v);
        break;
    }
    case 7:
    {
        i32 v = (i32)value;
        new(buffer) Meta::Value(v);
        break;
    }
    case 8:
    {
        i64 v = (i64)value;
        new(buffer) Meta::Value(v);
        break;
    }
    case 9:
    {
        float v = (float)value;
        new(buffer) Meta::Value(v);
        break;
    }
    case 10:
    {
        double v = (double)value;
        new(buffer) Meta::Value(v);
        break;
    }
    default: be_notreached(); new(buffer) Meta::Value(0);
    }
}

static inline void unpackString(PyObject* arg, const Meta::Type& type, void* buffer)
{
    be_assert(type.metaclass->type() == Meta::ClassType_String
                  || type.metaclass->type() == Meta::ClassType_Variant,
              "expected to unpack Python String into Meta::ClassType_String, got %s"
                  | type.metaclass->name);
    char*     string;
    PyObject* decodedUnicode = 0;
    if(arg->py_type->tp_flags & Py_TPFLAGS_UNICODE_SUBCLASS)
    {
        if(s_library->getVersion() >= 33)
        {
            string = s_library->m_PyUnicode_AsUTF8(arg);
        }
        else
        {
            decodedUnicode = s_library->m_PyUnicode_AsASCIIString(arg);
            string         = s_library->m_PyBytes_AsString(decodedUnicode);
        }
    }
    else
    {
        string = s_library->m_PyString_AsString(arg);
    }
    switch(type.metaclass->index())
    {
    case 0: new(buffer) Meta::Value(istring(string)); break;
    case 1: new(buffer) Meta::Value(inamespace(string)); break;
    case 2: new(buffer) Meta::Value(ifilename(string)); break;
    case 3: new(buffer) Meta::Value(ipath(string)); break;
    default: be_notreached(); new(buffer) Meta::Value();
    }

    if(decodedUnicode)
    {
        Py_DECREF(decodedUnicode);
    }
}

static inline void unpackPod(PyObject* arg, const Meta::Type& type, void* buffer)
{
    be_assert(type.metaclass->type() == Meta::ClassType_Pod,
              "expected to unpack Python Dict into Meta::ClassType_Pod, got %s"
                  | type.metaclass->name);

    Meta::Value* result = new(buffer) Meta::Value(type, Meta::Value::Reserve);
    Meta::Value* v      = (Meta::Value*)malloca(sizeof(Meta::Value));
    for(raw< const Meta::Class > c = type.metaclass; c; c = c->parent)
    {
        for(const Meta::Property* p = c->properties.begin(); p != c->properties.end(); ++p)
        {
            PyObject* value = s_library->m_PyDict_GetItemString(arg, p->name.c_str());

            PyBugObject::unpack(value, p->type, v);
            p->set(*result, *v);
            v->~Value();
        }
    }
    freea(v);
}

Meta::ConversionCost PyBugObject::distance(PyObject* object, const Meta::Type& desiredType)
{
    if(desiredType.metaclass->type() == Meta::ClassType_Variant)
    {
        if(object->py_type == &PyBugObject::s_pyType
           || object->py_type->tp_base == &PyBugObject::s_pyType
           || object->py_type->tp_flags & (Py_TPFLAGS_INT_SUBCLASS | Py_TPFLAGS_LONG_SUBCLASS)
           || object->py_type == s_library->m_PyFloat_Type)
        {
            return Meta::ConversionCost::s_variant;
        }
        else
        {
            return Meta::ConversionCost::s_incompatible;
        }
    }
    else if(object->py_type == &PyBugObject::s_pyType
            || object->py_type->tp_base == &PyBugObject::s_pyType
            || object->py_type->tp_base->tp_base == &PyBugObject::s_pyType)
    {
        PyBugObject* object_ = static_cast< PyBugObject* >(object);
        return object_->value.type().calculateConversion(desiredType);
    }
    else if(object->py_type == s_library->m_PyBool_Type)
    {
        return Meta::ConversionCalculator< bool >::calculate(desiredType);
    }
    else if(object->py_type->tp_flags & Py_TPFLAGS_INT_SUBCLASS)
    {
        return Meta::ConversionCalculator< i32 >::calculate(desiredType);
    }
    else if(object->py_type->tp_flags & Py_TPFLAGS_LONG_SUBCLASS)
    {
        return Meta::ConversionCalculator< i64 >::calculate(desiredType);
    }
    else if(object->py_type->tp_flags & (Py_TPFLAGS_LIST_SUBCLASS | Py_TPFLAGS_TUPLE_SUBCLASS))
    {
        if(desiredType.metaclass->type() == Meta::ClassType_Array)
        {
            PyTuple_SizeType    size = object->py_type->tp_flags & (Py_TPFLAGS_LIST_SUBCLASS)
                                           ? s_library->m_PyList_Size
                                           : s_library->m_PyTuple_Size;
            PyTuple_GetItemType get  = object->py_type->tp_flags & (Py_TPFLAGS_LIST_SUBCLASS)
                                           ? s_library->m_PyList_GetItem
                                           : s_library->m_PyTuple_GetItem;
            if(size(object) != 0)
            {
                raw< const Meta::ScriptingArrayAPI > api
                    = desiredType.metaclass->apiMethods->arrayScripting;
                const Meta::Type& subType     = api->value_type;
                PyObject*         firstObject = get(object, 0);
                return distance(firstObject, subType);
            }
            else
            {
                return Meta::ConversionCost();
            }
        }
        else
        {
            return Meta::ConversionCost::s_incompatible;
        }
    }
    else if(object->py_type->tp_flags & (Py_TPFLAGS_STRING_SUBCLASS | Py_TPFLAGS_UNICODE_SUBCLASS))
    {
        return desiredType.metaclass->type() == Meta::ClassType_String
                   ? Meta::ConversionCost()
                   : Meta::ConversionCost::s_incompatible;
    }
    else if(object->py_type->tp_flags & Py_TPFLAGS_DICT_SUBCLASS)
    {
        if(desiredType.metaclass->type() == Meta::ClassType_Pod)
        {
            u32 i = 0;
            for(raw< const Meta::Class > c = desiredType.metaclass; c; c = c->parent)
            {
                for(const Meta::Property* p = c->properties.begin(); p != c->properties.end(); ++p)
                {
                    PyObject* value = s_library->m_PyDict_GetItemString(object, p->name.c_str());
                    if(!value)
                    {
                        return Meta::ConversionCost::s_incompatible;
                    }
                    if(distance(value, p->type) >= Meta::ConversionCost::s_incompatible)
                    {
                        return Meta::ConversionCost::s_incompatible;
                    }
                }
            }
            return i == (u32)s_library->m_PyDict_Size(object)
                       ? Meta::ConversionCost()
                       : Meta::ConversionCost::s_incompatible;
        }
        else
        {
            be_unimplemented();
            return Meta::ConversionCost::s_incompatible;
        }
    }
    else if(object == s_library->m__Py_NoneStruct)
    {
        return desiredType.indirection >= Meta::Type::RawPtr ? Meta::ConversionCost()
                                                             : Meta::ConversionCost::s_incompatible;
    }
    else if(object->py_type == s_library->m_PyFloat_Type)
    {
        return Meta::ConversionCalculator< float >::calculate(desiredType);
    }
    else
    {
        return Meta::ConversionCost::s_incompatible;
    }
}

void PyBugObject::unpack(PyObject* object, const Meta::Type& desiredType, void* buffer)
{
    if(desiredType.metaclass->type() == Meta::ClassType_Variant)
    {
        unpackAny(object, buffer);
    }
    else if(object->py_type == &PyBugObject::s_pyType
            || object->py_type->tp_base == &PyBugObject::s_pyType)
    {
        PyBugObject* object_ = static_cast< PyBugObject* >(object);
        be_assert(desiredType <= object_->value.type(),
                  "incompatible types: %s is not compatible with %s"
                      | object_->value.type().name().c_str() | desiredType.name().c_str());
        new(buffer) Meta::Value(Meta::Value::ByRef(object_->value));
    }
    else if(object->py_type->tp_flags & (Py_TPFLAGS_INT_SUBCLASS | Py_TPFLAGS_LONG_SUBCLASS))
    {
        unpackNumber(object, desiredType, buffer);
    }
    else if(object->py_type->tp_flags & (Py_TPFLAGS_LIST_SUBCLASS | Py_TPFLAGS_TUPLE_SUBCLASS))
    {
        unpackArray(object, desiredType, buffer);
    }
    else if(object->py_type->tp_flags & (Py_TPFLAGS_STRING_SUBCLASS | Py_TPFLAGS_UNICODE_SUBCLASS))
    {
        unpackString(object, desiredType, buffer);
    }
    else if(object->py_type->tp_flags & (Py_TPFLAGS_DICT_SUBCLASS))
    {
        if(desiredType.metaclass->type() == Meta::ClassType_Pod)
        {
            unpackPod(object, desiredType, buffer);
        }
    }
    else if(object->py_type == s_library->m_PyFloat_Type)
    {
        unpackFloat(object, desiredType, buffer);
    }
    else if(object == s_library->m__Py_NoneStruct)
    {
        void* ptr = 0;
        new(buffer) Meta::Value(desiredType, &ptr);
    }
    else
    {
        be_notreached();
        new(buffer) Meta::Value();
    }
}

PyObject* PyBugObject::dir(raw< const Meta::Class > metaclass)
{
    PyObject* result = s_library->m_PyList_New(0);
    if(!result) return NULL;
    PyString_FromStringAndSizeType fromString = s_library->getVersion() >= 30
                                                    ? s_library->m_PyUnicode_FromStringAndSize
                                                    : s_library->m_PyString_FromStringAndSize;

    for(raw< const Meta::ObjectInfo > o = metaclass->objects; o; o = o->next)
    {
        PyObject* str = fromString(o->name.c_str(), o->name.size());
        if(!str)
        {
            Py_DECREF(result);
            return NULL;
        }
        if(s_library->m_PyList_Append(result, str) == -1)
        {
            Py_DECREF(str);
            Py_DECREF(result);
            return NULL;
        }
        Py_DECREF(str);
    }
    for(raw< const Meta::Class > cls = metaclass; cls; cls = cls->parent)
    {
        for(const Meta::Property* p = cls->properties.begin(); p != cls->properties.end(); ++p)
        {
            PyObject* str = fromString(p->name.c_str(), p->name.size());
            if(!str)
            {
                Py_DECREF(result);
                return NULL;
            }
            if(s_library->m_PyList_Append(result, str) == -1)
            {
                Py_DECREF(str);
                Py_DECREF(result);
                return NULL;
            }
            Py_DECREF(str);
        }
        for(const Meta::Method* m = cls->methods.begin(); m != cls->methods.end(); ++m)
        {
            PyObject* str = fromString(m->name.c_str(), m->name.size());
            if(!str)
            {
                Py_DECREF(result);
                return NULL;
            }
            if(s_library->m_PyList_Append(result, str) == -1)
            {
                Py_DECREF(str);
                Py_DECREF(result);
                return NULL;
            }
            Py_DECREF(str);
        }
    }
    return result;
}

PyObject* PyBugObject::dir(PyObject* self, PyObject* args)
{
    be_forceuse(args);
    return dir(static_cast< PyBugObject* >(self)->value.type().metaclass);
}

void PyBugObject::unpackAny(PyObject* object, void* buffer)
{
    if(object->py_type == &PyBugObject::s_pyType
       || object->py_type->tp_base == &PyBugObject::s_pyType)
    {
        PyBugObject* object_ = static_cast< PyBugObject* >(object);
        new(buffer) Meta::Value(object_->value);
    }
    else if(object->py_type->tp_flags & Py_TPFLAGS_INT_SUBCLASS)
    {
        unpackNumber(object, be_type< i32 >(), buffer);
    }
    else if(object->py_type->tp_flags & Py_TPFLAGS_LONG_SUBCLASS)
    {
        unpackNumber(object, be_type< i64 >(), buffer);
    }
    else if(object->py_type == s_library->m_PyFloat_Type)
    {
        unpackFloat(object, be_type< double >(), buffer);
    }
    else if(object == s_library->m__Py_NoneStruct)
    {
        new(buffer) Meta::Value((const void*)0);
    }
    else
    {
        be_notreached();
        new(buffer) Meta::Value();
    }
}

}}  // namespace BugEngine::Python
