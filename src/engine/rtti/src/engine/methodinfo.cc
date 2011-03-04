/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/value.inl>

namespace BugEngine { namespace RTTI
{

MethodInfo::MethodInfo()
    :   overloads(rttiArena())
{
}

Value MethodInfo::operator()(Value* params, u32 nparams) const
{
    // TODO: vararg?
    for (minitl::vector<OverloadInfo>::const_iterator it = overloads.begin(); it != overloads.end(); ++it)
    {
        if (it->params.size() == nparams || it->vararg)
        {
            return it->call(params, nparams);
        }
    }
    be_notreached();
    return Value();
}

}}
