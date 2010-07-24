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
    class ICallback : public minitl::refcountable
    {
    protected:
        ICallback();
    public:
        enum CallbackStatus
        {
            CallbackStatus_Pending,
            CallbackStatus_Completed
        };

        virtual ~ICallback();

        virtual void onCompleted(weak<Scheduler> scheduler, weak<const ITask> task) const = 0;
        virtual void onConnected(weak<ITask> to, CallbackStatus) = 0;
        virtual void onDisconnected(weak<ITask> from) = 0;
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
        virtual void onDisconnected(weak<ITask> from) override;
    private:
        ChainCallback(const ChainCallback& other);
        ChainCallback& operator=(const ChainCallback& other);
    };
public:
    const istring name;
    const color32 color;
private:
    minitl::vector< weak<ICallback> >   m_callbacks;
    ref<ICallback>                      m_start;
public:
    virtual ~ITask();

    virtual void run(weak<Scheduler> scheduler) const = 0;
    void end(weak<Scheduler> scheduler) const;

    void addCallback(weak<ICallback> callback, ICallback::CallbackStatus status = ICallback::CallbackStatus_Pending);
    void removeCallback(weak<ICallback> callback);
    weak<ICallback> startCallback();

protected:
    ITask(istring name, color32 color);
private:
    ITask& operator=(const ITask& other);
    ITask(const ITask& other);
};

}

/*****************************************************************************/
#endif
