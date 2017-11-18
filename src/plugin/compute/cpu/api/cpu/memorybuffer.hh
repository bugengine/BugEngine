/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_COMPUTE_CPU_MEMORYBUFFER_HH_
#define BE_COMPUTE_CPU_MEMORYBUFFER_HH_
/**************************************************************************************************/
#include    <stdafx.h>
#include    <scheduler/kernel/imemorybuffer.hh>
#include    <kernel/input/cpp/segment_part.hh>


namespace BugEngine { namespace Kernel { namespace CPU
{

class MemoryHost;
struct Buffer
{
    void*   m_memory;
    u32     m_size;
};

class MemoryBuffer : public IMemoryBuffer
{
private:
    Buffer* m_buffers;
    u32     m_bufferCount;
public:
    MemoryBuffer(weak<const MemoryHost> provider);
    ~MemoryBuffer();

    Buffer* buffers() const     { return m_buffers; }
    u32     bufferCount() const { return m_bufferCount; }


};

}}}

/**************************************************************************************************/
#endif
