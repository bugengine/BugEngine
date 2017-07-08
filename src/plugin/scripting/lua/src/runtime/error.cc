/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
 see LICENSE for detail */

#include    <stdafx.h>
#include    <runtime/error.hh>
#include    <context.hh>


namespace BugEngine { namespace Lua
{

int error(lua_State *state, const minitl::format<4096u>& message)
{
    lua_Debug ar;
    if (!lua_getstack(state, 0, &ar))
    {
        ar.name = "?";
    }
    else
    {
        lua_getinfo(state, "n", &ar);
        if (ar.name == NULL)
        {
            ar.name = "?";
        }
    }

    return luaL_error(state, LUA_QS ": " LUA_QS,
                             ar.name,
                             message.c_str());
}

}}
