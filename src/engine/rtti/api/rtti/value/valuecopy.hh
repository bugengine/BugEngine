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

#ifndef BE_RTTI_VALUECOPY_HH_
#define BE_RTTI_VALUECOPY_HH_
/*****************************************************************************/
#include    <minitl/type/variant.hh>
#include    <rtti/value/valuetypes.hh>

namespace BugEngine { namespace RTTI
{

class ValueCopy
{
    friend class Value;
private:
    minitl::variant<ValueTypes> m_value;
public:
    template< typename T >
    inline explicit ValueCopy(const T& value);
    template< typename T >
    inline void operator=(const T& value);

    template< typename T >
    T as() const;
    template< typename T >
    T member() const;

    ValueTypeIndex typeindex() const;
};


}}

#include    <rtti/value/valuecopy.inl>

/*****************************************************************************/
#endif
