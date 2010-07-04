/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MOBILE_WORLDSCENE_HH_
#define BE_MOBILE_WORLDSCENE_HH_
/*****************************************************************************/
#include    <mobile/world.hh>
#include    <graphics/scene/scene.hh>
#include    <system/scheduler/task/group.hh>

namespace BugEngine
{

struct ScenePart
{
    typedef int Node;
};

class be_api(MOBILE) WorldScene : public Graphics::Scene<ScenePart>
{
private:
    ref<World>      m_world;
    ref<TaskGroup>  m_worldUpdateTask;
public:
    WorldScene(ref<World> world);
    virtual ~WorldScene();

    virtual weak<ITask> updateTask() const override;
};

}


/*****************************************************************************/
#endif
