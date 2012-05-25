/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <world/stdafx.h>
#include    <world/world.script.hh>
#include    <system/scheduler/task/group.hh>
#include    <system/scheduler/task/method.hh>

#include    <state.hh>

BE_REGISTER_NAMESPACE_2_NAMED(game, BugEngine, World);

namespace BugEngine { namespace World
{

static const Entity s_defaultSlot = { {0, 0, 0} };
static i_u16 s_worldCount = 0;

World::RuleConnection::RuleConnection(weak<World> world, weak<const Rule> rule)
    :   world(world)
    ,   rule(rule)
    ,   start(be_checked_cast<TaskGroup>(world->m_task), rule->updateTask())
    ,   end(be_checked_cast<TaskGroup>(world->m_task), rule->updateTask())
    ,   dependencies(gameArena())
{
}

World::RuleConnection::~RuleConnection()
{
}

World::World()
:   m_task(ref<TaskGroup>::create(taskArena(), "world:update", color32(89, 89, 180)))
,   m_rules(gameArena())
,   m_emptyEntityState(scoped<State>::create(gameArena()))
,   m_freeEntityId(s_defaultSlot)
,   m_allocator16k(SystemAllocator::Block64kb, 2048)
,   m_allocator64k(SystemAllocator::Block64kb, 512)
,   m_entityBuffers(gameArena())
,   m_worldIndex(++s_worldCount)
{
    m_freeEntityId.index.world = m_worldIndex;
}

World::~World()
{
}

weak<ITask> World::updateWorldTask() const
{
    return m_task;
}

Entity World::spawn()
{
    Entity e = m_freeEntityId;
    be_assert(e.index.world == 0, "entity (%d) already in use" | e.index.world);

    if (e.index.block >= m_entityBuffers.size())
    {
        be_assert(e.index.block == m_entityBuffers.size(), "mismatch in the entity buffer!");
        Entity* newBuffer = static_cast<Entity*>(m_allocator64k.blockAlloc());
        size_t entityCount = m_allocator64k.blockSize()/sizeof(Entity);
        for (size_t i = 0; i < entityCount-1; ++i)
        {
            newBuffer[i].index.world = 0;
            newBuffer[i].index.block = e.index.block;
            newBuffer[i].index.offset = i+1;
        }
        newBuffer[entityCount-1].index.world = 0;
        newBuffer[entityCount-1].index.block = e.index.block+1;
        newBuffer[entityCount-1].index.offset = 0;
    }

    m_freeEntityId = m_entityBuffers[e.index.block][e.index.offset];
    e.index.world = m_worldIndex;
    ++ m_entityCount;
    return e;
}

void World::unspawn(Entity e)
{
    be_assert(e.index.world == m_worldIndex, "entity (%d) does not belong to this world (%d)" | e.index.world | m_worldIndex);
    m_entityBuffers[e.index.block][e.index.offset] = m_freeEntityId;
    e.index.world = 0;
    m_freeEntityId = e;
    -- m_entityCount;
}

void World::addComponent(Entity e, const Component& component, raw<const RTTI::Class> metaclass)
{
    be_assert(e.index.world == m_worldIndex, "entity (%d) does not belong to this world (%d)" | e.index.world | m_worldIndex);
    be_assert(metaclass->isA(be_typeid<Component>::klass()), "component of type %s is not a subclass of BugEngine::World::Component"|metaclass->name);
    be_forceuse(e);
    be_forceuse(component);
}

void World::addComponent(Entity e, RTTI::Value& component)
{
    be_assert(e.index.world == m_worldIndex, "entity (%d) does not belong to this world (%d)" | e.index.world | m_worldIndex);
    be_assert(be_typeid<const Component&>::type() <= component.type(), "component of type %s is not a subclass of BugEngine::World::Component"|component.type().name());
    addComponent(e, component.as<const Component&>(), component.type().metaclass);
}

void World::addRule(weak<const Rule> rule)
{
    m_rules.push_back(RuleConnection(this, rule));
}

void World::removeRule(weak<const Rule> rule)
{
    for (minitl::vector< RuleConnection >::iterator it = m_rules.begin(); it != m_rules.end(); ++it)
    {
        if (it->rule == rule)
        {
            m_rules.erase(it);
            return;
        }
    }
    be_notreached();
}

}}

