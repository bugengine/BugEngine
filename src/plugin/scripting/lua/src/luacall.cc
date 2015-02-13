/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
 see LICENSE for detail */

#include    <stdafx.h>
#include    <luacall.hh>
#include    <luacontext.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/engine/propertyinfo.script.hh>

namespace BugEngine { namespace Lua
{

static int get(lua_State *state, int index, const RTTI::Type& type, RTTI::Value* value);

static int convertNilToValue(lua_State *state, int index, const RTTI::Type& type, RTTI::Value* value)
{
    be_forceuse(state);
    be_forceuse(index);
    if (type.indirection >= RTTI::Type::RawPtr)
    {
        new (value) RTTI::Value(type, RTTI::Value::Reserve);
        *static_cast<void**>(value->memory()) = 0;
        return 0;
    }
    else
    {
        return -1;
    }
}
static int convertStringToValue(lua_State *state, int index, const RTTI::Type& type, RTTI::Value* value)
{
    if (type.metaclass->type() == RTTI::ClassType_String)
    {
        switch(type.metaclass->index())
        {
        case 0:
            be_assert(be_typeid<istring>::klass() == type.metaclass,
                      "mismatching index for class %s: mistaken for %s" | type.metaclass->fullname() | be_typeid<istring>::klass()->fullname());
            new (value) RTTI::Value(istring(lua_tostring(state, index)));
            return 0;
        case 1:
            be_assert(be_typeid<inamespace>::klass() == type.metaclass,
                      "mismatching index for class %s: mistaken for %s" | type.metaclass->fullname() | be_typeid<inamespace>::klass()->fullname());
            new (value) RTTI::Value(inamespace(lua_tostring(state, index)));
            return 0;
        case 2:
            be_assert(be_typeid<ifilename>::klass() == type.metaclass,
                      "mismatching index for class %s: mistaken for %s" | type.metaclass->fullname() | be_typeid<ifilename>::klass()->fullname());
            new (value) RTTI::Value(ifilename(lua_tostring(state, index)));
            return 0;
        case 3:
            be_assert(be_typeid<ipath>::klass() == type.metaclass,
                      "mismatching index for class %s: mistaken for %s" | type.metaclass->fullname() | be_typeid<ipath>::klass()->fullname());
            new (value) RTTI::Value(ipath(lua_tostring(state, index)));
            return 0;
        default:
            be_notreached();
            return -1;
        }
    }
    else
    {
        return -1;
    }
}


static int convertBooleanToValue(lua_State *state, int index, const RTTI::Type& type, RTTI::Value* value)
{
    if (type.metaclass == be_typeid<bool>::type().metaclass)
    {
        new (value) RTTI::Value(lua_toboolean(state, index)?true:false);
        return 0;
    }
    else
    {
        return -1;
    }
}

static int convertNumberToValue(lua_State *state, int index, const RTTI::Type& type, RTTI::Value* value)
{
    if (type.metaclass->type() == RTTI::ClassType_Number)
    {
        switch(type.metaclass->index())
        {
        case 0:
            {
                be_assert(be_typeid<bool>::klass() == type.metaclass,
                          "mismatching index for class %s: mistaken for %s" | type.metaclass->fullname() | be_typeid<bool>::klass()->fullname());
                lua_Number v = lua_tonumber(state, index);
                const lua_Number g_epsilon = 0.000001f;
                new (value) RTTI::Value(v > g_epsilon || v < -g_epsilon);
                return 1;
            }
        case 1:
            be_assert(be_typeid<u8>::klass() == type.metaclass,
                      "mismatching index for class %s: mistaken for %s" | type.metaclass->fullname() | be_typeid<u8>::klass()->fullname());
            new (value) RTTI::Value(static_cast<u8>(lua_tonumber(state, index)));
            return 0;
        case 2:
            be_assert(be_typeid<u16>::klass() == type.metaclass,
                      "mismatching index for class %s: mistaken for %s" | type.metaclass->fullname() | be_typeid<u16>::klass()->fullname());
            new (value) RTTI::Value(static_cast<u16>(lua_tonumber(state, index)));
            return 0;
        case 3:
            be_assert(be_typeid<u32>::klass() == type.metaclass,
                      "mismatching index for class %s: mistaken for %s" | type.metaclass->fullname() | be_typeid<u32>::klass()->fullname());
            new (value) RTTI::Value(static_cast<u32>(lua_tonumber(state, index)));
            return 0;
        case 4:
            be_assert(be_typeid<u64>::klass() == type.metaclass,
                      "mismatching index for class %s: mistaken for %s" | type.metaclass->fullname() | be_typeid<u64>::klass()->fullname());
            new (value) RTTI::Value(static_cast<u64>(lua_tonumber(state, index)));
            return 0;
        case 5:
            be_assert(be_typeid<i8>::klass() == type.metaclass,
                      "mismatching index for class %s: mistaken for %s" | type.metaclass->fullname() | be_typeid<i8>::klass()->fullname());
            new (value) RTTI::Value(static_cast<i8>(lua_tonumber(state, index)));
            return 0;
        case 6:
            be_assert(be_typeid<i16>::klass() == type.metaclass,
                      "mismatching index for class %s: mistaken for %s" | type.metaclass->fullname() | be_typeid<i16>::klass()->fullname());
            new (value) RTTI::Value(static_cast<i16>(lua_tonumber(state, index)));
            return 0;
        case 7:
            be_assert(be_typeid<i32>::klass() == type.metaclass,
                      "mismatching index for class %s: mistaken for %s" | type.metaclass->fullname() | be_typeid<i32>::klass()->fullname());
            new (value) RTTI::Value(static_cast<i32>(lua_tonumber(state, index)));
            return 0;
        case 8:
            be_assert(be_typeid<i64>::klass() == type.metaclass,
                      "mismatching index for class %s: mistaken for %s" | type.metaclass->fullname() | be_typeid<i64>::klass()->fullname());
            new (value) RTTI::Value(static_cast<i64>(lua_tonumber(state, index)));
            return 0;
        case 9:
            be_assert(be_typeid<float>::klass() == type.metaclass,
                      "mismatching index for class %s: mistaken for %s" | type.metaclass->fullname() | be_typeid<float>::klass()->fullname());
            new (value) RTTI::Value(static_cast<float>(lua_tonumber(state, index)));
            return 0;
        case 10:
            be_assert(be_typeid<double>::klass() == type.metaclass,
                      "mismatching index for class %s: mistaken for %s" | type.metaclass->fullname() | be_typeid<double>::klass()->fullname());
            new (value) RTTI::Value(static_cast<double>(lua_tonumber(state, index)));
            return 0;
        default:
        be_notreached();
            return -1;
        }
    }
    else
    {
        return -1;
    }
}

static int convertUserdataToValue(lua_State *state, int index, const RTTI::Type& type, RTTI::Value* value)
{
    lua_getmetatable(state, index);
    luaL_getmetatable(state, "BugEngine.Object");
    if (lua_rawequal(state, -1, -2))
    {
        lua_pop(state, 2);
        RTTI::Value* userdata = (RTTI::Value*)lua_touserdata(state, index);
        u32 distance = type.metaclass->type() == RTTI::ClassType_Variant ? 10 : userdata->type().distance(type);
        if (distance >= RTTI::Type::MaxTypeDistance)
        {
            return -1;
        }
        else
        {
            new (value) RTTI::Value(*userdata);
            return distance;
        }
    }
    else
    {
        lua_pop(state, 2);
        return -1;
    }
}

static int convertTableToValue(lua_State *state, int index, const RTTI::Type& type, RTTI::Value* value)
{
    if (type.metaclass->type() == RTTI::ClassType_Array)
    {
        RTTI::Type arrayType = RTTI::Value(type.metaclass)["value_type"].as<const RTTI::Type&>();
        int count = luaL_len(state, index);
        RTTI::Value* parameters = (RTTI::Value*)malloca(minitl::align(count * sizeof(RTTI::Value), be_alignof(RTTI::Value)));

        lua_pushnil(state);
        int i = 0;
        int score = 0;
        while (lua_next(state, index) != 0)
        {
            if (lua_type(state, -2) != LUA_TNUMBER)
            {
                lua_pop(state, 2);
                score = -1;
                count = i;
                break;
            }
            be_assert(lua_tonumber(state, -2) == i+1, "inconsistent LUA table");
            int elementScore = get(state, -1, arrayType, &parameters[i]);
            if (elementScore < 0)
            {
                lua_pop(state, 2);
                score = -1;
                count = i;
                break;
            }
            else
            {
                score += elementScore;
                i++;
                lua_pop(state, 1);
            }
        }
        if (score >= 0)
        {
            RTTI::Value array = type.metaclass->constructor->doCall(parameters, count);
            new (value) RTTI::Value(array);
        }
        for (int j = count-1; j >= 0; --j)
        {
            parameters[j].~Value();
        }
        freea(parameters);
        return score;
    }
    else if (type.metaclass->type() == RTTI::ClassType_Pod)
    {
        be_assert(type.indirection == RTTI::Type::Value, "POD type can only be value");
        RTTI::Type valueType = RTTI::Type::makeType(type.metaclass, RTTI::Type::Value, RTTI::Type::Mutable, RTTI::Type::Mutable);
        new (value) RTTI::Value(valueType, RTTI::Value::Reserve);
        lua_pushnil(state);
        while (lua_next(state, index) != 0)
        {
            if (lua_type(state, -2) != LUA_TSTRING)
            {
                lua_pop(state, 2);
                value->~Value();
                return -1;
            }
            const char *str = lua_tostring(state, -2);
            raw<const RTTI::Property> property = type.metaclass->getProperty(str);
            if (!property)
            {
                lua_pop(state, 2);
                value->~Value();
                return -1;
            }
            RTTI::Value* v = (RTTI::Value*)malloca(sizeof(RTTI::Value));
            int score = get(state, -1, property->type, v);
            if (score < 0)
            {
                lua_pop(state, 2);
                value->~Value();
                freea(v);
                return -1;
            }
            property->set(*value, *v);
            freea(v);
            lua_pop(state, 1);
        }
        return 0;
    }
    else
    {
        return -1;
    }
}

static int get(lua_State *state, int index, const RTTI::Type& type, RTTI::Value* value)
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
        return -1;
    }
}

