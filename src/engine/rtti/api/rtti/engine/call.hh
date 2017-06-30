/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_CALL_HH_
#define BE_RTTI_ENGINE_CALL_HH_
/**************************************************************************************************/
#include    <rtti/stdafx.h>
#include    <rtti/engine/methodinfo.script.hh>

namespace BugEngine { namespace RTTI
{

struct CallInfo
{
    u32                             distance;
    raw<const Method::Overload>     overload;
};

struct ArgInfo
{
    RTTI::Type                      type;
    raw<const Method::Parameter>    parameter;
};

struct NamedArgInfo : public ArgInfo
{
    istring                         name;
};

be_api(RTTI)
CallInfo resolve(raw<const Method> method,
                 ArgInfo* positionalArguments, u32 positionalArgumentCount,
                 NamedArgInfo* namedArguments, u32 namedArgumentCount);

template<u32 POSITIONAL_ARG_COUNT, u32 NAMED_ARG_COUNT>
CallInfo resolve(raw<const Method> method,
                 staticarray_n<POSITIONAL_ARG_COUNT, ArgInfo>& positionalArguments,
                 staticarray_n<NAMED_ARG_COUNT, NamedArgInfo>& namedArguments)
{
    return resolve(method, positionalArguments.begin(), POSITIONAL_ARG_COUNT,
                   namedArguments.begin(), NAMED_ARG_COUNT);
}

}}

/**************************************************************************************************/
#endif
