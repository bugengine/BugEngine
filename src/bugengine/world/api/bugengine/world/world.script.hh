/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_WORLD_WORLD_SCRIPT_HH_
#define BE_WORLD_WORLD_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/world/stdafx.h>
#include <bugengine/core/memory/allocators/system.hh>
#include <bugengine/resource/description.script.hh>
#include <bugengine/rtti/typeinfo.hh>
#include <bugengine/scheduler/kernel/iproduct.script.hh>
#include <bugengine/scheduler/task/group.hh>
#include <bugengine/world/component/storageconfiguration.script.hh>
#include <bugengine/world/entity/entity.script.hh>

namespace BugEngine { namespace World {

class be_api(WORLD) World : public Resource::Description
{
private:
    ref< Task::TaskGroup >                              m_task;
    Task::TaskGroup::TaskStartConnection                m_taskStart;
    Task::TaskGroup::TaskEndConnection                  m_taskEnd;
    minitl::array< Task::TaskGroup::TaskEndConnection > m_productEnds;

private:
    void addComponent(Entity e, const void* component, raw< const RTTI::Class > metaclass);
    void update();

public:
    weak< Task::ITask > updateWorldTask() const;
    template < typename T >
    void addComponent(Entity e, const T& component)
    {
        addComponent(e, &component, be_class< T >());
    }
published:
    Entity spawn();
    void   unspawn(Entity e);

    void        addComponent(Entity e, const RTTI::Value& v);
    void        removeComponent(Entity e, raw< const RTTI::Class > metaclass);
    bool        hasComponent(Entity e, raw< const RTTI::Class > metaclass) const;
    RTTI::Value getComponent(Entity e, raw< const RTTI::Class > metaclass) const;
published:
    World(ref< const Component::StorageConfiguration >             configuration,
          minitl::array< weak< const KernelScheduler::IProduct > > products);
    ~World();
};

}}  // namespace BugEngine::World

/**************************************************************************************************/
#endif
