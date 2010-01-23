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
    private:
        typedef std::vector< weak<BaseTask> > TaskList;
        TaskList        m_triggeredBy;
        TaskList        m_triggers;
        mutable i_u32   m_triggeredCompleted;
    protected:
        void completed(weak<Scheduler> sc, weak<const BaseTask> t);
    public:
        Callback();
        ~Callback();
        enum InitialState
        {
            InitialStatePending,
            InitialStateCompleted
        };

        void connectFrom(weak<BaseTask> t, InitialState initialState = InitialStatePending);
        void connectTo(weak<BaseTask> t);
        void disconnectFrom(weak<BaseTask> t);
        void disconnectTo(weak<BaseTask> t);
    };
    friend class Callback;
public:
    const istring           name;
    const color32           color;
private:
    typedef std::vector< weak<Callback> > CallbackList;
    CallbackList    m_startedBy;
    CallbackList    m_starting;
    mutable i_u32   m_taskCount;
    mutable i_u32   m_taskCompleted;
protected:
    virtual void runTask(weak<Scheduler> sc) const = 0;
public:
    BaseTask(const istring& name, color32 color);
    virtual ~BaseTask();

    void run(weak<Scheduler> sc) const;
    void end(weak<Scheduler> sc) const;
private:
    void addCallback(weak<Callback> callback);
    void removeCallback(weak<Callback> callback);
private:
    BaseTask& operator=(const BaseTask& other);
    BaseTask(const BaseTask& other);
};

template< typename Body >
class Task : public BaseTask
{
private:
    mutable Body m_body;
public:
    Task(const istring& name, color32 color, const Body& body);
    void runTask(weak<Scheduler> sc) const override;
private:
    Task(const Task& other);
    Task& operator=(const Task& other);
};

}

#include    <system/scheduler/task.inl>

/*****************************************************************************/
#endif
