/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.scripting.pythonlib/stdafx.h>
#include <bugengine/meta/engine/call.hh>
#include <bugengine/meta/engine/scriptingapi.hh>
#include <bugengine/plugin.scripting.pythonlib/pythonlib.hh>
#include <py_array.hh>

namespace BugEngine { namespace Python {

static PyTypeObject::Py2NumberMethods s_py2ArrayNumber
    = {{0, 0, 0}, 0, 0, 0, 0, 0, 0, 0, &PyBugArray::nonZero,
       0,         0, 0, 0, 0, 0, 0, 0, 0,
       0,         0, 0, 0, 0, 0, 0, 0, 0,
       0,         0, 0, 0, 0, 0, 0, 0, 0,
       0};

static PyTypeObject::Py3NumberMethods s_py3ArrayNumber
    = {{0, 0, 0}, 0, 0, 0, 0, 0, 0, &PyBugArray::nonZero,
       0,         0, 0, 0, 0, 0, 0, 0,
       0,         0, 0, 0, 0, 0, 0, 0,
       0,         0, 0, 0, 0, 0, 0, 0,
       0,         0};

static PyTypeObject::PySequenceMethods s_pyArraySequence
    = {&PyBugArray::length, 0, 0, &PyBugArray::item, 0, &PyBugArray::setItem, 0, 0, 0, 0};

PyTypeObject PyBugArray::s_pyType = {{{0, 0}, 0},
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
                                     0,
                                     0};

PyObject* PyBugArray::stealValue(PyObject* owner, Meta::Value& value)
{
    be_assert(value.type().metaclass->type() == Meta::ClassType_Array,
              "PyBugArray only accepts Array types");
    PyBugArray* result = static_cast< PyBugArray* >(s_pyType.tp_alloc(&s_pyType, 0));
    (result)->owner    = owner;

    if(owner)
    {
        Py_INCREF(owner);
    }

    raw< const Meta::Class > arrayClass = value.type().metaclass;
    new(&(static_cast< PyBugArray* >(result))->value) Meta::Value();
    (static_cast< PyBugArray* >(result))->value.swap(value);
    be_assert(arrayClass->apiMethods,
              "Array type %s does not implement API methods" | arrayClass->fullname());
    be_assert(arrayClass->apiMethods->arrayScripting,
              "Array type %s does not implement Array API methods" | arrayClass->fullname());
    be_forceuse(arrayClass);
    return result;
}

PyObject* PyBugArray::repr(PyObject* self)
{
    PyBugObject*       self_ = static_cast< PyBugObject* >(self);
    const Meta::Value& v     = self_->value;

    const char* constness = (v.type().constness == Meta::Type::Const) ? "const " : "mutable ";
    const char* reference;
    const char* closing;
    switch(v.type().indirection)
    {
    case Meta::Type::RefPtr:
        reference = "ref<";
        closing   = ">";
        break;
    case Meta::Type::WeakPtr:
        reference = "weak<";
        closing   = ">";
        break;
    case Meta::Type::RawPtr:
        reference = "raw<";
        closing   = ">";
        break;
    case Meta::Type::Value:
        reference = "";
        constness = "";
        closing   = "";
        break;
    default:
        reference = "??? <";
        constness = "??? ";
        closing   = ">";
        break;
    }
    const char* access = (v.type().access == Meta::Type::Const) ? "const " : "";

    if(s_library->getVersion() >= 30)
    {
        return s_library->m_PyUnicode_FromFormat("[%s%s%s%s%s ]", constness, reference, access,
                                                 v.type().metaclass->name.c_str(), closing, &v);
    }
    else
    {
        return s_library->m_PyString_FromFormat("[%s%s%s%s%s ]", constness, reference, access,
                                                v.type().metaclass->name.c_str(), closing, &v);
    }
}

int PyBugArray::nonZero(PyObject* self)
{
    return length(self) > 0;
}

Py_ssize_t PyBugArray::length(PyObject* self)
{
    PyBugArray*      self_ = static_cast< PyBugArray* >(self);
    const Meta::Type t     = self_->value.type();
    be_assert(t.metaclass->type() == Meta::ClassType_Array, "PyBugArray expected array value");
    return Py_ssize_t(t.metaclass->apiMethods->arrayScripting->size(self_->value));
}

PyObject* PyBugArray::item(PyObject* self, Py_ssize_t index)
{
    PyBugArray* self_ = static_cast< PyBugArray* >(self);
    if(index >= 0 && index < length(self))
    {
        u32              index_ = be_checked_numcast< u32 >(index);
        const Meta::Type t      = self_->value.type();
        Meta::Value      v
            = t.isConst()
                  ? t.metaclass->apiMethods->arrayScripting->indexConst(self_->value, index_)
                  : t.metaclass->apiMethods->arrayScripting->index(self_->value, index_);
        return PyBugObject::stealValue(0, v);
    }
    else
    {
        s_library->m_PyErr_Format(*s_library->m_PyExc_IndexError, "Index out of range");
        return 0;
    }
}

int PyBugArray::setItem(PyObject* self, Py_ssize_t index, PyObject* value)
{
    PyBugArray*                          self_    = static_cast< PyBugArray* >(self);
    const Meta::Type                     t        = self_->value.type();
    raw< const Meta::ScriptingArrayAPI > arrayApi = t.metaclass->apiMethods->arrayScripting;
    if(t.isConst())
    {
        s_library->m_PyErr_Format(*s_library->m_PyExc_TypeError, "instance of %s is const",
                                  self_->value.type().name().c_str());
        return -1;
    }
    else if(distance(value, arrayApi->value_type) >= Meta::ConversionCost::s_incompatible)
    {
        s_library->m_PyErr_Format(*s_library->m_PyExc_TypeError,
                                  "Cannot convert to array value_type %s",
                                  arrayApi->value_type.name().c_str());
        return -1;
    }
    else
    {
        u32          index_ = be_checked_numcast< u32 >(index);
        Meta::Value* v      = (Meta::Value*)malloca(sizeof(Meta::Value));
        PyBugObject::unpack(value, arrayApi->value_type, v);
        arrayApi->index(self_->value, index_) = *v;
        v->~Value();
        freea(v);
        return 0;
    }
}

void PyBugArray::registerType(PyObject* module)
{
    if(s_library->getVersion() >= 30)
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

}}  // namespace BugEngine::Python
