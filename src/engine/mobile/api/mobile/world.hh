/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MOBILE_WORLD_HH_
#define BE_MOBILE_WORLD_HH_
/*****************************************************************************/
#include    <system/plugin.hh>
#include    <graphics/world.hh>
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
    ref< BaseTask::Callback >                   m_start;
    ref< BaseTask::Callback >                   m_startSwap;
    weak< BaseTask::Callback >                  m_end;
    ref<Graphics::World>                        m_graphicsWorld;
    ref<Physics::IPhysicsWorld>                 m_physicsWorld;
    ref<Audio::World>                           m_soundWorld;
    std::vector< ref< BaseTask > >              m_tasks;
    std::vector< ref< BaseTask::Callback > >    m_edges;
private:
    void copyWorld();
    void swapWorld();
public:
    World(float3 worldExtents, weak<BaseTask::Callback> endJob);
    ~World();

    void run(weak<Scheduler> scheduler);

    weak< BaseTask::Callback > getStart() { return m_start; }
    weak< BaseTask::Callback > getEnd()   { return m_end; }

    be_metaclass(MOBILE,World,Object)
    be_properties
    be_end
};

}


/*****************************************************************************/
#endif
