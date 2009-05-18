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

#ifndef BE_RTTI_PROPERTYGETTERS_HH_
#define BE_RTTI_PROPERTYGETTERS_HH_
/*****************************************************************************/
#include    <minitl/type/typemanipulation>

namespace BugEngine { namespace RTTI
{

template< typename T,
          typename OWNER >
class GetImpossible
{
public:
    typedef T PropertyType;
    typedef OWNER   Owner;
    enum { RefPossible = 0 };
    static inline T get(OWNER* from);
};

//-----------------------------------------------------------------------------

template< typename T,
          typename OWNER,
          T (OWNER::*GETTER)() >
class GetFromGetter
{
public:
    typedef T       PropertyType;
    typedef OWNER   Owner;
    enum { RefPossible = minitl::is_reference<T>::Value };
    static inline T get(OWNER* from);
};

//-----------------------------------------------------------------------------

template< typename T,
          typename OWNER,
          T (OWNER::*GETTER)() const >
class GetFromGetterConst
{
public:
    typedef T       PropertyType;
    typedef OWNER   Owner;
    enum { RefPossible = minitl::is_reference<T>::Value };
    static inline T get(OWNER* from);
};

//-----------------------------------------------------------------------------

template< typename T,
          typename OWNER,
          size_t offset >
class GetFromField
{
public:
    typedef T PropertyType;
    typedef OWNER   Owner;
    enum { RefPossible = 1 };
    static inline T& get(OWNER* from);
};

//-----------------------------------------------------------------------------

template< typename ValueGetter,
          bool UseRef,
          typename Marshaller >
struct ValueGetterProxy;

template< typename ValueGetter,
          typename Marshaller >
struct ValueGetterProxy<ValueGetter, true, Marshaller >
{
public:
    static inline Value get(typename ValueGetter::Owner* from, const Marshaller* marshaller);
};
template< typename ValueGetter,
          typename Marshaller >
struct ValueGetterProxy<ValueGetter, false, Marshaller >
{
public:
    static inline Value get(typename ValueGetter::Owner* from, const Marshaller* marshaller);
};

}}

#include    <rtti/properties/getters.inl>

/*****************************************************************************/
#endif
