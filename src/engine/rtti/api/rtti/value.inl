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

template< typename T >
Value::Value(T t)
:   m_type(be_typeid<T>::type())
,   m_pointer(m_type.size() > sizeof(m_buffer) ? rttiArena().alloc(m_type.size()) : 0)
,   m_deallocate(m_pointer != 0)
,   m_reference(false)
{
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
    be_assert_recover(v.m_type.metaclass == m_type.metaclass, "Value has type %s; unable to copy from type %s" | m_type.name() | v.m_type.name(), return *this);
    be_assert_recover(m_type.constness != TypeInfo::Const, "Value is const", return *this);
    void* mem = memory();
    m_type.destroy(mem);
    m_type.copy(v.memory(), mem);
    return *this;
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
    be_assert(be_typeid<T>::type().metaclass == m_type.metaclass, "Value has type %s; unable to unbox to type %s" | m_type.name() | be_typeid<T>::type().name());
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

inline Value Value::operator()(const char *prop)
{
    return Value(ByRef(m_type.property(prop)->get(*this)));
}

inline bool Value::isConst() const
{
    return m_type.constness == TypeInfo::Const;
}

}

/*****************************************************************************/
#endif
