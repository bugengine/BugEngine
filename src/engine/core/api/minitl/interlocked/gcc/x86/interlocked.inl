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

namespace minitl { namespace interlocked_impl
{

template< unsigned size >
struct InterlockedType;


template<>
struct InterlockedType<8>
{
    typedef long long value_t;
    static inline value_t fetch_and_add(volatile value_t *p, value_t incr)
    {
        value_t old;
        __asm__ __volatile__ ("lock; xaddq %0,%1"
                      : "=a" (old), "=m" (*p)
                      : "a" (incr), "m" (*p)
                      : "memory", "cc");
        return old;
    }
    static inline value_t fetch_and_sub(volatile value_t *p, value_t incr)
    {
        return fetch_and_add(p, -incr);
    }
    static inline value_t fetch_and_set(volatile value_t *p, value_t v)
    {
        value_t prev;
        __asm__ __volatile__ ("xchgw %0, %1"
                      : "=a" (prev), "+m" (*p)
                      : "r" (v));
        return prev;
    }
    static inline value_t set_conditional(volatile value_t *p, value_t v, value_t condition)
    {
        value_t result;
    #if __PIC__
        __asm__ __volatile__ (
                "pushl %%ebx\n\t"
                "movl  (%%ecx),%%ebx\n\t"
                "movl  4(%%ecx),%%ecx\n\t"
                "lock\n\t cmpxchg8b %1\n\t"
                "popl  %%ebx"
                 : "=A"(result), "=m"(*(i64 *)p)
                 : "m"(*(i64 *)p), "0"(condition), "c"(&v)
                 : "memory", "esp"
    #if defined(BE_COMPILER_INTEL)
                 ,"ebx"
    #endif
        );
    #else
        union {
            i64 asI64;
            i32 asI32[2];
        };
        asI64 = v;
        __asm__ __volatile__ (
                "lock\n\t cmpxchg8b %1\n\t"
                 : "=A"(result), "=m"(*p)
                 : "m"(*p), "0"(condition), "b"(asI32[0]), "c"(asI32[1])
                 : "memory"
        );
    #endif
        return result;
    }
    static inline value_t set_and_fetch(volatile value_t *p, value_t v)
    {
        fetch_and_set(p, v);
        return v;
    }
};


template<>
struct InterlockedType<4>
{
    typedef long value_t;
    static inline value_t fetch_and_add(volatile value_t *p, value_t incr)
    {
        value_t old;
        __asm__ __volatile__ ("lock; xaddl %0,%1"
                      : "=a" (old), "=m" (*p)
                      : "a" (incr), "m" (*p)
                      : "memory", "cc");
        return old;
    }
    static inline value_t fetch_and_sub(volatile value_t *p, value_t incr)
    {
        return fetch_and_add(p, -incr);
    }
    static inline value_t fetch_and_set(volatile value_t *p, value_t v)
    {
        long prev;
        __asm__ __volatile__ ("xchgl %0, %1"
                      : "=a" (prev), "+m" (*p)
                      : "r" (v));
        return prev;
    }
    static inline value_t set_conditional(volatile value_t *p, value_t v, value_t condition)
    {
        long prev;
        __asm__ __volatile__ ("lock; cmpxchgl %1, %2"
                      : "=a" (prev)
                      : "r" (v), "m" (*(p)), "0"(condition)
                      : "memory", "cc");
        return prev;
    }
    static inline value_t set_and_fetch(volatile value_t *p, value_t v)
    {
        fetch_and_set(p, v);
        return v;
    }


    struct tagged_t
    {
        typedef long        value_t;
        typedef long        counter_t;
        typedef tagged_t    tag_t;
        union
        {
            BE_SET_ALIGNMENT(8) struct
            {
                counter_t   tag;
                value_t     value;
            };
            BE_SET_ALIGNMENT(8) long long asLongLong;
        };
        tagged_t(long long value)
            :   asLongLong(value)
        {
        }
        tagged_t(value_t value = 0)
            :   tag(0)
            ,   value(value)
        {
        }
        tagged_t(counter_t tag, value_t value)
            :   tag(0)
            ,   value(value)
        {
        }
        tagged_t(const tagged_t& other)
            :   tag(other.tag)
            ,   value(other.value)
        {
        }
        tagged_t(const volatile tagged_t& other)
            :   tag(other.tag)
            ,   value(other.value)
        {
        }
        tagged_t& operator=(const tagged_t& other)
        {
            tag = other.tag;
            value = other.value;
            return *this;
        }
        inline bool operator==(tagged_t& other) { return tag == other.tag && value == other.value; }
    };
    static inline tagged_t::tag_t get_ticket(const volatile tagged_t &p)
    {
        return p;
    }
    static inline bool set_conditional(volatile tagged_t *p, value_t v, tagged_t::tag_t& condition)
    {
        return InterlockedType<8>::set_conditional((i64*)p, v, condition.asLongLong) == condition.asLongLong;
    }
};

template<>
struct InterlockedType<1> : public InterlockedType<4>
{
};

template<>
struct InterlockedType<2> : public InterlockedType<4>
{
};


}}

/*****************************************************************************/
#endif
