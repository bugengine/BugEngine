/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
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
Storage<COMPONENT>::Storage()
    :   m_allocator(COMPONENT::ReservedSize)
    ,   m_components((COMPONENT*)m_allocator.buffer())
    ,   m_componentCount(0)
    ,   m_lastComponentFreed(s_invalidComponent)
{
}

template< typename COMPONENT >
Storage<COMPONENT>::~Storage()
{
    be_assert(m_lastComponentFreed == s_invalidComponent, "destroyed storage<%s> still had components" | typeid(COMPONENT).name());
    be_assert(m_componentCount == 0, "destroyed storage<%s> still had components" | typeid(COMPONENT).name());
}

template< typename COMPONENT >
COMPONENT& Storage<COMPONENT>::create(u32 index, const COMPONENT& templateComponent)
{
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

    m_allocator.setUsage(be_checked_numcast<u32>(m_componentCount*be_align(sizeof(COMPONENT), be_alignof(COMPONENT))));
    COMPONENT& component = operator[](index);
    component = templateComponent;
    component.created();
    return component;
}

template< typename COMPONENT >
void Storage<COMPONENT>::move(u32 component1, u32 component2)
{
    be_assert(
            (m_lastComponentFreed == s_invalidComponent && component2 == m_componentCount)
          ||(m_lastComponentFreed != s_invalidComponent && component2 == m_lastComponentFreed), "component %d is invalid");
    if (m_lastComponentFreed == s_invalidComponent)
    {
        ++m_componentCount;
        m_allocator.setUsage(be_checked_numcast<u32>(m_componentCount*be_align(sizeof(COMPONENT), be_alignof(COMPONENT))));
    }
    m_lastComponentFreed = component1;

    operator[](component2) = operator[](component1);

    if (m_lastComponentFreed == m_componentCount-1)
    {
        m_lastComponentFreed = s_invalidComponent;
        --m_componentCount;
        m_allocator.setUsage(be_checked_numcast<u32>(m_componentCount*be_align(sizeof(COMPONENT), be_alignof(COMPONENT))));
    }
}

template< typename COMPONENT >
void Storage<COMPONENT>::destroy(u32 component)
{
    operator[](component).destroyed();

    if (component == m_componentCount-1)
    {
        --m_componentCount;
        m_allocator.setUsage(be_checked_numcast<u32>(m_componentCount*be_align(sizeof(COMPONENT), be_alignof(COMPONENT))));
    }
    else
    {
        be_assert(m_lastComponentFreed == s_invalidComponent, "cannot destroy two components in the middle of the ");
        m_lastComponentFreed = component;
    }
}

template< typename COMPONENT >
COMPONENT& Storage<COMPONENT>::operator[](const u32 component)
{
    be_assert(component < m_componentCount, "component %d is out of range: %d" | component | m_componentCount);
    return m_components[component];
}

}}

/*****************************************************************************/
#endif
