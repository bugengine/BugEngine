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
                                          name, prop->type.name());
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
    be_unimplemented();
    be_forceuse(arg);
    be_forceuse(type);
    be_forceuse(buffer);
}

static inline void unpackInteger(PyObject* arg, const RTTI::Type& type, RTTI::Value* buffer)
{
    be_unimplemented();
    be_forceuse(arg);
    be_forceuse(type);
    be_forceuse(buffer);
}

static inline void unpackString(PyObject* arg, const RTTI::Type& type, RTTI::Value* buffer)
{
    be_unimplemented();
    be_forceuse(arg);
    be_forceuse(type);
    be_forceuse(buffer);
}

static inline void unpackObject(PyObject* arg, const RTTI::Type& type, RTTI::Value* buffer)
{
    be_unimplemented();
    be_forceuse(arg);
    be_forceuse(type);
    be_forceuse(buffer);
}

u32 PyBugObject::distance(PyObject* object, const RTTI::Type& desiredType)
{
    if (object->py_type == &s_pyType)
    {
        PyBugObject* object_ = reinterpret_cast<PyBugObject*>(object);
        return object_->value.type().distance(desiredType);
    }
    else
    {
        //be_unimplemented();
        return RTTI::Type::MaxTypeDistance;
    }
}

void PyBugObject::unpack(PyObject* object, const RTTI::Type& desiredType, RTTI::Value* buffer)
{
    if (object->py_type == &s_pyType)
    {
        PyBugObject* object_ = reinterpret_cast<PyBugObject*>(object);
        be_assert(desiredType <= object_->value.type(),
                  "incompatible types: %s is not compatible with %s"
                    | object_->value.type().name().c_str()
                    | desiredType.name().c_str());
        new (buffer) RTTI::Value(object_->value);
    }
    else
    {
        switch(desiredType.metaclass->type())
        {
        case RTTI::ClassType_Array:
            unpackArray(object, desiredType, buffer);
            break;
        case RTTI::ClassType_Enum:
        case RTTI::ClassType_Number:
            unpackInteger(object, desiredType, buffer);
            break;
        case RTTI::ClassType_String:
            unpackString(object, desiredType, buffer);
            break;
        case RTTI::ClassType_Object:
        case RTTI::ClassType_Struct:
        case RTTI::ClassType_Pod:
            unpackObject(object, desiredType, buffer);
            break;
        case RTTI::ClassType_Variant:
        default:
            be_notreached();
            new (buffer) RTTI::Value();
        }
    }
}


}}
