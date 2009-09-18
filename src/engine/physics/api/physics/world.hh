/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_PHYSICS_WORLD_HH_
#define BE_PHYSICS_WORLD_HH_
/*****************************************************************************/

namespace BugEngine { namespace Physics
{

class PHYSICSEXPORT World : public Object
{
private:
    class WorldImplementation;
private:
    scopedptr<WorldImplementation>              m_implementation;
public:
    World(float3 worldExtents);
    ~World();
    
    void step(float time);

    be_metaclass(PHYSICSEXPORT,World,Object)
        refptr<World> createWithSize(float worldExtentX, float worldExtentY, float worldExtentZ);
    be_properties
        //be_classmethod(createWithSize);
    be_end
};

}}


/*****************************************************************************/
#endif
