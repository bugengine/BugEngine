/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/scheduler/task.hh>
#include <algorithm>

namespace BugEngine
{

BaseTask::BaseTask(const istring& name, color32 color)
:   name(name)
,   color(color)
,   m_taskCount(0)
,   m_taskCompleted(0)
{
}

BaseTask::~BaseTask()
{
    while(!m_starting.empty())
        m_starting[0]->disconnectFrom(this);
    while(!m_startedBy.empty())
        m_startedBy[0]->disconnectTo(this);
}

void BaseTask::run(weak<Scheduler> sc) const
{
    m_taskCount = 0;
    m_taskCompleted = 0;
    runTask(sc);
}

void BaseTask::end(weak<Scheduler> sc) const
{
    for(CallbackList::const_iterator it = m_starting.begin(); it != m_starting.end(); ++it)
        (*it)->completed(sc, this);
}




BaseTask::Callback::Callback()
:   m_triggeredCompleted(0)
{
}

BaseTask::Callback::~Callback()
{
    for(TaskList::iterator it = m_triggeredBy.begin(); it != m_triggeredBy.end(); ++it)
    {
        for(CallbackList::const_iterator cb = (*it)->m_starting.begin(); cb != (*it)->m_starting.end(); )
        {
            if(*cb == this)
            {
                cb = (*it)->m_starting.erase(cb);
            }
            else
            {
                ++cb;
            }
        }
    }
    for(TaskList::iterator it = m_triggers.begin(); it != m_triggers.end(); ++it)
    {
        for(CallbackList::const_iterator cb = (*it)->m_startedBy.begin(); cb != (*it)->m_startedBy.end(); )
        {
            if(*cb == this)
            {
                cb = (*it)->m_startedBy.erase(cb);
            }
            else
            {
                ++cb;
            }
        }
    }
}

void BaseTask::Callback::connectFrom(weak<BaseTask> t)
{
    m_triggeredBy.push_back(t);
    t->m_starting.push_back(this);
}

void BaseTask::Callback::disconnectFrom(weak<BaseTask> t)
{
    {
        TaskList::iterator it = std::find(m_triggeredBy.begin(), m_triggeredBy.end(), t);
        if(it != m_triggeredBy.end())
            m_triggeredBy.erase(it);
        else
            be_notreached();
    }
    {
        CallbackList::iterator it = std::find(t->m_starting.begin(), t->m_starting.end(), this);
        if(it != t->m_starting.end())
            t->m_starting.erase(it);
        else
            be_notreached();
    }
}

void BaseTask::Callback::connectTo(weak<BaseTask> t)
{
    m_triggers.push_back(t);
    t->m_startedBy.push_back(this);
}

void BaseTask::Callback::disconnectTo(weak<BaseTask> t)
{
    {
        TaskList::iterator it = std::find(m_triggers.begin(), m_triggers.end(), t);
        if(it != m_triggers.end())
            m_triggers.erase(it);
        else
            be_notreached();
    }
    {
        CallbackList::iterator it = std::find(t->m_startedBy.begin(), t->m_startedBy.end(), this);
        if(it != t->m_startedBy.end())
            t->m_startedBy.erase(it);
        else
            be_notreached();
    }
}

void BaseTask::Callback::completed(weak<Scheduler> sc, weak<const BaseTask> t)
{
    UNUSED(t);
    if(++m_triggeredCompleted == m_triggeredBy.size())
    {
        m_triggeredCompleted = 0;
        for(TaskList::const_iterator it = m_triggers.begin(); it != m_triggers.end(); ++it)
        {
            (*it)->run(sc);
        }
    }
}

}
