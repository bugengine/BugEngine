/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MOBILE_WORLDSCENE_HH_
#define BE_MOBILE_WORLDSCENE_HH_
/*****************************************************************************/
#include    <mobile/world.hh>
#include    <graphics/scene/scene.hh>
#include    <system/scheduler/task/group.hh>
#include    <system/scheduler/range/onestep.hh>

namespace BugEngine
{

struct Space
{
    typedef range_onestep Node;
};

class be_api(MOBILE) WorldScene : public Graphics::Scene<Space>
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
