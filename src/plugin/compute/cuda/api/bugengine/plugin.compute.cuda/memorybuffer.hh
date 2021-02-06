/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_COMPUTE_CUDA_MEMORYBUFFER_HH_
#define BE_COMPUTE_CUDA_MEMORYBUFFER_HH_
/**************************************************************************************************/
#include <bugengine/plugin.compute.cuda/stdafx.h>
#include <bugengine/scheduler/kernel/imemorybuffer.hh>

namespace BugEngine { namespace KernelScheduler { namespace Cuda {

class MemoryHost;
struct Buffer
{
    void* m_memory;
    u32   m_size;
};

class MemoryBuffer : public IMemoryBuffer
{
private:
    Buffer* m_buffers;
    u32     m_bufferCount;

public:
    MemoryBuffer(weak< const MemoryHost > provider);
    ~MemoryBuffer();

    Buffer* buffers() const
    {
        return m_buffers;
    }
    u32 bufferCount() const
    {
        return m_bufferCount;
    }
};

}}}  // namespace BugEngine::KernelScheduler::Cuda

/**************************************************************************************************/
#endif
