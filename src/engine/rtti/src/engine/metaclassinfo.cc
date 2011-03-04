/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/engine/metaclassinfo.script.hh>
#include    <rtti/value.inl>

namespace BugEngine { namespace RTTI
{

Namespace::MetaClassInfo::MetaClassInfo(const inamespace& name, ref<const ClassInfo> parent)
    :   ClassInfo(name, parent)
{
}

Namespace::MetaClassInfo::MetaClassInfo(const inamespace& name, ref<const ClassInfo> parent, u32 size, i32 offset)
    :   ClassInfo(name, parent, size, offset)
{
}

Namespace::MetaClassInfo::~MetaClassInfo()
{
}

Value Namespace::MetaClassInfo::get(Value& from, istring name) const
{
    Value r = ClassInfo::get(from, name);
    if(!r)
    {
        r = from.as<const Namespace&>().get(name);
    }
    return r;
}


ClassInfo::MetaClassInfo::MetaClassInfo(const inamespace& name, ref<const ClassInfo> parent)
    :   Namespace::MetaClassInfo(name, parent)
{
}

ClassInfo::MetaClassInfo::MetaClassInfo(const inamespace& name, ref<const ClassInfo> parent, u32 size, i32 offset)
    :   Namespace::MetaClassInfo(name, parent, size, offset)
{
}

ClassInfo::MetaClassInfo::~MetaClassInfo()
{
}

Value ClassInfo::MetaClassInfo::get(Value& from, istring name) const
{
    Value r = Namespace::MetaClassInfo::get(from, name);
    if(!r)
    {
        
    }
    return r;
}

}}
