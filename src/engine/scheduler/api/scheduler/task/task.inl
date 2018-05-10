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
    ,   m_taskCount(i_u32::create(0))
    ,   m_taskCompleted(i_u32::create(0))
{
}

template< typename Body >
void Task< Body >::schedule(weak<Scheduler> sc)
{
    be_assert(m_taskCount == m_taskCompleted,
              "task %s is scheduled again without having completed the previous run: "
              "%d/%d parts completed"
              | name | m_taskCompleted | m_taskCount);
    m_taskCount = 0;
    m_taskCompleted = 0;

    typedef typename Body::Range Range;
    Range r = body.prepare();
    u32 splitCount = r.partCount(sc->workerCount());
    m_taskCount += splitCount;
    TaskItem<Range, Body>* head = 0, *tail = 0;
    for (u32 i = 0; i < splitCount; ++i)
    {
        void* mem = sc->allocateTask< TaskItem<Range, Body> >();
        TaskItem<Range, Body>* item = new (mem) TaskItem<Range, Body>(this, r, i, splitCount);
        if (!tail) tail = item;
        item->next = head;
        head = item;
    }
    sc->queueTasks(head, tail, splitCount);
}

}}

/**************************************************************************************************/
#endif
