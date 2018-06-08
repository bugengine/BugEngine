/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_KERNEL_INPUT_SEGMENTS_HH_
#define BE_KERNEL_INPUT_SEGMENTS_HH_
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
    segments(u32 count, u32 segmentCount, segments_part<T>* segments)
        :   m_count(count)
        ,   m_segmentCount(segmentCount)
        ,   m_segments(segments)
        ,   m_currentSegment(segments)
        ,   m_currentOffset(0)
        ,   m_totalOffset(0)
    {
    }
    operator void*() const
    {
        return reinterpret_cast<void*>(m_count - m_totalOffset);
    }

    bool operator !() const
    {
        return m_count == m_totalOffset;
    }

    segments& operator++()
    {
        ++m_totalOffset;
        if (++m_currentOffset == m_currentSegment->m_count)
        {
            m_currentOffset = 0;
            m_currentSegment++;
        }
        return *this;
    }

    segments& operator--()
    {
        --m_totalOffset;
        if (m_currentOffset == 0)
        {
            --m_currentSegment;
            m_currentOffset = m_currentSegment->m_count - 1;
        }
        return *this;
    }

    segments  operator++(int)
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

    segments  operator--(int)
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

    segments& operator+=(u32 count)
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

    u32 size() const
    {
        return m_count;
    }

    T* operator->() const
    {
        return m_currentSegment->begin + m_currentOffset;
    }

    T& operator*() const
    {
        return *(m_currentSegment->begin + m_currentOffset);
    }

};


}

/**************************************************************************************************/
#endif
