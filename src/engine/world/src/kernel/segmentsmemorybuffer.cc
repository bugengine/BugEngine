/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/world/stdafx.h>
#include <bugengine/world/kernel/segmentsmemorybuffer.hh>

namespace BugEngine { namespace World {

SegmentsMemoryBuffer::SegmentsMemoryBuffer(weak< const KernelScheduler::IMemoryHost > host)
    : IMemoryBuffer(host)
{
}

SegmentsMemoryBuffer::~SegmentsMemoryBuffer()
{
}

}}  // namespace BugEngine::World
