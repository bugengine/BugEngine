/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <world/stdafx.h>
#include    <world/entitystorage.script.hh>
#include    <scheduler/task/method.hh>

namespace BugEngine { namespace World
{

EntityStorage::EntityStorage()
    :   m_task(scoped< Task::Task< Task::MethodCaller<EntityStorage, &EntityStorage::start> > >::create(
                    Arena::task(),
                    "start",
                    Colors::Green::Green,
                    Task::MethodCaller<EntityStorage, &EntityStorage::start>(this)))
    ,   m_entityMappingBuffer(sizeof(u32)*4*1024*1024)
    ,   m_entityMapping((u32*)m_entityMappingBuffer.buffer())
    ,   m_componentTypes(Arena::game())
{
}

EntityStorage::~EntityStorage()
{
}

weak<Task::ITask> EntityStorage::initialTask() const
{
    return m_task;
}

void EntityStorage::start()
{
}

void EntityStorage::spawn(Entity e)
{
    m_entityMappingBuffer.setUsage(e.id * sizeof(u32));
    m_entityMapping[e.id] = 0;
}

void EntityStorage::unspawn(Entity e)
{
    m_entityMapping[e.id] = 0;
}

}}

