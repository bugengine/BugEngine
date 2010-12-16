/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/engine/propertyinfo.script.hh>

namespace BugEngine { namespace RTTI
{

PropertyInfo::PropertyInfo(const istring& name, const TypeInfo& type)
    :   name(name)
    ,   type(type)
{
}

PropertyInfo::~PropertyInfo()
{
}

}}
