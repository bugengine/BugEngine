/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MOBILE_WORLDSCENE_HH_
#define BE_MOBILE_WORLDSCENE_HH_
/*****************************************************************************/
#include    <mobile/world.script.hh>
#include    <graphics/scene/iscene.script.hh>
#include    <system/scheduler/task/group.hh>
#include    <system/scheduler/range/onestep.hh>

namespace BugEngine
{

class be_api(MOBILE) WorldScene : public Graphics::IScene
{
private:
    ref<World>      m_world;
public:
    WorldScene(ref<World> world);
    virtual ~WorldScene();

    virtual weak<ITask> updateTask() const override;
};

}


/*****************************************************************************/
#endif
