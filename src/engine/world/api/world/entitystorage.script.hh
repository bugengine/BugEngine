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
    struct ComponentGroup;
    struct EntityInfo;
    struct ComponentIndex;
    struct ComponentStorage;
    typedef minitl::pair< raw<const RTTI::Class>, ComponentIndex > ComponentInfo;
protected:
    struct WorldComposition
    {
        minitl::array< minitl::pair< raw<const RTTI::Class>, u32 > >    components;
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
    EntityInfo**                    m_entityInfoBuffer;
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
    ComponentIndex indexOf(raw<const RTTI::Class> componentType) const;

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
