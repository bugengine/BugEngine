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

#include    <rtti/stdafx.h>

#include    <rtti/marshallers/variant.hh>
#include    <rtti/object.hh>

namespace BugEngine { namespace RTTI
{

ValueTypeIndex Marshaller<Value>::type() const
{
    return PropertyTypeVariant;
}

ValueCopy Marshaller<Value>::get(void* source) const
{
    UNUSED(source);
    throw 0;
}

ValueCopy Marshaller<Value>::get(void* source, size_t index) const
{
    UNUSED(source);
    UNUSED(index);
    throw 0;
}

ValueCopy Marshaller<Value>::get(void* source, const Value& key) const
{
    UNUSED(source);
    UNUSED(key);
    throw 0;
}

void Marshaller<Value>::set(const Value& value, void* dst) const
{
    UNUSED(value);
    UNUSED(dst);
    throw 0;
}

void Marshaller<Value>::set(const Value& value, void* dst, size_t index) const
{
    UNUSED(value);
    UNUSED(dst);
    UNUSED(index);
    throw 0;
}

void Marshaller<Value>::set(const Value& value, void* dst, const Value& key) const
{
    UNUSED(value);
    UNUSED(dst);
    UNUSED(key);
    throw 0;
}

void Marshaller<Value>::push_back(const Value& value, void* dst) const
{
    UNUSED(value);
    UNUSED(dst);
    throw 0;
}

void Marshaller<Value>::push_front(const Value& value, void* dst) const
{
    UNUSED(value);
    UNUSED(dst);
    throw 0;
}

void Marshaller<Value>::pop_back(const Value& value, void* dst) const
{
    UNUSED(value);
    UNUSED(dst);
    throw 0;
}

void Marshaller<Value>::pop_front(const Value& value, void* dst) const
{
    UNUSED(value);
    UNUSED(dst);
    throw 0;
}

}}
