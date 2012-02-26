/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_WORLD_STATE_HH_
#define BE_WORLD_STATE_HH_
/*****************************************************************************/
#include    <world/entity.script.hh>

namespace BugEngine { namespace World
{


class State : public minitl::pointer
{
private:
    weak<State> m_transitions[s_maxComponentCount];
public:
    State();
    ~State();
};


}}


/*****************************************************************************/
#endif

