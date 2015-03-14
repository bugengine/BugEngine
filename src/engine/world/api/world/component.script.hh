/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_WORLD_COMPONENT_SCRIPT_HH_
#define BE_WORLD_COMPONENT_SCRIPT_HH_
/**************************************************************************************************/
#define     be_pod              struct

namespace BugEngine { namespace World
{

class World;

struct Component
{
    void created(World& world)      { (void)(world); }
    void destroyed(World& world)    { (void)(world); }
};

}}


/**************************************************************************************************/
#endif
