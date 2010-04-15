/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_SCHEDULER_TASK_GROUP_HH_
#define BE_SYSTEM_SCHEDULER_TASK_GROUP_HH_
/*****************************************************************************/
#include    <system/scheduler/task/itask.hh>

namespace BugEngine
{

class be_api(SYSTEM) TaskGroup : public ITask
{
private:
    class Callback : public ICallback
    {
    private:
        weak< TaskGroup > m_owner;
        mutable i_u32     m_completed;
    public:
        Callback(weak< TaskGroup > owner);
        virtual ~Callback();

        virtual void onCompleted(weak<Scheduler> scheduler, weak<const ITask> task) const override;
        virtual void onConnected(weak<ITask> to, CallbackStatus status) override;
        virtual void onDisconnected(weak<ITask> to) override;
    };
    friend class Callback;
public:
    TaskGroup(istring name, color32 color);
    ~TaskGroup();
public: //ITask
    virtual void run(weak<Scheduler> scheduler) const override;
public:
    void addStartTask(weak<ITask> task);
    void addEndTask(weak<ITask> task);
private:
    minitl::vector< weak<ITask> >   m_startTasks;
    minitl::vector< weak<ITask> >   m_endTasks;
    ref<Callback>                   m_completionCallback;
};

}

/*****************************************************************************/
#endif
