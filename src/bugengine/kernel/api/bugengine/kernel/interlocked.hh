/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_KERNEL_INTERLOCKED_HH_
#define BE_KERNEL_INTERLOCKED_HH_
/**************************************************************************************************/
#include <bugengine/kernel/stdafx.h>

#if defined(BE_COMPILER_MSVC) || (defined(BE_COMPILER_INTEL) && defined(_WIN32))
#    if defined(_X86)
#        include <bugengine/kernel/msvc/x86/interlocked.inl>
#    elif defined(_AMD64)
#        include <bugengine/kernel/msvc/amd64/interlocked.inl>
#    elif defined(_PPC)
#        include <bugengine/kernel/msvc/ppc/interlocked.inl>
#    elif defined(_ARM64)
#        include <bugengine/kernel/msvc/arm64/interlocked.inl>
#    elif defined(_ARM)
#        include <bugengine/kernel/msvc/arm/interlocked.inl>
#    else
#        error Architecture not implemented on MSVC
#    endif
#elif defined(BE_COMPILER_INTEL) || defined(BE_COMPILER_GCC) || defined(BE_COMPILER_CLANG)
#    if defined(_X86) || defined(_AMD64)
#        include <bugengine/kernel/gcc/x86/interlocked.inl>
#    elif defined(_POWERPC)
#        include <bugengine/kernel/gcc/ppc/interlocked.inl>
#    elif defined(_ARM64) && defined(_ILP32)
#        include <bugengine/kernel/gcc/arm64_32/interlocked.inl>
#    elif defined(_ARM64)
#        include <bugengine/kernel/gcc/arm64/interlocked.inl>
#    elif defined(_ARM)
#        include <bugengine/kernel/gcc/arm/interlocked.inl>
#    elif defined(_MIPS)
#        include <bugengine/kernel/gcc/mips/interlocked.inl>
#    else
#        error Architecture not implemented on GCC
#    endif
#elif defined(BE_COMPILER_SUNCC)
#    include <bugengine/kernel/suncc/interlocked.inl>
#elif defined(BE_COMPUTE)
#    include <bugengine/kernel/compute/interlocked.inl>
#else
#    error Compiler not implemented
#endif

namespace _Kernel {

template < typename T >
struct interlocked
{
private:
    typedef InterlockedType< sizeof(T) > impl;
    typedef typename impl::value_t       value_t;

private:
    value_t m_value;

public:
    __host __device static interlocked< T > create(T value)
    {
        interlocked< T > result;
        result.m_value = value_t(value);
        return result;
    }

    __host __device operator T() const
    {
        return static_cast< T >(m_value);
    }
    __host __device T operator=(T value)
    {
        return static_cast< T >(impl::set_and_fetch(&m_value, value));
    }
    __host __device T exchange(T value)
    {
        return static_cast< T >(impl::fetch_and_set(&m_value, value));
    }
    __host __device T addExchange(T value)
    {
        return static_cast< T >(impl::fetch_and_add(&m_value, value));
    }

    __host __device T operator++()
    {
        return static_cast< T >(impl::fetch_and_add(&m_value, 1) + 1);
    }
    __host __device T operator++(int)
    {
        return static_cast< T >(impl::fetch_and_add(&m_value, 1));
    }
    __host __device T operator+=(T value)
    {
        return static_cast< T >(impl::fetch_and_add(&m_value, value) + value);
    }
    __host __device T operator--()
    {
        return static_cast< T >(impl::fetch_and_sub(&m_value, 1) - 1);
    }
    __host __device T operator--(int)
    {
        return static_cast< T >(impl::fetch_and_sub(&m_value, 1));
    }
    __host __device T operator-=(T value)
    {
        return static_cast< T >(impl::fetch_and_sub(&m_value, value) - value);
    }

    __host __device T setConditional(T value, T condition)
    {
        return static_cast< T >(impl::set_conditional(&m_value, value, condition));
    }
};

}  // namespace _Kernel

template < typename T >
struct iptr
{
private:
    typedef _Kernel::InterlockedType< sizeof(T*) > impl;
    typedef typename impl::value_t                 value_t;

private:
    value_t m_value;

public:
    __host __device iptr(T* t) : m_value((typename impl::value_t)(t))
    {
    }
    __host __device operator const T*() const
    {
        return reinterpret_cast< T* >(impl::fetch_and_add(const_cast< value_t* >(&m_value), 0));
    }
    __host __device operator T*()
    {
        return reinterpret_cast< T* >(impl::fetch_and_add(&m_value, 0));
    }
    __host __device T* operator->()
    {
        return reinterpret_cast< T* >(impl::fetch_and_add(&m_value, 0));
    }
    __host __device const T* operator->() const
    {
        return reinterpret_cast< T* >(impl::fetch_and_add(&m_value, 0));
    }

    __host __device T* operator=(T* value)
    {
        return reinterpret_cast< T* >(impl::set_and_fetch((value_t*)&m_value, (value_t)value));
    }
    __host __device T* exchange(T* value)
    {
        return reinterpret_cast< T* >(impl::fetch_and_set((value_t*)&m_value, (value_t)value));
    }

    __host __device T* setConditional(T* value, T* condition)
    {
        return reinterpret_cast< T* >(
            impl::set_conditional((value_t*)&m_value, (value_t)value, (value_t)condition));
    }
};

typedef _Kernel::interlocked< bool >   i_bool;
typedef _Kernel::interlocked< u8 >     i_u8;
typedef _Kernel::interlocked< u16 >    i_u16;
typedef _Kernel::interlocked< u32 >    i_u32;
typedef _Kernel::interlocked< i8 >     i_i8;
typedef _Kernel::interlocked< i16 >    i_i16;
typedef _Kernel::interlocked< i32 >    i_i32;
typedef _Kernel::interlocked< size_t > i_size_t;

/**************************************************************************************************/
#endif