struct CallInfo
{
    raw<const RTTI::Method::Overload>   m_overload;
    raw<const RTTI::Method::Parameter>  m_currentParameter;
    u32                                 m_score;
    RTTI::Value*                        m_parameters;
    RTTI::Value*                        m_nextParameter;

    CallInfo(raw<const RTTI::Method::Overload> overload, void* paramBuffer);
    ~CallInfo();

    void swap(CallInfo& other);

    static CallInfo* findOverload(lua_State* state, CallInfo* overloads, u32 overloadCount);
};

void CallInfo::swap(CallInfo& other)
{
    minitl::swap(m_overload, other.m_overload);
    minitl::swap(m_currentParameter, other.m_currentParameter);
    minitl::swap(m_score, other.m_score);
    minitl::swap(m_parameters, other.m_parameters);
    minitl::swap(m_nextParameter, other.m_nextParameter);
}

CallInfo::CallInfo(raw<const RTTI::Method::Overload> overload, void* paramBuffer)
    :   m_overload(overload)
    ,   m_currentParameter(overload->params)
    ,   m_score(0)
    ,   m_parameters((RTTI::Value*)paramBuffer)
    ,   m_nextParameter((RTTI::Value*)paramBuffer)
{
}

CallInfo::~CallInfo()
{
    while (m_nextParameter != m_parameters)
    {
        m_nextParameter--;
        m_nextParameter->~Value();
    }
}

