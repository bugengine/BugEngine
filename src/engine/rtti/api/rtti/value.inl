/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_VALUE_INL_
#define BE_RTTI_VALUE_INL_
/*****************************************************************************/
#include   <rtti/value.hh>
#include   <rtti/engine/propertyinfo.script.hh>

namespace BugEngine
{

template< typename T >
Value::Value(T t)
:   m_type(be_typeid<T>::type())
,   m_pointer(m_type.size() > sizeof(m_buffer) ? Memory<Arena::General>::alloc(m_type.size()) : 0)
,   m_deallocate(m_pointer != 0)
{
    m_type.copy(&t, memory());
}

Value::Value(const Value& other)
:   m_type(other.m_type)
,   m_pointer(m_type.size() > sizeof(m_buffer) ? Memory<Arena::General>::alloc(m_type.size()) : 0)
,   m_deallocate(m_pointer != 0)
{
    m_type.copy(other.memory(), memory());
}

Value::~Value()
{
    m_type.destroy(memory());
    if(m_type.size() > sizeof(m_buffer) && m_deallocate)
    {
        Memory<Arena::General>::free(m_pointer);
    }
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
    be_assert(be_typeid<T>::type() == m_type, "Value has type %s; unable to unbox to type %s" | m_type.name() | be_typeid<T>::type().name());
    return *(T*)memory();
}

void* Value::memory()
{
    if(m_type.size() <= sizeof(m_buffer))
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
    if(m_type.size() <= sizeof(m_buffer))
    {
        return m_buffer;
    }
    else
    {
        return m_pointer;
    }
}

Value& Value::operator=(const Value& other)
{
    be_assert(m_type == other.m_type, "Value has type %s; unable to copy from type %s" | m_type.name() | other.m_type.name());
    m_type.destroy(memory());
    m_type.copy(other.memory(), memory());
    return *this;
}

inline Value Value::operator()(const char *prop)
{
    return m_type.property(prop)->get(*this);
}

}

/*****************************************************************************/
#endif
