/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <packagebuilder/nodes/package.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

Package::Package()
    :   m_nodes(gameArena())
{
}

Package::~Package()
{
}

}}}
