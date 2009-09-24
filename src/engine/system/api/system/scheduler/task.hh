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

class be_api(CORE) BaseTask : public minitl::refcountable<void>
{
    friend class ScheduledTasks::BaseTaskItem;
public:
    class Callback : public minitl::refcountable<void>
    {
        friend class BaseTask;
    protected:
        virtual void onAdded(const BaseTask* t) = 0;
        virtual void onRemoved(const BaseTask* t) = 0;
        virtual void onCompleted(Scheduler* sc, const BaseTask* t) = 0;
    };
public:
    const istring           name;
    const color32           color;
private:
    std::list< refptr<Callback> >   m_callbacks;
    mutable i_u32                   m_taskCount;
    mutable i_u32                   m_taskCompleted;
protected:
    virtual void runTask(Scheduler* sc) const = 0;
public:
    BaseTask(const istring& name, color32 color, bool simultaneous = false);
    ~BaseTask();

    void addCallback(Callback* c);
    void removeCallback(Callback* c);

    void run(Scheduler* sc) const;
    void end(Scheduler* sc) const;
private:
    BaseTask& operator=(const BaseTask& other);
    BaseTask(const BaseTask& other);
};

class be_api(CORE) ChainTaskCallback : public BaseTask::Callback
{
private:
    const BaseTask*         m_task;
    u32                     m_dependencies;
    mutable i_u32           m_dependenciesCompleted;
protected:
    void onAdded(const BaseTask* t) override;
    void onRemoved(const BaseTask* t) override;
    void onCompleted(Scheduler* sc, const BaseTask* t) override;
public:
    ChainTaskCallback(const BaseTask* task);
    ~ChainTaskCallback();
};

template< typename Body >
class Task : public BaseTask
{
private:
    mutable Body m_body;
public:
    Task(const istring& name, color32 color, const Body& body, bool simultaneous = false);
    void runTask(Scheduler* sc) const override;
};

}

#include    <system/scheduler/task.inl>

/*****************************************************************************/
#endif
