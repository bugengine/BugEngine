/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

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
    m_value = other.m_value;
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
        AssertNotReached();
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
