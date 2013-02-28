/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
 see LICENSE for detail */

#include    <stdafx.h>

#include    <luavalue.hh>
#include    <call.hh>
#include    <luacontext.hh>
#include    <rtti/classinfo.script.hh>


namespace BugEngine { namespace Lua
{

static int pushUserdataString(lua_State* L, RTTI::Value* userdata)
{
    const char* constness = (userdata->type().constness == RTTI::Type::Const) ? "const " : "mutable ";
    const char* reference;
    const char* closing;
    switch(userdata->type().indirection)
    {
        case RTTI::Type::RefPtr:
            reference = "ref<";
            closing = ">";
            break;
        case RTTI::Type::WeakPtr:
            reference = "weak<";
            closing = ">";
            break;
        case RTTI::Type::RawPtr:
            reference = "raw<";
            closing = ">";
            break;
        case RTTI::Type::Value:
            reference = "";
            constness = "";
            closing = "";
            break;
        default:
            reference = "??? <";
            constness = "??? ";
            closing = ">";
            break;
    }
    const char* access = (userdata->type().access == RTTI::Type::Const) ? "const " : "";
    lua_pushfstring(L, "[%s%s%s%s%s object @0x%p]", constness, reference, access, userdata->type().metaclass->name.c_str(), closing, userdata);
    return 1;
}

extern "C" int valueGC(lua_State *state)
{
    Context::checkArg(state, 1, "BugEngine.Object");

    RTTI::Value* userdata = (RTTI::Value*)lua_touserdata(state, -1);
    userdata->~Value();
    return 0;
}

extern "C" int valueToString(lua_State *state)
{
    Context::checkArg(state, 1, "BugEngine.Object");

    RTTI::Value* userdata = (RTTI::Value*)lua_touserdata(state, -1);
    if (userdata->type().indirection == RTTI::Type::Value)
    {
        raw<const RTTI::Class> metaclass = userdata->type().metaclass;
        if (metaclass == be_typeid< inamespace >::klass())
        {
            lua_pushfstring(state, "%s", userdata->as<const inamespace > ().str().name);
            return 1;
        }
        if (metaclass == be_typeid< istring >::klass())
        {
            lua_pushfstring(state, "%s", userdata->as<const istring > ().c_str());
            return 1;
        }
        if (metaclass == be_typeid< ifilename >::klass())
        {
            lua_pushfstring(state, "%s", userdata->as<const ifilename > ().str().name);
            return 1;
        }
    }
    return pushUserdataString(state, userdata);
}

extern "C" int valueGet(lua_State *state)
{
    Context::checkArg(state, 1, "BugEngine.Object");
    Context::checkArg(state, 2, LUA_TSTRING);

    RTTI::Value* userdata = (RTTI::Value*)lua_touserdata(state, -2);
    const char *name = lua_tostring(state, -1);
    RTTI::Value v = (*userdata)[name];
    if (!v)
    {
        lua_pushnil(state);
        return 1;
    }
    Context::push(state, v);
    return 1;
}

extern "C" int valueCall(lua_State* state)
{
    Context::checkArg(state, 1, "BugEngine.Object");

    RTTI::Value* userdata = (RTTI::Value*)lua_touserdata(state, 1);
    const RTTI::Method& method = (*userdata)["?call"].as<const RTTI::Method&>();
    raw<const RTTI::Method::Overload> overload = method.overloads;
    u32 overloadCount = 0;
    while(overload)
    {
        overloadCount ++;
        overload = overload->next;
    }
    if (overloadCount == 1)
    {
        return call(state, userdata, method.overloads);
    }
    else
    {
        return 0;
    }
}

const luaL_Reg s_valueMetaTable[] = {
    {"__gc", valueGC},
    {"__tostring", valueToString},
    {"__index", valueGet},
    {"__call", valueCall},
    {0, 0}
};

}}