CallInfo* CallInfo::findOverload(lua_State *state, CallInfo *overloads, u32 overloadCount)
{
    u32 nargs = lua_gettop(state) - 1;
    for (u32 arg = 0; arg < nargs; ++arg)
    {
        for (u32 overload = 0; overload < overloadCount; )
        {
            int score = get(state, arg+2, overloads[overload].m_currentParameter->type, overloads[overload].m_nextParameter);
            if (score >= 0)
            {
                overloads[overload].m_score += score;
                overloads[overload].m_nextParameter ++;
                overloads[overload].m_currentParameter = overloads[overload].m_currentParameter->next;
                ++overload;
            }
            else
            {
                overloads[overload].swap(overloads[overloadCount-1]);
                overloadCount --;
            }
        }
    }
    if (overloadCount != 0)
    {
        CallInfo* best = overloads;
        for (u32 i = 1; i < overloadCount; ++i)
        {
            if (overloads[i].m_score < best->m_score)
            {
                best = &overloads[i];
            }
        }
        return best;
    }
    else
    {
        return 0;
    }
}

int call(lua_State* state, raw<const RTTI::Method> method)
{
    u32 nargs = lua_gettop(state) - 1;
    u32 overloadCount = method->overloadCount;
    CallInfo* overloads = (CallInfo*)malloca(overloadCount * minitl::align(sizeof(CallInfo), be_alignof(CallInfo)));
    CallInfo* currentOverload = overloads;
    raw<const RTTI::Method::Overload> o = method->overloads;
    while (o)
    {
        if (o->parameterCount == nargs)
        {
            void* buffer = malloca(nargs * minitl::align(sizeof(RTTI::Value), be_alignof(RTTI::Value)));
            new (currentOverload) CallInfo(o, buffer);
            currentOverload ++;
        }
        else
        {
            overloadCount --;
        }
        o = o->next;
    }
    CallInfo* callinfo = CallInfo::findOverload(state, overloads, overloadCount);
    if (callinfo)
    {
        RTTI::Value result = (*callinfo->m_overload->call)(callinfo->m_parameters, nargs);
        while (currentOverload != overloads)
        {
            currentOverload --;
            void* buffer = currentOverload->m_parameters;
            currentOverload->~CallInfo();
            freea(buffer);
        }
        freea(overloads);
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
    else
    {
        while (currentOverload != overloads)
        {
            currentOverload --;
            void* buffer = currentOverload->m_parameters;
            currentOverload->~CallInfo();
            freea(buffer);
        }
        freea(overloads);


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
        char errorMessage[8192];
        errorMessage[0] = 0;
        if (nargs != 0)
        {
            strcat(errorMessage, "parameters:\n\t");
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
            strcat(errorMessage, ar.name);
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
        }

        return luaL_error(state, LUA_QS ": no overload can convert all parameters\n%s",
                          ar.name, errorMessage);
    }
}

}}
