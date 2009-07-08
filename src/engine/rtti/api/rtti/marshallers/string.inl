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

#ifndef BE_RTTI_STRING_MARSHALLER_INL_
#define BE_RTTI_STRING_MARSHALLER_INL_
/*****************************************************************************/
#include    <rtti/value.hh>


namespace BugEngine { namespace RTTI
{

ValueCopy Marshaller<std::string>::castfrom(const std::string& value) const
{
    return ValueCopy(value);
}

std::string Marshaller<std::string>::castto(const Value& value) const
{
    return value.member<std::string>();
}

//-----------------------------------------------------------------------------

ValueCopy Marshaller<istring>::castfrom(const istring& value) const
{
    return ValueCopy(value.str());
}

istring Marshaller<istring>::castto(const Value& value) const
{
    return istring(value.member<std::string>());
}

//-----------------------------------------------------------------------------

ValueCopy Marshaller<inamespace>::castfrom(const inamespace& value) const
{
    return ValueCopy(value.str());
}

inamespace Marshaller<inamespace>::castto(const Value& value) const
{
    return inamespace(value.member<std::string>());
}

//-----------------------------------------------------------------------------

ValueCopy Marshaller<ifilename>::castfrom(const ifilename& value) const
{
    return ValueCopy(value.str());
}

ifilename Marshaller<ifilename>::castto(const Value& value) const
{
    return ifilename(value.member<std::string>());
}

//-----------------------------------------------------------------------------

ValueCopy Marshaller<ipath>::castfrom(const ipath& value) const
{
    return ValueCopy(value.str());
}

ipath Marshaller<ipath>::castto(const Value& value) const
{
    return ipath(value.member<std::string>());
}

}}


/*****************************************************************************/
#endif
