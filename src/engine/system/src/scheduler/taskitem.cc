/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/scheduler/private/taskitem.hh>
#include    <system/scheduler/task/task.hh>


namespace BugEngine { namespace ScheduledTasks
{

ITaskItem::ITaskItem(weak<const ITask> owner)
:   m_affinity(~0u)
,   m_splitCount(0)
,   m_owner(owner.operator->())
{
}

ITaskItem::ITaskItem(ITaskItem& cpy)
:   m_affinity(cpy.m_affinity)
,   m_splitCount(++cpy.m_splitCount)
,   m_owner(cpy.m_owner)
{
}

}}
