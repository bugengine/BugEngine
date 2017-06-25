/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
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


Value Method::Parameter::getTag(const Type& tagType) const
{
    for (const Tag* tag = tags->begin(); tag != tags->end(); ++tag)
    {
        if (tagType <= tag->tag.type())
            return Value(Value::ByRef(tag->tag));
    }
    return Value();
}

Value Method::Parameter::getTag(raw<const Class> tagType) const
{
    return getTag(Type::makeType(tagType, Type::Value, Type::Const, Type::Const));
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
        const Parameter* selfp = params->begin();
        while(nparams && selfp != params->end())
        {
            distance += p->type().distance(selfp->type);
            selfp++;
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
    for (const Tag* tag = tags->begin(); tag != tags->end(); ++tag)
    {
        if (type <= tag->tag.type())
            return Value(Value::ByRef(tag->tag));
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
    for (const Overload* it = overloads->begin(); it != overloads->end(); ++it)
    {
        u32 distance = it->distance(params, nparams);
        if (distance < bestDistance)
        {
            bestDistance = distance;
            overload.set(it);
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
