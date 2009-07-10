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
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

#ifndef BE_SYSTEM_SCHEDULER_TASK_HH_
#define BE_SYSTEM_SCHEDULER_TASK_HH_
/*****************************************************************************/
#include    <core/threads/event.hh>
#include    <maths/vector.hh>

namespace BugEngine
{

class Scheduler;

namespace ScheduledTasks
{

class BaseTaskItem;

}

class COREEXPORT BaseTask : public minitl::refcountable<void>
{
    friend class ScheduledTasks::BaseTaskItem;
public:
    class Callback : public minitl::refcountable<void>
    {
        friend class BaseTask;
    protected:
        virtual void onAdded(const BaseTask* t) = 0;
        virtual void onRemoved(const BaseTask* t) = 0;
        virtual void onCompleted(Scheduler* sc, const BaseTask* t) = 0;
    };
public:
    const istring           name;
    const color32           color;
private:
    std::list< refptr<Callback> >   m_callbacks;
    mutable i_u32                   m_taskCount;
    mutable i_u32                   m_taskCompleted;
protected:
    virtual void runTask(Scheduler* sc) const = 0;
public:
    BaseTask(const istring& name, color32 color, bool simultaneous = false);
    ~BaseTask();

    void addCallback(Callback* c);
    void removeCallback(Callback* c);

    void run(Scheduler* sc) const;
    void end(Scheduler* sc) const;
private:
    BaseTask& operator=(const BaseTask& other);
    BaseTask(const BaseTask& other);
};

class COREEXPORT ChainTaskCallback : public BaseTask::Callback
{
private:
    const BaseTask*         m_task;
    u32                     m_dependencies;
    mutable i_u32           m_dependenciesCompleted;
protected:
    void onAdded(const BaseTask* t) override;
    void onRemoved(const BaseTask* t) override;
    void onCompleted(Scheduler* sc, const BaseTask* t) override;
public:
    ChainTaskCallback(const BaseTask* task);
    ~ChainTaskCallback();
};

template< typename Body >
class Task : public BaseTask
{
private:
    mutable Body m_body;
public:
    Task(const istring& name, color32 color, const Body& body, bool simultaneous = false);
    void runTask(Scheduler* sc) const override;
};

}

#include    <system/scheduler/task.inl>

/*****************************************************************************/
#endif
