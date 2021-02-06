/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin/stdafx.h>
#include <bugengine/plugin/hook.hh>

namespace BugEngine { namespace Plugin {

IPluginHook::~IPluginHook()
{
    this->unhook();
}

}}  // namespace BugEngine::Plugin
