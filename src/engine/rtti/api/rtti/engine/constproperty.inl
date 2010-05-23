/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_CONSTPROPERTY_INL_
#define BE_RTTI_ENGINE_CONSTPROPERTY_INL_
/*****************************************************************************/

namespace BugEngine { namespace RTTI
{

template< typename T >
ConstPropertyInfo<T>::ConstPropertyInfo(T value)
:   m_value(value)
{
}

template< typename T >
ConstPropertyInfo<T>::~ConstPropertyInfo()
{
}

template< typename T >
const TypeInfo* ConstPropertyInfo<T>::type() const
{
    return be_typeid<T>();
}

template< typename T >
Value ConstPropertyInfo<T>::get(void* /*from*/) const
{
    return m_value;
}

template< typename T >
void ConstPropertyInfo<T>::set(const Value& /*v*/, void* /*to*/) const
{
}

}}


/*****************************************************************************/
#endif
