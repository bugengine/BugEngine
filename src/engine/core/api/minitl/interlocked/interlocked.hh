/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_INTERLOCKED_INTERLOCKED_
#define BE_MINITL_INTERLOCKED_INTERLOCKED_
/*****************************************************************************/
#if defined(BE_COMPILER_MSVC)
# if defined(_X86)
#  include <minitl/interlocked/msvc/x86/interlocked.inl>
# elif defined(_AMD64)
#  include <minitl/interlocked/msvc/amd64/interlocked.inl>
# elif defined(_PPC)
#  include <minitl/interlocked/msvc/ppc/interlocked.inl>
# else
#  error Architecture not implemented on MSVC
# endif
#elif defined(BE_COMPILER_INTEL) || defined(BE_COMPILER_GCC)
# if defined(_X86) || defined(_AMD64)
#  include <minitl/interlocked/gcc/x86/interlocked.inl>
# elif defined(_POWERPC)
#  include <minitl/interlocked/gcc/ppc/interlocked.inl>
# elif defined(_ARM)
#  include <minitl/interlocked/gcc/arm/interlocked.inl>
# elif defined(_MIPS)
#  include <minitl/interlocked/gcc/mips/interlocked.inl>
# else
#  error Architecture not implemented on GCC
# endif
#elif defined(BE_COMPILER_SUNCC)
# include <minitl/interlocked/suncc/interlocked.inl>
#else
# error Compiler not implemented
#endif
#include <core/debug/assert.hh>

namespace minitl
{

template< typename T >
class interlocked
{
private:
    typedef interlocked_impl::InterlockedType< sizeof(T) >  impl;
    typedef typename impl::value_t                          value_t; 
private:
    volatile value_t m_value;
public:
    interlocked() : m_value(0)                      { be_assert(be_align(&m_value, sizeof(m_value)) == &m_value, "value is incorrectly aligned"); }
    interlocked(T value) : m_value(value)           { be_assert(be_align(&m_value, sizeof(m_value)) == &m_value, "value is incorrectly aligned"); }
    ~interlocked()                                  {}

    operator T() const                              { return (T)m_value; }
    T operator=(T value)                            { return (T)impl::set_and_fetch(&m_value, value); }
    T operator=(const interlocked& value)           { return (T)impl::set_and_fetch(&m_value, value); }
    T exchange(T value)                             { return (T)impl::fetch_and_set(&m_value, value); }
    T addExchange(T value)                          { return (T)impl::fetch_and_add(&m_value, value); }

    T operator++()                                  { return (T)impl::fetch_and_add(&m_value, 1)+1; }
    T operator++(int)                               { return (T)impl::fetch_and_add(&m_value, 1); }
    T operator+=(T value)                           { return (T)impl::fetch_and_add(&m_value, value)+value; }
    T operator--()                                  { return (T)impl::fetch_and_sub(&m_value, 1)-1; }
    T operator--(int)                               { return (T)impl::fetch_and_sub(&m_value, 1); }
    T operator-=(T value)                           { return (T)impl::fetch_and_sub(&m_value, value)-value; }

    T setConditional(T value, T condition)          { return (T)impl::set_conditional(&m_value, value, condition); }
};


template< typename T >
class iptr
{
private:
    typedef interlocked_impl::InterlockedType< sizeof(T*) > impl;
    typedef typename impl::value_t                          value_t;
private:
    value_t m_value;
public:
    iptr() : m_value(0)                             { be_assert(be_align(&m_value, sizeof(T*)) == &m_value, "value is incorrectly aligned"); }
    iptr(T* value) : m_value(value)                 { be_assert(be_align(&m_value, sizeof(T*)) == &m_value, "value is incorrectly aligned"); }
    ~iptr()                                         {}

    operator const T*() const                       { return (T*)m_value; }
    operator T*()                                   { return (T*)m_value; }
    T* operator->()                                 { return (T*)m_value; }
    const T* operator->() const                     { return (T*)m_value; }

    T* operator=(T* value)                          { return (T*)impl::set_and_fetch((value_t*)&m_value, (value_t)value); }
    T* exchange(T* value)                           { return (T*)impl::fetch_and_set((value_t*)&m_value, (value_t)value); }

    T* setConditional(T* value, T* condition)       { return (T*)impl::set_conditional((value_t*)&m_value, (value_t)value, (value_t)condition); }
};

template< typename T >
class itaggedptr
{
private:
    typedef interlocked_impl::InterlockedType< sizeof(T*) > impl;
    typedef typename impl::tagged_t                         type_t;
    typedef typename type_t::value_t                        value_t;
private:
    type_t m_value;
public:
    typedef typename type_t::tag_t                          ticket_t;

    itaggedptr() : m_value(0,0)                     { be_assert(be_align(&m_value, sizeof(value_t)) == &m_value, "value is incorrectly aligned"); }
    itaggedptr(T* value) : m_value((value_t)value)  { be_assert(be_align(&m_value, sizeof(value_t)) == &m_value, "value is incorrectly aligned"); }
    ~itaggedptr()                                   {}

    operator const T*() const                       { return (T*)m_value.value(); }
    operator T*()                                   { return (T*)m_value.value(); }
    T* operator->()                                 { return (T*)m_value.value(); }
    const T* operator->() const                     { return (T*)m_value.value(); }

    ticket_t getTicket()                              { return impl::get_ticket(m_value); }
    bool setConditional(T* value, ticket_t& condition){ return impl::set_conditional(&m_value, (value_t)value, condition); }
};

}


typedef minitl::interlocked<bool>   i_bool;
typedef minitl::interlocked<u8>     i_u8;
typedef minitl::interlocked<u16>    i_u16;
typedef minitl::interlocked<u32>    i_u32;
typedef minitl::interlocked<i8>     i_i8;
typedef minitl::interlocked<i16>    i_i16;
typedef minitl::interlocked<i32>    i_i32;
typedef minitl::interlocked<size_t> i_size_t;

/*****************************************************************************/
#endif
