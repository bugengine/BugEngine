/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <package/stdafx.h>
#include    <package/nodes/parameter.hh>
#include    <package/nodes/value.hh>

#include    <rtti/value.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

Parameter::Parameter(istring name, ref<Value> value)
    :   m_name(name)
    ,   m_value(value)
{
}

Parameter::~Parameter()
{
}

istring Parameter::name() const
{
    return m_name;
}

bool Parameter::isCompatible(const RTTI::Type& type) const
{
    return m_value->isCompatible(type);
}

RTTI::Value Parameter::as(const RTTI::Type& type) const
{
    return m_value->as(type);
}

}}}
