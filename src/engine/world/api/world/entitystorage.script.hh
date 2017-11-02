/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_WORLD_ENTITYSTORAGE_SCRIPT_HH_
#define BE_WORLD_ENTITYSTORAGE_SCRIPT_HH_
/**************************************************************************************************/
#include    <world/stdafx.h>
#include    <world/entity.script.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <scheduler/task/itask.hh>
#include    <core/memory/allocators/system.hh>

namespace BugEngine { namespace World
{

class World;
class ComponentGroup;
class MemoryHost;
struct Bucket;
struct Component;
struct EntityInfo;

class be_api(WORLD) EntityStorage : public minitl::refcountable
{
    friend class World;
    friend class ComponentGroup;
    friend struct TryMergeOperation;
private:
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
        SystemAllocator&    allocator;
        u32                 elementCount;
        const u32           componentSize;
        const u32           elementsPerPage;
        byte*               pages[1];

        ComponentStorage(SystemAllocator& allocator, u32 componentSize);
        ~ComponentStorage();
        byte* getElement(u32 index) const;
        void reserve(i32 delta);
        void shrink(i32 delta);
    private:
        static const u32    s_maxPageCount;
    private:
        ComponentStorage(const ComponentStorage& other);
        ComponentStorage& operator=(const ComponentStorage& other);
    };

    typedef minitl::tuple< raw<const RTTI::Class>, ComponentIndex, u32 > ComponentInfo;
protected:
    struct WorldComposition
    {
        typedef minitl::array< minitl::tuple< raw<const RTTI::Class>, u32 > > ComponentArray;
        typedef minitl::vector< minitl::array< raw<const RTTI::Class> > > PartitionVector;
        ComponentArray components;
        PartitionVector partitions;

        WorldComposition(u32 componentCount)
            :   components(Arena::temporary(), componentCount)
            ,   partitions(Arena::temporary())
        {
        }
    };
private:
    scoped<Task::ITask>                 m_task;
    u32                                 m_freeEntityId;
    SystemAllocator                     m_allocator4k;
    SystemAllocator                     m_allocator16k;
    SystemAllocator                     m_allocator64k;
    SystemAllocator                     m_allocator256k;
    u8**                                m_entityInfoBuffer;
    u32                                 m_entityCount;
    u32                                 m_entityBufferCount;
    const u32                           m_maxEntityBufferCount;
    const u32                           m_bufferCapacity;
    minitl::array<ComponentInfo>        m_componentTypes;
    minitl::vector<ComponentGroup>      m_componentGroups;
    minitl::vector<ComponentStorage*>   m_componentBackLinks;
    minitl::array<ComponentStorage*>    m_components;
    scoped<MemoryHost>                  m_segmentsMemoryHost;

private:
    void start();
    inline u32 getEntityInfoSize() const;
    EntityInfo& getRawEntityInfo(Entity e);
    const EntityInfo& getEntityInfo(Entity e) const;
    ComponentGroup& getComponentGroup(ComponentIndex index);

    void buildGroups(const WorldComposition& composition);
    u32 buildMask(const minitl::array< raw<const RTTI::Class> >& componentList);
    void registerType(raw<const RTTI::Class> componentType, u32 group, u32 index,
                      u32 totalIndex, u32 pageSize);

private: // friend World/ComponentGroup
    Entity spawn();
    void unspawn(Entity e);
    EntityInfo& getEntityInfo(Entity e);
    void addComponent(Entity e, const Component& c, raw<const RTTI::Class> componentType);
    void removeComponent(Entity e, raw<const RTTI::Class> componentType);
    bool hasComponent(Entity e, raw<const RTTI::Class> componentType) const;
    RTTI::Value getComponent(Entity e, raw<const RTTI::Class> componentType) const;
    ComponentIndex getComponentIndex(raw<const RTTI::Class> componentType) const;
    const ComponentInfo& getComponentInfo(raw<const RTTI::Class> componentType) const;
    void copyComponent(Entity e, u32 componentAbsoluteIndex, byte target[]) const;

private:
    EntityStorage(const EntityStorage& other);
    EntityStorage& operator=(const EntityStorage& other);
protected:
    EntityStorage(const WorldComposition& composition);
    ~EntityStorage();

    weak<Task::ITask>   initialTask() const;
};

}}


/**************************************************************************************************/
#endif
