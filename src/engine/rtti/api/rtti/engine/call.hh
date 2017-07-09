/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_CALL_HH_
#define BE_RTTI_ENGINE_CALL_HH_
/**************************************************************************************************/
#include    <rtti/stdafx.h>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/value.hh>

namespace BugEngine { namespace RTTI
{

struct CallInfo
{
    RTTI::Type::ConversionCost      conversion;
    raw<const Method::Overload>     overload;
};

template< typename T >
struct ArgInfo
{
    raw<const Method::Parameter>    parameter;
    istring                         name;
    const T                         type;
    explicit ArgInfo(const T& t)
        :   parameter()
        ,   name()
        ,   type(t)
    {
    }
    ArgInfo(istring name, const T& t)
        :   parameter()
        ,   name(name)
        ,   type(t)
    {
    }
};

template< typename T >
RTTI::Type::ConversionCost getCost(raw<const Method::Overload> overload,
                                   ArgInfo<T> arguments[], u32 argumentCount)
{
    RTTI::Type::ConversionCost cost;
    be_forceuse(overload);
    be_forceuse(arguments);
    be_forceuse(argumentCount);
    be_unimplemented();
    return cost;
}

template< typename T >
CallInfo resolve(raw<const Method> method,
                 ArgInfo<T> arguments[], u32 argumentCount)
{
    CallInfo best = {
        Type::s_incompatible,
        {0}
    };
    for (u32 i = 0; i < method->overloads->count; ++i)
    {
        raw<const Method::Overload> overload = { method->overloads->begin() + i };
        CallInfo c = {
            getCost(overload, arguments, argumentCount),
            overload
        };
        if (c.conversion < best.conversion)
        {
            best = c;
        }
    }
    return best;
}

template< typename T >
RTTI::Value call(CallInfo callInfo,
                 ArgInfo<T> arguments[], u32 argumentCount)
{
    be_forceuse(callInfo);
    be_forceuse(arguments);
    be_forceuse(argumentCount);
    be_unimplemented();
    return RTTI::Value();
}

}}

/**************************************************************************************************/
#endif
