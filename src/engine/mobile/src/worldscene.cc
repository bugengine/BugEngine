/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <mobile/stdafx.h>
#include    <mobile/worldscene.script.hh>

namespace BugEngine
{

WorldScene::WorldScene(ref<World> world)
:   m_world(world)
{
}

WorldScene::~WorldScene()
{
}

weak<ITask> WorldScene::updateTask() const
{
    return m_world->updateWorldTask();
}

}

