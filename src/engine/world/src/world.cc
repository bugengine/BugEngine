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

static const Entity s_defaultSlot = { 0 };

World::World(weak<EntityStorage> storage, minitl::array<Kernel::IProduct> products)
:   m_task(ref<Task::TaskGroup>::create(Arena::task(), "world:update", Colors::make(89, 89, 180)))
,   m_storage(storage)
,   m_taskStart(Task::TaskGroup::TaskStartConnection(m_task, m_storage->initialTask()))
,   m_taskEnd(Task::TaskGroup::TaskEndConnection(m_task, m_storage->initialTask()))
,   m_productEnds(Arena::task(), products.size())
,   m_freeEntityId(s_defaultSlot)
,   m_entityAllocator(sizeof(Entity)*4*1024*1024)
,   m_entityBuffer((Entity*)m_entityAllocator.buffer())
,   m_entityCount(0)
,   m_entityCapacity(0)
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
    Entity e = m_freeEntityId;

    if (e.id >= m_entityCapacity)
    {
        m_entityCapacity += 1024;
        m_entityAllocator.setUsage(m_entityCapacity*sizeof(Entity));
        for (u32 i = m_entityCapacity - 1024; i < m_entityCapacity; ++i)
        {
            m_entityBuffer[i].id = i+1;
        }
    }

    m_freeEntityId = m_entityBuffer[e.id];
    ++ m_entityCount;
    m_storage->spawn(e);
    return e;
}

void World::unspawn(Entity e)
{
    m_storage->unspawn(e);
    m_entityBuffer[e.id] = m_freeEntityId;
    m_freeEntityId = e;
    -- m_entityCount;
}

void World::addComponent(Entity e, const Component& component, raw<const RTTI::Class> metaclass)
{
    be_assert(metaclass->isA(be_typeid<Component>::klass()), "component of type %s is not a subclass of BugEngine::World::Component"|metaclass->name);
    be_forceuse(e);
    be_forceuse(component);
    be_forceuse(metaclass);
}

void World::addComponent(Entity e, RTTI::Value& component)
{
    be_assert(component.type().isA(be_typeid<const Component&>::type()), "component of type %s is not a subclass of BugEngine::World::Component"|component.type());
    addComponent(e, component.as<const Component&>(), component.type().metaclass);
}

}}

