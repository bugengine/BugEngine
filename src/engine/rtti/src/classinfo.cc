/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/classinfo.hh>
#include    <rtti/propertyinfo.hh>

namespace BugEngine
{

ClassInfo::ClassInfo(const inamespace& name)
:   TypeInfo(name)
{
}

ClassInfo::~ClassInfo()
{
}

}
