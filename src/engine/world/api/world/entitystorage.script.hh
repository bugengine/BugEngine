/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_WORLD_ENTITYSTORAGE_SCRIPT_HH_
#define BE_WORLD_ENTITYSTORAGE_SCRIPT_HH_
/*****************************************************************************/
#include    <world/entity.script.hh>
#include    <scheduler/task/itask.hh>
#include    <core/memory/allocators/system.hh>

namespace BugEngine { namespace World
{

class World;

class be_api(WORLD) EntityStorage : public minitl::refcountable
{
    friend class World;
private:
    scoped<Task::ITask>                         m_task;
    SystemAllocator                             m_entityMappingBuffer;
    u32*                                        m_entityMapping;
    minitl::vector< raw<const RTTI::Class> >    m_componentTypes;
private:
    void start();
    void spawn(Entity e);
    void unspawn(Entity e);
protected:
    EntityStorage();
    ~EntityStorage();

    weak<Task::ITask>   initialTask() const;
};

}}


/*****************************************************************************/
#endif
