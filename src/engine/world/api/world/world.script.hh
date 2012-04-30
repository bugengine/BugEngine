/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_WORLD_WORLD_SCRIPT_HH_
#define BE_WORLD_WORLD_SCRIPT_HH_
/*****************************************************************************/
#include    <system/scheduler/task/itask.hh>
#include    <system/resource/resource.script.hh>
#include    <world/entity.script.hh>


namespace BugEngine { namespace World
{

class State;
class Storage;
template< typename T, size_t SIZEKB >
struct Block;
class BlockManager;

class be_api(WORLD) World : public Resource
{
    friend class Storage;
private:
    union EntitySlot
    {
        Entity data;
        Entity next;
    };
private:
    ref<ITask>                      m_task;
    scoped<State>                   m_emptyEntityState;
    scoped<BlockManager>            m_blockManager;
    Entity                          m_freeEntityId;
    Entity                          m_lastEntityId;
    scoped< Block<EntitySlot,64> >  m_entityBlocks;
public:
    weak<ITask> updateWorldTask() const;
published:
    Entity spawn();
published:
    World();
    ~World();
};

}}


/*****************************************************************************/
#endif
