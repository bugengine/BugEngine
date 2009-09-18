/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

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
