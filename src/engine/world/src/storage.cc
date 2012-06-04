/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <world/stdafx.h>
#include    <storage.hh>

#include "world/world.script.hh"

namespace BugEngine { namespace World
{

Storage::Storage(weak<World> world, raw<const RTTI::Class> componentClass)
    :   m_pointers(Arena::game())
    ,   m_allocator(world->m_allocator16k)
    ,   m_componentClass(componentClass)
    ,   m_componentSize(be_align(m_componentClass->size, m_componentClass->align))
    ,   m_componentsPerBlock(m_allocator.blockSize() / m_componentSize)
    ,   m_last(0)
{
}

Storage::~Storage()
{
    for (minitl::vector<byte*>::iterator it = m_pointers.begin(); it != m_pointers.end(); ++it)
    {
        m_allocator.blockFree(*it);
    }
}

Component& Storage::add(const Component& templateComponent)
{
    u32 blockIndex = m_last / m_componentsPerBlock;
    u32 componentIndex = m_last % m_componentsPerBlock;

    if (m_pointers.size() <= blockIndex)
    {
        be_assert(m_pointers.size() == blockIndex, "block index %d is invalid" | blockIndex);
        m_pointers.push_back(m_allocator.blockAlloc());
    }
    be_assert(blockIndex < m_pointers.size(), "block index %d is not a valid block index" | blockIndex);
    byte* pool = m_pointers[blockIndex];
    byte* offset = pool + m_componentSize * componentIndex;
    (*(m_componentClass->copyconstructor))(&templateComponent, offset);
    return *reinterpret_cast<Component*>(offset);
}


void Storage::swap(u32 component1, u32 component2)
{
    Component& c1 = operator[](component1);
    Component& c2 = operator[](component2);
    Allocator::Block<byte> temp(Arena::stack(), m_componentClass->size, m_componentClass->align);
    (*(m_componentClass->copyconstructor))(&c1, temp.data());
    (*(m_componentClass->copyconstructor))(&c2, &c1);
    (*(m_componentClass->copyconstructor))(temp.data(), &c2);
}

Component& Storage::operator[](const u32 component)
{
    u32 blockIndex = component / m_componentsPerBlock;
    u32 componentIndex = component % m_componentsPerBlock;
    be_assert(blockIndex < m_pointers.size(), "block index %d is not a valid block index" | blockIndex);
    byte* pool = m_pointers[blockIndex];
    byte* offset = pool + m_componentSize * componentIndex;
    return *reinterpret_cast<Component*>(offset);
}

}}

