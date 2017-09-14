/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
 see LICENSE for detail */

#include    <stdafx.h>
#include    <runtime/call.hh>
#include    <runtime/value.hh>
#include    <runtime/error.hh>
#include    <context.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/engine/propertyinfo.script.hh>
#include    <rtti/engine/call.hh>
#include    <rtti/conversion.script.hh>

namespace BugEngine { namespace Lua
{

struct LuaParameterType
{
    lua_State*  state;
    int         index;

    LuaParameterType(lua_State* state, int index)
        :   state(state)
        ,   index(index)
    {
    }
};

struct LuaTableType
{
    lua_State*  state;
    istring     name;
    int         key;

    LuaTableType(lua_State* state, istring name, int key)
        :   state(state)
        ,   name(name)
        ,   key(key)
    {
    }
};

static RTTI::ConversionCost calculateConversion(const LuaParameterType& type,
                                                const RTTI::Type& target)
{
    if (target.metaclass->type() == RTTI::ClassType_Variant)
    {
        switch (lua_type(type.state, type.index))
        {
        case LUA_TNIL:
        case LUA_TSTRING:
        case LUA_TBOOLEAN:
        case LUA_TNUMBER:
        case LUA_TUSERDATA:
        case LUA_TTABLE:
            return RTTI::ConversionCost::s_variant;
        default:
            return RTTI::ConversionCost::s_incompatible;
        }
    }
    else switch (lua_type(type.state, type.index))
    {
    case LUA_TNIL:
        return target.indirection >= RTTI::Type::RawPtr
                ?   RTTI::ConversionCost()
                :   RTTI::ConversionCost::s_incompatible;
    case LUA_TSTRING:
        return target.metaclass->type() == RTTI::ClassType_String
                ?   RTTI::ConversionCost()
                :   RTTI::ConversionCost::s_incompatible;
    case LUA_TBOOLEAN:
        return RTTI::calculateConversion<bool>(target);
    case LUA_TNUMBER:
        return RTTI::calculateConversion<LUA_NUMBER>(target);
    case LUA_TUSERDATA:
        lua_getmetatable(type.state, type.index);
        luaL_getmetatable(type.state, "BugEngine.Object");
        if (lua_rawequal(type.state, -1, -2))
        {
            lua_pop(type.state, 2);
            RTTI::Value* userdata = (RTTI::Value*)lua_touserdata(type.state, type.index);
            return RTTI::calculateConversion(userdata->type(), target);
        }
        else
        {
            lua_pop(type.state, 2);
            return RTTI::ConversionCost::s_incompatible;
        }
    case LUA_TTABLE:
        return RTTI::ConversionCost::s_incompatible;
    default:
        return RTTI::ConversionCost::s_incompatible;
    }
}

static RTTI::ConversionCost calculateConversion(const LuaTableType& type,
                                                const RTTI::Type& target)
{
    if (type.name.size() > 0)
    {
        lua_pushstring(type.state, type.name.c_str());
        lua_rawget(type.state, 1);
    }
    else
    {
        lua_rawgeti(type.state, 1, type.key);
    }
    if (target.metaclass->type() == RTTI::ClassType_Variant)
    {
        lua_pop(type.state, 1);
        switch (lua_type(type.state, -1))
        {
        case LUA_TNIL:
        case LUA_TSTRING:
        case LUA_TBOOLEAN:
        case LUA_TNUMBER:
        case LUA_TUSERDATA:
        case LUA_TTABLE:
            return RTTI::ConversionCost::s_variant;
        default:
            return RTTI::ConversionCost::s_incompatible;
        }
    }
    else switch (lua_type(type.state, -1))
    {
    case LUA_TNIL:
        lua_pop(type.state, 1);
        return target.indirection >= RTTI::Type::RawPtr
                ?   RTTI::ConversionCost()
                :   RTTI::ConversionCost::s_incompatible;
    case LUA_TSTRING:
        lua_pop(type.state, 1);
        return target.metaclass->type() == RTTI::ClassType_String
                ?   RTTI::ConversionCost()
                :   RTTI::ConversionCost::s_incompatible;
    case LUA_TBOOLEAN:
        lua_pop(type.state, 1);
        return RTTI::calculateConversion<bool>(target);
    case LUA_TNUMBER:
        lua_pop(type.state, 1);
        return RTTI::calculateConversion<LUA_NUMBER>(target);
    case LUA_TUSERDATA:
        lua_getmetatable(type.state, -1);
        luaL_getmetatable(type.state, "BugEngine.Object");
        if (lua_rawequal(type.state, -1, -2))
        {
            lua_pop(type.state, 2);
            RTTI::Value* userdata = (RTTI::Value*)lua_touserdata(type.state, -1);
            lua_pop(type.state, 1);
            return RTTI::calculateConversion(userdata->type(), target);
        }
        else
        {
            lua_pop(type.state, 3);
            return RTTI::ConversionCost::s_incompatible;
        }
    case LUA_TTABLE:
        lua_pop(type.state, 1);
        return RTTI::ConversionCost::s_incompatible;
    default:
        lua_pop(type.state, 1);
        return RTTI::ConversionCost::s_incompatible;
    }
}

static void convert(const LuaParameterType& type, void* buffer, const RTTI::Type& target)
{
    bool result = createValue(type.state, type.index, target, buffer);
    be_assert(result, "could not convert lua value %s to %s"
                     | Context::tostring(type.state, type.index)
                     | target.name());
    be_forceuse(result);
}

static void convert(const LuaTableType& type, void* buffer, const RTTI::Type& target)
{
    if (type.name.size() > 0)
    {
        lua_pushstring(type.state, type.name.c_str());
        lua_rawget(type.state, 1);
    }
    else
    {
        lua_rawgeti(type.state, 1, type.key);
    }
    bool result = createValue(type.state, -1, target, buffer);
    lua_pop(type.state, 1);
    be_assert(result, "could not convert lua value %s to %s"
                     | Context::tostring(type.state, -1)
                     | target.name());
    be_forceuse(result);
}

typedef RTTI::ArgInfo<LuaParameterType> LuaParameterInfo;
typedef RTTI::ArgInfo<LuaTableType> LuaTableInfo;

int call(lua_State *state, raw<const RTTI::Method> method)
{
    u32 nargs = lua_gettop(state) - 1;
    if (nargs == 1 && lua_type(state, 2) == LUA_TTABLE)
    {
        bool error = false;
        u32 parameterCount = luaL_len(state, 2);
        LuaTableInfo* parameters = (LuaTableInfo*)malloca(sizeof(LuaTableInfo) * (parameterCount + 1));

        lua_pushnil(state);
        for (u32 i = 0; !error && lua_next(state, 2); ++i)
        {
            /* removes value */
            lua_pop(state, 1);
            switch(lua_type(state, -1))
            {
            case LUA_TNUMBER:
                {
                    u32 j = (u32)lua_tonumber(state, -1);
                    new (&parameters[i]) LuaTableInfo(LuaTableType(state, istring(), j));
                }
                break;
            case LUA_TSTRING:
                {
                    istring key = istring(lua_tostring(state, -1));
                    new (&parameters[i]) LuaTableInfo(LuaTableType(state, key, 0));
                }
                break;
            default:
                error = true;
                break;
            }
        }
        if (!error)
        {
            RTTI::CallInfo result = RTTI::resolve(method, parameters, parameterCount);
            if (result.conversion < RTTI::ConversionCost::s_incompatible)
            {
                RTTI::Value v = RTTI::call(result, parameters, parameterCount);
                freea(parameters);
                return Context::push(state, v);
            }
            else
            {
                freea(parameters);
            }
        }
        else
        {
            freea(parameters);
        }
    }
    LuaParameterInfo* parameters = (LuaParameterInfo*)malloca(sizeof(LuaParameterInfo) * (nargs + 1));
    for (u32 i = 0; i < nargs; ++i)
    {
        new (&parameters[i]) LuaParameterInfo(LuaParameterType(state, 2+i));
    }
    RTTI::CallInfo result = RTTI::resolve(method, parameters, nargs);
    if (result.conversion < RTTI::ConversionCost::s_incompatible)
    {
        RTTI::Value v = RTTI::call(result, parameters, nargs);
        freea(parameters);
        return Context::push(state, v);
    }
    else
    {
        /*minitl::format<4096u> errorMessage("no overload can convert all parameters\n");
        if (nargs != 0)
        {
            errorMessage.put( "parameters:\n\t");
            for (u32 i = 0; i < nargs; ++i)
            {
                strcat(errorMessage, Context::tostring(state, i+2).c_str());
                if (i != nargs-1)
                {
                    strcat(errorMessage, ", ");
                }
            }
            strcat(errorMessage, "\n");
        }
        strcat(errorMessage, "candidates:\n");
        raw<const RTTI::Method::Overload> overload = method->overloads;
        while (overload)
        {
            strcat(errorMessage, "\t");
            strcat(errorMessage, method->name.c_str());
            strcat(errorMessage, "(");
            raw<const RTTI::Method::Parameter> parameter = overload->params;
            while(parameter)
            {
                strcat(errorMessage, parameter->type.name().c_str());
                if (parameter->next)
                {
                    strcat(errorMessage, ", ");
                }
                parameter = parameter->next;
            }
            overload = overload->next;
            strcat(errorMessage, ")\n");
        }*/
        freea(parameters);
        return error(state, "no overload can convert all parameters");
    }
}


}}
