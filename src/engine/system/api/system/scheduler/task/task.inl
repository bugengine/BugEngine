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
Task< Body >::Task(istring name, color32 color, const Body& body)
:   ITask(name, color)
,   m_body(body)
,   m_taskCount(0)
,   m_taskCompleted(0)
{
}

template< typename Body >
void Task< Body >::run(weak<Scheduler> sc) const
{
    m_taskCount = 0;
    m_taskCompleted = 0;

    typedef typename Body::Range Range;
    Range r = m_body.prepare();
    void* item = sc->allocate_task< ScheduledTasks::TaskItem<Range, Body > >();
    weak< const Task<Body> > dis = this;
    sc->queue(new(item) ScheduledTasks::TaskItem<Range, Body >(dis, r, m_body));
}

}


/*****************************************************************************/
#endif
