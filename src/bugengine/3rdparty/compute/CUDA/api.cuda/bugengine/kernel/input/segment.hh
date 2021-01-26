/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_KERNEL_CUDA_INPUT_SEGMENT_HH_
#define BE_KERNEL_CUDA_INPUT_SEGMENT_HH_
/**************************************************************************************************/
#include    <bugengine/kernel/stdafx.h>


namespace Kernel
{

template < typename T >
struct segment
{
private:
    T*  const   m_begin;
    u32 const   m_count;

public:
    __device segment(T* begin, T* end)
        :   m_begin(begin)
        ,   m_count(static_cast<u32>(end - begin))
    {
    }

    typedef T* iterator;

    __device u32 size() const
    {
        return m_count;
    }

    __device iterator begin() const
    {
        return m_begin;
    }
    __device iterator end() const
    {
        return m_begin + m_count;
    }
};


}

/**************************************************************************************************/
#endif
