/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_VALUE_INL_
#define BE_RTTI_VALUE_INL_
/*****************************************************************************/
#include    <rtti/marshallers/marshallers.hh>

namespace BugEngine
{

Value::Value()
{
}

Value::Value(const RTTI::ValueRef& valueref) :
    m_value(valueref)
{
}

Value::Value(const RTTI::ValueCopy& cpy) :
    m_value(cpy)
{
}

template< typename T >
Value::Value(const T& immediate) :
    m_value(RTTI::ValueCopy(immediate))
{
}

Value::Value(const Value& other)
    :   m_value(other.m_value)
{
}

Value::~Value()
{
}

Value& Value::operator=(const Value& other)
{
    if(m_value.typeindex() != minitl::indexof<RTTI::ValueRef, typelist>::Value)
        m_value = other.m_value;
    else
        m_value.as<RTTI::ValueRef>() = other;
    return *this;
}

template< typename T >
Value& Value::operator=(const T& other)
{
    if(m_value.typeindex() != minitl::indexof<RTTI::ValueRef, typelist>::Value)
        m_value.as<RTTI::ValueCopy>() = other;
    else
        m_value.as<RTTI::ValueRef>() = Value(other);
    return *this;
}

template< typename T >
T Value::as() const
{
    return RTTI::Marshaller<T>().castto(*this);
}

template< typename T >
T Value::member() const
{
    if(m_value.typeindex() == minitl::indexof<RTTI::ValueCopy, typelist>::Value)
    {
        return m_value.as<RTTI::ValueCopy>().member<T>();
    }
    else if(m_value.typeindex() == minitl::indexof<RTTI::ValueRef, typelist>::Value)
    {
        return m_value.as<RTTI::ValueRef>().member<T>();
    }
    else
    {
        be_notreached();
        throw 0;
    }
}


RTTI::ValueTypeIndex Value::type() const
{
    if(m_value.typeindex() == minitl::indexof<RTTI::ValueCopy, typelist>::Value)
    {
        return m_value.as<RTTI::ValueCopy>().typeindex();
    }
    else if(m_value.typeindex() == minitl::indexof<RTTI::ValueRef, typelist>::Value)
    {
        return m_value.as<RTTI::ValueRef>().m_marshaller->type();
    }
    else
        return RTTI::ValueTypeIndex(0);
}

Value::operator void*() const
{
    return (void*)m_value.typeindex();
}

}


/*****************************************************************************/
#endif
