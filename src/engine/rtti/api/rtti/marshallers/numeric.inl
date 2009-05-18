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

#ifndef BE_RTTI_NUMERIC_MARSHALLER_INL_
#define BE_RTTI_NUMERIC_MARSHALLER_INL_
/*****************************************************************************/

namespace BugEngine { namespace RTTI
{

template< typename FROM, typename TO >
ValueTypeIndex NumericMarshaller<FROM, TO>::type() const
{
    return ValueTypeIndex(minitl::indexof< TO, ValueTypes >::Value);
}

template< typename FROM, typename TO >
ValueCopy NumericMarshaller<FROM, TO>::get(void* source) const
{
    return castfrom(*reinterpret_cast<FROM*>(source));
}

template< typename FROM, typename TO >
ValueCopy NumericMarshaller<FROM, TO>::get(void* source, size_t index) const
{
    UNUSED(source);
    UNUSED(index);
    throw 0;
}

template< typename FROM, typename TO >
ValueCopy NumericMarshaller<FROM, TO>::get(void* source, const Value& key) const
{
    UNUSED(source);
    UNUSED(key);
    throw 0;
}

template< typename FROM, typename TO >
void NumericMarshaller<FROM, TO>::set(const Value& value, void* dst) const
{
    *reinterpret_cast<FROM*>(dst) = castto(value);
}

template< typename FROM, typename TO >
void NumericMarshaller<FROM, TO>::set(const Value& value, void* dst, size_t index) const
{
    UNUSED(value);
    UNUSED(dst);
    UNUSED(index);
    throw 0;
}

template< typename FROM, typename TO >
void NumericMarshaller<FROM, TO>::set(const Value& value, void* dst, const Value& key) const
{
    UNUSED(value);
    UNUSED(dst);
    UNUSED(key);
    throw 0;
}

template< typename FROM, typename TO >
void NumericMarshaller<FROM, TO>::push_back(const Value& value, void* dst) const
{
    UNUSED(value);
    UNUSED(dst);
    throw 0;
}

template< typename FROM, typename TO >
void NumericMarshaller<FROM, TO>::push_front(const Value& value, void* dst) const
{
    UNUSED(value);
    UNUSED(dst);
    throw 0;
}

template< typename FROM, typename TO >
void NumericMarshaller<FROM, TO>::pop_back(const Value& value, void* dst) const
{
    UNUSED(value);
    UNUSED(dst);
    throw 0;
}

template< typename FROM, typename TO >
void NumericMarshaller<FROM, TO>::pop_front(const Value& value, void* dst) const
{
    UNUSED(value);
    UNUSED(dst);
    throw 0;
}

template< typename FROM, typename TO >
ValueCopy NumericMarshaller<FROM, TO>::castfrom(FROM value) const
{
    return ValueCopy(checked_numcast<TO>(value));
}

template< typename FROM, typename TO >
FROM NumericMarshaller<FROM, TO>::castto(const Value& value) const
{
    return checked_numcast<FROM>(value.member<TO>());
}

}}


/*****************************************************************************/
#endif
