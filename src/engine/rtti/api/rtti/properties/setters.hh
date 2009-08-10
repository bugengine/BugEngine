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

#ifndef BE_RTTI_PROPERTYSETTERS_HH_
#define BE_RTTI_PROPERTYSETTERS_HH_
/*****************************************************************************/

namespace BugEngine { namespace RTTI
{


template< typename OWNER,
          typename T >
class SetImpossible
{
public:
    typedef T PropertyType;
    typedef OWNER   Owner;
    enum
    {
        Write = 0,
        Ref = 0
    };
    static inline void set(OWNER* from, const T& value);
};

template< typename OWNER,
          typename T,
          void (OWNER::*SETTER)(T value) >
class SetFromSetter
{
public:
    typedef typename minitl::remove_const< typename minitl::remove_reference<T>::type >::type   PropertyType;
    typedef OWNER                                                                               Owner;
    enum
    {
        Write = 1
    };
    static inline void set(OWNER* from, const PropertyType& value);
};

template< typename OWNER, typename T, size_t offset >
class SetFromField
{
public:
    typedef T PropertyType;
    typedef OWNER   Owner;
    enum
    {
        Write = 1
    };
    static inline void set(OWNER* from, const T& value);
};

}}

#include    <rtti/properties/setters.inl>

/*****************************************************************************/
#endif
