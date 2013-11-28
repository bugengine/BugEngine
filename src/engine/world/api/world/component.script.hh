/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
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

