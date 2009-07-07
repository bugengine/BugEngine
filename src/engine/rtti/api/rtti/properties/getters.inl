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

#ifndef BE_RTTI_PROPERTYGETTERS_INL_
#define BE_RTTI_PROPERTYGETTERS_INL_
/*****************************************************************************/

namespace BugEngine { namespace RTTI
{

template< typename OWNER,
          typename T >
inline T GetImpossible<OWNER,T>::get(OWNER* from)
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
