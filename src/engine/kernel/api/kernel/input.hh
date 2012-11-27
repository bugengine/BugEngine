/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_KERNEL_INPUT_HH_
#define BE_KERNEL_INPUT_HH_
/*****************************************************************************/
#include    <kernel/compilers.hh>

namespace Kernel
{

template< typename T >
struct in
{
private:
    const T* const  m_begin;
    const T* const  m_end;
    const T*        m_current;
public:
    in(const T* begin, const T* end)
        :   m_begin(begin)
        ,   m_end(end)
        ,   m_current(begin)
    {
    }
    operator void*() const { return (void*)(m_end - m_current); }
    in& operator++() { m_current++; return *this; }
    in& operator--() { m_current--; return *this; }
    in  operator++(int) { in result = *this; m_current++; return result; }
    in  operator--(int) { in result = *this; m_current--; return result; }
    in& operator+=(u32 count) { m_current += count; return *this; }

    const T* operator->() const { return m_current; }
    const T& operator*() const { return *m_current; }
};

template< typename T >
struct inout
{
private:
    T* const  m_begin;
    T* const  m_end;
    T*        m_current;
public:
    inout(T* begin, T* end)
        :   m_begin(begin)
        ,   m_end(end)
        ,   m_current(begin)
    {
    }
    operator void*() const { return (void*)(m_end - m_current); }
    inout& operator++() { m_current++; return *this; }
    inout& operator--() { m_current--; return *this; }
    inout  operator++(int) { inout result = *this; m_current++; return result; }
    inout  operator--(int) { inout result = *this; m_current--; return result; }
    inout& operator+=(u32 count) { m_current += count; return *this; }

    T* operator->() const { return m_current; }
    T& operator*() const { return *m_current; }
};

}

/*****************************************************************************/
#endif
