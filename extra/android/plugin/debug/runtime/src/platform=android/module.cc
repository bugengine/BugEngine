/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <runtime/stdafx.h>
#include    <runtime/module.hh>

namespace BugEngine { namespace Runtime
{

ref<const Module> Module::self()
{
    static ref<Module> s_module;
    return s_module;
}

}}
