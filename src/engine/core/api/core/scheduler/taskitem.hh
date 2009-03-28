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

#ifndef BE_CORE_THREAD_SCHEDULER_TASKITEM_HH_
#define BE_CORE_THREAD_SCHEDULER_TASKITEM_HH_
/*****************************************************************************/
#include    <minitl/interlocked/interlocked>
#include    <minitl/interlocked/stack>
#include    <core/scheduler/scheduler.hh>
#include    <core/threads/semaphore.hh>

namespace BugEngine
{

class BaseTask;

namespace ScheduledTasks
{

class COREEXPORT BaseTaskItem : public minitl::inode
{
    friend class ::BugEngine::Scheduler;
    friend class ::BugEngine::Scheduler::Worker;
protected:
    enum TaskState
    {
        // only relevant when task is in the queue
        Finished,   // task is already finished and should be unhooked
        Executing,  // task is being executed
        // tasks queued for execution should always have these flags
        Ready,      // task is ready to be executed, but may be waiting for a dependancy
        // everything above task ready is a temporary state
        // the scheduler can wait for the state to return to ready in a loop
        Reserved,   // It's being temporarily accessed
    };
    BaseTaskItem* volatile              m_next;
    minitl::interlocked<TaskState>      m_currentState;
    size_t                              m_affinity;
    size_t                              m_splitCount;
    BaseTaskItem*                       m_dependsOn;
    const BaseTask*                     m_owner;
protected: //friend Worker
    virtual void            run(Scheduler* sc) = 0;
    virtual BaseTaskItem*   split(Scheduler* sc) = 0;
    virtual bool            atomic() const = 0;
protected:
    void postrun(Scheduler* sc);
public:
    explicit BaseTaskItem(const BaseTask* owner, TaskState initialState = Ready);
    BaseTaskItem(BaseTaskItem& cpy);
};

template< typename Range, typename Body >
class TaskItem : public BaseTaskItem
{
private:
    Range   m_range;
    Body    m_body;
public:
    TaskItem(const BaseTask* owner, const Range& r, const Body& b);
    TaskItem(TaskItem& split);

    virtual void            run(Scheduler* sc) override;
    virtual BaseTaskItem*   split(Scheduler* sc) override;
    virtual bool            atomic() const override;
};

}}

#include    <core/scheduler/taskitem.inl>

/*****************************************************************************/
#endif
