/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <world/stdafx.h>
#include    <world/kernel/segmentsmemorybuffer.hh>

namespace BugEngine { namespace World
{

SegmentsMemoryBuffer::SegmentsMemoryBuffer(weak<const KernelScheduler::IMemoryHost> host)
    :   IMemoryBuffer(host)
{
}

SegmentsMemoryBuffer::~SegmentsMemoryBuffer()
{
}

}}
