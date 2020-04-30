/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/plugin.scripting.package/stdafx.h>
#include <bugengine/plugin.scripting.package/nodes/parameter.hh>
#include <bugengine/plugin.scripting.package/nodes/value.hh>

#include <bugengine/rtti/value.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes {

Parameter::Parameter(istring name, ref< Value > value) : m_name(name), m_value(value)
{
}

Parameter::~Parameter()
{
}

istring Parameter::name() const
{
    return m_name;
}

RTTI::ConversionCost Parameter::calculateConversion(const RTTI::Type& type) const
{
    return m_value->calculateConversion(type);
}

RTTI::Value Parameter::as(const RTTI::Type& type) const
{
    return m_value->as(type);
}

}}}  // namespace BugEngine::PackageBuilder::Nodes
