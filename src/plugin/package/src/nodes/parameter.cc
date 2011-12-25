/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <package/stdafx.h>
#include    <package/nodes/parameter.hh>
#include    <package/nodes/value.hh>

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

TypeInfo Parameter::getType() const
{
    return m_value->getType();
}

}}}
