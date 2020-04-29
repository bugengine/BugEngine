/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_KERNEL_CL_INPUT_SEGMENTS_HH_
#define BE_KERNEL_CL_INPUT_SEGMENTS_HH_
/**************************************************************************************************/
#include    <kernel/stdafx.h>
#include    <kernel/input/segment_part.hh>


namespace Kernel
{

template< typename T >
struct segments
{
private:
    u32 const               m_count;
    u32 const               m_segmentCount;
    segments_part<T>* const m_segments;
    segments_part<T>*       m_currentSegment;
    u32                     m_currentOffset;
    u32                     m_totalOffset;
public:
    __device segments(u32 count, u32 segmentCount, segments_part<T>* parts)
        :   m_count(count)
        ,   m_segmentCount(segmentCount)
        ,   m_segments(parts)
        ,   m_currentSegment(parts)
        ,   m_currentOffset(0)
        ,   m_totalOffset(0)
    {
    }
    __device operator void*() const
    {
        return reinterpret_cast<void*>(m_count - m_totalOffset);
    }

    __device bool operator !() const
    {
        return m_count == m_totalOffset;
    }

    __device segments& operator++()
    {
        ++m_totalOffset;
        if (++m_currentOffset == m_currentSegment->m_count)
        {
            m_currentOffset = 0;
            m_currentSegment++;
        }
        return *this;
    }

    __device segments& operator--()
    {
        --m_totalOffset;
        if (m_currentOffset == 0)
        {
            --m_currentSegment;
            m_currentOffset = m_currentSegment->m_count - 1;
        }
        return *this;
    }

    __device segments  operator++(int)
    {
        segments result = *this;
        ++m_totalOffset;
        if (++m_currentOffset == m_currentSegment->m_count)
        {
            m_currentOffset = 0;
            m_currentSegment++;
        }
        return result;
    }

    __device segments  operator--(int)
    {
        segments result = *this;
        --m_totalOffset;
        if (m_currentOffset == 0)
        {
            --m_currentSegment;
            m_currentOffset = m_currentSegment->m_count - 1;
        }
        return result;
    }

    __device segments& operator+=(u32 count)
    {
        while (count)
        {
            if (m_currentOffset + count >= m_currentSegment->size)
            {
                u32 progress = m_currentSegment->m_count - m_currentOffset;
                m_totalOffset += progress;
                count -= progress;
                ++m_currentSegment;
                m_currentOffset = 0;
            }
        }
        return *this;
    }

    __device u32 size() const
    {
        return m_count;
    }

    __device T* operator->() const
    {
        return m_currentSegment->begin + m_currentOffset;
    }

    __device T& operator*() const
    {
        return *(m_currentSegment->begin + m_currentOffset);
    }

};


}

/**************************************************************************************************/
#endif
