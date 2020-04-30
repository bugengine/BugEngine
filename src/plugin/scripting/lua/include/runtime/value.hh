/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
 see LICENSE for detail */

#ifndef BE_LUA_RUNTIME_VALUE_HH_
#define BE_LUA_RUNTIME_VALUE_HH_
/**************************************************************************************************/
#include <stdafx.h>

namespace BugEngine { namespace Lua {

bool createValue(lua_State* state, int index, const RTTI::Type& type, void* buffer);

}}  // namespace BugEngine::Lua

/**************************************************************************************************/
#endif
