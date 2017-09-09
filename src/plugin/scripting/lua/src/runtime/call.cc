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

static RTTI::ConversionCost calculateConversion(const LuaParameterType& type,
                                                const RTTI::Type& target)
{
    RTTI::ConversionCost result;
    be_forceuse(type);
    be_forceuse(target);
    be_unimplemented();
    return result;
}

static void convert(const LuaParameterType& type, void* buffer, const RTTI::Type& target)
{
    be_forceuse(type);
    be_forceuse(target);
    new (buffer) RTTI::Value();
}

typedef RTTI::ArgInfo<LuaParameterType> LuaArgInfo;

int call(lua_State *state, raw<const RTTI::Method> method)
{
    u32 nargs = lua_gettop(state) - 1;
    if (nargs == 1 && lua_type(state, 2) == LUA_TTABLE)
    {
        u32 parameterCount = luaL_len(state, -2);
        LuaArgInfo* parameters = (LuaArgInfo*)malloca(sizeof(LuaArgInfo) * parameterCount);

        be_unimplemented();
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
    LuaArgInfo* parameters = (LuaArgInfo*)malloca(sizeof(LuaArgInfo) * nargs);

    be_unimplemented();

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
