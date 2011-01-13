/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_VALUE_INL_
#define BE_RTTI_VALUE_INL_
/*****************************************************************************/
#include   <rtti/value.hh>
#include   <rtti/engine/propertyinfo.script.hh>
#include   <minitl/type/typemanipulation.hh>

namespace BugEngine
{

Value::Value()
:   m_type(be_typeid<void>::type())
,   m_pointer(0)
,   m_deallocate(0)
,   m_reference(false)
{
}

template< typename T >
Value::Value(T t)
:   m_type(be_typeid<T>::type())
,   m_pointer(m_type.size() > sizeof(m_buffer) ? rttiArena().alloc(m_type.size()) : 0)
,   m_deallocate(m_pointer != 0)
,   m_reference(false)
{
    m_type.copy(&t, memory());
}

template< typename T >
Value::Value(T t, ref<const RTTI::ClassInfo> metaclass)
:   m_type(TypeInfo(metaclass, TypeInfo::Type(RTTI::RefType<T>::Reference), TypeInfo::Constness(RTTI::RefType<T>::Constness)))
,   m_pointer(m_type.size() > sizeof(m_buffer) ? rttiArena().alloc(m_type.size()) : 0)
,   m_deallocate(m_pointer != 0)
,   m_reference(false)
{
    be_assert(be_typeid<T>::type() <= m_type, "specific typeinfo %s and typeid %s are not compatible" | m_type.name() | be_typeid<T>::type().name());
    m_type.copy(&t, memory());
}
template< typename T >
Value::Value(T t, TypeInfo typeinfo)
:   m_type(typeinfo)
,   m_pointer(m_type.size() > sizeof(m_buffer) ? rttiArena().alloc(m_type.size()) : 0)
,   m_deallocate(m_pointer != 0)
,   m_reference(false)
{
    be_assert(be_typeid<T>::type() <= typeinfo, "specific typeinfo %s and typeid %s are not compatible" | typeinfo.name() | be_typeid<T>::type().name());
    m_type.copy(&t, memory());
}

Value::Value(const Value& other)
:   m_type(other.m_type)
,   m_pointer(m_type.size() > sizeof(m_buffer) ? rttiArena().alloc(m_type.size()) : 0)
,   m_deallocate(m_pointer != 0)
,   m_reference(false)
{
    m_type.copy(other.memory(), memory());
}

template< typename T >
Value::Value(ByRefType<T> t)
:   m_type(be_typeid<T>::type())
,   m_pointer(const_cast<void*>((const void*)&t.value))
,   m_deallocate(0)
,   m_reference(true)
{
}

template<>
inline Value::Value(ByRefType<Value> t)
:   m_type(t.value.m_type)
,   m_pointer(t.value.m_pointer)
,   m_deallocate(0)
,   m_reference(true)
{
}

template<>
inline Value::Value(ByRefType<const Value> t)
:   m_type(t.value.m_type)
,   m_pointer(t.value.m_pointer)
,   m_deallocate(0)
,   m_reference(true)
{
}

Value::~Value()
{
    if(!m_reference)
    {
        m_type.destroy(memory());
        if(m_type.size() > sizeof(m_buffer) && m_deallocate)
        {
            rttiArena().free(m_pointer);
        }
    }
}

Value& Value::operator=(const Value& v)
{
    if(m_reference)
    {
        be_assert_recover(v.m_type.metaclass == m_type.metaclass, "Value has type %s; unable to copy from type %s" | m_type.name() | v.m_type.name(), return *this);
        be_assert_recover(m_type.constness != TypeInfo::Const, "Value is const", return *this);
        void* mem = memory();
        m_type.destroy(mem);
        m_type.copy(v.memory(), mem);
        return *this;
    }
    else
    {
        this->~Value();
        new ((void*)this) Value(v);
        return *this;
    }
}

template< typename T >
Value& Value::operator=(const T& t)
{
    be_assert_recover(be_typeid<T>::type().metaclass == m_type.metaclass, "Value has type %s; unable to copy from type %s" | m_type.name() | be_typeid<T>::type().name(), return *this);
    be_assert_recover(m_type.constness != TypeInfo::Const, "Value is const", return *this);
    void* mem = memory();
    m_type.destroy(mem);
    m_type.copy(&t, mem);
    return *this;
}

TypeInfo Value::type() const
{
    return m_type;
}

template< typename T >
const T& Value::as() const
{
    be_assert(be_typeid<T>::type() == m_type, "Value has type %s; unable to unbox to type %s" | m_type.name() | be_typeid<T>::type().name());
    return *(const T*)memory();
}

template< typename T >
T& Value::as()
{
    be_assert(minitl::is_const<T>::Value || (m_type.constness != TypeInfo::Const), "Value is const");
    be_assert(be_typeid<T>::type()<=m_type, "Value has type %s; unable to unbox to type %s" | m_type.name() | be_typeid<T>::type().name());
    return *(T*)memory();
}

void* Value::memory()
{
    if(!m_reference && m_type.size() <= sizeof(m_buffer))
    {
        return m_buffer;
    }
    else
    {
        return m_pointer;
    }
}

const void* Value::memory() const
{
    if(!m_reference && m_type.size() <= sizeof(m_buffer))
    {
        return m_buffer;
    }
    else
    {
        return m_pointer;
    }
}

bool Value::isConst() const
{
    return m_type.constness == TypeInfo::Const;
}

Value::operator const void*() const
{
    return (const void*)(m_type.metaclass != be_typeid<void>::klass());
}

bool Value::operator!() const
{
    return m_type.metaclass == be_typeid<void>::klass();
}

void* Value::rawget() const
{
    return m_type.rawget(memory());
}

Value Value::operator[](const istring& name)
{
    void* data = rawget();
    weak<const RTTI::ClassInfo> klass = m_type.metaclass;
    weak<const RTTI::PropertyInfo> prop = klass->getProperty(name);
    while(!prop)
    {
        data = (void*)((char*)data + klass->offset);
        klass = klass->parent;
        if(!klass)
            return Value();
        else
            prop = klass->getProperty(name);
    }
    if(prop->get)
    {
        return prop->get(prop, data);
    }
    else
    {
        be_error("Property %s of class %s (actual type %s) is not readable" | name | klass->name | m_type.metaclass->name);
        return Value();
    }
}

}

/*****************************************************************************/
#endif
