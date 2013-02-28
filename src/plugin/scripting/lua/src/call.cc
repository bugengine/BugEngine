/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
 see LICENSE for detail */

#include    <stdafx.h>
#include    <call.hh>
#include    <luacontext.hh>
#include    <rtti/engine/methodinfo.script.hh>

namespace BugEngine { namespace Lua
{

static RTTI::Value get(lua_State *state, int index, const RTTI::Type& type)
{
    int t = lua_type(state, index);
    switch (t)
    {
    case LUA_TSTRING:
        {
            if (type.metaclass->type() == RTTI::ClassType_String)
            {
                switch(type.metaclass->index())
                {
                case 0:
                    be_assert(be_typeid<istring>::klass() == type.metaclass,
                              "mismatching index for class %s: mistaken for %s" | type.metaclass->fullname() | be_typeid<istring>::klass()->fullname());
                    return RTTI::Value(istring(lua_tostring(state, index)));
                case 1:
                    be_assert(be_typeid<inamespace>::klass() == type.metaclass,
                              "mismatching index for class %s: mistaken for %s" | type.metaclass->fullname() | be_typeid<inamespace>::klass()->fullname());
                    return RTTI::Value(inamespace(lua_tostring(state, index)));
                case 2:
                    be_assert(be_typeid<ifilename>::klass() == type.metaclass,
                              "mismatching index for class %s: mistaken for %s" | type.metaclass->fullname() | be_typeid<ifilename>::klass()->fullname());
                    return RTTI::Value(ifilename(lua_tostring(state, index)));
                case 3:
                    be_assert(be_typeid<ipath>::klass() == type.metaclass,
                              "mismatching index for class %s: mistaken for %s" | type.metaclass->fullname() | be_typeid<ipath>::klass()->fullname());
                    return RTTI::Value(ipath(lua_tostring(state, index)));
                default:
                    be_notreached();
                    return RTTI::Value();
                }
            }
            else
            {
                be_notreached();
                return RTTI::Value();
            }
        }
    case LUA_TBOOLEAN:
        {
            if (type.metaclass == be_typeid<bool>::type().metaclass)
            {
                return RTTI::Value(lua_toboolean(state, index)?true:false);
            }
            else
            {
                be_notreached();
                return RTTI::Value();
            }
        }
    case LUA_TNUMBER:
        {
            if (type.metaclass->type() == RTTI::ClassType_Integer)
            {
                switch(type.metaclass->index())
                {
                case 0:
                    be_assert(be_typeid<bool>::klass() == type.metaclass,
                              "mismatching index for class %s: mistaken for %s" | type.metaclass->fullname() | be_typeid<bool>::klass()->fullname());
                    return RTTI::Value(lua_tonumber(state, index) != 0);
                case 1:
                    be_assert(be_typeid<u8>::klass() == type.metaclass,
                              "mismatching index for class %s: mistaken for %s" | type.metaclass->fullname() | be_typeid<u8>::klass()->fullname());
                    return RTTI::Value(static_cast<u8>(lua_tonumber(state, index)));
                case 2:
                    be_assert(be_typeid<u16>::klass() == type.metaclass,
                              "mismatching index for class %s: mistaken for %s" | type.metaclass->fullname() | be_typeid<u16>::klass()->fullname());
                    return RTTI::Value(static_cast<u16>(lua_tonumber(state, index)));
                case 3:
                    be_assert(be_typeid<u32>::klass() == type.metaclass,
                              "mismatching index for class %s: mistaken for %s" | type.metaclass->fullname() | be_typeid<u32>::klass()->fullname());
                    return RTTI::Value(static_cast<u32>(lua_tonumber(state, index)));
                case 4:
                    be_assert(be_typeid<u64>::klass() == type.metaclass,
                              "mismatching index for class %s: mistaken for %s" | type.metaclass->fullname() | be_typeid<u64>::klass()->fullname());
                    return RTTI::Value(static_cast<u64>(lua_tonumber(state, index)));
                case 5:
                    be_assert(be_typeid<i8>::klass() == type.metaclass,
                              "mismatching index for class %s: mistaken for %s" | type.metaclass->fullname() | be_typeid<i8>::klass()->fullname());
                    return RTTI::Value(static_cast<i8>(lua_tonumber(state, index)));
                case 6:
                    be_assert(be_typeid<i16>::klass() == type.metaclass,
                              "mismatching index for class %s: mistaken for %s" | type.metaclass->fullname() | be_typeid<i16>::klass()->fullname());
                    return RTTI::Value(static_cast<i16>(lua_tonumber(state, index)));
                case 7:
                    be_assert(be_typeid<i32>::klass() == type.metaclass,
                              "mismatching index for class %s: mistaken for %s" | type.metaclass->fullname() | be_typeid<i32>::klass()->fullname());
                    return RTTI::Value(static_cast<i32>(lua_tonumber(state, index)));
                case 8:
                    be_assert(be_typeid<i64>::klass() == type.metaclass,
                              "mismatching index for class %s: mistaken for %s" | type.metaclass->fullname() | be_typeid<i64>::klass()->fullname());
                    return RTTI::Value(static_cast<i64>(lua_tonumber(state, index)));
                case 9:
                    be_assert(be_typeid<float>::klass() == type.metaclass,
                              "mismatching index for class %s: mistaken for %s" | type.metaclass->fullname() | be_typeid<float>::klass()->fullname());
                    return RTTI::Value(static_cast<float>(lua_tonumber(state, index)));
                case 10:
                    be_assert(be_typeid<double>::klass() == type.metaclass,
                              "mismatching index for class %s: mistaken for %s" | type.metaclass->fullname() | be_typeid<double>::klass()->fullname());
                    return RTTI::Value(static_cast<double>(lua_tonumber(state, index)));
                default:
                    be_notreached();
                    return RTTI::Value();
                }
            }
            else
            {
                be_notreached();
                return RTTI::Value();
            }
        }
    case LUA_TUSERDATA:
        {
            lua_getmetatable(state, index);
            luaL_getmetatable(state, "BugEngine.Object");
            if (lua_rawequal(state, -1, -2))
            {
                lua_pop(state, 2);
                return *(RTTI::Value*)lua_touserdata(state, index);
            }
            else
            {
                lua_pop(state, 2);
                be_notreached();
                return RTTI::Value();
            }
        }
    case LUA_TTABLE:
        {
            be_info("%s" | type);
            if (type.metaclass->type() == RTTI::ClassType_Array)
            {
                RTTI::Type arrayType = RTTI::Value(type.metaclass)["value_type"].as<const RTTI::Type&>();
                int count = luaL_len(state, index);
                RTTI::Value* parameters = (RTTI::Value*)malloca(minitl::align(count * sizeof(RTTI::Value), be_alignof(RTTI::Value)));
                for (int i = 0; i < count; ++i)
                {
                    lua_pushnumber(state, i+1);
                    lua_gettable(state, -2);
                    new ((void*)&parameters[i]) RTTI::Value(get(state, -1, arrayType));
                    lua_pop(state, 1);
                }
                RTTI::Value array = type.metaclass->constructor->doCall(parameters, count);
                for (int i = count-1; i >= 0; --i)
                {
                    parameters[i].~Value();
                }
                freea(parameters);
                return array;
            }
            else
            {
                lua_pop(state, 2);
                be_notreached();
                return RTTI::Value();
            }
        }
    default:
        be_notreached();
        return RTTI::Value();
    }
}

int call(lua_State* state, RTTI::Value* value, raw<const RTTI::Method::Overload> overload)
{
    int top = lua_gettop(state);
    raw<const RTTI::Method::Overload::Parameter> parameter = overload->params;

    void* v = malloca(minitl::align(sizeof (RTTI::Value), be_alignof(RTTI::Value))*(top - 1));
    RTTI::Value* values = (RTTI::Value*)v;

    for (int i = 1; i < top; i++, parameter = parameter->next)
    {
        new((void*) (&values[i-1])) RTTI::Value(get(state, i+1, parameter->type));
    }

    RTTI::Value result = (*value)(values, top - 1);

    for (int i = top - 1; i > 0; --i)
        values[i - 1].~Value();
    freea(v);

    if (result)
    {
        Context::push(state, result);
        return 1;
    }
    else
    {
        return 0;
    }
}

}}