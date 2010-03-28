/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/scheduler/task/group.hh>

namespace BugEngine
{

TaskGroup::TaskGroup(istring name, color32 color)
:   ITask(name, color)
,   m_startTasks()
,   m_endTasks()
,   m_completionCallback(ref<Callback>::create(this))
{
}

TaskGroup::~TaskGroup()
{
    for(minitl::vector< weak<ITask> >::const_iterator it = m_endTasks.begin(); it != m_endTasks.end(); ++it)
    {
        (*it)->removeCallback(m_completionCallback);
    }
}

void TaskGroup::run(weak<Scheduler> scheduler) const
{
    for(minitl::vector< weak<ITask> >::const_iterator it = m_startTasks.begin(); it != m_startTasks.end(); ++it)
    {
        (*it)->run(scheduler);
    }
}

void TaskGroup::addStartTask(weak<ITask> task)
{
    m_startTasks.push_back(task);
}

void TaskGroup::addEndTask(weak<ITask> task)
{
    m_endTasks.push_back(task);
    task->addCallback(m_completionCallback);
}

TaskGroup::Callback::Callback(weak<TaskGroup> owner)
:   ICallback()
,   m_owner(owner)
{
}

TaskGroup::Callback::~Callback()
{
}

void TaskGroup::Callback::onCompleted(weak<Scheduler> scheduler, weak<const ITask> task) const
{
    if(++m_completed == m_owner->m_endTasks.size())
    {
        m_completed = 0;
        m_owner->end(scheduler);
    }
}

void TaskGroup::Callback::onConnected(weak<ITask> to)
{
}

void TaskGroup::Callback::onDisconnected(weak<ITask> from)
{
}

}
