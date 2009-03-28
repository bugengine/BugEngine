/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA                                                                  *
* 02110-1301  USA                                                             *
\*****************************************************************************/

#include    <core/stdafx.h>
#include    <core/scheduler/task.hh>


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
        addCallback(new ChainTaskCallback(this));
}

BaseTask::~BaseTask()
{
}

void BaseTask::addCallback(Callback* callback)
{
    m_callbacks.push_back(callback);
    callback->onAdded(this);
}

void BaseTask::removeCallback(Callback* callback)
{
    for(std::list< refptr<Callback> >::iterator it = m_callbacks.begin(); it != m_callbacks.end(); )
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

void BaseTask::run(Scheduler* sc) const
{
    m_taskCount = 0;
    m_taskCompleted = 0;
    runTask(sc);
}

void BaseTask::end(Scheduler* sc) const
{
    for(std::list< refptr<Callback> >::const_iterator it = m_callbacks.begin(); it != m_callbacks.end(); ++it)
        (*it)->onCompleted(sc, this);
}


ChainTaskCallback::ChainTaskCallback(const BaseTask* task)
:   m_task(task)
,   m_dependencies(0)
,   m_dependenciesCompleted(0)
{
}

ChainTaskCallback::~ChainTaskCallback()
{
}

void ChainTaskCallback::onAdded(const BaseTask* t)
{
    UNUSED(t);
    m_dependencies++;
}

void ChainTaskCallback::onRemoved(const BaseTask* t)
{
    UNUSED(t);
    m_dependencies--;
}

void ChainTaskCallback::onCompleted(Scheduler* sc, const BaseTask* t)
{
    UNUSED(t);
    ++m_dependenciesCompleted;
    if(m_dependenciesCompleted == m_dependencies)
        m_task->run(sc);
}

}
