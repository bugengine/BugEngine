/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_KERNEL_CL_INPUT_SEGMENTS_HH_
#define BE_KERNEL_CL_INPUT_SEGMENTS_HH_
/**************************************************************************************************/
#include <bugengine/kernel/stdafx.h>
#include <bugengine/kernel/input/segment.hh>

namespace Kernel {

template < typename T >
struct segments : public segment< T >
{
};

}  // namespace Kernel

/**************************************************************************************************/
#endif
