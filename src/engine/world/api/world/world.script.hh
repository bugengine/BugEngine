/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_WORLD_WORLD_SCRIPT_HH_
#define BE_WORLD_WORLD_SCRIPT_HH_
/*****************************************************************************/
#include    <system/scheduler/task/itask.hh>
#include    <system/resource/resource.script.hh>
#include    <world/entity.script.hh>
#include    <core/memory/allocators/system.hh>
#include    <system/scheduler/task/group.hh>


namespace BugEngine { namespace World
{

class State;
class Storage;
class Rule;
struct Component;

class be_api(WORLD) World : public Resource
{
    friend class Storage;
    friend class Rule;
    struct RuleConnection;
    friend struct RuleConnection;
private:
    struct RuleConnection
    {
        weak<World> const                                   world;
        weak<const Rule> const                              rule;
        TaskGroup::TaskStartConnection const                start;
        TaskGroup::TaskEndConnection const                  end;
        minitl::vector< ITask::CallbackConnection > const   dependencies;

        RuleConnection(weak<World> world, weak<const Rule> rule);
        ~RuleConnection();
    };
private:
    ref<ITask>                          m_task;
    minitl::vector< RuleConnection >    m_rules;
    scoped<State>                       m_emptyEntityState;
    Entity                              m_freeEntityId;
    SystemAllocator                     m_allocator16k;
    SystemAllocator                     m_allocator64k;
    minitl::vector<Entity*>             m_entityBuffers;
    i_u32                               m_entityCount;
    u16                                 m_worldIndex;
private:
    void addComponent(Entity e, const Component& component, raw<const RTTI::Class> metaclass);
    void addRule(weak<const Rule> rule);
    void removeRule(weak<const Rule> rule);
public:
    weak<ITask> updateWorldTask() const;
published:
    Entity spawn();
    void unspawn(Entity e);

    void addComponent(Entity e, RTTI::Value& v);
published:
    World();
    ~World();
};

}}


/*****************************************************************************/
#endif
