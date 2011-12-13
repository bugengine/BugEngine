/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <packagebuilder/stdafx.h>
#include    <packagebuilder/nodes/reference.hh>
#include    <packagebuilder/nodes/package.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

Reference::Reference(weak<Package> owner)
    :   m_owner(owner)
    ,   m_name("")
    ,   m_value()
{
    m_owner->addReference(this);
}

Reference::~Reference()
{
    unhook();
}

void Reference::setName(const inamespace& name)
{
    m_name = name;
    m_owner->resolveReference(this);
}

}}}
