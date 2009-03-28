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

#ifndef BE_RTTI_VALUECOPY_INL_
#define BE_RTTI_VALUECOPY_INL_
/*****************************************************************************/

namespace BugEngine { namespace RTTI
{

template< typename T >
inline ValueCopy::ValueCopy(const T& value) :
    m_value(value)
{
}

template< typename T >
inline T ValueCopy::as() const
{
    return T();
}

template< typename T >
inline T ValueCopy::member() const
{
    return m_value.as<T>();
}


inline ValueTypeIndex ValueCopy::typeindex() const
{
    return ValueTypeIndex(m_value.typeindex());
}


}}


/*****************************************************************************/
#endif
