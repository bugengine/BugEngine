/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_PHYSICS_IPHYSICSWORLD_HH_
#define BE_PHYSICS_IPHYSICSWORLD_HH_
/*****************************************************************************/

namespace BugEngine { namespace Physics
{

class be_api(PHYSICS) IPhysicsWorld : public Object
{
public:
    IPhysicsWorld();
    ~IPhysicsWorld();
    
    virtual void step(float time) = 0;

    be_metaclass(PHYSICS,IPhysicsWorld,Object)
    be_properties
        //be_classmethod(createWithSize);
    be_end
};

}}


/*****************************************************************************/
#endif
