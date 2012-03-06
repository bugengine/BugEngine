/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <package/stdafx.h>
#include    <package/nodes/overloadmatch.hh>
#include    <package/nodes/parameter.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

static const u32 s_parameterTooMany = 1000000;
static const u32 s_parameterBadType = 10000;
static const u32 s_parameterMissing = 100;

OverloadMatch::OverloadMatch(raw<const RTTI::Method::Overload> overload)
    :   m_overload(overload)
    ,   m_params(packageBuilderArena())
    ,   m_score(0)
{
    for (raw<const RTTIParameter> param = m_overload->params; param; param = param->next)
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
            else
            {
                it->parameter = param;
                m_score -= s_parameterMissing;
                if (! param->isCompatible(it->match->type))
                {
                    m_score += s_parameterBadType;
                }
            }
            return;
        }
    }
    ParameterMatch unusedParam;
    unusedParam.parameter = param;
    unusedParam.match = raw<const RTTIParameter>::null();
    m_params.push_back(unusedParam);
}

bool OverloadMatch::operator<(const OverloadMatch& other) const
{
    return m_score < other.m_score;
}

BugEngine::Value OverloadMatch::create() const
{
    be_assert(m_score == 0, "cannot create object when no perfect match was found");
    be_assert(m_params.size() <= 16, "parameter count larger than buffer; grow buffer");
    BugEngine::Value v[16];
    for(size_t i = 0; i < m_params.size(); ++i)
    {
        v[i] = m_params[i].parameter->as(m_params[i].match->type);
    }
    return (*(m_overload->call))(v, be_checked_numcast<u32>(m_params.size()));
}

}}}
