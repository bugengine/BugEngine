/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_PHYSICS_IWORLD_HH_
#define BE_PHYSICS_IWORLD_HH_
/*****************************************************************************/
#include    <system/scheduler/task/task.hh>

namespace BugEngine { namespace Physics
{

class be_api(PHYSICS) IWorld : public minitl::refcountable
{
public:
    IWorld();
    ~IWorld();
    
    virtual void step() = 0;
};

}}


/*****************************************************************************/
#endif
