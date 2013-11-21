/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_WORLD_ENTITYSTORAGE_SCRIPT_HH_
#define BE_WORLD_ENTITYSTORAGE_SCRIPT_HH_
/*****************************************************************************/
#include    <world/stdafx.h>
#include    <world/entity.script.hh>
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
    struct Bucket
    {
        u64 acceptMask;
        u64 rejectMask;
        u32* componentCounts;

        Bucket();
        Bucket(u32 componentCount, u64 acceptMask, u64 rejectMask);
        ~Bucket();
    };
    struct ComponentGroup
    {
        u64 componentMask;
        minitl::array<EntityStorage::Bucket> buckets;

        ComponentGroup(u64 mask);
        ~ComponentGroup();
    };
    struct EntityInfo;
private:
    scoped<Task::ITask>                         m_task;
    u32                                         m_freeEntityId;
    SystemAllocator                             m_entityAllocator;
    EntityInfo**                                m_entityInfoBuffer;
    u32                                         m_entityCount;
    u32                                         m_entityBufferCount;
    const u32                                   m_maxEntityBufferCount;
    const u32                                   m_bufferCapacity;
    minitl::array< raw<const RTTI::Class> >     m_componentTypes;
    minitl::vector<ComponentGroup>              m_componentGroups;
private:
    void start();
    EntityInfo& getEntityInfo(Entity e);
    const EntityInfo& getEntityInfo(Entity e) const;
    ComponentGroup& getComponentGroup(u32 index);
    void registerType(raw<const RTTI::Class> componentType, u32 index, u32 maximumCount);
private: // friend World
    Entity spawn();
    void unspawn(Entity e);
    void addComponent(Entity e, const Component& c, raw<const RTTI::Class> componentType);
    void removeComponent(Entity e, raw<const RTTI::Class> componentType);
    bool hasComponent(Entity e, raw<const RTTI::Class> componentType) const;
protected:
    struct WorldComposition
    {
        struct Partition
        {
            minitl::array< raw<const RTTI::Class> > components;
        };

        minitl::array< minitl::pair< raw<const RTTI::Class>, u32 > >    components;
        minitl::vector< Partition >                                     partitions;

        WorldComposition(u32 componentCount)
            :   components(Arena::temporary(), componentCount)
            ,   partitions(Arena::temporary())
        {
        }
    };
protected:
    EntityStorage(const WorldComposition& composition);
    ~EntityStorage();

    weak<Task::ITask>   initialTask() const;

    u32 indexOf(raw<const RTTI::Class> componentType) const;
};

}}


/*****************************************************************************/
#endif
