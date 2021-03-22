/* BugEngine <bugengine.devel@gmail.com>
 see LICENSE for detail */

#include <stdafx.h>
#include <bugengine/meta/engine/methodinfo.script.hh>
#include <bugengine/meta/engine/objectinfo.script.hh>
#include <bugengine/meta/engine/propertyinfo.script.hh>
#include <bugengine/meta/engine/scriptingapi.hh>
#include <context.hh>
#include <runtime/value.hh>

namespace BugEngine { namespace Lua {

static bool convertNilToValue(lua_State* state, int index, const Meta::Type& type, void* buffer)
{
    be_forceuse(state);
    be_forceuse(index);
    if(type.indirection >= Meta::Type::RawPtr)
    {
        Meta::Value* value = new(buffer) Meta::Value(type, Meta::Value::Reserve);
        *static_cast< void** >(value->memory()) = 0;
        return true;
    }
    else
    {
        return false;
    }
}

static bool convertStringToValue(lua_State* state, int index, const Meta::Type& type, void* buffer)
{
    if(type.metaclass->type() == Meta::ClassType_String)
    {
        switch(type.metaclass->index())
        {
        case Meta::ClassIndex_istring:
            new(buffer) Meta::Value(istring(lua_tostring(state, index)));
            return true;
        case Meta::ClassIndex_inamespace:
            new(buffer) Meta::Value(inamespace(lua_tostring(state, index)));
            return true;
        case Meta::ClassIndex_ifilename:
            new(buffer) Meta::Value(ifilename(lua_tostring(state, index)));
            return true;
        case Meta::ClassIndex_ipath:
            new(buffer) Meta::Value(ipath(lua_tostring(state, index)));
            return true;
        case Meta::ClassIndex_text:
            new(buffer) Meta::Value(text(lua_tostring(state, index)));
            return true;
        default: be_notreached(); return false;
        }
    }
    else
    {
        return false;
    }
}

static bool convertBooleanToValue(lua_State* state, int index, const Meta::Type& type, void* buffer)
{
    if(type.metaclass == be_type< bool >().metaclass)
    {
        new(buffer) Meta::Value(lua_toboolean(state, index) ? true : false);
        return true;
    }
    else if(type.metaclass->type() == Meta::ClassType_Number)
    {
        be_warning("%s: - dubious cast: Lua bool to %s" | Context::getCallInfo(state)
                   | type.metaclass->name);
        switch(type.metaclass->index())
        {
        case Meta::ClassIndex_u8:
            new(buffer) Meta::Value(static_cast< u8 >(lua_toboolean(state, index)));
            return true;
        case Meta::ClassIndex_u16:
            new(buffer) Meta::Value(static_cast< u16 >(lua_toboolean(state, index)));
            return true;
        case Meta::ClassIndex_u32:
            new(buffer) Meta::Value(static_cast< u32 >(lua_toboolean(state, index)));
            return true;
        case Meta::ClassIndex_u64:
            new(buffer) Meta::Value(static_cast< u64 >(lua_toboolean(state, index)));
            return true;
        case Meta::ClassIndex_i8:
            new(buffer) Meta::Value(static_cast< i8 >(lua_toboolean(state, index)));
            return true;
        case Meta::ClassIndex_i16:
            new(buffer) Meta::Value(static_cast< i16 >(lua_toboolean(state, index)));
            return true;
        case Meta::ClassIndex_i32:
            new(buffer) Meta::Value(static_cast< i32 >(lua_toboolean(state, index)));
            return true;
        case Meta::ClassIndex_i64:
            new(buffer) Meta::Value(static_cast< i64 >(lua_toboolean(state, index)));
            return true;
        case Meta::ClassIndex_float:
            new(buffer) Meta::Value(static_cast< float >(lua_toboolean(state, index)));
            return true;
        case Meta::ClassIndex_double:
            new(buffer) Meta::Value(static_cast< double >(lua_toboolean(state, index)));
            return true;
        default: be_notreached(); return false;
        }
    }
    else
    {
        return false;
    }
}

static bool convertNumberToValue(lua_State* state, int index, const Meta::Type& type, void* buffer)
{
    if(type.metaclass->type() == Meta::ClassType_Number)
    {
        switch(type.metaclass->index())
        {
        case Meta::ClassIndex_bool:
        {
            lua_Number       v         = lua_tonumber(state, index);
            const lua_Number g_epsilon = 0.000001f;
            new(buffer) Meta::Value(v > g_epsilon || v < -g_epsilon);
            return true;
        }
        case Meta::ClassIndex_u8:
            new(buffer) Meta::Value(static_cast< u8 >(lua_tonumber(state, index)));
            return true;
        case Meta::ClassIndex_u16:
            new(buffer) Meta::Value(static_cast< u16 >(lua_tonumber(state, index)));
            return true;
        case Meta::ClassIndex_u32:
            new(buffer) Meta::Value(static_cast< u32 >(lua_tonumber(state, index)));
            return true;
        case Meta::ClassIndex_u64:
            new(buffer) Meta::Value(static_cast< u64 >(lua_tonumber(state, index)));
            return true;
        case Meta::ClassIndex_i8:
            new(buffer) Meta::Value(static_cast< i8 >(lua_tonumber(state, index)));
            return true;
        case Meta::ClassIndex_i16:
            new(buffer) Meta::Value(static_cast< i16 >(lua_tonumber(state, index)));
            return true;
        case Meta::ClassIndex_i32:
            new(buffer) Meta::Value(static_cast< i32 >(lua_tonumber(state, index)));
            return true;
        case Meta::ClassIndex_i64:
            new(buffer) Meta::Value(static_cast< i64 >(lua_tonumber(state, index)));
            return true;
        case Meta::ClassIndex_float:
            new(buffer) Meta::Value(static_cast< float >(lua_tonumber(state, index)));
            return true;
        case Meta::ClassIndex_double:
            new(buffer) Meta::Value(static_cast< double >(lua_tonumber(state, index)));
            return true;
        default: be_notreached(); return false;
        }
    }
    else
    {
        return false;
    }
}

static bool convertUserdataToValue(lua_State* state, int index, const Meta::Type& type,
                                   void* buffer)
{
    lua_getmetatable(state, index);
    luaL_getmetatable(state, "BugEngine.Object");
    if(lua_rawequal(state, -1, -2))
    {
        lua_pop(state, 2);
        Meta::Value*         userdata   = (Meta::Value*)lua_touserdata(state, index);
        Meta::ConversionCost conversion = userdata->type().calculateConversion(type);
        if(conversion >= Meta::ConversionCost::s_incompatible)
        {
            return false;
        }
        else
        {
            new(buffer) Meta::Value(*userdata);
            return true;
        }
    }
    else
    {
        lua_pop(state, 2);
        return true;
    }
}

static bool convertTableToValue(lua_State* state, int index, const Meta::Type& type, void* buffer)
{
    if(type.metaclass->type() == Meta::ClassType_Array)
    {
        Meta::Type   arrayType  = type.metaclass->apiMethods->arrayScripting->value_type;
        u32          count      = be_checked_numcast< u32 >(luaL_len(state, index));
        Meta::Value* parameters = (Meta::Value*)malloca(
            minitl::align(count * sizeof(Meta::Value), be_alignof(Meta::Value)));

        lua_pushnil(state);
        int  i      = 0;
        bool result = true;
        while(lua_next(state, index) != 0)
        {
            if(lua_type(state, -2) != LUA_TNUMBER)
            {
                lua_pop(state, 2);
                result = false;
                count  = i;
                break;
            }
            be_assert(lua_tonumber(state, -2) == i + 1, "inconsistent LUA table");
            result |= createValue(state, -1, arrayType, &parameters[i]);
            if(!result)
            {
                lua_pop(state, 2);
                result = false;
                count  = i;
                break;
            }
            else
            {
                i++;
                lua_pop(state, 1);
            }
        }
        if(result)
        {
            Meta::Value array = type.metaclass->constructor->doCall(parameters, count);
            new(buffer) Meta::Value(array);
        }
        for(int j = count - 1; j >= 0; --j)
        {
            parameters[j].~Value();
        }
        freea(parameters);
        return result;
    }
    else if(type.metaclass->type() == Meta::ClassType_Pod)
    {
        be_assert(type.indirection == Meta::Type::Value, "POD type can only be value");
        Meta::Type   valueType = Meta::Type::makeType(type.metaclass, Meta::Type::Value,
                                                    Meta::Type::Mutable, Meta::Type::Mutable);
        Meta::Value* value     = new(buffer) Meta::Value(valueType, Meta::Value::Reserve);
        lua_pushnil(state);
        while(lua_next(state, index) != 0)
        {
            if(lua_type(state, -2) != LUA_TSTRING)
            {
                lua_pop(state, 2);
                value->~Value();
                return false;
            }
            const char*                 str      = lua_tostring(state, -2);
            raw< const Meta::Property > property = type.metaclass->getProperty(str);
            if(!property)
            {
                lua_pop(state, 2);
                value->~Value();
                return false;
            }
            Meta::Value* v      = (Meta::Value*)malloca(sizeof(Meta::Value));
            bool         result = createValue(state, -1, property->type, v);
            if(!result)
            {
                lua_pop(state, 2);
                value->~Value();
                freea(v);
                return false;
            }
            property->set(*value, *v);
            freea(v);
            lua_pop(state, 1);
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool createValue(lua_State* state, int index, const Meta::Type& type, void* value)
{
    int t = lua_type(state, index);
    switch(t)
    {
    case LUA_TNIL: return convertNilToValue(state, index, type, value);
    case LUA_TSTRING: return convertStringToValue(state, index, type, value);
    case LUA_TBOOLEAN: return convertBooleanToValue(state, index, type, value);
    case LUA_TNUMBER: return convertNumberToValue(state, index, type, value);
    case LUA_TUSERDATA: return convertUserdataToValue(state, index, type, value);
    case LUA_TTABLE: return convertTableToValue(state, index, type, value);
    default: return false;
    }
}

}}  // namespace BugEngine::Lua
