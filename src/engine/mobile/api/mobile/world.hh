/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MOBILE_WORLD_HH_
#define BE_MOBILE_WORLD_HH_
/*****************************************************************************/
#include    <system/plugin.hh>
#include    <graphics/world.hh>
#include    <graphics/scene/scene.hh>
#include    <physics/iphysicsworldmanager.hh>
#include    <sound/world.hh>

namespace BugEngine
{

class be_api(MOBILE) World : public Object
{
private:
    Plugin<Physics::IPhysicsWorldManager>   m_physicsSystem;
    ref<Graphics::World>                    m_graphicsWorld;
    ref<Physics::IPhysicsWorld>             m_physicsWorld;
    ref<Sound::World>                       m_soundWorld;
public:
    World(float3 worldExtents);
    ~World();

    ref<BaseTask>    m_task;

    void createView(Graphics::WindowFlags f, ref<Graphics::Scene> scene);

    be_metaclass(MOBILE,World,Object)
    be_properties
        be_property(GraphicWorld)
            [be_read(m_graphicsWorld)]
            [be_write(m_graphicsWorld)];

        be_property(PhysicsWorld)
            [be_read(m_physicsWorld)]
            [be_write(m_physicsWorld)];

        be_property(SoundWorld)
            [be_read(m_soundWorld)]
            [be_write(m_soundWorld)];
    be_end
};

}


/*****************************************************************************/
#endif
