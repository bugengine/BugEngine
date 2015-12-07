/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_KERNEL_INPUT_SEGMENT_HH_
#define BE_KERNEL_INPUT_SEGMENT_HH_
/**************************************************************************************************/
#include    <kernel/stdafx.h>
#include    <kernel/compilers.hh>

namespace Kernel
{

template< typename T >
struct segment
{
private:
    T* const    m_begin;
    T* const    m_end;
    T*          m_current;
public:
    segment(T* begin, T* end)
        :   m_begin(begin)
        ,   m_end(end)
        ,   m_current(begin)
    {
    }
    operator void*() const          { return reinterpret_cast<void*>(m_end - m_current); }
    bool operator !() const         { return m_current == m_end; }
    segment& operator++()           { m_current++; return *this; }
    segment& operator--()           { m_current--; return *this; }
    segment  operator++(int)        { segment result = *this; m_current++; return result; }
    segment  operator--(int)        { segment result = *this; m_current--; return result; }
    segment& operator+=(u32 count)  { m_current += count; return *this; }

    u32 size() const                { return static_cast<u32>(m_end - m_begin); }

    T* operator->() const           { return m_current; }
    T& operator*() const            { return *m_current; }
};


}

/**************************************************************************************************/
#endif
