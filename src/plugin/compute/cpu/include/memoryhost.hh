/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_COMPUTE_CPU_MEMORYHOST_HH_
#define BE_COMPUTE_CPU_MEMORYHOST_HH_
/**************************************************************************************************/
#include    <stdafx.h>
#include    <scheduler/kernel/imemoryhost.hh>

namespace BugEngine { namespace KernelScheduler { namespace CPU
{

class MemoryHost : public IMemoryHost
{
public:
    MemoryHost();
    ~MemoryHost();
};

}}}

/**************************************************************************************************/
#endif
