/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <world/stdafx.h>
#include    <world/world.script.hh>
#include    <world/entitystorage.script.hh>
#include    <world/component.script.hh>
#include    <scheduler/task/group.hh>
#include    <rtti/engine/namespace.hh>


namespace BugEngine { namespace World
{

World::World(weak<EntityStorage> storage, minitl::array< weak<const Kernel::IProduct> > products)
:   m_task(ref<Task::TaskGroup>::create(Arena::task(), "world:update", Colors::make(89, 89, 180)))
,   m_storage(storage)
,   m_taskStart(Task::TaskGroup::TaskStartConnection(m_task, m_storage->initialTask()))
,   m_taskEnd(Task::TaskGroup::TaskEndConnection(m_task, m_storage->initialTask()))
,   m_productEnds(Arena::task(), products.size())
{
    minitl::array<Task::TaskGroup::TaskEndConnection>::iterator connection = m_productEnds.begin();
    for (minitl::array< weak<const Kernel::IProduct> >::const_iterator product = products.begin();
         product != products.end();
         ++product, ++connection)
    {
        *connection = Task::TaskGroup::TaskEndConnection(m_task, (*product)->producer());
    }
}

World::~World()
{
}

weak<Task::ITask> World::updateWorldTask() const
{
    return m_task;
}

Entity World::spawn()
{
    return m_storage->spawn();
}

void World::unspawn(Entity e)
{
    m_storage->unspawn(e);
}

void World::addComponent(Entity e, const void* component, raw<const RTTI::Class> componentType)
{
    m_storage->addComponent(e, component, componentType);
}

void World::removeComponent(Entity e, raw<const RTTI::Class> componentType)
{
    m_storage->removeComponent(e, componentType);
}

bool World::hasComponent(Entity e, raw<const RTTI::Class> componentType) const
{
    return m_storage->hasComponent(e, componentType);
}

void World::addComponent(Entity e, const RTTI::Value& component)
{
    addComponent(e, component.as<const void*>(), component.type().metaclass);
}

RTTI::Value World::getComponent(Entity e, raw<const RTTI::Class> metaclass) const
{
    return m_storage->getComponent(e, metaclass);
}

}}

