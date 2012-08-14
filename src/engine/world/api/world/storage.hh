/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_WORLD_STORAGE_HH_
#define BE_WORLD_STORAGE_HH_
/*****************************************************************************/
#include    <core/memory/allocators/system.hh>

namespace BugEngine { namespace World
{

template< typename COMPONENT >
class Storage : public Kernel::Stream<COMPONENT>
{
private:
    SystemAllocator         m_allocator;
    COMPONENT*              m_components;
    u32                     m_componentCount;
    u32                     m_lastComponentFreed;
public:
    Storage();
    ~Storage();

    COMPONENT& create(u32 index, const COMPONENT& templateComponent);
    void destroy(u32 index);
    void move(u32 index1, u32 index2);
    COMPONENT& operator[](u32 index);
};


}}

#include <world/storage.inl>

/*****************************************************************************/
#endif
