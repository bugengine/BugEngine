/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/value.inl>

namespace BugEngine { namespace RTTI
{

static const u32 s_overloadMaxDistance = 1000000;
static const u32 s_overloadVarargDistance = s_overloadMaxDistance-1;

u32 OverloadInfo::distance(Value* p, u32 nparams) const
{
    if (vararg)
        return s_overloadVarargDistance;
    else if (nparams != params.size())
        return s_overloadMaxDistance;
    else
    {
        u32 distance = 0;
        for (minitl::vector<const ParamInfo>::const_iterator it = params.begin(); it != params.end(); ++it, ++p)
        {
            distance += p->type().distance(it->type);
        }
        return distance;
    }
}

MethodInfo::MethodInfo()
    :   overloads(rttiArena())
{
}

Value MethodInfo::operator()(Value* params, u32 nparams) const
{
    u32 bestDistance = s_overloadMaxDistance;
    const OverloadInfo* overload = 0;
    for (minitl::vector<OverloadInfo>::const_iterator it = overloads.begin(); it != overloads.end(); ++it)
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
