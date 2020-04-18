/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_WORLD_COMPONENTMEMORYHOST_HH_
#define BE_WORLD_COMPONENTMEMORYHOST_HH_
/**************************************************************************************************/
#include    <world/stdafx.h>
#include    <scheduler/kernel/imemoryhost.hh>

namespace BugEngine { namespace World
{

class be_api(WORLD) ComponentMemoryHost : public KernelScheduler::IMemoryHost
{
public:
    static const istring s_hostId;

public:
    ComponentMemoryHost();
    ~ComponentMemoryHost();
};

}}

/**************************************************************************************************/
#endif
