/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <package/stdafx.h>
#include    <package/nodes/overloadmatch.hh>
#include    <package/nodes/parameter.hh>
#include    <rtti/value.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

static RTTI::Type::ConversionCost calculateConversion(ref<const Parameter> parameter,
                                                      const RTTI::Type& target)
{
    return parameter->calculateConversion(target);
}

static void convert(ref<const Parameter> parameter, void* buffer, const RTTI::Type& target)
{
    new(buffer) RTTI::Value(parameter->as(target));
}

OverloadMatch::OverloadMatch(raw<const RTTI::Method::Overload> overload)
    :   m_args(Arena::packageBuilder(), overload->params ? overload->params->count : 0)
    ,   m_callInfo()
{
    m_callInfo.overload = overload;
}

void OverloadMatch::update(const minitl::vector< ref<const Parameter> >& parameters)
{
    m_args.clear();
    m_args.reserve(parameters.size());
    for (minitl::vector< ref<const Parameter> >::const_iterator it = parameters.begin();
         it != parameters.end();
         ++it)
    {
        m_args.push_back(ArgInfo((*it)->name(),
                                 (*it)));
    }
    m_callInfo.conversion = RTTI::getCost(m_callInfo.overload,
                                          &m_args[0],
                                          be_checked_numcast<u32>(m_args.size()));
}

RTTI::Value OverloadMatch::create(istring name) const
{
    be_forceuse(name);
    return RTTI::call(m_callInfo, &m_args[0], be_checked_numcast<u32>(m_args.size()));
}

}}}
