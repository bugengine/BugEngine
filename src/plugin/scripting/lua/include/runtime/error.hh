/* BugEngine <bugengine.devel@gmail.com>
 see LICENSE for detail */

#ifndef BE_LUA_RUNTIME_ERROR_HH_
#define BE_LUA_RUNTIME_ERROR_HH_
/**************************************************************************************************/
#include <stdafx.h>
#include <bugengine/rtti/engine/methodinfo.script.hh>

namespace BugEngine { namespace Lua {

int error(lua_State* state, const minitl::format< 4096u >& message);

}}  // namespace BugEngine::Lua

/**************************************************************************************************/
#endif
