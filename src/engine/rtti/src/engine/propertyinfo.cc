/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/engine/propertyinfo.script.hh>
#include    <rtti/value.hh>

namespace BugEngine { namespace RTTI
{

PropertyInfo::PropertyInfo(const TypeInfo& type, PropertyFlags flags)
    :   type(type)
    ,   flags(flags)
{
}

PropertyInfo::~PropertyInfo()
{
}

Value PropertyInfo::get(Value& object) const
{
    be_notreached();
    return Value();
}

void PropertyInfo::set(Value& object, Value& value) const
{
    be_notreached();
}


}}
