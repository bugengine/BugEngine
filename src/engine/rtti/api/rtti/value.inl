/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_VALUE_INL_
#define BE_RTTI_VALUE_INL_
/*****************************************************************************/
#include   <rtti/value.hh>
#include   <rtti/typeinfo.hh>
#include   <rtti/propertyinfo.hh>

namespace BugEngine
{

template< typename T >
Value::Value(const T& /*t*/)
:   m_type(be_typeid<T>::type())
{
}

Value::~Value()
{
}

template< typename T >
const T& Value::as() const
{
    be_assert(be_typeid<T>::type() == m_type, "Value has type %s; unable to unbox to type %s" | m_type->name | be_typeid<T>()->name);
    return *(const T*)pointer();
}

}

/*****************************************************************************/
#endif
