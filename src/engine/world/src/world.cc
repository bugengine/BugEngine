/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <world/stdafx.h>
#include    <world/world.script.hh>
#include    <world/entitystorage.script.hh>
#include    <scheduler/task/group.hh>
#include    <rtti/engine/namespace.hh>

BE_REGISTER_NAMESPACE_2_NAMED(game, BugEngine, World);

namespace BugEngine { namespace World
{

World::World(weak<EntityStorage> storage, minitl::array<Kernel::IProduct> products)
:   m_task(ref<Task::TaskGroup>::create(Arena::task(), "world:update", Colors::make(89, 89, 180)))
,   m_storage(storage)
,   m_taskStart(Task::TaskGroup::TaskStartConnection(m_task, m_storage->initialTask()))
,   m_taskEnd(Task::TaskGroup::TaskEndConnection(m_task, m_storage->initialTask()))
,   m_productEnds(Arena::task(), products.size())
{
    minitl::array<Task::TaskGroup::TaskEndConnection>::iterator connection = m_productEnds.begin();
    for (minitl::array<Kernel::IProduct>::const_iterator product = products.begin(); product != products.end(); ++product, ++connection)
    {
        *connection = Task::TaskGroup::TaskEndConnection(m_task, product->producer);
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

void World::addComponent(Entity e, const Component& component, raw<const RTTI::Class> componentType)
{
    be_assert(componentType->isA(be_typeid<Component>::klass()), "component of type %s is not a subclass of BugEngine::World::Component"|componentType->name);
    m_storage->addComponent(e, component, componentType);
}

void World::removeComponent(Entity e, raw<const RTTI::Class> componentType)
{
    be_assert(componentType->isA(be_typeid<Component>::klass()), "component of type %s is not a subclass of BugEngine::World::Component"|componentType->name);
    m_storage->removeComponent(e, componentType);
}

bool World::hasComponent(Entity e, raw<const RTTI::Class> componentType) const
{
    be_assert(componentType->isA(be_typeid<Component>::klass()), "component of type %s is not a subclass of BugEngine::World::Component"|componentType->name);
    return m_storage->hasComponent(e, componentType);
}
    
void World::addComponent(Entity e, const RTTI::Value& component)
{
    be_assert(component.type().isA(be_typeid<const Component&>::type()), "component of type %s is not a subclass of BugEngine::World::Component"|component.type());
    addComponent(e, component.as<const Component&>(), component.type().metaclass);
}

}}

