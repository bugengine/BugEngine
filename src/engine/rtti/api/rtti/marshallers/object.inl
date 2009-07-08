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

#ifndef BE_RTTI_OBJECT_MARSHALLER_INL_
#define BE_RTTI_OBJECT_MARSHALLER_INL_
/*****************************************************************************/
#include    <rtti/value.hh>

namespace BugEngine { namespace RTTI
{

template< typename T >
ValueTypeIndex Marshaller< refptr<T> >::type() const
{
    return ValueTypeIndex(minitl::indexof< refptr<Object>, ValueTypes >::Value);
}

template< typename T >
ValueCopy Marshaller< refptr<T> >::get(void* source) const
{
    return castfrom(*reinterpret_cast<refptr<T>*>(source));
}

template< typename T >
ValueCopy Marshaller< refptr<T> >::get(void* source, size_t index) const
{
    UNUSED(source);
    UNUSED(index);
    throw 0;
}

template< typename T >
ValueCopy Marshaller< refptr<T> >::get(void* source, const Value& key) const
{
    UNUSED(source);
    UNUSED(key);
    throw 0;
}

template< typename T >
void Marshaller< refptr<T> >::set(const Value& value, void* dst) const
{
    *reinterpret_cast<refptr<T>*>(dst) = castto(value);
}

template< typename T >
void Marshaller< refptr<T> >::set(const Value& value, void* dst, size_t index) const
{
    UNUSED(value);
    UNUSED(dst);
    UNUSED(index);
    throw 0;
}

template< typename T >
void Marshaller< refptr<T> >::set(const Value& value, void* dst, const Value& key) const
{
    UNUSED(value);
    UNUSED(dst);
    UNUSED(key);
    throw 0;
}

template< typename T >
void Marshaller< refptr<T> >::push_back(const Value& value, void* dst) const
{
    UNUSED(value);
    UNUSED(dst);
    throw 0;
}

template< typename T >
void Marshaller< refptr<T> >::push_front(const Value& value, void* dst) const
{
    UNUSED(value);
    UNUSED(dst);
    throw 0;
}

template< typename T >
void Marshaller< refptr<T> >::pop_back(const Value& value, void* dst) const
{
    UNUSED(value);
    UNUSED(dst);
    throw 0;
}

template< typename T >
void Marshaller< refptr<T> >::pop_front(const Value& value, void* dst) const
{
    UNUSED(value);
    UNUSED(dst);
    throw 0;
}

template< typename T >
ValueCopy Marshaller< refptr<T> >::castfrom(refptr<T> value) const
{
    return ValueCopy(refptr<Object>(value.get()));
}

template< typename T >
refptr<T> Marshaller< refptr<T> >::castto(const Value& value) const
{
    return refptr<T>(checked_cast<T*>(value.member< refptr<Object> >().get()));
}

//-----------------------------------------------------------------------------

template< typename T >
ValueTypeIndex Marshaller<T*>::type() const
{
    return ValueTypeIndex(minitl::indexof< Object*, ValueTypes >::Value);
}

template< typename T >
ValueCopy Marshaller<T*>::get(void* source) const
{
    return castfrom(*reinterpret_cast<T**>(source));
}

template< typename T >
ValueCopy Marshaller<T*>::get(void* /*source*/, size_t /*index*/) const
{
    throw 0;
}

template< typename T >
ValueCopy Marshaller<T*>::get(void* /*source*/, const Value& /*key*/) const
{
    throw 0;
}

template< typename T >
void Marshaller<T*>::set(const Value& value, void* dst) const
{
    *reinterpret_cast<T**>(dst) = castto(value);
}

template< typename T >
void Marshaller<T*>::set(const Value& /*value*/, void* /*dst*/, size_t /*index*/) const
{
    throw 0;
}

template< typename T >
void Marshaller<T*>::set(const Value& /*value*/, void* /*dst*/, const Value& /*key*/) const
{
    throw 0;
}

template< typename T >
void Marshaller<T*>::push_back(const Value& /*value*/, void* /*dst*/) const
{
    throw 0;
}

template< typename T >
void Marshaller<T*>::push_front(const Value& /*value*/, void* /*dst*/) const
{
    throw 0;
}

template< typename T >
void Marshaller<T*>::pop_back(const Value& /*value*/, void* /*dst*/) const
{
    throw 0;
}

template< typename T >
void Marshaller<T*>::pop_front(const Value& /*value*/, void* /*dst*/) const
{
    throw 0;
}

template< typename T >
ValueCopy Marshaller<T*>::castfrom(T* value) const
{
    return ValueCopy(static_cast<Object*>(value));
}

template< typename T >
T* Marshaller<T*>::castto(const Value& value) const
{
    return checked_cast<T*>(value.member<Object*>());
}


}}


/*****************************************************************************/
#endif
