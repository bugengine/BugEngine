/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <packagebuilder/nodes/overloadmatch.hh>
#include    <packagebuilder/nodes/parameter.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

static const u32 s_parameterTooMany = 1000000;
static const u32 s_parameterBadType = 10000;
static const u32 s_parameterMissing = 100;

OverloadMatch::OverloadMatch(raw<const RTTI::MethodInfo::OverloadInfo> overload)
    :   m_params(packageBuilderArena())
    ,   m_score(0)
{
    for (raw<const ParamInfo> param = overload->params; param; param = param->next)
    {
        ParameterMatch match;
        match.match = param;
        m_params.push_back(match);
        m_score += s_parameterMissing;
    }
}

void OverloadMatch::addParameter(weak<const Parameter> param)
{
    for (minitl::vector< ParameterMatch >::iterator it = m_params.begin(); it != m_params.end(); ++it)
    {
        if (it->match && it->match->name == param->name())
        {
            if (it->parameter)
            {
                be_unimplemented();
            }
            it->parameter = param;
            m_score -= s_parameterMissing;
            return;
        }
    }
    ParameterMatch unusedParam;
    unusedParam.parameter = param;
    m_params.push_back(unusedParam);
}

bool OverloadMatch::operator<(const OverloadMatch& other) const
{
    return m_score < other.m_score;
}

}}}
