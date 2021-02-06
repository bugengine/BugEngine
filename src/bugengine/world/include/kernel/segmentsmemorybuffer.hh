/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_WORLD_KERNEL_SEGMENTSMEMORYBUFFER_HH_
#define BE_WORLD_KERNEL_SEGMENTSMEMORYBUFFER_HH_
/**************************************************************************************************/
#include <bugengine/world/stdafx.h>
#include <bugengine/minitl/intrusive_list.hh>
#include <bugengine/scheduler/kernel/imemorybuffer.hh>

namespace BugEngine { namespace World {

class be_api(WORLD) SegmentsMemoryBuffer
    : public KernelScheduler::IMemoryBuffer
    , public minitl::intrusive_list< SegmentsMemoryBuffer >::item
{
public:
    SegmentsMemoryBuffer(weak< const KernelScheduler::IMemoryHost > host);
    ~SegmentsMemoryBuffer();
};

}}  // namespace BugEngine::World

/**************************************************************************************************/
#endif
