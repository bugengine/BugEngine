/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <world/stdafx.h>
#include    <world/rule.script.hh>
#include    <world/world.script.hh>

namespace BugEngine { namespace World
{

Rule::Rule(weak<World> world)
    :   m_world(world)
{
    m_world->addRule(this);
}

Rule::~Rule()
{
    m_world->removeRule(this);
}

}}

