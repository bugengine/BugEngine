/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_KERNEL_STDAFX_H_
#define BE_KERNEL_STDAFX_H_
/**************************************************************************************************/
#include    <kernel/compilers.hh>

#ifndef BE_BUILD_KERNEL
namespace BugEngine
{

inline u32 bitCount(u32 bitMask)
{
    u32 result = 0;
    for (u32 i = 0; i < 32; ++i, bitMask >>= 1)
    {
        result += bitMask & 0x1;
    }
    return result;
}

inline u32 getFirstBit(u32 bitMask)
{
    for (u32 i = 0; i < 32; ++i, bitMask >>= 1)
    {
       if (bitMask & 0x1)
           return i;
    }
    return (u32)-1;
}

}
#endif

/**************************************************************************************************/
#endif
