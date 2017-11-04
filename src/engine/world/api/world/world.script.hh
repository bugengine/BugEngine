/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_WORLD_WORLD_SCRIPT_HH_
#define BE_WORLD_WORLD_SCRIPT_HH_
/**************************************************************************************************/
#include    <world/stdafx.h>
#include    <scheduler/task/group.hh>
#include    <scheduler/kernel/iproduct.script.hh>
#include    <resource/description.script.hh>
#include    <world/entity.script.hh>
#include    <world/entitystorage.script.hh>
#include    <core/memory/allocators/system.hh>
#include    <rtti/typeinfo.hh>

namespace BugEngine { namespace World
{

class EntityStorage;
struct Component;

class be_api(WORLD) World : public Resource::Description
{
private:
    ref<Task::TaskGroup>                                m_task;
    weak<EntityStorage> const                           m_storage;
    Task::TaskGroup::TaskStartConnection                m_taskStart;
    Task::TaskGroup::TaskEndConnection                  m_taskEnd;
    minitl::array<Task::TaskGroup::TaskEndConnection>   m_productEnds;
private:
    void addComponent(Entity e, const Component& component, raw<const RTTI::Class> metaclass);
public:
    weak<Task::ITask> updateWorldTask() const;
    template< typename T > void addComponent(Entity e, const T& component)
    {
        addComponent(e, component, be_typeid<T>::klass());
    }
published:
    Entity spawn();
    void unspawn(Entity e);

    void addComponent(Entity e, const RTTI::Value& v);
    void removeComponent(Entity e, raw<const RTTI::Class> metaclass);
    bool hasComponent(Entity e, raw<const RTTI::Class> metaclass) const;
    RTTI::Value getComponent(Entity e, raw<const RTTI::Class> metaclass) const;
published:
    World(weak<EntityStorage> storage, minitl::array< weak<const Kernel::IProduct> > products);
    ~World();
};

}}


/**************************************************************************************************/
#endif
