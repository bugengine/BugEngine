/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_WORLD_ENTITYSTORAGE_SCRIPT_HH_
#define BE_WORLD_ENTITYSTORAGE_SCRIPT_HH_
/*****************************************************************************/
#include    <world/stdafx.h>
#include    <world/entity.script.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <scheduler/task/itask.hh>
#include    <core/memory/allocators/system.hh>

namespace BugEngine { namespace World
{

class World;
struct Component;

class be_api(WORLD) EntityStorage : public minitl::refcountable
{
    friend class World;
private:
    struct EntityInfo;
    struct ComponentGroup
    {
        struct Bucket
        {
            u32* componentCounts;
            u32 acceptMask;
            u32 maskSize;

            Bucket();
            Bucket(u32* componentOffsets, u32 acceptMask);
        };
        struct ComponentInfo
        {
            raw<const RTTI::Class> componentType;
            u32 size;
            raw<const RTTI::Method::Overload> created;
            raw<const RTTI::Method::Overload> destroyed;
        };
        typedef minitl::tuple<Bucket*, Bucket*> BucketPair;

        minitl::array<Bucket> buckets;
        minitl::array<ComponentInfo> components;
        u32 firstComponent;
        u32 lastComponent;
        u32 componentsTotalSize;
        u32* componentCounts;
        u8* componentOperations;
        i_u32 operationOffset;

        ComponentGroup(u32 firstComponent, u32 componentCount, u32 componentsTotalSize,
                       u32* componentOffsets, const minitl::vector<u32>& bucketMasks,
                       u8* operationBuffer);
        ~ComponentGroup();
        BucketPair findBuckets(u32 mask1, u32 mask2);
        void runEntityOperations(u8* buffer);
        void mergeEntityOperation(u8* source, const u8* merge);
    };
    struct ComponentIndex
    {
        u16 group;
        u8  relativeIndex;
        u8  absoluteIndex;

        ComponentIndex();
        ComponentIndex(u32 group, u32 relativeIndex, u32 absoluteIndex);
        operator void*() const;
        bool operator!() const;
    };
    struct ComponentStorage
    {
        u8* memory;
        u32* backLink;
        u32 current;
        u32 maximum;
        u32 elementSize;
    };
    typedef minitl::tuple<
        raw<const RTTI::Class>,
        ComponentIndex > ComponentInfo;
protected:
    struct WorldComposition
    {
        minitl::array< minitl::tuple< raw<const RTTI::Class>, u32 > >    components;
        minitl::vector< minitl::array< raw<const RTTI::Class> > >       partitions;

        WorldComposition(u32 componentCount)
            :   components(Arena::temporary(), componentCount)
            ,   partitions(Arena::temporary())
        {
        }
    };
private:
    scoped<Task::ITask>             m_task;
    u32                             m_freeEntityId;
    SystemAllocator                 m_entityAllocator;
    SystemAllocator*                m_operationAllocator;
    u8**                            m_entityInfoBuffer;
    u32                             m_entityCount;
    u32                             m_entityBufferCount;
    const u32                       m_maxEntityBufferCount;
    const u32                       m_bufferCapacity;
    minitl::array<ComponentInfo>    m_componentTypes;
    minitl::vector<ComponentGroup>  m_componentGroups;
    minitl::vector<u32*>            m_componentCountsList;
    minitl::array<ComponentStorage> m_components;
private:
    void start();
    inline u32 getEntityInfoSize() const;
    EntityInfo& getEntityInfo(Entity e);
    const EntityInfo& getEntityInfo(Entity e) const;
    ComponentGroup& getComponentGroup(ComponentIndex index);

    void buildGroups(const WorldComposition& composition);
    void registerType(raw<const RTTI::Class> componentType, u32 group, u32 index, u32 totalIndex, u32 maximumCount);

private: // friend World
    Entity spawn();
    void unspawn(Entity e);
    void addComponent(Entity e, const Component& c, raw<const RTTI::Class> componentType);
    void removeComponent(Entity e, raw<const RTTI::Class> componentType);
    bool hasComponent(Entity e, raw<const RTTI::Class> componentType) const;
    RTTI::Value getComponent(Entity e, raw<const RTTI::Class> componentType) const;
    ComponentIndex getComponentIndex(raw<const RTTI::Class> componentType) const;
    const ComponentInfo& getComponentInfo(raw<const RTTI::Class> componentType) const;

protected:
    EntityStorage(const WorldComposition& composition);
    ~EntityStorage();

    weak<Task::ITask>   initialTask() const;

public:
    u32 buildMask(const minitl::array< raw<const RTTI::Class> >& componentList);
};

}}


/*****************************************************************************/
#endif
