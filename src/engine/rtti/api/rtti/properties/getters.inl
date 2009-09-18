/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_PROPERTYGETTERS_INL_
#define BE_RTTI_PROPERTYGETTERS_INL_
/*****************************************************************************/

namespace BugEngine { namespace RTTI
{

template< typename OWNER,
          typename T >
inline T GetImpossible<OWNER,T>::get(OWNER* /*from*/)
{
    throw 0;
}

template< typename OWNER,
          typename T,
          T (OWNER::*GETTER)() >
T GetFromGetter<OWNER,T,GETTER>::get(OWNER* from)
{
    return (from->*GETTER)();
}

template< typename OWNER,
          typename T,
          T (OWNER::*GETTER)() const >
T GetFromGetterConst<OWNER,T,GETTER>::get(OWNER* from)
{
    return (from->*GETTER)();
}

template< typename OWNER, typename T, size_t offset >
T& GetFromField<OWNER,T,offset>::get(OWNER* from)
{
    return *reinterpret_cast<T*>(reinterpret_cast<char*>(from)+offset);
}

template< typename ValueGetter,
          typename Marshaller >
Value ValueGetterProxy<ValueGetter, true, Marshaller >::get(typename ValueGetter::Owner* from, const Marshaller* marshaller)
{
    return Value(ValueRef(&ValueGetter::get(from),marshaller));
}

template< typename ValueGetter,
          typename Marshaller >
Value ValueGetterProxy<ValueGetter, false, Marshaller >::get(typename ValueGetter::Owner* from, const Marshaller* marshaller)
{
    return Value(marshaller->castfrom(ValueGetter::get(from)));
}

}}

/*****************************************************************************/
#endif
