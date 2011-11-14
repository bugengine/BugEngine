/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/engine/taginfo.script.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/value.inl>

namespace BugEngine { namespace RTTI
{

static const u32 s_overloadMaxDistance = 1000000;
static const u32 s_overloadVarargDistance = s_overloadMaxDistance-1;


Value MethodInfo::OverloadInfo::ParamInfo::getTag(const TypeInfo& type) const
{
    TagInfo* tag = tags;
    while(tag)
    {
        if (type <= tag->tag.type())
            return Value(Value::ByRef(tag->tag));
        tag = tag->next;
    }
    return Value();
}

Value MethodInfo::OverloadInfo::ParamInfo::getTag(const ClassInfo* type) const
{
    return getTag(TypeInfo::makeType(type, TypeInfo::Class, TypeInfo::Mutable));
}

u32 MethodInfo::OverloadInfo::distance(Value* p, u32 nparams) const
{
    if (vararg)
    {
        return s_overloadVarargDistance;
    }
    else
    {
        u32 distance = 0;
        const ParamInfo* selfp = params;
        while(nparams && selfp)
        {
            distance += p->type().distance(selfp->type);
            selfp = selfp->next;
            nparams--;
            p++;
        }
        if (nparams || selfp)
            return s_overloadMaxDistance;
        else
            return distance;
    }
}

Value MethodInfo::OverloadInfo::getTag(const TypeInfo& type) const
{
    TagInfo* tag = tags;
    while(tag)
    {
        if (type <= tag->tag.type())
            return Value(Value::ByRef(tag->tag));
        tag = tag->next;
    }
    return Value();
}

Value MethodInfo::OverloadInfo::getTag(const ClassInfo* type) const
{
    return getTag(TypeInfo::makeType(type, TypeInfo::Class, TypeInfo::Mutable));
}

Value MethodInfo::doCall(Value* params, u32 nparams) const
{
    u32 bestDistance = s_overloadMaxDistance;
    const OverloadInfo* overload = 0;
    for (const OverloadInfo* it = overloads; it; it = it->next)
    {
        u32 distance = it->distance(params, nparams);
        if (distance < bestDistance)
        {
            bestDistance = distance;
            overload = &(*it);
        }
    }
    if (overload)
    {
        return overload->call(params, nparams);
    }
    else
    {
        be_error("No overload can convert all parameters");
        return Value();
    }
}

}}
