/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/scene/scene.hh>
#include    <system/scheduler/task/group.hh>

namespace BugEngine { namespace Graphics
{

IScene::IScene()
:   m_updateTask(ref<TaskGroup>::create("update", color32(0, 0, 255)))
{
}

IScene::~IScene()
{
}

weak<ITask> IScene::updateTask() const
{
    return m_updateTask;
}

}}

