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
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

#ifndef BE_CORE_THREAD_SCHEDULER_TASKITEM_INL_
#define BE_CORE_THREAD_SCHEDULER_TASKITEM_INL_
/*****************************************************************************/
#include    <core/scheduler/scheduler.hh>

namespace BugEngine { namespace ScheduledTasks
{

template< typename Range, typename Body >
TaskItem<Range, Body>::TaskItem(const BaseTask* owner, const Range& r, const Body& b)
:   BaseTaskItem(owner)
,   m_range(r)
,   m_body(b)
{
}

template< typename Range, typename Body >
TaskItem<Range, Body>::TaskItem(TaskItem& split)
:   BaseTaskItem(split)
,   m_range(split.m_range.split())
,   m_body(split.m_body)
{
}

template< typename Range, typename Body >
void TaskItem<Range, Body>::run(Scheduler* sc)
{
    Assert(m_currentState == Executing);
    m_body(m_range);
    postrun(sc);
    sc->release_task(this);
}

template< typename Range, typename Body >
BaseTaskItem* TaskItem<Range, Body>::split(Scheduler* sc)
{
    void* mem = sc->allocate_task< TaskItem<Range, Body> >();
    BaseTaskItem* result = new(mem) TaskItem<Range, Body>(*this);
    return result;
}

template< typename Range, typename Body >
bool TaskItem<Range, Body>::atomic() const
{
    return m_range.atomic();
}

}}

/*****************************************************************************/
#endif
