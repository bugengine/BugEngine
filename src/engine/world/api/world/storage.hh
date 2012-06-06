/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_WORLD_STORAGE_HH_
#define BE_WORLD_STORAGE_HH_
/*****************************************************************************/

namespace BugEngine { namespace World
{

class IStorage : public minitl::pointer
{
public:
    virtual ~IStorage();
    virtual Component& create(u32 index, const Component& templateComponent) = 0;
    virtual void destroy(u32 index) = 0;
    virtual void move(u32 index1, u32 index2) = 0;
    virtual Component& operator[](u32 index) = 0;
};

template< typename COMPONENT >
class Storage : public IStorage
{
private:
    minitl::vector<byte*>   m_pointers;
    SystemAllocator&        m_allocator;
    u32                     m_componentCount;
    u32                     m_lastComponentFreed;
public:
    Storage(weak<World> world);
    ~Storage();

    virtual COMPONENT& create(u32 index, const Component& templateComponent) override;
    virtual void destroy(u32 index) override;
    virtual void move(u32 index1, u32 index2) override;
    virtual COMPONENT& operator[](u32 index) override;
};


}}

#include <world/storage.inl>

/*****************************************************************************/
#endif
