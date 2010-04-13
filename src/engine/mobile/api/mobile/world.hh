/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MOBILE_WORLD_HH_
#define BE_MOBILE_WORLD_HH_
/*****************************************************************************/
#include    <system/plugin.hh>
#include    <physics/iworldmanager.hh>
#include    <audio/iworld.hh>
#include    <system/scheduler/task/itask.hh>

namespace BugEngine
{

class be_api(MOBILE) World : public Object
{
    friend class WorldScene;
private:
    Plugin<Physics::IWorldManager>          m_physicsSystem;
    Plugin<Audio::IWorldManager>            m_audioSystem;
    ref<Physics::IWorld>                    m_physicsWorld;
    ref<Audio::IWorld>                      m_audioWorld;
    minitl::vector< ref<ITask> >            m_tasks;
    int                                     m_worldIndex;
private:
    void copyWorld();
    void updateWorld();
private: // friend class WorldScene
    weak<ITask> updateWorldTask();
public:
    World(float3 worldExtents);
    ~World();


    be_metaclass(MOBILE,World,Object)
    be_properties
    be_end
};

}


/*****************************************************************************/
#endif
