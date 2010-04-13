/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <mobile/stdafx.h>
#include    <mobile/worldscene.hh>

namespace BugEngine
{

WorldScene::WorldScene(ref<World> world)
:   m_world(world)
,   m_worldUpdateTask(ref<TaskGroup>::create("worldupdate", color32(0, 0, 255)))
{
    m_worldUpdateTask->addStartTask(m_updateTask);
    m_worldUpdateTask->addEndTask(m_updateTask);
    m_worldUpdateTask->addStartTask(world->updateWorldTask());
    m_worldUpdateTask->addEndTask(world->updateWorldTask());
}

WorldScene::~WorldScene()
{
}

weak<ITask> WorldScene::updateTask() const
{
    return m_worldUpdateTask;
}

}
