/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_WORLD_STATE_HH_
#define BE_WORLD_STATE_HH_
/*****************************************************************************/
#include    <world/component.script.hh>
#include    <minitl/container/bitset.hh>

namespace BugEngine { namespace World
{

class Rule;
class IStorage;

class State : public minitl::pointer
{
private:
    struct Group
    {
        minitl::vector< weak<Rule> >    rules;
        u32                             count;
        u32                             componentMask;
    };
    minitl::vector< Group >             m_groups;
    minitl::array< scoped<IStorage> >   m_storage;
public:
    State();
    ~State();
};


}}


/*****************************************************************************/
#endif
