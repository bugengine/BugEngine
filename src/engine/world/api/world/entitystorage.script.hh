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
struct Component;

class be_api(WORLD) EntityStorage : public minitl::refcountable
{
    friend class World;
private:
    struct EntityInfo;
private:
    scoped<Task::ITask>                         m_task;
    Entity                                      m_freeEntityId;
    SystemAllocator                             m_entityAllocator;
    EntityInfo*                                 m_entityInfoBuffer;
    u32                                         m_entityCount;
    u32                                         m_entityCapacity;
    minitl::vector< raw<const RTTI::Class> >    m_componentTypes;
private:
    void start();
private: // friend World
    Entity spawn();
    void unspawn(Entity e);
    void addComponent(Entity e, const Component& c, raw<const RTTI::Class> componentType);
    void removeComponent(Entity e, raw<const RTTI::Class> componentType);
protected:
    EntityStorage();
    ~EntityStorage();

    weak<Task::ITask>   initialTask() const;
};

}}


/*****************************************************************************/
#endif
