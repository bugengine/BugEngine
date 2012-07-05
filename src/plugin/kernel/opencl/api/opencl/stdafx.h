/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_KERNEL_OPENCL_STDAFX_H_
#define BE_KERNEL_OPENCL_STDAFX_H_
/*****************************************************************************/

#include    <core/stdafx.h>
#include    <rtti/stdafx.h>
#include    <system/stdafx.h>

#ifdef BE_PLATFORM_MACOS
# include   <OpenCL/opencl.h>
#else
# include   <opencl.h>
#endif

/*****************************************************************************/
#endif
