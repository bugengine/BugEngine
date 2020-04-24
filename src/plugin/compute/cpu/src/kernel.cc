/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <cpu/stdafx.h>
#include    <kernelobject.hh>

namespace BugEngine { namespace KernelScheduler { namespace CPU
{

CPUKernelTask::CPUKernelTask(weak<KernelObject> object)
    :   object(object)
    ,   params(Arena::task(), 0)
{
}

CPUKernelTask::Range CPUKernelTask::prepare()
{
    return Range(1);
}

void CPUKernelTask::operator()(const Range& range) const
{
    object->run(range.index, range.total, params);
}

class KernelObject::Callback : public Task::ITask::ICallback
{
public:
    Callback()
    {
    }
private:
    virtual void onCompleted(weak<BugEngine::Scheduler> scheduler, weak<Task::ITask> task) override
    {
        weak< Task::Task<CPUKernelTask> > t = be_checked_cast<Task::Task<CPUKernelTask> >(task);
        weak<Task::ITask> sourceTask = t->body.sourceTask;
        t->body.sourceTask.clear();
        sourceTask->completed(scheduler);
    }
    virtual void onConnected(weak<Task::ITask> /*to*/, CallbackStatus /*status*/) override
    {
    }
    virtual bool onDisconnected(weak<Task::ITask> /*from*/) override
    {
        return true;
    }
};


KernelObject::KernelObject(const inamespace& name)
    :   m_kernel(name, ipath("kernel"))
    ,   m_entryPoint(m_kernel.getSymbol<KernelMain>("_kmain"))
    ,   m_task(scoped< Task::Task<CPUKernelTask> >::create(Arena::task(), istring(name.str().name),
                                                           Colors::make(231, 231, 231, 0),
                                                           CPUKernelTask(this)))
    ,   m_callback(scoped<Callback>::create(Arena::task()))
    ,   m_callbackConnection(m_task, m_callback)
{
    be_debug("kernel entry point: %p" | m_entryPoint);
}

KernelObject::~KernelObject()
{
}

void KernelObject::run(const u32 index, const u32 total,
                       const minitl::array< weak<const IMemoryBuffer> >& params)
{

    if (m_entryPoint)
        (*m_entryPoint)(index, total, params);
}

}}}
