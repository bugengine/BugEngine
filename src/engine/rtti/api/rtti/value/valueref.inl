/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_VALUEREF_INL_
#define BE_RTTI_VALUEREF_INL_
/*****************************************************************************/
#include    <rtti/marshallers/abstract.hh>

namespace BugEngine { namespace RTTI
{

ValueRef::ValueRef(void* data, const AbstractMarshaller* marshaller) :
    m_data(data),
    m_marshaller(marshaller)
{
}

ValueRef::ValueRef(const ValueRef& other) :
    m_data(other.m_data),
    m_marshaller(other.m_marshaller)
{
}

ValueRef::~ValueRef()
{
}

void ValueRef::operator=(const Value& v)
{
    m_marshaller->set(v, m_data);
}

template< typename T >
T ValueRef::as() const
{
    return m_marshaller->get(m_data).as<T>();
}

template< typename T >
T ValueRef::member() const
{
    return m_marshaller->get(m_data).member<T>();
}

}}


/*****************************************************************************/
#endif
