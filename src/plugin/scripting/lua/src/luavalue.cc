/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
 see LICENSE for detail */

#include    <stdafx.h>

#include    <luavalue.hh>
#include    <luacall.hh>
#include    <luacontext.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/engine/propertyinfo.script.hh>


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
    }
    else if (v.type().indirection >= RTTI::Type::RawPtr && v.as<const void* const>() == 0)
    {
        lua_pushnil(state);
    }
    else
    {
        Context::push(state, v);
    }
    return 1;
}

extern "C" int valueSet(lua_State *state)
{
    Context::checkArg(state, 1, "BugEngine.Object");
    Context::checkArg(state, 2, LUA_TSTRING);

    RTTI::Value* userdata = (RTTI::Value*)lua_touserdata(state, -3);
    const istring name = istring(lua_tostring(state, -2));
    raw<const RTTI::Property> p = userdata->type().metaclass->properties;
    while (p && p->name != name) p = p->next;
    if (!p)
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
        return luaL_error(state, LUA_QS ": object " LUA_QS " has no property " LUA_QS,
                                 ar.name,  userdata->type().name().c_str(), name.c_str());
    }
    else if (userdata->type().constness == RTTI::Type::Const)
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
        return luaL_error(state, LUA_QS ": object " LUA_QS " is const",
                          ar.name,  userdata->type().name().c_str());
    }
    else if (p->type.constness == RTTI::Type::Const)
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
        return luaL_error(state, LUA_QS ": object " LUA_QS " property " LUA_QS " is const",
                                 ar.name,  userdata->type().name().c_str(), name.c_str());
    }
    else
    {
        RTTI::Value* v = (RTTI::Value*)malloca(sizeof(RTTI::Value));
        if (get(state, -1, p->type, v) != -1)
        {
            p->set(*userdata, *v);
            v->~Value();
        }
        else
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
            return luaL_error(state, LUA_QS ": object " LUA_QS " property " LUA_QS
                                            " has incompatible type " LUA_QS,
                                     ar.name,  userdata->type().name().c_str(), name.c_str(),
                              p->type.name().c_str());
        }
        freea(v);
    }

    return 0;
}

extern "C" int valueCall(lua_State* state)
{
    Context::checkArg(state, 1, "BugEngine.Object");
    RTTI::Value* userdata = (RTTI::Value*)lua_touserdata(state, 1);
    RTTI::Value value = (*userdata)["?call"];
    if (!value)
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
        return luaL_error(state, LUA_QS ": object is not callable", ar.name);
    }
    raw<const RTTI::Method> method = value.as< raw<const RTTI::Method> >();
    if (method)
    {
        return call(state, method);
    }
    else
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
        return luaL_error(state, LUA_QS ": method is not implemented", ar.name);
    }
}

const luaL_Reg s_valueMetaTable[] = {
    {"__gc", valueGC},
    {"__tostring", valueToString},
    {"__index", valueGet},
    {"__newindex", valueSet},
    {"__call", valueCall},
    {0, 0}
};

}}
