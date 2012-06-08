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
class Rule;
template< typename T >
class Storage;
struct Component;

class be_api(WORLD) World : public Resource
{
    friend class Rule;
    template< typename T >
    friend class Storage;
private:
    ref<ITask>                      m_task;
    minitl::array< scoped<Rule> >   m_rules;
    scoped<State>                   m_emptyEntityState;
    Entity                          m_freeEntityId;
    SystemAllocator                 m_entityAllocator;
    Entity*                         m_entityBuffer;
    u32                             m_entityCount;
    u32                             m_entityCapacity;
private:
    void addComponent(Entity e, const Component& component, raw<const RTTI::Class> metaclass);

    template< typename INPUT, typename OUTPUT >
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
