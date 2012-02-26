/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_WORLD_WORLD_SCRIPT_HH_
#define BE_WORLD_WORLD_SCRIPT_HH_
/*****************************************************************************/
#include    <system/scheduler/task/itask.hh>
#include    <system/resource/resource.script.hh>

namespace BugEngine { namespace World
{

class State;

class be_api(WORLD) World : public Resource
{
private:
    ref<ITask>      m_task;
    scoped<State>   m_emptyEntityState;
public:
    weak<ITask> updateWorldTask() const;
published:
    World();
    ~World();
};

}}


/*****************************************************************************/
#endif
