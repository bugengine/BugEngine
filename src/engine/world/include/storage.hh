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
    minitl::vector<void*>   m_pointers;
    u32                     m_last;
public:
    Storage(weak<World> world, weak<const RTTI::Class> componentClass);
    ~Storage();

    u32  insert(Component& component);
    void swap(u32 index1, u32 index2);
    void pop();
    Component& operator[](u32 index);
};


}}


/*****************************************************************************/
#endif

