/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/engine/propertyinfo.script.hh>
#include    <rtti/value.hh>

namespace BugEngine { namespace RTTI
{

PropertyInfo::PropertyInfo(const TypeInfo& type, Value (*get)(weak<const PropertyInfo> _this, void* from, bool isConst), void  (*set)(weak<const PropertyInfo> _this, void* from, Value& value, bool isConst))
    :   type(type)
    ,   get(get)
    ,   set(set)
{
}

PropertyInfo::~PropertyInfo()
{
}

}}
