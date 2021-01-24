/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_KERNEL_CUDA_INPUT_SEGMENTS_HH_
#define BE_KERNEL_CUDA_INPUT_SEGMENTS_HH_
/**************************************************************************************************/
#include    <bugengine/kernel/input/segment.hh>
#include    <bugengine/kernel/stdafx.h>


namespace Kernel
{

template< typename T >
struct segments : public segment<T>
{
};


}

/**************************************************************************************************/
#endif
