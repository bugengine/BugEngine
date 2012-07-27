/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_KERNEL_INTERLOCKED_HH_
#define BE_KERNEL_INTERLOCKED_HH_
/*****************************************************************************/
#include <kernel/compilers.hh>

#if defined(BE_COMPILER_MSVC)
# if defined(_X86)
#  include <kernel/msvc/x86/interlocked.inl>
# elif defined(_AMD64)
#  include <kernel/msvc/amd64/interlocked.inl>
# elif defined(_PPC)
#  include <kernel/msvc/ppc/interlocked.inl>
# else
#  error Architecture not implemented on MSVC
# endif
#elif defined(BE_COMPILER_INTEL) || defined(BE_COMPILER_GCC)
# if defined(_X86) || defined(_AMD64)
#  include <kernel/gcc/x86/interlocked.inl>
# elif defined(_POWERPC)
#  include <kernel/gcc/ppc/interlocked.inl>
# elif defined(_ARM)
#  include <kernel/gcc/arm/interlocked.inl>
# elif defined(_MIPS)
#  include <kernel/gcc/mips/interlocked.inl>
# else
#  error Architecture not implemented on GCC
# endif
#elif defined(BE_COMPILER_SUNCC)
# include <kernel/suncc/interlocked.inl>
#else
# error Compiler not implemented
#endif


namespace _Kernel
{

template< typename T >
class interlocked
{
private:
    typedef InterlockedType< sizeof(T) >  impl;
    typedef typename impl::value_t                          value_t;
public:
    value_t m_value;
public:
    static const interlocked<T> Zero;
    static const interlocked<T> One;

    operator T() const                              { return static_cast<T>(m_value); }
    T operator=(T value)                            { return static_cast<T>(impl::set_and_fetch(&m_value, value)); }
    T operator=(const interlocked& value)           { return static_cast<T>(impl::set_and_fetch(&m_value, value)); }
    T exchange(T value)                             { return static_cast<T>(impl::fetch_and_set(&m_value, value)); }
    T addExchange(T value)                          { return static_cast<T>(impl::fetch_and_add(&m_value, value)); }

    T operator++()                                  { return static_cast<T>(impl::fetch_and_add(&m_value, 1)+1); }
    T operator++(int)                               { return static_cast<T>(impl::fetch_and_add(&m_value, 1)); }
    T operator+=(T value)                           { return static_cast<T>(impl::fetch_and_add(&m_value, value)+value); }
    T operator--()                                  { return static_cast<T>(impl::fetch_and_sub(&m_value, 1)-1); }
    T operator--(int)                               { return static_cast<T>(impl::fetch_and_sub(&m_value, 1)); }
    T operator-=(T value)                           { return static_cast<T>(impl::fetch_and_sub(&m_value, value)-value); }

    T setConditional(T value, T condition)          { return static_cast<T>(impl::set_conditional(&m_value, value, condition)); }
};

template< typename T >
const interlocked<T> interlocked<T>::Zero = { 0 };
template< typename T >
const interlocked<T> interlocked<T>::One = { 1 };


template< typename T >
class iptr
{
private:
    typedef InterlockedType< sizeof(T*) >   impl;
    typedef typename impl::value_t          value_t;
private:
    value_t m_value;
public:
    iptr(T* t)
        :   m_value((typename impl::value_t)(t))
    {
    }
    operator const T*() const                       { return static_cast<T*>(m_value); }
    operator T*()                                   { return static_cast<T*>(m_value); }
    T* operator->()                                 { return static_cast<T*>(m_value); }
    const T* operator->() const                     { return static_cast<T*>(m_value); }

    T* operator=(T* value)                          { return static_cast<T*>(impl::set_and_fetch((value_t*)&m_value, (value_t)value)); }
    T* exchange(T* value)                           { return static_cast<T*>(impl::fetch_and_set((value_t*)&m_value, (value_t)value)); }

    T* setConditional(T* value, T* condition)       { return static_cast<T*>(impl::set_conditional((value_t*)&m_value, (value_t)value, (value_t)condition)); }
};

template< typename T >
class itaggedptr
{
private:
    typedef InterlockedType< sizeof(T*) > impl;
    typedef typename impl::tagged_t                         type_t;
    typedef typename type_t::value_t                        value_t;
private:
    type_t m_value;
public:
    itaggedptr(T* t)
        :   m_value((typename type_t::value_t)(t))
    {
    }
    typedef typename type_t::tag_t                          ticket_t;

    operator const T*() const                       { return reinterpret_cast<T*>(m_value.value()); }
    operator T*()                                   { return reinterpret_cast<T*>(m_value.value()); }
    T* operator->()                                 { return reinterpret_cast<T*>(m_value.value()); }
    const T* operator->() const                     { return reinterpret_cast<T*>(m_value.value()); }

    ticket_t getTicket()                              { return impl::get_ticket(m_value); }
    bool setConditional(T* value, ticket_t& condition){ return impl::set_conditional(&m_value, reinterpret_cast<value_t>(value), condition); }
};

}


typedef _Kernel::interlocked<bool>   i_bool;
typedef _Kernel::interlocked<u8>     i_u8;
typedef _Kernel::interlocked<u16>    i_u16;
typedef _Kernel::interlocked<u32>    i_u32;
typedef _Kernel::interlocked<i8>     i_i8;
typedef _Kernel::interlocked<i16>    i_i16;
typedef _Kernel::interlocked<i32>    i_i32;
typedef _Kernel::interlocked<size_t> i_size_t;

/*****************************************************************************/
#endif
