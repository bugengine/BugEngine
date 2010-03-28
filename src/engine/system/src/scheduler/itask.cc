/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/scheduler/task/itask.hh>

namespace BugEngine
{

ITask::ITask(istring name, color32 color)
:   color(color)
,   name(name)
{
}

ITask::~ITask()
{
    for(minitl::vector< weak<ICallback> >::iterator it = m_callbacks.begin(); it != m_callbacks.end(); ++it)
    {
        (*it)->onDisconnected(this);
    }
}

void ITask::end(weak<Scheduler> sc) const
{
    for(minitl::vector< weak<ICallback > >::const_iterator it = m_callbacks.begin(); it != m_callbacks.end(); ++it)
        (*it)->onCompleted(sc, this);
}

void ITask::addCallback(weak<ICallback> callback)
{
    m_callbacks.push_back(callback);
}

void ITask::removeCallback(weak<ICallback> callback)
{
    for(minitl::vector< weak<ICallback > >::iterator it = m_callbacks.begin(); it != m_callbacks.end(); )
    {
        if(*it == callback)
            it = m_callbacks.erase(it);
        else
            ++it;
    }
}
/*----------------------------------------------------------------------------*/

ITask::ICallback::ICallback()
{
}

ITask::ICallback::~ICallback()
{
}

/*----------------------------------------------------------------------------*/

ITask::ChainCallback::ChainCallback()
:   ICallback()
,   m_starts()
,   m_startedBy(0)
,   m_completed(0)
{
}

ITask::ChainCallback::~ChainCallback()
{
}

void ITask::ChainCallback::onCompleted(weak<Scheduler> scheduler, weak<const ITask> task) const
{
    if(++m_completed == m_startedBy)
    {
        m_completed = 0;
        for(minitl::vector< weak<ITask> >::const_iterator it = m_starts.begin(); it != m_starts.end(); ++it)
        {
            (*it)->run(scheduler);
        }
    }
}

void ITask::ChainCallback::onConnected(weak<ITask> to)
{
    m_startedBy++;
}

void ITask::ChainCallback::onDisconnected(weak<ITask> from)
{
    m_startedBy--;
}

void ITask::ChainCallback::makeStart(weak<ITask> task)
{
    m_starts.push_back(task);
}

}
