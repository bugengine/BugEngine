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
        virtual ~ICallback();

        virtual void onCompleted(weak<Scheduler> scheduler, weak<const ITask> task) const = 0;
        virtual void onConnected(weak<ITask> to) = 0;
        virtual void onDisconnected(weak<ITask> from) = 0;
    };
    class ChainCallback : public ICallback
    {
    private:
        minitl::vector< weak<ITask> >   m_starts;
        i_u32                           m_startedBy;
        mutable i_u32                   m_completed;
    public:
        ChainCallback();
        virtual ~ChainCallback();

        virtual void onCompleted(weak<Scheduler> scheduler, weak<const ITask> task) const override;
        virtual void onConnected(weak<ITask> to) override;
        virtual void onDisconnected(weak<ITask> from) override;
        void makeStart(weak<ITask> task);
    };
private:
    minitl::vector< weak<ICallback> > m_callbacks;
public:
    const istring name;
    const color32 color;
public:
    virtual ~ITask();
public:
    virtual void run(weak<Scheduler> scheduler) const = 0;
    void end(weak<Scheduler> scheduler) const;

    void addCallback(weak<ICallback> callback);
    void removeCallback(weak<ICallback> callback);

protected:
    ITask(istring name, color32 color);
private:
    ITask& operator=(const ITask& other);
    ITask(const ITask& other);
};

}

/*****************************************************************************/
#endif
