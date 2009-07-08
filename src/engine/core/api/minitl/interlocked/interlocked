/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

#ifndef BE_MINITL_INTERLOCKED_INTERLOCKED_
#define BE_MINITL_INTERLOCKED_INTERLOCKED_
/*****************************************************************************/
#if defined(BE_COMPILER_MSVC)
# if defined(_X86)
#  include <minitl/interlocked/msvc/x86/interlocked.inl>
# elif defined(_AMD64)
#  include <minitl/interlocked/msvc/amd64/interlocked.inl>
# else
#  error Architecture not implemented on MSVC
# endif
#elif defined(BE_COMPILER_INTEL) || defined(BE_COMPILER_GCC)
# if defined(_X86)
#  include <minitl/interlocked/gcc/x86/interlocked.inl>
# elif defined(_AMD64)
#  include <minitl/interlocked/gcc/amd64/interlocked.inl>
# else
#  error Architecture not implemented on GCC
# endif
#elif defined(BE_COMPILER_SUNCC)
# include <minitl/interlocked/suncc/interlocked.inl>
#else
# error Compiler not implemented
#endif

namespace minitl
{

template< typename T >
class interlocked
{
private:
    typedef interlocked_impl::InterlockedType< sizeof(T) >  impl;
    typedef typename impl::value_t                          value_t; 
private:
    BE_SET_ALIGNMENT(4) volatile value_t m_value;
public:
    interlocked()                                   {}
    interlocked(T value) : m_value(value)           {}
    ~interlocked()                                  {}

    operator T() const                              { return (T)m_value; }
    T operator=(T value)                            { return (T)impl::set_and_fetch(&m_value, value); }
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
    BE_SET_ALIGNMENT(4) T* volatile m_value;
public:
    iptr()                                          {}
    iptr(T* value) : m_value(value)                 {}
    ~iptr()                                         {}

    operator const T*() const                       { return m_value; }
    operator T*()                                   { return m_value; }
    T* operator->()                                 { return m_value; }
    const T* operator->() const                     { return m_value; }

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

    itaggedptr()                                    {}
    itaggedptr(T* value) : m_value(value)           {}
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
typedef minitl::interlocked<u64>    i_u64;
typedef minitl::interlocked<i8>     i_i8;
typedef minitl::interlocked<i16>    i_i16;
typedef minitl::interlocked<i32>    i_i32;
typedef minitl::interlocked<i64>    i_i64;
typedef minitl::interlocked<size_t> i_size_t;

/*****************************************************************************/
#endif
