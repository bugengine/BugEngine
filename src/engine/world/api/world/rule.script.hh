/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_WORLD_IRULE_SCRIPT_HH_
#define BE_WORLD_IRULE_SCRIPT_HH_
/*****************************************************************************/
#include    <world/entity.script.hh>
#include    <world/component.script.hh>
#include    <system/scheduler/task/itask.hh>
#include    <rtti/tags/editor.script.hh>
#include    <core/memory/kernel/stream.hh>

namespace BugEngine { namespace World
{

class IRule : public minitl::refcountable
{
public:
    virtual weak<Task::ITask> task() const = 0;
};


}}

/*****************************************************************************/
#endif
