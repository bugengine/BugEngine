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

#ifndef BE_MINITL_INTERLOCKED_GCC_AMD64_INTERLOCKED_INL_
#define BE_MINITL_INTERLOCKED_GCC_AMD64_INTERLOCKED_INL_
/*****************************************************************************/
#include    <core/debug/assert.hh>

namespace minitl { namespace interlocked_impl
{

template< unsigned size >
struct InterlockedType;

template<>
struct InterlockedType<1>
{
    typedef char value_t;

    static inline value_t fetch_and_add(volatile value_t *p, value_t incr)
    {
        value_t result;
        __asm__ __volatile__ ("lock; xaddb %0, %1"
                            : "=r" (result), "=m" (*p)
                            : "0" (incr), "m" (*p)
                            : "memory");
        return result;
    }
};

template<>
struct InterlockedType<2>
{
    typedef short value_t;
    inline value_t fetch_and_add(volatile value_t *p, value_t incr)
    {
        value_t result;
        __asm__ __volatile__ ("lock; xaddw %0, %1"
                            : "=r" (result), "=m" (*p) : "0" (incr), "m" (*p)
                            : "memory");
        return result;
    }

};

template<>
struct InterlockedType<4>
{
    typedef long value_t;
    inline value_t fetch_and_add(volatile value_t *p, value_t incr)
    {
        value_t result;
        __asm__ __volatile__ ("lock; xaddl %0, %1"
                            : "=r" (result), "=m" (*p) : "0" (incr), "m" (*p)
                            : "memory");
        return result;
    }
};

template<>
struct InterlockedType<8>
{
    typedef long long value_t;
    inline value_t fetch_and_add(volatile value_t *p, value_t incr)
    {
        value_t result;
        __asm__ __volatile__ ("lock; xaddq %0, %1"
                            : "=r" (result), "=m" (*p) : "0" (incr), "m" (*p)
                            : "memory");
        return result;
    }
};


inline long set_conditional(volatile long* dst, long compare, long value)
{
    long prev;
    __asm__ __volatile__ ("lock; cmpxchgl %1, %2"
                  : "=a" (prev)
                  : "r" (value), "m" (*(dst)), "0"(compare)
                  : "memory", "cc");
    return prev;
}

inline long long set_conditional(volatile long long* dst, long long compare, long long value)
{
    long long result;
#if __PIC__
    __asm__ __volatile__ (
            "pushl %%ebx\n\t"
            "movl  (%%ecx),%%ebx\n\t"
            "movl  4(%%ecx),%%ecx\n\t"
            "lock\n\t cmpxchg8b %1\n\t"
            "popl  %%ebx"
             : "=A"(result), "=m"(*(i64 *)dst)
             : "m"(*(i64 *)dst)
             , "0"(compare)
             , "c"(&value)
             : "memory", "esp"
#if defined(BE_COMPILER_INTEL)
             ,"ebx"
#endif
    );
#else
    union {
        i64 i64;
        i32 i32[2];
    };
    i64 = value;
    __asm__ __volatile__ (
            "lock\n\t cmpxchg8b %1\n\t"
             : "=A"(result), "=m"(*dst)
             : "m"(*dst)
             , "0"(compare)
             , "b"(i32[0]), "c"(i32[1])
             : "memory"
    );
#endif
    return result;
}

inline void* set_conditional(void* volatile* dst, void* compare, void* value)
{
    void* prev;
    __asm__ volatile ("lock; cmpxchgl %1, %2"
                  : "=a" (prev)
                  : "r" (value), "m" (*(dst)), "0"(compare)
                  : "memory", "cc");
    return prev;
}

inline TaggedValue get_ticket(volatile TaggedValue* t)
{
    Assert((((size_t)t) & 7) == 0);
    return *t;
}

inline bool set_conditional(volatile TaggedValue* t, long value, TaggedValue& ticket)
{
    Assert((((size_t)t) & 7) == 0);
    long long m = (((long long)value) << 32) + ticket.tag+1;
    return set_conditional((volatile long long*)t, ticket.asLongLong, m) == ticket.asLongLong;
}

inline long set_and_fetch(volatile long* dst, long value)
{
    *dst = value;
    return value;
}

inline long long set_and_fetch(volatile long long* dst, long long value)
{
    *dst = value;
    return value;
}

inline void* set_and_fetch(void * volatile * dst, void* value)
{
    *dst = value;
    return value;
}


inline long fetch_and_set(volatile long* dst, long value)
{
    long prev;
    __asm__ __volatile__ ("xchgl %0, %1"
                  : "=a" (prev), "+m" (*dst)
                  : "r" (value));
    return prev;
}

inline long long fetch_and_set(volatile long long* dst, long long value)
{
    Assert(false);
    return 0;
}

inline void* fetch_and_set(void * volatile * dst, void* value)
{
    void* prev;
    __asm__ __volatile__ ("xchgl %0, %1"
                  : "=a" (prev), "+m" (*dst)
                  : "r" (value));
    return prev;
}


inline long fetch_and_add(volatile long* dst, long value)
{
    long old;
    __asm__ __volatile__ ("lock; xaddl %0,%1"
                  : "=a" (old), "=m" (*dst)
                  : "a" (value), "m" (*dst)
                  : "memory", "cc");
    return old;
}

inline long long fetch_and_add(volatile long long* dst, long long value)
{
    Assert(false);
    return 0;
}


inline long add_and_fetch(volatile long* dst, long value)
{
    long old;
    __asm__ __volatile__ ("lock; xaddl %0,%1"
                  : "=a" (old), "=m" (*dst)
                  : "a" (value), "m" (*dst)
                  : "memory", "cc");
    return old+value;
}

inline long long add_and_fetch(volatile long long* dst, long long value)
{
    Assert(false);
    return 0;
}


inline long fetch_and_sub(volatile long* dst, long value)
{
    return fetch_and_add(dst, -value);
}

inline long long fetch_and_sub(volatile long long* dst, long long value)
{
    return fetch_and_add(dst, -value);
}


inline long sub_and_fetch(volatile long* dst, long value)
{
    return add_and_fetch(dst, -value);
}

inline long long sub_and_fetch(volatile long long* dst, long long value)
{
    return add_and_fetch(dst, -value);
}


inline long or_and_fetch(volatile long* dst, long value)
{
    Assert(false);
    return 0;
}

inline long long or_and_fetch(volatile long long* dst, long long value)
{
    Assert(false);
    return 0;
}


inline long xor_and_fetch(volatile long* dst, long value)
{
    Assert(false);
    return 0;
}

inline long long xor_and_fetch(volatile long long* dst, long long value)
{
    Assert(false);
    return 0;
}


inline long and_and_fetch(volatile long* dst, long value)
{
    Assert(false);
    return 0;
}

inline long long and_and_fetch(volatile long long* dst, long long value)
{
    Assert(false);
    return 0;
}

}}

/*****************************************************************************/
#endif
