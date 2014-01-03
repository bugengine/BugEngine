/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <scheduler/stdafx.h>
#include    <scheduler/private/taskitem.hh>
#include    <scheduler/task/task.hh>


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

ITaskItem::~ITaskItem()
{
    
}

}}
