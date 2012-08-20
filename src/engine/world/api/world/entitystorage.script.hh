/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_WORLD_ENTITYSTORAGE_SCRIPT_HH_
#define BE_WORLD_ENTITYSTORAGE_SCRIPT_HH_
/*****************************************************************************/
#include    <scheduler/task/itask.hh>

namespace BugEngine { namespace World
{

class World;

class be_api(WORLD) EntityStorage : public minitl::refcountable
{
    friend class World;
private:
    scoped<Task::ITask> m_task;
private:
    void start();
protected:
    EntityStorage();
    ~EntityStorage();

    weak<Task::ITask>   initialTask() const;
};

}}


/*****************************************************************************/
#endif
