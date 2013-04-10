/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_WORLD_COMPONENT_SCRIPT_HH_
#define BE_WORLD_COMPONENT_SCRIPT_HH_
/*****************************************************************************/

#define     be_pod              struct

namespace BugEngine { namespace World
{

struct Component
{
    void created()      {}
    void destroyed()    {}
};

}}


/*****************************************************************************/
#endif

