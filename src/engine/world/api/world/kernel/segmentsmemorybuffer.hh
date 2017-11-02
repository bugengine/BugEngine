/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_WORLD_KERNEL_SEGMENTSMEMORYBUFFER_HH_
#define BE_WORLD_KERNEL_SEGMENTSMEMORYBUFFER_HH_
/**************************************************************************************************/
#include    <world/stdafx.h>
#include    <scheduler/kernel/imemorybuffer.hh>
#include    <minitl/intrusive_list.hh>


namespace BugEngine { namespace World
{

class be_api(WORLD) SegmentsMemoryBuffer : public Kernel::IMemoryBuffer
                                         , public minitl::intrusive_list<SegmentsMemoryBuffer>::item
{
public:
    SegmentsMemoryBuffer(weak<const Kernel::IMemoryHost> host);
    ~SegmentsMemoryBuffer();
};

}}


/**************************************************************************************************/
#endif
