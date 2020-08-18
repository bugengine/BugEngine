/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_WORLD_COMPONENTGROUP_HH_
#define BE_WORLD_COMPONENTGROUP_HH_
/**************************************************************************************************/
#include <bugengine/world/stdafx.h>
#include <bugengine/core/memory/allocators/system.hh>
#include <bugengine/core/threads/mutex.hh>
#include <bugengine/kernel/interlocked_stack.hh>
#include <bugengine/minitl/array.hh>
#include <bugengine/rtti/engine/methodinfo.script.hh>
#include <bugengine/world/component.script.hh>
#include <bugengine/world/entity.script.hh>
#include <bugengine/world/entitystorage.script.hh>
#include <componentbucket.hh>

namespace BugEngine { namespace World {

class ComponentGroup
{
    friend class EntityStorage;
    friend struct MergeOperation;
    friend struct TryMergeOperation;

private:
    struct EntityOperation;
    struct EntityOperationRemove;
    struct EntityOperationRemoveIterator;
    struct OperationBuffer;
    struct OperationDelta;
    struct EntityOperationCompare;
    struct ComponentInfo
    {
        raw< const RTTI::Class >            componentType;
        raw< const RTTI::Method::Overload > created;
        raw< const RTTI::Method::Overload > destroyed;
        u32                                 size;
    };
    typedef minitl::tuple< Bucket*, Bucket* > BucketPair;

    SystemAllocator&               m_allocator;
    minitl::array< Bucket >        m_buckets;
    minitl::array< ComponentInfo > m_components;
    u32                            m_componentsTotalSize;
    OperationBuffer*               m_entityOperation;
    iptr< OperationBuffer >        m_entityOperationCurrent;

public:
    u32 const firstComponent;
    u32 const lastComponent;
    u32 const index;

private:
    void  clearBuffers(OperationBuffer* head) const;
    byte* allocOperation(u32 componentSize);

    void packComponents(u32 mask, const byte source[], byte target[], const u32 offset[]) const;
    void unpackComponents(u32 mask, const byte source[], byte target[], const u32 offset[]) const;
    void copyComponents(weak< EntityStorage > storage, u32 owner, byte target[], u32 mask,
                        const u32 offset[]) const;

    void             groupEntityOperations(weak< EntityStorage > storage, OperationDelta deltas[]);
    OperationBuffer* sortEntityOperations(OperationDelta deltas[]);
    void executeEntityOperations(weak< EntityStorage > storage, const OperationDelta deltas[]);
    void repack(weak< EntityStorage > storage, u32 componentIndex, Bucket& bucket, u32 firstEntity,
                u32 firstEntityStorage, OperationDelta operations, i32 offset) const;
    void runEntityOperations(weak< EntityStorage > storage, OperationDelta deltas[]);
    BucketPair findBuckets(u32 mask1, u32 mask2);

public:
    ComponentGroup(u32 index, u32 firstComponent,
                   const minitl::vector< raw< const RTTI::Class > >& componentTypes,
                   const minitl::vector< u32 >& bucketMasks, SystemAllocator& allocator);
    ~ComponentGroup();

    void freeBuffers();
    void runEntityOperations(weak< EntityStorage > storage);

    void addComponent(Entity e, u32 originalMask, const void* c, u32 componentIndex);
    void removeComponent(Entity e, u32 originalMask, u32 componentIndex);
};

}}  // namespace BugEngine::World

/**************************************************************************************************/
#endif
