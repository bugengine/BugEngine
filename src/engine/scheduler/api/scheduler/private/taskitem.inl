/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_TASKITEM_INL_
#define BE_SCHEDULER_TASKITEM_INL_
/**************************************************************************************************/
#include    <scheduler/stdafx.h>
#include    <scheduler/scheduler.hh>
#include    <scheduler/task/task.hh>

namespace BugEngine { namespace Task
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
TaskItem<Range, Body>::TaskItem(const TaskItem& split, u32 index, u32 total)
    :   ITaskItem(split)
    ,   m_range(split.m_range.part(index, total))
    ,   m_body(split.m_body)
{
}

template< typename Range, typename Body >
void TaskItem<Range, Body>::run(weak<Scheduler> sc)
{
    weak< const Task<Body> > owner = be_checked_cast< const Task<Body> >(m_owner);
    if (m_range.atomic())
    {
        m_body(m_range);
    }
    else
    {
        u32 splitCount = m_range.partCount(sc->workerCount());
        owner->m_taskCount += splitCount;
        for (u32 i = 0; i < splitCount; ++i)
        {
            void* mem = this->allocate< TaskItem<Range, Body> >(sc);
            this->schedule(sc, new(mem) TaskItem<Range, Body>(*this, i, splitCount));
        }
    }
    if (++owner->m_taskCompleted == owner->m_taskCount)
    {
        owner->completed(sc);
    }
    this->release< TaskItem<Range, Body> >(sc);
}

}}

/**************************************************************************************************/
#endif
