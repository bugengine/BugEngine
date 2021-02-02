/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/world/stdafx.h>
#include <bugengine/rtti/engine/namespace.hh>
#include <bugengine/scheduler/task/group.hh>
#include <bugengine/scheduler/task/method.hh>
#include <bugengine/world/world.script.hh>

namespace BugEngine { namespace World {

World::World(ref< const Component::StorageConfiguration >             configuration,
             minitl::array< weak< const KernelScheduler::IProduct > > products)
    : m_task(
        ref< Task::TaskGroup >::create(Arena::task(), "world:update", Colors::make(89, 89, 180)))
    , m_taskStart(m_task, configuration->updateStart())
    , m_productEnds(Arena::task(), products.size() + 1)
{
    minitl::array< Task::TaskGroup::TaskEndConnection >::iterator connection
        = m_productEnds.begin();
    for(minitl::array< weak< const KernelScheduler::IProduct > >::const_iterator product
        = products.begin();
        product != products.end(); ++product, ++connection)
    {
        *connection = Task::TaskGroup::TaskEndConnection(m_task, (*product)->producer());
    }
    m_productEnds.last() = Task::TaskGroup::TaskEndConnection(m_task, configuration->updateStart());
}

World::~World()
{
}

weak< Task::ITask > World::updateWorldTask() const
{
    return m_task;
}

Entity World::spawn()
{
    Entity result = {42};
    return result;
}

void World::unspawn(Entity e)
{
    be_forceuse(e);
}

void World::addComponent(Entity e, const void* component, raw< const RTTI::Class > componentType)
{
    be_forceuse(e);
    be_forceuse(component);
    be_forceuse(componentType);
}

void World::removeComponent(Entity e, raw< const RTTI::Class > componentType)
{
    be_forceuse(e);
    be_forceuse(componentType);
}

bool World::hasComponent(Entity e, raw< const RTTI::Class > componentType) const
{
    be_forceuse(e);
    be_forceuse(componentType);
    return false;
}

void World::addComponent(Entity e, const RTTI::Value& component)
{
    be_forceuse(e);
    be_forceuse(component);
}

RTTI::Value World::getComponent(Entity e, raw< const RTTI::Class > metaclass) const
{
    be_forceuse(e);
    be_forceuse(metaclass);
    return RTTI::Value();
}

}}  // namespace BugEngine::World
