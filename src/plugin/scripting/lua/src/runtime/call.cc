/* BugEngine <bugengine.devel@gmail.com>
 see LICENSE for detail */

#include <stdafx.h>
#include <bugengine/meta/conversion.script.hh>
#include <bugengine/meta/engine/call.hh>
#include <bugengine/meta/engine/methodinfo.script.hh>
#include <bugengine/meta/engine/propertyinfo.script.hh>
#include <bugengine/meta/engine/scriptingapi.hh>
#include <context.hh>
#include <runtime/call.hh>
#include <runtime/error.hh>
#include <runtime/value.hh>

namespace BugEngine { namespace Lua {

struct LuaParameterType
{
    lua_State* state;
    int        index;
    istring    name;
    int        key;

    LuaParameterType(lua_State* state, int index) : state(state), index(index), name(""), key(-1)
    {
    }
    LuaParameterType(lua_State* state, int index, istring name, int key)
        : state(state)
        , index(index)
        , name(name)
        , key(key)
    {
    }
};

struct LuaPop
{
    lua_State* state;
    int        index;
    bool       autoPop;

    LuaPop(lua_State* state, int index, bool autoPop) : state(state), index(index), autoPop(autoPop)
    {
    }
    ~LuaPop()
    {
        if(autoPop)
        {
            lua_pop(state, 1);
        }
    }
};

Meta::ConversionCost calculateConversion(const LuaParameterType& type, const Meta::Type& target)
{
    int index;
    if(type.key != -1)
    {
        if(type.name.size() > 0)
        {
            lua_pushstring(type.state, type.name.c_str());
            lua_rawget(type.state, type.index);
        }
        else
        {
            lua_rawgeti(type.state, type.index, type.key);
        }
        index = -1;
    }
    else
    {
        index = type.index;
    }

    LuaPop p(type.state, index, type.key != -1);
    if(target.metaclass->type() == Meta::ClassType_Variant)
    {
        switch(lua_type(type.state, index))
        {
        case LUA_TNIL:
        case LUA_TSTRING:
        case LUA_TBOOLEAN:
        case LUA_TNUMBER:
        case LUA_TUSERDATA:
        case LUA_TTABLE: return Meta::ConversionCost::s_variant;
        default: return Meta::ConversionCost::s_incompatible;
        }
    }
    else
        switch(lua_type(type.state, index))
        {
        case LUA_TNIL:
            return target.indirection >= Meta::Type::RawPtr ? Meta::ConversionCost()
                                                            : Meta::ConversionCost::s_incompatible;
        case LUA_TSTRING:
            return target.metaclass->type() == Meta::ClassType_String
                       ? Meta::ConversionCost()
                       : Meta::ConversionCost::s_incompatible;
        case LUA_TBOOLEAN: return Meta::ConversionCalculator< bool >::calculate(target);
        case LUA_TNUMBER: return Meta::ConversionCalculator< LUA_NUMBER >::calculate(target);
        case LUA_TUSERDATA:
            lua_getmetatable(type.state, index);
            luaL_getmetatable(type.state, "BugEngine.Object");
            if(lua_rawequal(type.state, -1, -2))
            {
                lua_pop(type.state, 2);
                Meta::Value* userdata = (Meta::Value*)lua_touserdata(type.state, index);
                return Meta::calculateConversion(userdata->type(), target);
            }
            else
            {
                lua_pop(type.state, 2);
                return Meta::ConversionCost::s_incompatible;
            }
        case LUA_TTABLE:
            if(target.metaclass->type() == Meta::ClassType_Array)
            {
                const Meta::Type& valueType
                    = target.metaclass->apiMethods->arrayScripting->value_type;
                Meta::ConversionCost c;
                lua_pushnil(type.state);
                while(lua_next(type.state, index < 0 ? index - 1 : index))
                {
                    int keyType = lua_type(type.state, -2);
                    if(keyType != LUA_TNUMBER)
                    {
                        lua_pop(type.state, 2);
                        return Meta::ConversionCost::s_incompatible;
                    }
                    else
                    {
                        c += calculateConversion(LuaParameterType(type.state, -1), valueType);
                        lua_pop(type.state, 1);
                        if(c >= Meta::ConversionCost::s_incompatible) return c;
                    }
                }
                return c;
            }
            else if(target.metaclass->type() == Meta::ClassType_Pod)
            {
                if(target.metaclass->constructor)
                {
                    Meta::ConversionCost c;
                    lua_pushnil(type.state);
                    while(lua_next(type.state, index < 0 ? index - 1 : index))
                    {
                        if(lua_type(type.state, -2) != LUA_TSTRING)
                        {
                            lua_pop(type.state, 2);
                            return Meta::ConversionCost::s_incompatible;
                        }
                        const char*                 str      = lua_tostring(type.state, -2);
                        raw< const Meta::Property > property = target.metaclass->getProperty(str);
                        if(!property)
                        {
                            lua_pop(type.state, 2);
                            return Meta::ConversionCost::s_incompatible;
                        }
                        else
                        {
                            c += calculateConversion(LuaParameterType(type.state, -1),
                                                     property->type);
                            lua_pop(type.state, 1);
                            if(c >= Meta::ConversionCost::s_incompatible) return c;
                        }
                    }
                    return c;
                }
                else
                {
                    return Meta::ConversionCost::s_incompatible;
                }
            }
            else
            {
                return Meta::ConversionCost::s_incompatible;
            }
        default: return Meta::ConversionCost::s_incompatible;
        }
}

void convert(const LuaParameterType& type, void* buffer, const Meta::Type& target)
{
    int index;
    if(type.key != -1)
    {
        if(type.name.size() > 0)
        {
            lua_pushstring(type.state, type.name.c_str());
            lua_rawget(type.state, type.index);
        }
        else
        {
            lua_rawgeti(type.state, type.index, type.key);
        }
        index = -1;
    }
    else
    {
        index = type.index;
    }
    LuaPop p(type.state, index, type.key != -1);
    bool   result = createValue(type.state, index, target, buffer);
    be_assert(result, "could not convert lua value %s to %s" | Context::tostring(type.state, index)
                          | target.name());
    be_forceuse(result);
}

typedef Meta::ArgInfo< LuaParameterType > LuaParameterInfo;

int call(lua_State* state, raw< const Meta::Method > method)
{
    u32 nargs = lua_gettop(state) - 1;
    if((nargs == 1 && lua_type(state, 2) == LUA_TTABLE)
       || (nargs == 2 && lua_type(state, 3) == LUA_TTABLE))
    {
        bool error          = false;
        u32  parameterCount = 0;
        lua_pushnil(state);
        for(; lua_next(state, nargs + 1); ++parameterCount)
        {
            lua_pop(state, 1);
        }
        u32               positionParameterCount = 0;
        u32               keywordParameterCount  = 0;
        LuaParameterInfo* parameters
            = (LuaParameterInfo*)malloca(sizeof(LuaParameterInfo) * (parameterCount + 1));

        if(nargs == 2)
        {
            new(&parameters[positionParameterCount++])
                LuaParameterInfo(LuaParameterType(state, nargs));
            parameterCount++;
        }
        lua_pushnil(state);
        for(u32 i = 0; !error && lua_next(state, nargs + 1); ++i)
        {
            /* removes value */
            lua_pop(state, 1);
            switch(lua_type(state, -1))
            {
            case LUA_TNUMBER:
            {
                u32 j = (u32)lua_tonumber(state, -1);
                new(&parameters[positionParameterCount])
                    LuaParameterInfo(LuaParameterType(state, nargs + 1, istring(), j));
                positionParameterCount++;
            }
            break;
            case LUA_TSTRING:
            {
                const char* keyStr = lua_tostring(state, -1);
                istring     key    = istring(keyStr);
                keywordParameterCount++;
                new(&parameters[parameterCount - keywordParameterCount])
                    LuaParameterInfo(key, LuaParameterType(state, nargs + 1, key, 0));
            }
            break;
            default:
                lua_pop(state, 1);
                error = true;
                break;
            }
        }
        if(!error)
        {
            Meta::CallInfo result
                = Meta::resolve(method, parameters, positionParameterCount,
                                parameters + positionParameterCount, keywordParameterCount);
            if(result.conversion < Meta::ConversionCost::s_incompatible)
            {
                Meta::Value v
                    = Meta::call(result, parameters, positionParameterCount,
                                 parameters + positionParameterCount, keywordParameterCount);
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
    LuaParameterInfo* parameters
        = (LuaParameterInfo*)malloca(sizeof(LuaParameterInfo) * (nargs + 1));
    for(u32 i = 0; i < nargs; ++i)
    {
        new(&parameters[i]) LuaParameterInfo(LuaParameterType(state, 2 + i));
    }
    Meta::CallInfo result = Meta::resolve(method, parameters, nargs);
    if(result.conversion < Meta::ConversionCost::s_incompatible)
    {
        Meta::Value v = Meta::call(result, parameters, nargs);
        freea(parameters);
        return Context::push(state, v);
    }
    else
    {
        freea(parameters);
        char message[4096] = "no overload can convert all parameters\n  ";
        for(u32 i = 0; i < nargs; ++i)
        {
            strcat(message, minitl::format< 128u >("%s%s") | Context::tostring(state, 2 + i)
                                | (i < nargs - 1 ? ", " : ""));
        }
        return error(state, message);
    }
}

}}  // namespace BugEngine::Lua
