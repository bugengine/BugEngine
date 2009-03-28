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

#ifndef BE_MINITL_INTERLOCKED_GCC_X86_INTERLOCKED_INL_
#define BE_MINITL_INTERLOCKED_GCC_X86_INTERLOCKED_INL_
/*****************************************************************************/
#include    <core/debug/assert.hh>

namespace minitl { namespace interlocked_
{

template< unsigned size >
struct InterlockedType;

template<>
struct InterlockedType<1>
{
    typedef long type;
};
template<>
struct InterlockedType<2>
{
    typedef long type;
};
template<>
struct InterlockedType<4>
{
    typedef long type;
};
template<>
struct InterlockedType<8>
{
    typedef long long type;
};
struct BE_SET_ALIGNMENT(4) TaggedValue
{
    typedef long        ValueType;
    typedef TaggedValue TagType;
    union
    {
        struct
        {
            long    tag;
            long    value;
        };
        long long asLongLong;
    };
    TaggedValue(long value = 0)
        :   tag(0)
        ,   value(value)
    {
    }
    TaggedValue(const volatile TaggedValue& other)
        :   tag(other.tag)
        ,   value(other.value)
    {
    }
    TaggedValue(const TaggedValue& other)
        :   tag(other.tag)
        ,   value(other.value)
    {
    }
    TaggedValue& operator=(const TaggedValue& other)
    {
        tag = other.tag;
        value = other.value;
        return *this;
    }
    inline bool operator==(TaggedValue& other) { return tag == other.tag && value == other.value; }
};

inline long set_conditional(volatile long* dst, long compare, long value)
{
    long prev;
    asm volatile ("lock; cmpxchgl %1, %2"
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
             : "=A"(result), "=m"(*(int64_t *)dst)
             : "m"(*(int64_t *)dst)
             , "0"(compare)
             , "c"(&value)
             : "memory", "esp"
#if __INTEL_COMPILER
             ,"ebx"
#endif
    );
#else
    union {
        int64_t i64;
        int32_t i32[2];
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
    asm volatile ("lock; cmpxchgl %1, %2"
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
    asm volatile ("xchgl %0, %1"
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
    asm volatile ("xchgl %0, %1"
                  : "=a" (prev), "+m" (*dst)
                  : "r" (value));
    return prev;
}


inline long fetch_and_add(volatile long* dst, long value)
{
    long old;
    asm volatile ("lock; xaddl %0,%1"
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
    asm volatile ("lock; xaddl %0,%1"
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
