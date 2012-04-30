/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <world/stdafx.h>
#include    <blockmanager.hh>

namespace BugEngine { namespace World
{

BlockManager::BlockManager()
:   m_pool256k(0)
,   m_pool128k(0)
,   m_pool64k(0)
,   m_pool32k(0)
,   m_pool16k(0)
,   m_usedBlocks(0)
{
}

BlockManager::~BlockManager()
{
    be_assert(m_usedBlocks == 0, "not all memory reclaimed when the block manager was destroyed");
}

}}

