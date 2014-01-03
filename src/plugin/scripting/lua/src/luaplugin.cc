/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
 see LICENSE for detail */

#include    <stdafx.h>

#include    <luaplugin.hh>
#include    <luacontext.hh>
#include    <plugin/plugin.hh>


namespace BugEngine { namespace Lua
{

extern "C" int pluginGC(lua_State *state)
{
    Context::checkArg(state, 1, "BugEngine.Plugin");
    Plugin::Plugin<void>* userdata = (Plugin::Plugin<void>*)lua_touserdata(state, 1);
    userdata->~Plugin();
    return 0;
}

extern "C" int pluginToString(lua_State *state)
{
    Context::checkArg(state, 1, "BugEngine.Plugin");
    Plugin::Plugin<void>* userdata = (Plugin::Plugin<void>*)lua_touserdata(state, 1);
    lua_pushfstring(state, "plugin[%s]", userdata->name().str().name);
    return 1;
}

extern "C" int pluginGet(lua_State *state)
{
    Context::checkArg(state, 1, "BugEngine.Plugin");
    Context::checkArg(state, 2, LUA_TSTRING);

    Plugin::Plugin<void>* userdata = (Plugin::Plugin<void>*)lua_touserdata(state, 1);
    const char *name = lua_tostring(state, 2);
    RTTI::Value v(userdata->pluginNamespace());
    Context::push(state, v[name]);
    return 1;
}

const luaL_Reg s_pluginMetaTable[] = {
    {"__gc", pluginGC},
    {"__tostring", pluginToString},
    {"__index", pluginGet},
    {0, 0}
};

}}
