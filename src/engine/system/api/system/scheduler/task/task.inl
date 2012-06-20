/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_SCHEDULER_TASL_TASK_INL_
#define BE_SYSTEM_SCHEDULER_TASL_TASK_INL_
/*****************************************************************************/
#include    <system/scheduler/scheduler.hh>
#include    <system/scheduler/private/taskitem.hh>

namespace BugEngine
{

template< typename Body >
    Task< Body >::Task(istring name, color32 color, const Body& body, Scheduler::Priority priority, Scheduler::Affinity affinity)
:   ITask(name, color, priority, affinity)
,   m_body(body)
,   m_taskCount(0)
,   m_taskCompleted(0)
{
}

template< typename Body >
void Task< Body >::schedule(weak<Scheduler> sc) const
{
    m_taskCount = 0;
    m_taskCompleted = 0;

    typedef typename Body::Range Range;
    Range r = m_body.prepare();
    void* item = sc->allocateTask< ScheduledTasks::TaskItem<Range, Body> >();
    sc->queueTask(new(item) ScheduledTasks::TaskItem<Range, Body>(this, r, m_body));
}

}


/*****************************************************************************/
#endif
