/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <world/stdafx.h>
#include    <state.hh>

namespace BugEngine { namespace World
{

State::State()
    :   m_groups(Arena::game(), 0)
    ,   m_storage(Arena::game(), 0)
{
}

State::~State()
{
}

}}

