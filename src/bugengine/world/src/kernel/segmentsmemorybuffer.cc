/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/world/stdafx.h>
#include <kernel/segmentsmemorybuffer.hh>

namespace BugEngine { namespace World {

SegmentsMemoryBuffer::SegmentsMemoryBuffer(weak< const KernelScheduler::IMemoryHost > host)
    : IMemoryBuffer(host)
{
}

SegmentsMemoryBuffer::~SegmentsMemoryBuffer()
{
}

}}  // namespace BugEngine::World
