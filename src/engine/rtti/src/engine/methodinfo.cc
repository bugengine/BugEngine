/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/engine/methodinfo.script.hh>

namespace BugEngine { namespace RTTI
{

MethodInfo::MethodInfo()
    :   overloads(rttiArena())
    ,   vararg(be_typeid<void>::type(), NULL)
{
}

Value MethodInfo::operator()(Value* params, u32 nparams) const
{
    for (minitl::vector<OverloadInfo>::const_iterator it = overloads.begin(); it != overloads.end(); ++it)
    {
        if (it->params.size() == nparams)
        {
            return it->call(params, nparams);
        }
    }
    if(vararg.call)
        return vararg.call(params, nparams);
    be_notreached();
    return Value();
}

}}
