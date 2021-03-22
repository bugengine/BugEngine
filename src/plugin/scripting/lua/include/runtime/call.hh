/* BugEngine <bugengine.devel@gmail.com>
 see LICENSE for detail */

#ifndef BE_LUA_RUNTIME_CALL_HH_
#define BE_LUA_RUNTIME_CALL_HH_
/**************************************************************************************************/
#include <stdafx.h>
#include <bugengine/meta/engine/methodinfo.script.hh>

namespace BugEngine { namespace Lua {

int call(lua_State* state, raw< const Meta::Method > method);

}}  // namespace BugEngine::Lua

/**************************************************************************************************/
#endif
