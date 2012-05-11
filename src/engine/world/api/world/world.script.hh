/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_WORLD_WORLD_SCRIPT_HH_
#define BE_WORLD_WORLD_SCRIPT_HH_
/*****************************************************************************/
#include    <system/scheduler/task/itask.hh>
#include    <system/resource/resource.script.hh>
#include    <world/entity.script.hh>
#include    <core/memory/allocators/system.hh>


namespace BugEngine { namespace World
{

class State;
class Storage;
struct Component;

class be_api(WORLD) World : public Resource
{
    friend class Storage;
private:
    ref<ITask>              m_task;
    scoped<State>           m_emptyEntityState;
    Entity                  m_freeEntityId;
    SystemAllocator         m_allocator16k;
    SystemAllocator         m_allocator64k;
    minitl::vector<Entity*> m_entityBuffers;
    i_u32                   m_entityCount;
public:
    weak<ITask> updateWorldTask() const;
public:
    void addComponent(Entity e, const Component& component, raw<const RTTI::Class> metaclass);
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
