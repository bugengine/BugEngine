/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_PHYSICS_IPHYSICSWORLDMANAGER_HH_
#define BE_PHYSICS_IPHYSICSWORLDMANAGER_HH_
/*****************************************************************************/
#include    <physics/iphysicsworld.hh>

namespace BugEngine { namespace Physics
{

class be_api(PHYSICS) IPhysicsWorldManager : public Object
{
public:
    IPhysicsWorldManager();
    ~IPhysicsWorldManager();
    
    virtual ref<IPhysicsWorld> createWorld(float3 worldextents) const = 0;

    be_metaclass(PHYSICS,IPhysicsWorldManager,Object)
    be_properties
        //be_classmethod(createWorld);
    be_end
};

}}


/*****************************************************************************/
#endif
