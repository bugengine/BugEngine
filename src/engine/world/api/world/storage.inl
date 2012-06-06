/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_WORLD_STORAGE_INL_
#define BE_WORLD_STORAGE_INL_
/*****************************************************************************/

#include    <world/stdafx.h>
#include    <world/storage.hh>
#include    <world/world.script.hh>


namespace BugEngine { namespace World
{

static const u32 s_invalidComponent = ~0u;

template< typename COMPONENT >
Storage<COMPONENT>::Storage(weak<World> world)
    :   m_pointers(Arena::game())
    ,   m_allocator(world->m_allocator16k)
    ,   m_componentCount(0)
    ,   m_lastComponentFreed(s_invalidComponent)
{
}

template< typename COMPONENT >
Storage<COMPONENT>::~Storage()
{
#if BE_ENABLE_COMPONENT_DEBUGGING
    be_assert(m_lastComponentFreed == s_invalidComponent, "destroyed storage<%s> still had components" | typeid(COMPONENT).name());
    be_assert(m_componentCount == 0, "destroyed storage<%s> still had components" | typeid(COMPONENT).name());
#endif
    for (minitl::vector<byte*>::iterator it = m_pointers.begin(); it != m_pointers.end(); ++it)
    {
        m_allocator.blockFree(*it);
    }
}

template< typename COMPONENT >
COMPONENT& Storage<COMPONENT>::create(u32 index, const Component& templateComponent)
{
#if BE_ENABLE_COMPONENT_DEBUGGING
    be_assert(
            (m_lastComponentFreed == s_invalidComponent && index == m_componentCount)
          ||(m_lastComponentFreed != s_invalidComponent && index == m_lastComponentFreed), "component %d is invalid"|index);
    if (m_lastComponentFreed != s_invalidComponent)
    {
        m_lastComponentFreed = s_invalidComponent;
    }
    else
    {
        ++m_componentCount;
    }
#endif
    COMPONENT& component = operator[](index);
    component = static_cast<const COMPONENT&>(templateComponent);
    component.created();
    return component;
}

template< typename COMPONENT >
void Storage<COMPONENT>::move(u32 component1, u32 component2)
{
#if BE_ENABLE_COMPONENT_DEBUGGING
    be_assert(
            (m_lastComponentFreed == s_invalidComponent && component2 == m_componentCount)
          ||(m_lastComponentFreed != s_invalidComponent && component2 == m_lastComponentFreed), "component %d is invalid");
    if (m_lastComponentFreed == s_invalidComponent)
    {
        ++m_componentCount;
    }
    m_lastComponentFreed = component1;
#endif
    operator[](component2) = operator[](component1);
#if BE_ENABLE_COMPONENT_DEBUGGING
    if (m_lastComponentFreed == m_componentCount-1)
    {
        m_lastComponentFreed = s_invalidComponent;
        --m_componentCount;
    }
#endif
}

template< typename COMPONENT >
void Storage<COMPONENT>::destroy(u32 component)
{
    operator[](component).destroyed();
#if BE_ENABLE_COMPONENT_DEBUGGING
    if (component == m_componentCount-1)
    {
        --m_componentCount;
    }
    else
    {
        be_assert(m_lastComponentFreed == s_invalidComponent, "cannot destroy two components in the middle of the ");
        m_lastComponentFreed = component;
    }
#endif
}

template< typename COMPONENT >
COMPONENT& Storage<COMPONENT>::operator[](const u32 component)
{
#if BE_ENABLE_COMPONENT_DEBUGGING
    be_assert(component < m_componentCount, "component %d is out of range: %d" | component | m_componentCount);
#endif
    const u32 size = be_align(sizeof(COMPONENT), be_alignof(COMPONENT));
    const u32 componentsPerBlock = m_allocator.blockSize() / size;
    u32 blockIndex = component / componentsPerBlock;
    u32 componentIndex = component % componentsPerBlock;
    if (blockIndex >= m_pointers.size())
    {
        be_assert(component == m_componentCount - 1, "component %d is invalid" | component);
        be_assert(componentIndex == 0, "component %d is invalid" | component);
        be_assert(blockIndex == m_pointers.size(), "component %d is invalid" | component);
        m_pointers.push_back(m_allocator.blockAlloc());
    }
    byte* pool = m_pointers[blockIndex];
    byte* offset = pool + size * componentIndex;
    return *reinterpret_cast<COMPONENT*>(offset);
}

}}

/*****************************************************************************/
#endif
