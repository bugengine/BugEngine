/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_COMPUTE_CUDA_MEMORYHOST_HH_
#define BE_COMPUTE_CUDA_MEMORYHOST_HH_
/**************************************************************************************************/
#include    <cuda/stdafx.h>
#include    <scheduler/kernel/imemoryhost.hh>

namespace BugEngine { namespace KernelScheduler { namespace Cuda
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
