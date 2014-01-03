/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
 see LICENSE for detail */

#ifndef BE_LUA_CALL_HH_
#define BE_LUA_CALL_HH_
/*****************************************************************************/
#include    <rtti/engine/methodinfo.script.hh>

namespace BugEngine { namespace Lua
{

int call(lua_State* state, raw<const RTTI::Method> method);

}}

/*****************************************************************************/
#endif
