/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/scheduler/stdafx.h>
#include <bugengine/scheduler/task/group.hh>

namespace BugEngine { namespace Task {

TaskGroup::TaskGroup(istring name, color32 color)
    : ITask(name, color, Scheduler::High, Scheduler::WorkerThread)
    , m_startTasks(Arena::task())
    , m_endTaskCount(i_u32::create(0))
    , m_completionCallback(ref< Callback >::create(Arena::task(), this))
{
}

TaskGroup::~TaskGroup()
{
}

void TaskGroup::schedule(weak< Scheduler > scheduler)
{
    Scheduler::WorkItem item(scheduler);
    if(!m_startTasks.empty())
    {
        for(minitl::vector< weak< ITask > >::const_iterator it = m_startTasks.begin();
            it != m_startTasks.end(); ++it)
        {
            (*it)->startCallback()->onCompleted(scheduler, this);
        }
    }
    else
    {
        completed(scheduler);
    }
}

void TaskGroup::addStartTask(weak< ITask > task)
{
    m_startTasks.push_back(task);
    task->startCallback()->onConnected(this, ICallback::Pending);
}

bool TaskGroup::removeStartTask(weak< ITask > task)
{
    for(minitl::vector< weak< ITask > >::iterator it = m_startTasks.begin();
        it != m_startTasks.end(); ++it)
    {
        if(*it == task)
        {
            bool result = (*it)->startCallback()->onDisconnected(this);
            be_forceuse(result);
            be_assert(result,
                      "unable to disconnect task %s from group %s" | task->name | this->name);
            m_startTasks.erase(it);
            return true;
        }
    }
    return false;
}

TaskGroup::Callback::Callback(weak< TaskGroup > owner)
    : ICallback()
    , m_owner(owner)
    , m_completed(i_u32::create(0))
{
}

TaskGroup::Callback::~Callback()
{
}

void TaskGroup::Callback::onCompleted(weak< Scheduler > scheduler, weak< ITask > /*task*/)
{
    if(++m_completed == m_owner->m_endTaskCount)
    {
        m_completed = 0;
        m_owner->completed(scheduler);
    }
}

void TaskGroup::Callback::onConnected(weak< ITask > /*to*/, CallbackStatus status)
{
    if(status == Completed) m_completed++;
}

bool TaskGroup::Callback::onDisconnected(weak< ITask > /*from*/)
{
    return true;
}

/*----------------------------------------------------------------------------*/

TaskGroup::TaskStartConnection::TaskStartConnection() : m_group(), m_task()
{
}

TaskGroup::TaskStartConnection::TaskStartConnection(weak< TaskGroup > group, weak< ITask > task)
    : m_group(group)
    , m_task(task)
{
    if(m_group)
    {
        m_group->addStartTask(m_task);
    }
}

TaskGroup::TaskStartConnection::TaskStartConnection(const TaskStartConnection& other)
    : m_group(other.m_group)
    , m_task(other.m_task)
{
    if(m_group)
    {
        m_group->addStartTask(m_task);
    }
}

TaskGroup::TaskStartConnection&
TaskGroup::TaskStartConnection::operator=(const TaskStartConnection& other)
{
    if(m_group)
    {
        bool result = m_group->removeStartTask(m_task);
        be_forceuse(result);
        be_assert(result,
                  "could not disconnect task %s from group %s" | m_task->name | m_group->name);
    }
    m_group = other.m_group;
    m_task  = other.m_task;
    if(m_group)
    {
        m_group->addStartTask(m_task);
    }
    return *this;
}

TaskGroup::TaskStartConnection::~TaskStartConnection()
{
    if(m_group)
    {
        bool result = m_group->removeStartTask(m_task);
        be_forceuse(result);
        be_assert(result,
                  "could not disconnect task %s from group %s" | m_task->name | m_group->name);
    }
}

TaskGroup::TaskEndConnection::TaskEndConnection() : m_group(), m_task(), m_callback()
{
}

TaskGroup::TaskEndConnection::TaskEndConnection(weak< TaskGroup > group, weak< ITask > task)
    : m_group(group)
    , m_task(task)
    , m_callback(task, group->m_completionCallback)
{
    m_group->m_endTaskCount++;
}

TaskGroup::TaskEndConnection::TaskEndConnection(const TaskEndConnection& other)
    : m_group(other.m_group)
    , m_task(other.m_task)
    , m_callback(other.m_callback)
{
    if(m_group)
    {
        m_group->m_endTaskCount++;
    }
}

TaskGroup::TaskEndConnection&
TaskGroup::TaskEndConnection::operator=(const TaskEndConnection& other)
{
    weak< TaskGroup > previousGroup = m_group;
    m_group                         = other.m_group;
    m_task                          = other.m_task;
    m_callback                      = other.m_callback;
    if(m_group)
    {
        m_group->m_endTaskCount++;
    }
    if(previousGroup)
    {
        previousGroup->m_endTaskCount--;
    }
    return *this;
}

TaskGroup::TaskEndConnection::~TaskEndConnection()
{
    if(m_group)
    {
        m_group->m_endTaskCount--;
    }
}

}}  // namespace BugEngine::Task
