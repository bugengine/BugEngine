/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_PROPERTYSETTERS_INL_
#define BE_RTTI_PROPERTYSETTERS_INL_
/*****************************************************************************/


namespace BugEngine { namespace RTTI
{

template< typename OWNER,
          typename T >
void SetImpossible<OWNER,T>::set(weak<OWNER> /*from*/, const T& /*value*/)
{
    throw 0;
}


template< typename OWNER,
          typename T,
          void (OWNER::*SETTER)(T value) >
void SetFromSetter<OWNER,T,SETTER>::set(weak<OWNER> from, const PropertyType& value)
{
    (from.operator->()->*SETTER)(value);
}

template< typename OWNER, typename T, size_t offset >
void SetFromField<OWNER,T,offset>::set(weak<OWNER> from, const T& value)
{
    *reinterpret_cast<T*>(reinterpret_cast<char*>(from.operator->())+offset) = value;
}

}}

/*****************************************************************************/
#endif
