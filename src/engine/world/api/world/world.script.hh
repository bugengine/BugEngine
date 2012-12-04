/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_WORLD_WORLD_SCRIPT_HH_
#define BE_WORLD_WORLD_SCRIPT_HH_
/*****************************************************************************/
#include    <scheduler/task/group.hh>
#include    <scheduler/kernel/iproduct.script.hh>
#include    <resource/description.script.hh>
#include    <world/entity.script.hh>
#include    <world/entitystorage.script.hh>
#include    <core/memory/allocators/system.hh>



namespace BugEngine { namespace World
{

class Rule;
class EntityStorage;
struct Component;

class be_api(WORLD) World : public Resource::Description
{
    friend class Rule;
private:
    ref<Task::TaskGroup>                                m_task;
    weak<EntityStorage> const                           m_storage;
    Task::TaskGroup::TaskStartConnection                m_taskStart;
    Task::TaskGroup::TaskEndConnection                  m_taskEnd;
    minitl::array<Task::TaskGroup::TaskEndConnection>   m_productEnds;
    Entity                                              m_freeEntityId;
    SystemAllocator                                     m_entityAllocator;
    Entity*                                             m_entityBuffer;
    u32                                                 m_entityCount;
    u32                                                 m_entityCapacity;
private:
    void addComponent(Entity e, const Component& component, raw<const RTTI::Class> metaclass);

    template< typename INPUT, typename OUTPUT >
    void addRule(weak<const Rule> rule);
    void removeRule(weak<const Rule> rule);
public:
    weak<Task::ITask> updateWorldTask() const;
published:
    Entity spawn();
    void unspawn(Entity e);

    void addComponent(Entity e, RTTI::Value& v);
published:
    World(weak<EntityStorage> storage, minitl::array<Kernel::IProduct> products);
    ~World();
};

}}


/*****************************************************************************/
#endif
