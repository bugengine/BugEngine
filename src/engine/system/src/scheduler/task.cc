/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/scheduler/task.hh>


namespace BugEngine
{

BaseTask::BaseTask(const istring& name, color32 color, bool simultaneous)
:   name(name)
,   color(color)
,   m_callbacks()
,   m_taskCount(0)
,   m_taskCompleted(0)
{
    if(!simultaneous)
        addCallback(ref<ChainTaskCallback>::create(this));
}

BaseTask::~BaseTask()
{
}

void BaseTask::addCallback(ref<Callback> callback)
{
    m_callbacks.push_back(callback);
    callback->onAdded(this);
}

void BaseTask::removeCallback(ref<Callback> callback)
{
    for(std::list< ref<Callback> >::iterator it = m_callbacks.begin(); it != m_callbacks.end(); )
    {
        if(*it == callback)
        {
            callback->onRemoved(this);
            it = m_callbacks.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void BaseTask::run(weak<Scheduler> sc) const
{
    m_taskCount = 0;
    m_taskCompleted = 0;
    runTask(sc);
}

void BaseTask::end(weak<Scheduler> sc) const
{
    for(std::list< ref<Callback> >::const_iterator it = m_callbacks.begin(); it != m_callbacks.end(); ++it)
        (*it)->onCompleted(sc, this);
}


ChainTaskCallback::ChainTaskCallback(weak<const BaseTask> task)
:   m_task(task)
,   m_dependencies(0)
,   m_dependenciesCompleted(0)
{
}

ChainTaskCallback::~ChainTaskCallback()
{
}

void ChainTaskCallback::onAdded(weak<const BaseTask> t)
{
    UNUSED(t);
    m_dependencies++;
}

void ChainTaskCallback::onRemoved(weak<const BaseTask> t)
{
    UNUSED(t);
    m_dependencies--;
}

void ChainTaskCallback::onCompleted(weak<Scheduler> sc, weak<const BaseTask> t)
{
    UNUSED(t);
    ++m_dependenciesCompleted;
    if(m_dependenciesCompleted == m_dependencies)
        m_task->run(sc);
}

}
