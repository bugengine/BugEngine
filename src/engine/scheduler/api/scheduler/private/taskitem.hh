/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_TASKITEM_HH_
#define BE_SCHEDULER_TASKITEM_HH_
/**************************************************************************************************/
#include    <scheduler/stdafx.h>
#include    <scheduler/scheduler.hh>

namespace BugEngine { namespace Task
{

class ITask;
template< typename BODY > class Task;
class TaskScheduler;

class be_api(SCHEDULER) ITaskItem : public minitl::istack<ITaskItem>::node
{
    friend class TaskScheduler;
protected:
    weak<ITask> m_owner;
protected:
    template< typename T >
    void release(weak<Scheduler> scheduler)
    {
        return scheduler->releaseTask<T>(minitl::be_checked_cast<T>(this));
    }
public:
    virtual void    run(weak<Scheduler> sc) = 0;
public:
    ITaskItem(weak<ITask> owner);
    virtual ~ITaskItem();

private:
    ITaskItem(const ITaskItem& other);
    ITaskItem& operator=(const ITaskItem& other);
};

template< typename Range, typename Body >
class TaskItem : public ITaskItem
{
    friend class Task<Body>;
private:
    Range       m_range;
    const Body& m_body;
public:
    virtual void    run(weak<Scheduler> sc) override;
private:
    TaskItem(weak< Task<Body> > owner, const Range& r, u32 index, u32 total);
private:
    TaskItem(const TaskItem& other);
    TaskItem& operator=(const TaskItem& other);
};

}}

#include    <scheduler/private/taskitem.inl>

/**************************************************************************************************/
#endif
