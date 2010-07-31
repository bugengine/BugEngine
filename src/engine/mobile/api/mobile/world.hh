/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MOBILE_WORLD_HH_
#define BE_MOBILE_WORLD_HH_
/*****************************************************************************/
#include    <system/plugin.hh>
#include    <physics/iworld.hh>
#include    <audio/iworld.hh>
#include    <system/scheduler/task/itask.hh>

namespace BugEngine
{

class be_api(MOBILE) World : public minitl::refcountable
{
    friend class WorldScene;
private:
    Plugin<Physics::IWorld>         m_physicsSystem;
    Plugin<Audio::IWorld>           m_audioSystem;
    minitl::vector< ref<ITask> >    m_tasks;
private:
    void copyWorld();
    void updateWorld();
private: // friend class WorldScene
    weak<ITask> updateWorldTask();
public:
    World(istring physics, istring audio, float3 worldExtents);
    ~World();
};

}


/*****************************************************************************/
#endif
