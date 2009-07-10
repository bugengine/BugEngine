/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA                                                                  *
* 02110-1301  USA                                                             *
\*****************************************************************************/

#include    <system/stdafx.h>
#include    <system/scheduler/taskitem.hh>
#include    <system/scheduler/task.hh>


namespace BugEngine { namespace ScheduledTasks
{

BaseTaskItem::BaseTaskItem(const BaseTask* owner, TaskState initialState)
:   m_currentState(initialState)
,   m_next(0)
,   m_dependsOn(0)
,   m_owner(owner)
,   m_splitCount(0)
{
    m_owner->m_taskCount++;
}

BaseTaskItem::BaseTaskItem(BaseTaskItem& cpy)
:   m_currentState(cpy.m_currentState)
,   m_next(0)
,   m_affinity(cpy.m_affinity)
,   m_dependsOn(cpy.m_dependsOn)
,   m_owner(cpy.m_owner)
,   m_splitCount(++cpy.m_splitCount)
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
