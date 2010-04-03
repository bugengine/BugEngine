/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/scene/scene.hh>
#include    <system/scheduler/task/group.hh>

namespace BugEngine { namespace Graphics
{

Scene::Scene()
:   m_updateTask(ref<TaskGroup>::create("update", color32(0, 0, 255)))
{
}

Scene::~Scene()
{
}

weak<ITask> Scene::updateTask() const
{
    return m_updateTask;
}

}}

