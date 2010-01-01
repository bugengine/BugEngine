/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_PHYSICS_IPHYSICSWORLD_HH_
#define BE_PHYSICS_IPHYSICSWORLD_HH_
/*****************************************************************************/
#include    <system/scheduler/task.hh>

namespace BugEngine { namespace Physics
{

class be_api(PHYSICS) IPhysicsWorld : public Object
{
private:
    ref< BaseTask::Callback >                   m_start;
    weak< BaseTask::Callback >                  m_end;
    std::vector< ref< BaseTask > >              m_tasks;
    std::vector< ref< BaseTask::Callback > >    m_edges;
public:
    IPhysicsWorld(weak<BaseTask::Callback> endJob);
    ~IPhysicsWorld();
    
    virtual void step() = 0;

    weak< BaseTask::Callback > getStart() { return m_start; }
    weak< BaseTask::Callback > getEnd()   { return m_end; }

    be_metaclass(PHYSICS,IPhysicsWorld,Object)
    be_properties
    be_end
};

}}


/*****************************************************************************/
#endif
