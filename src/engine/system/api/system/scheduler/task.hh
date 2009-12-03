/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_SCHEDULER_TASK_HH_
#define BE_SYSTEM_SCHEDULER_TASK_HH_
/*****************************************************************************/
#include    <core/threads/event.hh>
#include    <maths/vector.hh>

namespace BugEngine
{

class Scheduler;

namespace ScheduledTasks
{

class BaseTaskItem;

}

class be_api(SYSTEM) BaseTask : public minitl::refcountable
{
    friend class ScheduledTasks::BaseTaskItem;
public:
    class Callback : public minitl::refcountable
    {
        friend class BaseTask;
    protected:
        virtual void onAdded(weak<const BaseTask> t) = 0;
        virtual void onRemoved(weak<const BaseTask> t) = 0;
        virtual void onCompleted(weak<Scheduler> sc, weak<const BaseTask> t) = 0;
    };
public:
    const istring           name;
    const color32           color;
private:
    std::list< ref<Callback> >   m_callbacks;
    mutable i_u32                   m_taskCount;
    mutable i_u32                   m_taskCompleted;
protected:
    virtual void runTask(weak<Scheduler> sc) const = 0;
public:
    BaseTask(const istring& name, color32 color, bool simultaneous = false);
    ~BaseTask();

    void addCallback(ref<Callback> c);
    void removeCallback(ref<Callback> c);

    void run(weak<Scheduler> sc) const;
    void end(weak<Scheduler> sc) const;
private:
    BaseTask& operator=(const BaseTask& other);
    BaseTask(const BaseTask& other);
};

class be_api(SYSTEM) ChainTaskCallback : public BaseTask::Callback
{
private:
    weak<const BaseTask>    m_task;
    u32                     m_dependencies;
    mutable i_u32           m_dependenciesCompleted;
protected:
    void onAdded(weak<const BaseTask> t) override;
    void onRemoved(weak<const BaseTask> t) override;
    void onCompleted(weak<Scheduler> sc, weak<const BaseTask> t) override;
public:
    ChainTaskCallback(weak<const BaseTask> task);
    ~ChainTaskCallback();
};

template< typename Body >
class Task : public BaseTask
{
private:
    mutable Body m_body;
public:
    Task(const istring& name, color32 color, const Body& body, bool simultaneous = false);
    void runTask(weak<Scheduler> sc) const override;
};

}

#include    <system/scheduler/task.inl>

/*****************************************************************************/
#endif
