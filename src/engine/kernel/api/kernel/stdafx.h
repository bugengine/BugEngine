/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_KERNEL_STDAFX_H_
#define BE_KERNEL_STDAFX_H_
/*****************************************************************************/

#include    <kernel/compilers.hh>
#include    <kernel/interlocked.hh>
#include    <kernel/simd.hh>

namespace BugEngine
{

inline u32 bitCount(u32 bitMask)
{
    u32 result = 0;
    for (u32 i = 0; i < 32; ++i)
    {
        result += bitMask & 0x1;
        bitMask >>= 1;
    }
    return result;
}

}

/*****************************************************************************/
#endif
