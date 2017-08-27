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

    bool operator<(const CallInfo& other)
    {
        return conversion < other.conversion;
    }
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
Type::ConversionCost getCost(raw<const Method::Overload> overload,
                             ArgInfo<T> arguments[], u32 argumentCount)
{
    Type::ConversionCost cost;
    const Method::Parameter* p = overload->params->begin();
    for (u32 i = 0; i < argumentCount && p != overload->params->end(); ++i, ++p)
    {
        cost += calculateConversion(arguments[i].type, p->type);
    }
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
Value call(CallInfo callInfo,
           const ArgInfo<T> arguments[], u32 argumentCount)
{
    Value* v = (Value*)malloca(sizeof(Value) * argumentCount);
    const Method::Parameter* p = callInfo.overload->params->begin();
    for (u32 i = 0; i < argumentCount; ++i, ++p)
    {
        be_assert(p != callInfo.overload->params->end(),
                  "too few arguments passed to call");
        convert(arguments[i].type, static_cast<void*>(&v[i]), p->type);
    }
    Value result = callInfo.overload->call(v, argumentCount);
    for (u32 i = argumentCount; i > 0; --i)
    {
        v[i-1].~Value();
    }
    freea(v);
    return result;
}

}}

/**************************************************************************************************/
#endif
