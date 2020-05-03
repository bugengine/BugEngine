/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/plugin.debug.runtime/stdafx.h>
#include <bugengine/plugin.debug.runtime/module.hh>

namespace BugEngine { namespace Runtime {

ref< const Module > Module::self()
{
    static ref< Module > s_module;
    return s_module;
}

}}  // namespace BugEngine::Runtime
