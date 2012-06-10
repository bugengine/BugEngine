/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <world/stdafx.h>
#include    <world/world.script.hh>
#include    <system/scheduler/task/group.hh>
#include    <system/scheduler/task/method.hh>

#include <world/storage.hh>
#include <world/component.script.hh>
struct A : public BugEngine::World::Component
{
    int i;
};

struct B : public A
{
    int j;
    
    enum { ReservedSize = 4*1024 };
};

#include    <state.hh>

BE_REGISTER_NAMESPACE_2_NAMED(game, BugEngine, World);

namespace BugEngine { namespace World
{

static const Entity s_defaultSlot = { 0 };

World::World()
:   m_task(ref<TaskGroup>::create(Arena::task(), "world:update", color32(89, 89, 180)))
//,   m_rules(Arena::game(), 0)
,   m_emptyEntityState(scoped<State>::create(Arena::game()))
,   m_freeEntityId(s_defaultSlot)
,   m_entityAllocator(20*1024*1024)
,   m_entityBuffer((Entity*)m_entityAllocator.buffer())
,   m_entityCount(0)
,   m_entityCapacity(0)
{
    scoped< Storage<B> > s (scoped< Storage<B> >::create(Arena::game(), this));
    s->create(0, B());
    s->create(1, B());
    s->create(2, B());
    s->create(3, B());
    s->create(4, B());
    s->destroy(3);
    s->move(4, 3);

    s->move(2, 4);
    s->move(1, 2);
    s->move(0, 1);
    s->create(0, B());

    s->destroy(4);
    s->destroy(3);
    s->destroy(2);
    s->destroy(1);
    s->destroy(0);
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
    return e;
}

void World::unspawn(Entity e)
{
    m_entityBuffer[e.id] = m_freeEntityId;
    m_freeEntityId = e;
    -- m_entityCount;
}

void World::addComponent(Entity e, const Component& component, raw<const RTTI::Class> metaclass)
{
    be_assert(metaclass->isA(be_typeid<Component>::klass()), "component of type %s is not a subclass of BugEngine::World::Component"|metaclass->name);
    be_forceuse(e);
    be_forceuse(component);
}

void World::addComponent(Entity e, RTTI::Value& component)
{
    be_assert(component.type().isA(be_typeid<const Component&>::type()), "component of type %s is not a subclass of BugEngine::World::Component"|component.type().name());
    addComponent(e, component.as<const Component&>(), component.type().metaclass);
}

}}

