/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
 see LICENSE for detail */

#ifndef BE_LUA_RUNTIME_ERROR_HH_
#define BE_LUA_RUNTIME_ERROR_HH_
/**************************************************************************************************/
#include    <stdafx.h>
#include    <rtti/engine/methodinfo.script.hh>

namespace BugEngine { namespace Lua
{

int error(lua_State* state, const minitl::format<4096u>& message);

}}

/**************************************************************************************************/
#endif
