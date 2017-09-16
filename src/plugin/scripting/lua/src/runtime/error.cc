/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
 see LICENSE for detail */

#include    <stdafx.h>
#include    <runtime/error.hh>
#include    <context.hh>


namespace BugEngine { namespace Lua
{

int error(lua_State *state, const minitl::format<4096u>& message)
{
    return luaL_error(state, "%s: %s", Context::getCallInfo(state).c_str(), message.c_str());
}

}}
