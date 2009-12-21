/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_SCHEDULER_TASKITEM_HH_
#define BE_SYSTEM_SCHEDULER_TASKITEM_HH_
/*****************************************************************************/
#include    <minitl/interlocked/interlocked.hh>
#include    <minitl/interlocked/stack.hh>
#include    <system/scheduler/scheduler.hh>
#include    <core/threads/semaphore.hh>

namespace BugEngine
{

class BaseTask;

namespace ScheduledTasks
{

class be_api(SYSTEM) BaseTaskItem : public minitl::inode
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
        Reserved    // It's being temporarily accessed
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
	virtual ~BaseTaskItem() { }
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

#include    <system/scheduler/taskitem.inl>

/*****************************************************************************/
#endif
