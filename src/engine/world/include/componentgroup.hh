/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_WORLD_COMPONENTGROUP_HH_
#define BE_WORLD_COMPONENTGROUP_HH_
/*****************************************************************************/
#include    <world/stdafx.h>
#include    <world/entity.script.hh>
#include    <world/component.script.hh>
#include    <core/memory/allocators/system.hh>
#include    <minitl/array.hh>
#include    <rtti/engine/methodinfo.script.hh>

namespace BugEngine { namespace World
{

class EntityStorage;

struct Bucket
{
    u32* componentCounts;
    u32 acceptMask;
    u32 maskSize;
    u32 firstComponent;

    Bucket();
    Bucket(u32* componentCounts, u32 acceptMask);
};

class ComponentGroup
{
private:
    struct ComponentInfo
    {
        raw<const RTTI::Class> componentType;
        u32 size;
        raw<const RTTI::Method::Overload> created;
        raw<const RTTI::Method::Overload> destroyed;
    };
    struct Delta;
    struct Offset;
    typedef minitl::tuple<Bucket*, Bucket*> BucketPair;
    
    SystemAllocator&                m_operationAllocator;
    minitl::array<Bucket>           m_buckets;
    minitl::array<ComponentInfo>    m_components;
    u32                             m_componentsTotalSize;
    u32*                            m_componentCounts;
    u32*                            m_backBuffer;
    u32                             m_offset;
    u8*                             m_componentOperations;
    i_u32                           m_operationOffset;
public:
    u32 const                       firstComponent;
    u32 const                       lastComponent;
public:
    ComponentGroup(u32 firstComponent, const minitl::vector< raw<const RTTI::Class> >& componentTypes,
                   const minitl::vector<u32>& bucketMasks, SystemAllocator& allocator);
    ~ComponentGroup();

    void freeBuffers();
    BucketPair findBuckets(u32 mask1, u32 mask2);
    void runEntityOperations(weak<EntityStorage> storage, u8* buffer, u8* componentBuffer);
    void mergeEntityOperation(u8* source, const u8* merge);
    void moveComponents(u32 componentIndex, Bucket* first, Bucket* last, u8* operations,
                        Offset* operationOffsetPerBucket, Delta* deltas,
                        const u8* componentBuffer);
    void moveBucketComponents(u32 componentIndex, Bucket* bucket,
                              u8* operationsRemove, u8* operationsAdd, u8* operationsEnd,
                              const u8* componentBuffer,
                              i32 delta);

    void addComponent(Entity e, u32 originalMask, const Component& c, u32 componentIndex);
    void removeComponent(Entity e, u32 originalMask, u32 componentIndex);
};

}}


/*****************************************************************************/
#endif