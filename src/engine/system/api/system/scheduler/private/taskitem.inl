/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_SCHEDULER_TASKITEM_INL_
#define BE_SYSTEM_SCHEDULER_TASKITEM_INL_
/*****************************************************************************/
#include    <system/scheduler/scheduler.hh>
#include    <system/scheduler/task/task.hh>

namespace BugEngine { namespace ScheduledTasks
{

template< typename Range, typename Body >
TaskItem<Range, Body>::TaskItem(weak<const Task<Body> > owner, const Range& r, const Body& b)
:   ITaskItem(owner)
,   m_range(r)
,   m_body(b)
{
    owner->m_taskCount++;
}

template< typename Range, typename Body >
TaskItem<Range, Body>::TaskItem(TaskItem& split)
:   ITaskItem(split)
,   m_range(split.m_range.split())
,   m_body(split.m_body)
{
    weak< const Task<Body> > owner = be_checked_cast< const Task<Body> >(m_owner);
    owner->m_taskCount++;
}

template< typename Range, typename Body >
ITaskItem* TaskItem<Range, Body>::split(weak<Scheduler> sc)
{
    void* mem = sc->allocate_task< TaskItem<Range, Body> >();
    ITaskItem* result = new(mem) TaskItem<Range, Body>(*this);
    return result;
}

template< typename Range, typename Body >
bool TaskItem<Range, Body>::atomic() const
{
    return m_range.atomic();
}

template< typename Range, typename Body >
void TaskItem<Range, Body>::run(weak<Scheduler> sc)
{
    m_body(m_range);
    weak< const Task<Body> > owner = be_checked_cast< const Task<Body> >(m_owner);
    if (++owner->m_taskCompleted == owner->m_taskCount)
    {
        owner->end(sc);
    }
    sc->release_task(this);
}

}}

/*****************************************************************************/
#endif
