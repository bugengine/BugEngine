/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_COMPUTE_OPENCL_MEMORYHOST_HH_
#define BE_COMPUTE_OPENCL_MEMORYHOST_HH_
/**************************************************************************************************/
#include    <opencl/stdafx.h>
#include    <scheduler/kernel/imemoryhost.hh>

namespace BugEngine { namespace KernelScheduler { namespace OpenCL
{

class MemoryHost : public IMemoryHost
{
public:
    MemoryHost();
    ~MemoryHost();

    void release(weak<KernelScheduler::IMemoryBuffer> buffer);
};

}}}

/**************************************************************************************************/
#endif
