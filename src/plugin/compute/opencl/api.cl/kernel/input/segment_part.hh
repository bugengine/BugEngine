/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_KERNEL_CL_INPUT_SEGMENT_PART_HH_
#define BE_KERNEL_CL_INPUT_SEGMENT_PART_HH_
/**************************************************************************************************/
#include    <kernel/stdafx.h>


namespace Kernel
{

template< typename T >
struct segment;
template< typename T >
struct segments;

template< typename T >
struct segments_part
{
    friend struct segment<T>;
    friend struct segments<T>;
private:
    T*  const   m_begin;
    u32 const   m_count;
public:
    __device segments_part(T* begin, T* end)
        :   m_begin(begin)
        ,   m_count(static_cast<u32>(end - begin))
    {
    }
};

}

/**************************************************************************************************/
#endif
