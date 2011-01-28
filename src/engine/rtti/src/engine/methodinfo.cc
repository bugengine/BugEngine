/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/engine/methodinfo.script.hh>

namespace BugEngine { namespace RTTI
{

MethodInfo::MethodInfo()
    :   overloads(rttiArena())
{
}

Value MethodInfo::call(Value* params, size_t nparams) const
{
    for (minitl::vector<OverloadInfo>::const_iterator it = overloads.begin(); it != overloads.end(); ++it)
    {
        if (it->params.size() == nparams)
        {
            return it->call(params, nparams);
        }
    }
    be_notreached();
    return Value();
}

void MethodInfo::test(ref<ClassInfo const> p1, ref<ClassInfo const> p2)
{
    printf("Hello, world!\n");
}

}}
