/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/scheduler/taskitem.hh>
#include    <system/scheduler/task.hh>


namespace BugEngine { namespace ScheduledTasks
{

BaseTaskItem::BaseTaskItem(const BaseTask* owner, TaskState initialState)
:   m_next(0)
,   m_currentState(initialState)
,   m_splitCount(0)
,   m_dependsOn(0)
,   m_owner(owner)
{
    m_owner->m_taskCount++;
}

BaseTaskItem::BaseTaskItem(BaseTaskItem& cpy)
:   m_next(0)
,   m_currentState(cpy.m_currentState)
,   m_affinity(cpy.m_affinity)
,   m_splitCount(++cpy.m_splitCount)
,   m_dependsOn(cpy.m_dependsOn)
,   m_owner(cpy.m_owner)
{
    m_owner->m_taskCount++;
}

void BaseTaskItem::postrun(Scheduler* sc)
{
    if(++m_owner->m_taskCompleted == m_owner->m_taskCount)
    {
        m_owner->end(sc);
    }
}

}}
