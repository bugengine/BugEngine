/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <stdafx.h>
#include    <kernelobject.hh>
#include    <scheduler/kernel/imemoryprovider.hh>

namespace BugEngine
{

CPUKernelTask::CPUKernelTask(weak<KernelObject> object)
    :   object(object)
    ,   params()
    ,   splitCount(i_u32::Zero)
{
}

CPUKernelTask::Range CPUKernelTask::prepare()
{
    splitCount = i_u32::Zero;
    return Range(splitCount);
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
    virtual void onCompleted(weak<Scheduler> scheduler, weak<const Task::ITask> task) const override
    {
        be_checked_cast<const Task::Task<CPUKernelTask> >(task)->body.sourceTask->completed(scheduler);
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
    :   m_kernel(name, "kernel")
    ,   m_entryPoint(m_kernel.getSymbol<KernelMain>("_kmain"))
    ,   m_task(scoped< Task::Task<CPUKernelTask> >::create(Arena::task(), istring(name.str().name), Colors::make(231, 231, 231, 0), CPUKernelTask(this), Scheduler::Immediate))
    ,   m_callback(scoped<Callback>::create(Arena::task()))
    ,   m_callbackConnection(m_task, m_callback)
{
    be_debug("kernel entry point: %p"|m_entryPoint);
}

KernelObject::~KernelObject()
{
}

void KernelObject::run(const u32 index, const u32 total, const KernelParameterList params)
{
    (*m_entryPoint)(index, total, params);
}

}
