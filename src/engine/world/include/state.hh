/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_WORLD_STATE_HH_
#define BE_WORLD_STATE_HH_
/*****************************************************************************/
#include    <world/component.script.hh>
#include    <minitl/container/bitset.hh>

namespace BugEngine { namespace World
{


class State : public minitl::pointer
{
private:
    struct Transition
    {
        minitl::bitset<128> m_requirements;
        weak<State>         m_transition;
    };
public:
    State();
    ~State();
};


}}


/*****************************************************************************/
#endif
