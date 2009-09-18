/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_SCHEDULER_TASK_INL_
#define BE_SYSTEM_SCHEDULER_TASK_INL_
/*****************************************************************************/
#include    <system/scheduler/scheduler.hh>
#include    <system/scheduler/taskitem.hh>

namespace BugEngine
{

template< typename Body >
Task< Body >::Task(const istring& name, color32 color, const Body& body, bool simultaneous)
:   BaseTask(name, color, simultaneous)
,   m_body(body)
{
}

template< typename Body >
void Task< Body >::runTask(Scheduler* sc) const
{
    typedef typename Body::Range Range;
    Range r = m_body.prepare();
    void* item = sc->allocate_task< ScheduledTasks::TaskItem<Range, Body > >();
    sc->queue(new(item) ScheduledTasks::TaskItem<Range, Body >(this, r, m_body));
}

}


/*****************************************************************************/
#endif
