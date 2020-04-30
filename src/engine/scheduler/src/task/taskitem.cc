/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/scheduler/stdafx.h>
#include <bugengine/scheduler/private/taskitem.hh>
#include <bugengine/scheduler/task/task.hh>

namespace BugEngine { namespace Task {

ITaskItem::ITaskItem(weak< ITask > owner) : m_owner(owner)
{
}

ITaskItem::~ITaskItem()
{
}

}}  // namespace BugEngine::Task
