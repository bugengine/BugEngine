/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SAMPLES_KERNEL_COMPONENTS_SCRIPT_HH_
#define BE_SAMPLES_KERNEL_COMPONENTS_SCRIPT_HH_
/*****************************************************************************/
#include    <world/component.script.hh>

namespace BugEngine
{

struct A : public World::Component
{
    u32 value;
};

struct B : public World::Component
{
    u32 value;
};

}

/*****************************************************************************/
#endif
