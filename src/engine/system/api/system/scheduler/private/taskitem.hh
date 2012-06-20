/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_SCHEDULER_TASKITEM_HH_
#define BE_SYSTEM_SCHEDULER_TASKITEM_HH_
/*****************************************************************************/
#include    <minitl/interlocked/interlocked.hh>
#include    <system/scheduler/scheduler.hh>

namespace BugEngine
{

class ITask;
template< typename BODY > class Task;
class TaskScheduler;


namespace ScheduledTasks
{

class be_api(SYSTEM) ITaskItem : public minitl::inode
{
    friend class ::BugEngine::TaskScheduler;
protected:
    size_t              m_splitCount;
    weak<const ITask>   m_owner;
public:
    virtual void            run(weak<Scheduler> sc) = 0;
    virtual ITaskItem*      split(weak<Scheduler> sc) = 0;
    virtual bool            atomic() const = 0;
public:
    explicit ITaskItem(weak<const ITask> owner);
    ITaskItem(ITaskItem& cpy);
    virtual ~ITaskItem() { }
};

template< typename Range, typename Body >
class TaskItem : public ITaskItem
{
private:
    Range   m_range;
    Body    m_body;
public:
    TaskItem(weak< const Task<Body> > owner, const Range& r, const Body& b);
    TaskItem(TaskItem& split);

    virtual void            run(weak<Scheduler> sc) override;
    virtual ITaskItem*      split(weak<Scheduler> sc) override;
    virtual bool            atomic() const override;
};

}}

#include    <system/scheduler/private/taskitem.inl>

/*****************************************************************************/
#endif
