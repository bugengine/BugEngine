/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_WORLD_STORAGE_HH_
#define BE_WORLD_STORAGE_HH_
/*****************************************************************************/

namespace BugEngine { namespace World
{


class Storage
{
private:
    minitl::vector<byte*>   m_pointers;
    SystemAllocator&        m_allocator;
    raw<const RTTI::Class>  m_componentClass;
    u32 const               m_componentSize;
    u32 const               m_componentsPerBlock;
    u32                     m_last;
public:
    Storage(weak<World> world, raw<const RTTI::Class> componentClass);
    ~Storage();

    Component& add(const Component& templateComponent);
    void swap(u32 index1, u32 index2);
    void pop();
    Component& operator[](u32 index);
};


}}


/*****************************************************************************/
#endif

