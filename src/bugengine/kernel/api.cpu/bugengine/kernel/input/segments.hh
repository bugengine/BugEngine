/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_KERNEL_INPUT_SEGMENTS_HH_
#define BE_KERNEL_INPUT_SEGMENTS_HH_
/**************************************************************************************************/
#include <bugengine/kernel/stdafx.h>
#include <bugengine/kernel/input/segment_part.hh>

namespace Kernel {

template < typename T >
struct segments
{
private:
    segments_part< T >* const m_segments;
    u32 const                 m_segmentCount;
    u32 const                 m_elementCount;

public:
    segments(segments_part< T >* parts, u32 segmentCount, u32 count)
        : m_segments(parts)
        , m_segmentCount(segmentCount)
        , m_elementCount(count)
    {
    }

    struct iterator
    {
        friend struct segments;

    private:
        segments_part< T >* m_currentSegment;
        u32                 m_currentOffset;

    private:
        iterator(segments_part< T >* segment, u32 offset)
            : m_currentSegment(segment)
            , m_currentOffset(offset)
        {
        }

    public:
        ~iterator()
        {
        }

    public:
        bool operator==(const iterator& other)
        {
            return m_currentSegment == other.m_currentSegment
                   && m_currentOffset == other.m_currentOffset;
        }
        bool operator!=(const iterator& other)
        {
            return m_currentSegment != other.m_currentSegment
                   || m_currentOffset != other.m_currentOffset;
        }
        iterator& operator++()
        {
            if(++m_currentOffset == m_currentSegment->m_count)
            {
                m_currentOffset = 0;
                m_currentSegment++;
            }
            return *this;
        }

        iterator& operator--()
        {
            if(m_currentOffset == 0)
            {
                --m_currentSegment;
                m_currentOffset = m_currentSegment->m_count - 1;
            }
            return *this;
        }

        iterator operator++(int)
        {
            segments result = *this;
            if(++m_currentOffset == m_currentSegment->m_count)
            {
                m_currentOffset = 0;
                m_currentSegment++;
            }
            return result;
        }

        iterator operator--(int)
        {
            segments result = *this;
            if(m_currentOffset == 0)
            {
                --m_currentSegment;
                m_currentOffset = m_currentSegment->m_count - 1;
            }
            return result;
        }

        iterator& operator+=(u32 count)
        {
            while(count)
            {
                if(m_currentOffset + count >= m_currentSegment->m_count)
                {
                    u32 progress = m_currentSegment->m_count - m_currentOffset;
                    count -= progress;
                    ++m_currentSegment;
                    m_currentOffset = 0;
                }
            }
            return *this;
        }

        T* operator->() const
        {
            return m_currentSegment->m_begin + m_currentOffset;
        }

        T& operator*() const
        {
            return *(m_currentSegment->m_begin + m_currentOffset);
        }
    };

    u32 size() const
    {
        return m_elementCount;
    }

    iterator begin() const
    {
        return iterator(m_segments, 0);
    }
    iterator end() const
    {
        segments_part< T >* last_segment = m_segments + m_segmentCount - 1;
        return iterator(last_segment, last_segment->m_count);
    }
};

}  // namespace Kernel

/**************************************************************************************************/
#endif
