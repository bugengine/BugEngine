/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_KERNEL_INPUT_SEGMENTS_HH_
#define BE_KERNEL_INPUT_SEGMENTS_HH_
/**************************************************************************************************/
#include    <kernel/stdafx.h>
#include    <kernel/compilers.hh>
#include    <kernel/input/segment.hh>

namespace Kernel
{

template< typename T >
struct segments
{
private:
    segment<T>* const   m_begin;
    segment<T>* const   m_end;
    segment<T>*         m_current;
public:
    segments(segment<T>* begin, segment<T>* end)
        :   m_begin(begin)
        ,   m_end(end)
        ,   m_current(begin)
    {
    }
    operator void*() const          { return reinterpret_cast<void*>(m_end - m_current); }
    bool operator !() const         { return m_current == m_end; }
    segments& operator++()          { m_current++; return *this; }
    segments& operator--()          { m_current--; return *this; }
    segments  operator++(int)       { segments result = *this; m_current++; return result; }
    segments  operator--(int)       { segments result = *this; m_current--; return result; }
    segments& operator+=(u32 count) { m_current += count; return *this; }

    u32 size() const                { return static_cast<u32>(m_end - m_begin); }

    segment<T>* operator->() const  { return m_current; }
    segment<T>& operator*() const   { return *m_current; }
};


}

/**************************************************************************************************/
#endif
