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

#ifndef BE_MINITL_TYPE_CAST_
#define BE_MINITL_TYPE_CAST_
/*****************************************************************************/
#include    <core/debug/assert.hh>
#include    <typeinfo>
#include    <minitl/ptr/refptr.hh>

namespace minitl
{

template< typename U, typename T >
BE_ALWAYSINLINE U checked_cast(T value)
{
    be_assert(dynamic_cast<U>(value), "invalid cast from %s to %s" | typeid(T).name() | typeid(U).name());
    return static_cast<U>(value);
}

template< typename U, typename T >
BE_ALWAYSINLINE refptr<U> checked_cast(refptr<T> value)
{
    be_assert(dynamic_cast<U*>(value.get()), "invalid cast from %s* to %s*" | typeid(T).name() | typeid(U).name());
    return refptr<U>(static_cast<U*>(value.get()));
}

template< typename U, typename T >
BE_ALWAYSINLINE weakptr<U> checked_cast(weakptr<T> value)
{
    be_assert(dynamic_cast<U*>(value.get()), "invalid cast from %s* to %s*" | typeid(T).name() | typeid(U).name());
    return weakptr<U>(checked_cast<U>(refptr<T>(value.get())));
}

#pragma warning(push)
#pragma warning(disable:4800)

template< typename U, typename T >
BE_ALWAYSINLINE U checked_numcast(T value)
{
    be_assert(static_cast<T>(static_cast<U>(value)) == value, "precision loss during cast from %s to %s" | typeid(T).name() | typeid(U).name());
    return static_cast<U>(value);
}

#pragma warning(pop)

}

/*****************************************************************************/
#endif
