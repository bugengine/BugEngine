/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_TASKITEM_INL_
#define BE_SCHEDULER_TASKITEM_INL_
/**************************************************************************************************/
#include <bugengine/scheduler/stdafx.h>
#include <bugengine/scheduler/scheduler.hh>
#include <bugengine/scheduler/task/task.hh>

namespace BugEngine { namespace Task {

template < typename Range, typename Body >
TaskItem< Range, Body >::TaskItem(weak< Task< Body > > owner, const Range& r, u32 index, u32 total)
    : ITaskItem(owner)
    , m_range(r.part(index, total))
    , m_body(owner->body)
{
}

template < typename Range, typename Body >
void TaskItem< Range, Body >::run(weak< Scheduler > sc)
{
    weak< Task< Body > > owner = be_checked_cast< Task< Body > >(m_owner);
    m_body(m_range);
    if(++owner->m_taskCompleted == owner->m_taskCount)
    {
        owner->completed(sc);
    }
    this->release< TaskItem< Range, Body > >(sc);
}

}}  // namespace BugEngine::Task

/**************************************************************************************************/
#endif
