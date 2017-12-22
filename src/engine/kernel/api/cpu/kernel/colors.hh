/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_KERNEL_COLORS_HH_
#define BE_KERNEL_COLORS_HH_
/**************************************************************************************************/
#include    <kernel/stdafx.h>
#include    <kernel/simd.hh>

namespace BugEngine { namespace Colors
{

namespace Red
{
static const kernel::color32 Red = { {255, 0, 0, 255} };
}

namespace Blue
{
static const kernel::color32 Blue = { {0, 0, 255, 255} };
}

namespace Yellow
{
static const kernel::color32 Yellow = { {255, 255, 0, 255} };
}

namespace Green
{
static const kernel::color32 Green = { {0, 255, 0, 255} };
}

static inline kernel::color32 make(u8 r, u8 g, u8 b, u8 a = 255)
{
    kernel::color32 result = { { r, g, b, a } };
    return result;
}

}}

/**************************************************************************************************/
#endif
