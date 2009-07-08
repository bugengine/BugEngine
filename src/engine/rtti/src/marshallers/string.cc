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

#include    <rtti/marshallers/string.hh>

namespace BugEngine { namespace RTTI
{

ValueTypeIndex Marshaller<std::string>::type() const
{
    return ValueTypeIndex(minitl::indexof< std::string, ValueTypes >::Value);
}

ValueCopy Marshaller<std::string>::get(void* source) const
{
    return castfrom(*reinterpret_cast<std::string*>(source));
}

void Marshaller<std::string>::set(const Value& value, void* dst) const
{
    *reinterpret_cast<std::string*>(dst) = castto(value);
}

//-----------------------------------------------------------------------------

ValueTypeIndex Marshaller<istring>::type() const
{
    return ValueTypeIndex(minitl::indexof< std::string, ValueTypes >::Value);
}

ValueCopy Marshaller<istring>::get(void* source) const
{
    return castfrom(*reinterpret_cast<istring*>(source));
}

void Marshaller<istring>::set(const Value& value, void* dst) const
{
    *reinterpret_cast<istring*>(dst) = castto(value);
}

//-----------------------------------------------------------------------------

ValueTypeIndex Marshaller<inamespace>::type() const
{
    return ValueTypeIndex(minitl::indexof< std::string, ValueTypes >::Value);
}

ValueCopy Marshaller<inamespace>::get(void* source) const
{
    return castfrom(*reinterpret_cast<inamespace*>(source));
}

void Marshaller<inamespace>::set(const Value& value, void* dst) const
{
    *reinterpret_cast<inamespace*>(dst) = castto(value);
}

//-----------------------------------------------------------------------------

ValueTypeIndex Marshaller<ipath>::type() const
{
    return ValueTypeIndex(minitl::indexof< std::string, ValueTypes >::Value);
}

ValueCopy Marshaller<ipath>::get(void* source) const
{
    return castfrom(*reinterpret_cast<ipath*>(source));
}

void Marshaller<ipath>::set(const Value& value, void* dst) const
{
    *reinterpret_cast<ipath*>(dst) = castto(value);
}

//-----------------------------------------------------------------------------

ValueTypeIndex Marshaller<ifilename>::type() const
{
    return ValueTypeIndex(minitl::indexof< std::string, ValueTypes >::Value);
}

ValueCopy Marshaller<ifilename>::get(void* source) const
{
    return castfrom(*reinterpret_cast<ifilename*>(source));
}

void Marshaller<ifilename>::set(const Value& value, void* dst) const
{
    *reinterpret_cast<ifilename*>(dst) = castto(value);
}

}}
