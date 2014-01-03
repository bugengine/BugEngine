/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_WORLD_COMPONENT_SCRIPT_HH_
#define BE_WORLD_COMPONENT_SCRIPT_HH_
/*****************************************************************************/
#define     be_pod              struct
#include    <world/entity.script.hh>

namespace BugEngine { namespace World
{

struct Component
{
    Entity owner;
    void created()      {}
    void destroyed()    {}
};

}}


/*****************************************************************************/
#endif

