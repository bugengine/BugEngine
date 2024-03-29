/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/scheduler/stdafx.h>
#include <bugengine/scheduler/task/itask.hh>

namespace BugEngine { namespace Task {

ITask::ITask(istring name, color32 color, Scheduler::Priority priority,
             Scheduler::Affinity affinity)
    : name(name)
    , color(color)
    , priority(priority)
    , affinity(affinity)
    , m_callbacks(Arena::task())
    , m_start(ref< ChainCallback >::create(Arena::task(), this))
{
}

ITask::~ITask()
{
    ScopedCriticalSection scope(m_cs);
    for(minitl::vector< weak< ICallback > >::iterator it = m_callbacks.begin();
        it != m_callbacks.end(); ++it)
    {
        bool result = (*it)->onDisconnected(this);
        be_forceuse(result);
        be_assert(result, "unable to disconnect callback");
    }
}

void ITask::completed(weak< Scheduler > sc)
{
    ScopedCriticalSection scope(m_cs);
    for(minitl::vector< weak< ICallback > >::iterator it = m_callbacks.begin();
        it != m_callbacks.end(); ++it)
    {
        (*it)->onCompleted(sc, this);
    }
}

void ITask::addCallback(weak< ICallback > callback, ICallback::CallbackStatus status)
{
    ScopedCriticalSection scope(m_cs);
    m_callbacks.push_back(callback);
    callback->onConnected(this, status);
}

bool ITask::removeCallback(weak< ICallback > callback)
{
    ScopedCriticalSection scope(m_cs);
    for(minitl::vector< weak< ICallback > >::iterator it = m_callbacks.begin();
        it != m_callbacks.end(); ++it)
    {
        if(*it == callback)
        {
            bool result = (*it)->onDisconnected(this);
            m_callbacks.erase(it);
            be_forceuse(result);
            be_assert(result, "unable to disconnect callback");
            return true;
        }
    }
    return false;
}

weak< ITask::ICallback > ITask::startCallback()
{
    return m_start;
}

/*----------------------------------------------------------------------------*/

ITask::ICallback::ICallback()
{
}

ITask::ICallback::~ICallback()
{
}

/*----------------------------------------------------------------------------*/

ITask::ChainCallback::ChainCallback(weak< ITask > task)
    : ICallback()
    , m_starts(task)
    , m_startedBy(Arena::task())
    , m_completed(i_u32::create(0))
{
}

ITask::ChainCallback::~ChainCallback()
{
    while(!m_startedBy.empty())
    {
        bool result = m_startedBy.back()->removeCallback(this);
        be_forceuse(result);
        be_assert(result, "unable to disconnect from task %s" | m_startedBy.back()->name.c_str());
    }
}

void ITask::ChainCallback::onCompleted(weak< Scheduler > scheduler, weak< ITask > /*task*/)
{
    if(++m_completed == m_startedBy.size())
    {
        m_completed = 0;
        m_starts->schedule(scheduler);
    }
}

void ITask::ChainCallback::onConnected(weak< ITask > to, CallbackStatus status)
{
    m_startedBy.push_back(to);
    if(status == Completed)
    {
        m_completed++;
    }
}

bool ITask::ChainCallback::onDisconnected(weak< ITask > from)
{
    for(minitl::vector< weak< ITask > >::iterator it = m_startedBy.begin(); it != m_startedBy.end();
        ++it)
    {
        if((*it) == from)
        {
            m_startedBy.erase(it);
            return true;
        }
    }
    return false;
}

/*----------------------------------------------------------------------------*/

ITask::CallbackConnection::CallbackConnection() : m_task(0), m_callback(0)
{
}

ITask::CallbackConnection::CallbackConnection(weak< ITask > task, weak< ICallback > callback,
                                              ICallback::CallbackStatus status)
    : m_task(task)
    , m_callback(callback)
{
    if(m_task)
    {
        m_task->addCallback(m_callback, status);
    }
}

ITask::CallbackConnection::CallbackConnection(const CallbackConnection& other)
    : m_task(other.m_task)
    , m_callback(other.m_callback)
{
    if(m_task)
    {
        m_task->addCallback(m_callback, ICallback::Pending);
    }
}

ITask::CallbackConnection& ITask::CallbackConnection::operator=(const CallbackConnection& other)
{
    if(m_task)
    {
        bool result = m_task->removeCallback(m_callback);
        be_forceuse(result);
        be_assert(result, "could not disconnect callback from task %s" | m_task->name);
    }
    m_task     = other.m_task;
    m_callback = other.m_callback;
    if(m_task)
    {
        m_task->addCallback(m_callback, ICallback::Pending);
    }
    return *this;
}

ITask::CallbackConnection::~CallbackConnection()
{
    if(m_task)
    {
        bool result = m_task->removeCallback(m_callback);
        be_forceuse(result);
        be_assert(result, "could not disconnect callback from task %s" | m_task->name);
    }
}

}}  // namespace BugEngine::Task
