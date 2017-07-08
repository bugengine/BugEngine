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

namespace BugEngine { namespace Lua
{

struct LuaParameterType
{
    lua_State*  state;
    int         index;
    int         key;
};

static const char* luaTypeName(const LuaParameterType& type)
{
    switch(lua_type(type.state, type.index))
    {
    case LUA_TNIL:
        return "nil";
    case LUA_TBOOLEAN:
        return "bool";
    case LUA_TLIGHTUSERDATA:
        return "ligh userdata";
    case LUA_TNUMBER:
        return "number";
    case LUA_TSTRING:

    case LUA_TTABLE:
    case LUA_TFUNCTION:
    case LUA_TUSERDATA:
    case LUA_TTHREAD:
    }
}

static RTTI::Type::Distance distance(const LuaParameterType& type, const RTTI::Type& target)
{
    RTTI::Type::Distance result; result.value = 0;
    be_forceuse(type);
    be_forceuse(target);
    be_unimplemented();
    return result;
}

typedef RTTI::ArgInfo<LuaParameterType> LuaArgInfo;

static int callOverload(lua_State* state, raw<const RTTI::Method::Overload> overload,
                        LuaArgInfo parameters[], int parameterCount)
{
    RTTI::Value* values = (RTTI::Value*)malloca(sizeof(RTTI::Value) * overload->params->count);
    RTTI::Value* v = values;
    LuaArgInfo* currentParam = parameters;
    for (const RTTI::Method::Parameter* p = overload->params->begin();
         p != overload->params->end();
         ++p, ++v)
    {
        if (p == currentParam->parameter)
        {
            bool result = createValue(state, currentParam->type.index, p->type, v);
            be_assert(result, "could not convert parameter %s to %s" | p->type.name()
                                                                     | luaTypeName(currentParam->type));
            be_forceuse(result);
            ++currentParam;
        }
        else
        {
            /* todo: default values */
            be_unimplemented();
        }
    }
    RTTI::Value result = overload->call(values, overload->params->count);
    freea(values);
    return push(state, result);
}

static int call(lua_State *state, raw<const RTTI::Method> method)
{
    u32 nargs = lua_gettop(state) - 1;
    if (nargs == 1 && lua_type(state, 2) == LUA_TTABLE)
    {
        u32 parameterCount = luaL_len(state, -2);
        LuaArgInfo* parameters = (LuaArgInfo*)malloca(sizeof(LuaArgInfo) * parameterCount);

        RTTI::CallInfo result = RTTI::resolve(method, parameters, parameterCount);
        freea(parameters);
        if (result.distance.value < RTTI::Type::s_incompatible.value)
        {
            return callOverload(state, result.overload, parameters, parameterCount);
        }
    }
    return 0;
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


        minitl::format<4096u> errorMessage("no overload can convert all parameters\n");
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
        }

        return error(state, "no overload can convert all parameters\n" LUA_QS,
                          ar.name, errorMessage);
    }
}

}}
