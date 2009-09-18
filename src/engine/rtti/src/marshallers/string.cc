/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

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
