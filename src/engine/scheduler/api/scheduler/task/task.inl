/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_TASL_TASK_INL_
#define BE_SCHEDULER_TASL_TASK_INL_
/*****************************************************************************/
#include    <scheduler/scheduler.hh>
#include    <scheduler/private/taskitem.hh>

namespace BugEngine { namespace Task
{

template< typename Body >
Task< Body >::Task(istring name, color32 color, const Body& body, Scheduler::Priority priority, Scheduler::Affinity affinity)
    :   ITask(name, color, priority, affinity)
    ,   m_body(body)
    ,   m_taskCount(i_u32::Zero)
    ,   m_taskCompleted(i_u32::Zero)
{
}

template< typename Body >
void Task< Body >::schedule(weak<Scheduler> sc) const
{
    m_taskCount = 0;
    m_taskCompleted = 0;

    typedef typename Body::Range Range;
    Range r = m_body.prepare();
    void* item = sc->allocateTask< TaskItem<Range, Body> >();
    sc->queueTask(new(item) TaskItem<Range, Body>(this, r, m_body));
}

}}


/*****************************************************************************/
#endif
