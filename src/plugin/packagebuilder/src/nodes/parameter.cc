/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <packagebuilder/stdafx.h>
#include    <packagebuilder/nodes/parameter.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

Parameter::Parameter(istring name)
    :   m_name(name)
{
}

Parameter::~Parameter()
{
}

}}}
