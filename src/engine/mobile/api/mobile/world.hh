/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MOBILE_WORLD_HH_
#define BE_MOBILE_WORLD_HH_
/*****************************************************************************/
#include    <system/plugin.hh>
#include    <graphics/scene/scene.hh>
#include    <physics/iphysicsworldmanager.hh>
#include    <audio/world.hh>
#include    <system/scheduler/task.hh>

namespace BugEngine
{

class be_api(MOBILE) World : public Object
{
private:
    Plugin<Physics::IPhysicsWorldManager>       m_physicsSystem;
    Plugin<Audio::WorldManager>                 m_audioSystem;
    ref<Physics::IPhysicsWorld>                 m_physicsWorld;
    ref<Audio::World>                           m_audioWorld;
private:
    void copyWorld();
    void swapWorld();
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
