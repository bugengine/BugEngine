/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_KERNEL_INPUT_CPP_SEGMENT_HH_
#define BE_KERNEL_INPUT_CPP_SEGMENT_HH_
/**************************************************************************************************/
#include    <kernel/stdafx.h>
#include    <kernel/input/cpp/segment_part.hh>


namespace Kernel
{

template< typename T >
struct segment
{
private:
    segments_part<T>    m_segment;
    u32                 m_current;
public:
    segment(T* begin, T* end)
        :   m_segment(begin, end)
        ,   m_current(0)
    {
    }
    segment(const segments_part<T>& s)
        :   m_segment(s)
        ,   m_current(0)
    {
    }
    operator void*() const          { return reinterpret_cast<void*>(m_segment.m_count - m_current); }
    bool operator !() const         { return m_current == m_segment.m_count; }
    segment& operator++()           { m_current++; return *this; }
    segment& operator--()           { m_current--; return *this; }
    segment  operator++(int)        { segment result = *this; m_current++; return result; }
    segment  operator--(int)        { segment result = *this; m_current--; return result; }
    segment& operator+=(u32 count)  { m_current += count; return *this; }

    u32 size() const                { return m_segment.m_count; }

    T* operator->() const           { return m_segment.m_begin + m_current; }
    T& operator*() const            { return *(m_segment.m_begin + m_current); }
};

}

/**************************************************************************************************/
#endif
