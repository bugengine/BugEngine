/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_WORLD_COMPONENTGROUP_HH_
#define BE_WORLD_COMPONENTGROUP_HH_
/**************************************************************************************************/
#include    <world/stdafx.h>
#include    <world/entity.script.hh>
#include    <world/component.script.hh>
#include    <componentbucket.hh>
#include    <core/memory/allocators/system.hh>
#include    <core/threads/mutex.hh>
#include    <minitl/array.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <kernel/interlocked_stack.hh>

namespace BugEngine { namespace World
{

class EntityStorage;

class ComponentGroup
{
private:
    struct OperationBuffer
    {
        iptr<OperationBuffer>   m_next;
        i_u32                   m_offset;
        i_u32                   m_used;
        byte                    m_data[1];
        OperationBuffer()
            :   m_next(0)
            ,   m_offset(i_u32::Zero)
            ,   m_used(i_u32::Zero)
            ,   m_data()
        {
        }
    };

    struct ComponentInfo
    {
        raw<const RTTI::Class> componentType;
        raw<const RTTI::Method::Overload> created;
        raw<const RTTI::Method::Overload> destroyed;
        u32 size;
    };
    typedef minitl::tuple<Bucket*, Bucket*> BucketPair;

    SystemAllocator&                m_allocator;
    minitl::array<Bucket>           m_buckets;
    minitl::array<ComponentInfo>    m_components;
    u32                             m_componentsTotalSize;
    u32*                            m_componentCounts;
    u32*                            m_backBuffer;
    iptr<OperationBuffer>           m_entityOperation;
public:
    u32 const                       firstComponent;
    u32 const                       lastComponent;
private:
    byte* allocOperation(u32 componentSize);
    void packComponents(u32 mask, const byte source[], byte target[], const u32 offset[]) const;
    void unpackComponents(u32 mask, const byte source[], byte target[], const u32 offset[]) const;
    void groupEntityOperations();
    BucketPair findBuckets(u32 mask1, u32 mask2);
public:
    ComponentGroup(u32 firstComponent,
                   const minitl::vector< raw<const RTTI::Class> >& componentTypes,
                   const minitl::vector<u32>& bucketMasks,
                   SystemAllocator& allocator);
    ~ComponentGroup();

    void freeBuffers();
    void runEntityOperations(weak<EntityStorage> storage);

    void addComponent(Entity e, u32 originalMask, const Component& c, u32 componentIndex);
    void removeComponent(Entity e, u32 originalMask, u32 componentIndex);
};

}}


/**************************************************************************************************/
#endif
