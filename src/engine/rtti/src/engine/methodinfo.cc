/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/engine/taginfo.script.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/value.hh>

namespace BugEngine { namespace RTTI
{

static const u32 s_overloadMaxDistance = 1000000;
static const u32 s_overloadVarargDistance = s_overloadMaxDistance-1;


Value Method::Overload::Parameter::getTag(const Type& type) const
{
    raw<Tag> tag = tags;
    while(tag)
    {
        if (type <= tag->tag.type())
            return Value(Value::ByRef(tag->tag));
        tag = tag->next;
    }
    return Value();
}

Value Method::Overload::Parameter::getTag(raw<const Class> type) const
{
    return getTag(Type::makeType(type, Type::Value, Type::Const, Type::Const));
}

u32 Method::Overload::distance(Value* p, u32 nparams) const
{
    if (vararg)
    {
        return s_overloadVarargDistance;
    }
    else
    {
        u32 distance = 0;
        raw<const Parameter> selfp = params;
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

Value Method::Overload::getTag(const Type& type) const
{
    raw<Tag> tag = tags;
    while(tag)
    {
        if (type <= tag->tag.type())
            return Value(Value::ByRef(tag->tag));
        tag = tag->next;
    }
    return Value();
}

Value Method::Overload::getTag(raw<const Class> type) const
{
    return getTag(Type::makeType(type, Type::Value, Type::Const, Type::Const));
}

Value Method::doCall(Value* params, u32 nparams) const
{
    u32 bestDistance = s_overloadMaxDistance;
    raw<const Overload> overload = {0};
    for (raw<const Overload> it = overloads; it; it = it->next)
    {
        u32 distance = it->distance(params, nparams);
        if (distance < bestDistance)
        {
            bestDistance = distance;
            overload = it;
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
