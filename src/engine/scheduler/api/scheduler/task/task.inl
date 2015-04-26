/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_TASL_TASK_INL_
#define BE_SCHEDULER_TASL_TASK_INL_
/**************************************************************************************************/
#include    <scheduler/stdafx.h>
#include    <scheduler/scheduler.hh>
#include    <scheduler/private/taskitem.hh>
#include    <minitl/typemanipulation.hh>

namespace BugEngine { namespace Task
{

template< typename Body >
Task< Body >::Task(istring name, color32 color, const Body& body,
                   Scheduler::Priority priority, Scheduler::Affinity affinity)
    :   ITask(name, color, priority, affinity)
    ,   body(body)
    ,   m_taskCount(i_u32::Zero)
    ,   m_taskCompleted(i_u32::Zero)
{
}

template< typename Body >
void Task< Body >::schedule(weak<Scheduler> sc)
{
    m_taskCount = 0;
    m_taskCompleted = 0;

    typedef typename Body::Range Range;
    Range r = body.prepare();
    void* item = sc->allocateTask< TaskItem<Range, Body> >();
    sc->queueTask(new(item) TaskItem<Range, Body>(this, r, body));
}

}}

/**************************************************************************************************/
#endif
