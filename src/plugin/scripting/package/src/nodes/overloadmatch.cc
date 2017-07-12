/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <package/stdafx.h>
#include    <package/nodes/overloadmatch.hh>
#include    <package/nodes/parameter.hh>
#include    <rtti/value.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

OverloadMatch::OverloadMatch(raw<const RTTI::Method::Overload> overload)
    :   m_overload(overload)
    ,   m_cost()
{
}

void OverloadMatch::update(const minitl::vector<ref<const Parameter> >& parameters)
{
    be_forceuse(parameters);
    be_unimplemented();
}

bool OverloadMatch::operator<(const OverloadMatch& other) const
{
    return m_cost < other.m_cost;
}

RTTI::Value OverloadMatch::create(istring name,
                                  const minitl::vector<ref<const Parameter> >& parameters) const
{
    be_forceuse(name);
    be_forceuse(parameters);
    be_unimplemented();
    return RTTI::Value();
}

}}}
