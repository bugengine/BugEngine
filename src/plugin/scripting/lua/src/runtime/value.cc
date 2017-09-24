/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
 see LICENSE for detail */

#include    <stdafx.h>
#include    <runtime/value.hh>
#include    <context.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/engine/propertyinfo.script.hh>
#include    <rtti/engine/objectinfo.script.hh>
#include    <rtti/engine/scriptingapi.hh>


namespace BugEngine { namespace Lua
{

static bool convertNilToValue(lua_State *state, int index, const RTTI::Type& type, void* buffer)
{
    be_forceuse(state);
    be_forceuse(index);
    if (type.indirection >= RTTI::Type::RawPtr)
    {
        RTTI::Value* value = new (buffer) RTTI::Value(type, RTTI::Value::Reserve);
        *static_cast<void**>(value->memory()) = 0;
        return true;
    }
    else
    {
        return false;
    }
}

static bool convertStringToValue(lua_State *state, int index, const RTTI::Type& type, void* buffer)
{
    if (type.metaclass->type() == RTTI::ClassType_String)
    {
        switch(type.metaclass->index())
        {
        case RTTI::ClassIndex_istring:
            new (buffer) RTTI::Value(istring(lua_tostring(state, index)));
            return true;
        case RTTI::ClassIndex_inamespace:
            new (buffer) RTTI::Value(inamespace(lua_tostring(state, index)));
            return true;
        case RTTI::ClassIndex_ifilename:
            new (buffer) RTTI::Value(ifilename(lua_tostring(state, index)));
            return true;
        case RTTI::ClassIndex_ipath:
            new (buffer) RTTI::Value(ipath(lua_tostring(state, index)));
            return true;
        case RTTI::ClassIndex_text:
            new (buffer) RTTI::Value(text(lua_tostring(state, index)));
            return true;
        default:
            be_notreached();
            return false;
        }
    }
    else
    {
        return false;
    }
}

static bool convertBooleanToValue(lua_State *state, int index, const RTTI::Type& type, void* buffer)
{
    if (type.metaclass == be_typeid<bool>::type().metaclass)
    {
        new (buffer) RTTI::Value(lua_toboolean(state, index)
                                  ? true
                                  : false);
        return true;
    }
    else if (type.metaclass->type() == RTTI::ClassType_Number)
    {
        be_warning("%s: - dubious cast: Lua bool to %s" | Context::getCallInfo(state) | type.metaclass->name);
        switch(type.metaclass->index())
        {
        case RTTI::ClassIndex_u8:
            new (buffer) RTTI::Value(static_cast<u8>(lua_toboolean(state, index)));
            return true;
        case RTTI::ClassIndex_u16:
            new (buffer) RTTI::Value(static_cast<u16>(lua_toboolean(state, index)));
            return true;
        case RTTI::ClassIndex_u32:
            new (buffer) RTTI::Value(static_cast<u32>(lua_toboolean(state, index)));
            return true;
        case RTTI::ClassIndex_u64:
            new (buffer) RTTI::Value(static_cast<u64>(lua_toboolean(state, index)));
            return true;
        case RTTI::ClassIndex_i8:
            new (buffer) RTTI::Value(static_cast<i8>(lua_toboolean(state, index)));
            return true;
        case RTTI::ClassIndex_i16:
            new (buffer) RTTI::Value(static_cast<i16>(lua_toboolean(state, index)));
            return true;
        case RTTI::ClassIndex_i32:
            new (buffer) RTTI::Value(static_cast<i32>(lua_toboolean(state, index)));
            return true;
        case RTTI::ClassIndex_i64:
            new (buffer) RTTI::Value(static_cast<i64>(lua_toboolean(state, index)));
            return true;
        case RTTI::ClassIndex_float:
            new (buffer) RTTI::Value(static_cast<float>(lua_toboolean(state, index)));
            return true;
        case RTTI::ClassIndex_double:
            new (buffer) RTTI::Value(static_cast<double>(lua_toboolean(state, index)));
            return true;
        default:
            be_notreached();
            return false;
        }
    }
    else
    {
        return false;
    }
}

static bool convertNumberToValue(lua_State *state, int index, const RTTI::Type& type, void* buffer)
{
    if (type.metaclass->type() == RTTI::ClassType_Number)
    {
        switch(type.metaclass->index())
        {
        case RTTI::ClassIndex_bool:
            {
                lua_Number v = lua_tonumber(state, index);
                const lua_Number g_epsilon = 0.000001f;
                new (buffer) RTTI::Value(v > g_epsilon || v < -g_epsilon);
                return true;
            }
        case RTTI::ClassIndex_u8:
            new (buffer) RTTI::Value(static_cast<u8>(lua_tonumber(state, index)));
            return true;
        case RTTI::ClassIndex_u16:
            new (buffer) RTTI::Value(static_cast<u16>(lua_tonumber(state, index)));
            return true;
        case RTTI::ClassIndex_u32:
            new (buffer) RTTI::Value(static_cast<u32>(lua_tonumber(state, index)));
            return true;
        case RTTI::ClassIndex_u64:
            new (buffer) RTTI::Value(static_cast<u64>(lua_tonumber(state, index)));
            return true;
        case RTTI::ClassIndex_i8:
            new (buffer) RTTI::Value(static_cast<i8>(lua_tonumber(state, index)));
            return true;
        case RTTI::ClassIndex_i16:
            new (buffer) RTTI::Value(static_cast<i16>(lua_tonumber(state, index)));
            return true;
        case RTTI::ClassIndex_i32:
            new (buffer) RTTI::Value(static_cast<i32>(lua_tonumber(state, index)));
            return true;
        case RTTI::ClassIndex_i64:
            new (buffer) RTTI::Value(static_cast<i64>(lua_tonumber(state, index)));
            return true;
        case RTTI::ClassIndex_float:
            new (buffer) RTTI::Value(static_cast<float>(lua_tonumber(state, index)));
            return true;
        case RTTI::ClassIndex_double:
            new (buffer) RTTI::Value(static_cast<double>(lua_tonumber(state, index)));
            return true;
        default:
            be_notreached();
            return false;
        }
    }
    else
    {
        return false;
    }
}

static bool convertUserdataToValue(lua_State *state, int index, const RTTI::Type& type, void* buffer)
{
    lua_getmetatable(state, index);
    luaL_getmetatable(state, "BugEngine.Object");
    if (lua_rawequal(state, -1, -2))
    {
        lua_pop(state, 2);
        RTTI::Value* userdata = (RTTI::Value*)lua_touserdata(state, index);
        RTTI::ConversionCost conversion = userdata->type().calculateConversion(type);
        if (conversion >= RTTI::ConversionCost::s_incompatible)
        {
            return false;
        }
        else
        {
            new (buffer) RTTI::Value(*userdata);
            return true;
        }
    }
    else
    {
        lua_pop(state, 2);
        return true;
    }
}

static bool convertTableToValue(lua_State *state, int index, const RTTI::Type& type, void* buffer)
{
    if (type.metaclass->type() == RTTI::ClassType_Array)
    {
        RTTI::Type arrayType = type.metaclass->apiMethods->arrayScripting->value_type;
        int count = luaL_len(state, index);
        RTTI::Value* parameters = (RTTI::Value*)malloca(minitl::align(count * sizeof(RTTI::Value), be_alignof(RTTI::Value)));

        lua_pushnil(state);
        int i = 0;
        bool result = true;
        while (lua_next(state, index) != 0)
        {
            if (lua_type(state, -2) != LUA_TNUMBER)
            {
                lua_pop(state, 2);
                result = false;
                count = i;
                break;
            }
            be_assert(lua_tonumber(state, -2) == i+1, "inconsistent LUA table");
            result |= createValue(state, -1, arrayType, &parameters[i]);
            if (!result)
            {
                lua_pop(state, 2);
                result = false;
                count = i;
                break;
            }
            else
            {
                i++;
                lua_pop(state, 1);
            }
        }
        if (result)
        {
            RTTI::Value array = type.metaclass->constructor->doCall(parameters, count);
            new (buffer) RTTI::Value(array);
        }
        for (int j = count-1; j >= 0; --j)
        {
            parameters[j].~Value();
        }
        freea(parameters);
        return result;
    }
    else if (type.metaclass->type() == RTTI::ClassType_Pod)
    {
        be_assert(type.indirection == RTTI::Type::Value, "POD type can only be value");
        RTTI::Type valueType = RTTI::Type::makeType(type.metaclass, RTTI::Type::Value,
                                                    RTTI::Type::Mutable, RTTI::Type::Mutable);
        RTTI::Value* value = new (buffer) RTTI::Value(valueType, RTTI::Value::Reserve);
        lua_pushnil(state);
        while (lua_next(state, index) != 0)
        {
            if (lua_type(state, -2) != LUA_TSTRING)
            {
                lua_pop(state, 2);
                value->~Value();
                return false;
            }
            const char *str = lua_tostring(state, -2);
            raw<const RTTI::Property> property = type.metaclass->getProperty(str);
            if (!property)
            {
                lua_pop(state, 2);
                value->~Value();
                return false;
            }
            RTTI::Value* v = (RTTI::Value*)malloca(sizeof(RTTI::Value));
            bool result = createValue(state, -1, property->type, v);
            if (!result)
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

bool createValue(lua_State *state, int index, const RTTI::Type& type, void* value)
{
    int t = lua_type(state, index);
    switch (t)
    {
    case LUA_TNIL:
        return convertNilToValue(state, index, type, value);
    case LUA_TSTRING:
        return convertStringToValue(state, index, type, value);
    case LUA_TBOOLEAN:
        return convertBooleanToValue(state, index, type, value);
    case LUA_TNUMBER:
        return convertNumberToValue(state, index, type, value);
    case LUA_TUSERDATA:
        return convertUserdataToValue(state, index, type, value);
    case LUA_TTABLE:
        return convertTableToValue(state, index, type, value);
    default:
        return false;
    }
}

}}
