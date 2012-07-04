/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/scheduler/private/taskitem.hh>
#include    <system/scheduler/task/task.hh>


namespace BugEngine { namespace Task
{

ITaskItem::ITaskItem(weak<const ITask> owner)
:   m_owner(owner)
,   m_splitCount(0)
{
}

ITaskItem::ITaskItem(ITaskItem& cpy)
:   m_owner(cpy.m_owner)
,   m_splitCount(++cpy.m_splitCount)
{
}

}}
