/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_SCHEDULER_TASK_ITASK_HH_
#define BE_SYSTEM_SCHEDULER_TASK_ITASK_HH_
/*****************************************************************************/
#include    <system/scheduler/scheduler.hh>
#include    <maths/vector.hh>

namespace BugEngine
{

class be_api(SYSTEM) ITask : public minitl::refcountable
{
public:
    class CallbackConnection;
    friend class CallbackConnection;
    friend class ScheduledTasks::ITaskItem;
public:
    class ICallback : public minitl::refcountable
    {
    protected:
        ICallback();
    public:
        enum CallbackStatus
        {
            Pending,
            Completed
        };

        virtual ~ICallback();

        virtual void onCompleted(weak<Scheduler> scheduler, weak<const ITask> task) const = 0;
        virtual void onConnected(weak<ITask> to, CallbackStatus) = 0;
        virtual bool onDisconnected(weak<ITask> from) = 0;
    };
private:
    class ChainCallback : public ICallback
    {
    private:
        weak<ITask> const               m_starts;
        minitl::vector< weak<ITask> >   m_startedBy;
        mutable i_u32                   m_completed;
    public:
        ChainCallback(weak<ITask> task);
        virtual ~ChainCallback();

        virtual void onCompleted(weak<Scheduler> scheduler, weak<const ITask> task) const override;
        virtual void onConnected(weak<ITask> to, CallbackStatus status) override;
        virtual bool onDisconnected(weak<ITask> from) override;
    private:
        ChainCallback(const ChainCallback& other);
        ChainCallback& operator=(const ChainCallback& other);
    };
public:
    const istring               name;
    const color32               color;
    const Scheduler::Priority   priority;
private:
    minitl::list< weak<ICallback> >     m_callbacks;
    ref<ICallback>                      m_start;
    CriticalSection                     m_cs;
private:
    void addCallback(weak<ICallback> callback, ICallback::CallbackStatus status);
    bool removeCallback(weak<ICallback> callback);
public:
    virtual ~ITask();

    virtual void run(weak<Scheduler> scheduler) const = 0;
    void end(weak<Scheduler> scheduler) const;

    weak<ICallback> startCallback();

protected:
    ITask(istring name, color32 color, Scheduler::Priority priority);
private:
    ITask& operator=(const ITask& other);
    ITask(const ITask& other);
};

class be_api(SYSTEM) ITask::CallbackConnection
{
private:
    weak<ITask>             m_task;
    weak<ITask::ICallback>  m_callback;
public:
    CallbackConnection();
    CallbackConnection(weak<ITask> task, weak<ICallback> callback, ICallback::CallbackStatus status = ICallback::Pending);
    CallbackConnection(const CallbackConnection& other);
    CallbackConnection& operator=(const CallbackConnection& other);
    ~CallbackConnection();
};

}

/*****************************************************************************/
#endif
