/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_SCHEDULER_TASK_GROUP_HH_
#define BE_SYSTEM_SCHEDULER_TASK_GROUP_HH_
/*****************************************************************************/
#include    <system/scheduler/task/itask.hh>

namespace BugEngine { namespace Task
{

class be_api(SYSTEM) TaskGroup : public ITask
{
public:
    class TaskStartConnection;
    friend class TaskStartConnection;
    class TaskEndConnection;
    friend class TaskEndConnection;
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
        virtual bool onDisconnected(weak<ITask> to) override;
    };
    friend class Callback;
public:
    TaskGroup(istring name, color32 color);
    ~TaskGroup();
public: //ITask
    virtual void schedule(weak<Scheduler> scheduler) const override;
public:
    void addStartTask(weak<ITask> task);
    bool removeStartTask(weak<ITask> task);
private:
    minitl::vector< weak<ITask> >   m_startTasks;
    i_u32                           m_endTaskCount;
    ref<Callback>                   m_completionCallback;
};

class be_api(SYSTEM) TaskGroup::TaskStartConnection
{
private:
    weak<TaskGroup> m_group;
    weak<ITask>     m_task;
public:
    TaskStartConnection();
    TaskStartConnection(weak<TaskGroup> group, weak<ITask> task);
    TaskStartConnection(const TaskStartConnection& other);
    TaskStartConnection& operator=(const TaskStartConnection& other);
    ~TaskStartConnection();
};

class be_api(SYSTEM) TaskGroup::TaskEndConnection
{
private:
    weak<TaskGroup>     m_group;
    weak<ITask>         m_task;
    CallbackConnection  m_callback;
public:
    TaskEndConnection();
    TaskEndConnection(weak<TaskGroup> group, weak<ITask> task);
    TaskEndConnection(const TaskEndConnection& other);
    TaskEndConnection& operator=(const TaskEndConnection& other);
    ~TaskEndConnection();
};

}}

/*****************************************************************************/
#endif
