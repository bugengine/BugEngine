/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MOBILE_WORLD_HH_
#define BE_MOBILE_WORLD_HH_
/*****************************************************************************/
#include    <graphics/world.hh>
#include    <graphics/scene/scene.hh>
#include    <physics/world.hh>
#include    <sound/world.hh>

namespace BugEngine
{

class MOBILEEXPORT World : public Object
{
private:
    refptr<Graphics::World>         m_graphicsSystem;
    refptr<Physics::World>          m_physicsSystem;
    refptr<Sound::World>            m_soundSystem;
public:
    World(float3 worldExtents);
    ~World();

    refptr<BaseTask>    m_task;

    void createView(Graphics::WindowFlags f, refptr<Graphics::Scene> scene);

    be_metaclass(MOBILEEXPORT,World,Object)
    be_properties
        be_property(GraphicSystem)
            [be_read(m_graphicsSystem)]
            [be_write(m_graphicsSystem)];

        be_property(PhysicsSystem)
            [be_read(m_physicsSystem)]
            [be_write(m_physicsSystem)];

        be_property(SoundSystem)
            [be_read(m_soundSystem)]
            [be_write(m_soundSystem)];
    be_end
};

}


/*****************************************************************************/
#endif
