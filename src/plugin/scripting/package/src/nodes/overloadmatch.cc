/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/plugin.scripting.package/stdafx.h>
#include <bugengine/plugin.scripting.package/nodes/overloadmatch.hh>
#include <bugengine/plugin.scripting.package/nodes/parameter.hh>
#include <bugengine/rtti/value.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes {

namespace {

struct ArgumentSort
{
    bool operator()(const OverloadMatch::ArgInfo& arg1, const OverloadMatch::ArgInfo& arg2) const
    {
        return arg1.parameter < arg2.parameter;
    }
};

}  // namespace

RTTI::ConversionCost calculateConversion(ref< const Parameter > parameter,
                                                const RTTI::Type&      target)
{
    return parameter->calculateConversion(target);
}

void convert(ref< const Parameter > parameter, void* buffer, const RTTI::Type& target)
{
    new(buffer) RTTI::Value(parameter->as(target));
}

OverloadMatch::OverloadMatch(raw< const RTTI::Method::Overload > overload)
    : m_args(Arena::packageBuilder(), overload->params.count)
    , m_indices(Arena::packageBuilder(), overload->params.count)
    , m_callInfo()
{
    m_callInfo.overload = overload;
}

void OverloadMatch::update(const minitl::vector< ref< const Parameter > >& parameters)
{
    m_args.clear();
    m_args.reserve(parameters.size());
    m_indices.clear();
    m_indices.reserve(parameters.size());
    for(minitl::vector< ref< const Parameter > >::const_iterator it = parameters.begin();
        it != parameters.end(); ++it)
    {
        m_args.push_back(ArgInfo((*it)->name(), (*it)));
    }
    m_callInfo = RTTI::getCost(m_callInfo.overload, &m_indices[0], static_cast< const ArgInfo* >(0),
                               0, &m_args[0], be_checked_numcast< u32 >(m_args.size()));
    for(u32 i = 0; i < m_args.size(); ++i)
    {
        m_args[i].parameter = m_indices[i];
    }
    minitl::sort(m_args.begin(), m_args.end(), ArgumentSort());
}

RTTI::Value OverloadMatch::create(istring name) const
{
    be_forceuse(name);
    return RTTI::call(m_callInfo, static_cast< const ArgInfo* >(0), 0, &m_args[0],
                      be_checked_numcast< u32 >(m_args.size()));
}

}}}  // namespace BugEngine::PackageBuilder::Nodes
