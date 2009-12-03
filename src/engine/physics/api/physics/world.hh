/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_PHYSICS_WORLD_HH_
#define BE_PHYSICS_WORLD_HH_
/*****************************************************************************/

namespace BugEngine { namespace Physics
{

class be_api(PHYSICS) World : public Object
{
private:
    class WorldImplementation;
private:
    scoped<WorldImplementation> m_implementation;
public:
    World(float3 worldExtents);
    ~World();
    
    void step(float time);

    be_metaclass(PHYSICS,World,Object)
        ref<World> createWithSize(float worldExtentX, float worldExtentY, float worldExtentZ);
    be_properties
        //be_classmethod(createWithSize);
    be_end
};

}}


/*****************************************************************************/
#endif
