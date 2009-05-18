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

#ifndef BE_CORE_THREAD_SCHEDULER_TASK_INL_
#define BE_CORE_THREAD_SCHEDULER_TASK_INL_
/*****************************************************************************/
#include    <core/scheduler/scheduler.hh>
#include    <core/scheduler/taskitem.hh>

namespace BugEngine
{

template< typename Body >
Task< Body >::Task(const istring& name, color32 color, const Body& body, bool simultaneous)
:   BaseTask(name, color, simultaneous)
,   m_body(body)
{
}

template< typename Body >
void Task< Body >::runTask(Scheduler* sc) const
{
    typedef typename Body::Range Range;
    Range r = m_body.prepare();
    void* item = sc->allocate_task< ScheduledTasks::TaskItem<Range, Body > >();
    sc->queue(new(item) ScheduledTasks::TaskItem<Range, Body >(this, r, m_body));
}

}


/*****************************************************************************/
#endif
