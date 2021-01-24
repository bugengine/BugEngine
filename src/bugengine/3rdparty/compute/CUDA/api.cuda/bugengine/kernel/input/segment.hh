/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_KERNEL_CUDA_INPUT_SEGMENT_HH_
#define BE_KERNEL_CUDA_INPUT_SEGMENT_HH_
/**************************************************************************************************/
#include    <bugengine/kernel/input/segment_part.hh>
#include    <bugengine/kernel/stdafx.h>


namespace Kernel
{

template < typename T >
struct segment
{
private:
    segments_part< T > m_segment;

public:
    __device segment(T* begin, T* end) : m_segment(begin, end)
    {
    }
    __device segment(const segments_part< T >& s) : m_segment(s)
    {
    }

    typedef T* iterator;

    __device u32 size() const
    {
        return m_segment.m_count;
    }

    __device iterator begin() const
    {
        return m_segment.m_begin;
    }
    __device iterator end() const
    {
        return m_segment.m_begin + m_segment.m_count;
    }
};


}

/**************************************************************************************************/
#endif
