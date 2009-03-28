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

#ifndef BE_RTTI_PROPERTYSETTERS_INL_
#define BE_RTTI_PROPERTYSETTERS_INL_
/*****************************************************************************/


namespace BugEngine { namespace RTTI
{

template< typename T,
          typename OWNER >
void SetImpossible<T,OWNER>::set(OWNER* from, const T& value)
{
    throw 0;
}


template< typename T,
          typename OWNER,
          void (OWNER::*SETTER)(T value) >
void SetFromSetter<T,OWNER,SETTER>::set(OWNER* from, const T& value)
{
    (from->*SETTER)(value);
}

template< typename T, typename OWNER, size_t offset >
void SetFromField<T,OWNER,offset>::set(OWNER* from, const T& value)
{
    *reinterpret_cast<T*>(reinterpret_cast<char*>(from)+offset) = value;
}

}}

/*****************************************************************************/
#endif
