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

#ifndef BE_MINITL_INTERLOCKED_MSVC_X86_INTERLOCKED_INL_
#define BE_MINITL_INTERLOCKED_MSVC_X86_INTERLOCKED_INL_
/*****************************************************************************/
#include    <intrin.h>
#pragma intrinsic(_InterlockedExchange)
#pragma intrinsic(_InterlockedExchangeAdd)
#pragma intrinsic(_InterlockedOr)
#pragma intrinsic(_InterlockedAnd)
#pragma intrinsic(_InterlockedXor)
#pragma intrinsic(_InterlockedCompareExchange)
#pragma intrinsic(_InterlockedCompareExchange64)
#define _InterlockedExchange64          InterlockedExchange64
#define _InterlockedExchangeAdd64       InterlockedExchangeAdd64
#define _InterlockedOr64                InterlockedOr64
#define _InterlockedAnd64               InterlockedAnd64
#define _InterlockedXor64               InterlockedXor64

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
struct TaggedValue
{
    BE_SET_ALIGNMENT(8);
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
    TaggedValue& operator=(const TaggedValue& other)
    {
        tag = other.tag;
        value = other.value;
        return *this;
    }
    inline bool operator==(TaggedValue& other) { return tag == other.tag && value == other.value; }
};


inline long set_and_fetch(volatile long* dst, long value)
{
    _InterlockedExchange(dst, value);
    return value;
}

inline long long set_and_fetch(volatile long long* dst, long long value)
{
    
    (dst, value);
    return value;
}

inline void* set_and_fetch(void * volatile * dst, void* value)
{
    _InterlockedExchange((long volatile *)dst, (long)value);
    return value;
}


inline long fetch_and_set(volatile long* dst, long value)
{
    return _InterlockedExchange(dst, value);
}

inline long long fetch_and_set(volatile long long* dst, long long value)
{
    Assert((((size_t)dst) & 7) == 0);
    return _InterlockedExchange64(dst, value);
}

inline void* fetch_and_set(void * volatile * dst, void* value)
{
    return (void*)_InterlockedExchange((long volatile *)dst, (long)value);
}


inline long fetch_and_add(volatile long* dst, long value)
{
    return _InterlockedExchangeAdd(dst, value);
}

inline long long fetch_and_add(volatile long long* dst, long long value)
{
    Assert((((size_t)dst) & 7) == 0);
    return _InterlockedExchangeAdd64(dst, value)-value;
}


inline long add_and_fetch(volatile long* dst, long value)
{
    return _InterlockedExchangeAdd(dst, value)+value;
}

inline long long add_and_fetch(volatile long long* dst, long long value)
{
    Assert((((size_t)dst) & 7) == 0);
    return _InterlockedExchangeAdd64(dst, value)+value;
}


inline long fetch_and_sub(volatile long* dst, long value)
{
    return _InterlockedExchangeAdd(dst, -value);
}

inline long long fetch_and_sub(volatile long long* dst, long long value)
{
    Assert((((size_t)dst) & 7) == 0);
    return _InterlockedExchangeAdd64(dst, -value);
}


inline long sub_and_fetch(volatile long* dst, long value)
{
    return _InterlockedExchangeAdd(dst, -value)-value;
}

inline long long sub_and_fetch(volatile long long* dst, long long value)
{
    Assert((((size_t)dst) & 7) == 0);
    return _InterlockedExchangeAdd64(dst, -value)-value;
}


inline long or_and_fetch(volatile long* dst, long value)
{
    return _InterlockedOr(dst, value);
}

inline long long or_and_fetch(volatile long long* dst, long long value)
{
    Assert((((size_t)dst) & 7) == 0);
    return _InterlockedOr64(dst, value);
}


inline long xor_and_fetch(volatile long* dst, long value)
{
    return _InterlockedXor(dst, value);
}

inline long long xor_and_fetch(volatile long long* dst, long long value)
{
    Assert((((size_t)dst) & 7) == 0);
    return _InterlockedXor64(dst, value);
}


inline long and_and_fetch(volatile long* dst, long value)
{
    return _InterlockedAnd(dst, value);
}

inline long long and_and_fetch(volatile long long* dst, long long value)
{
    Assert((((size_t)dst) & 7) == 0);
    return _InterlockedAnd64(dst, value);
}


inline long set_conditional(volatile long* dst, long compare, long value)
{
    return _InterlockedCompareExchange(dst, compare, value);
}

inline long long set_conditional(volatile long long* dst, long long compare, long long value)
{
    Assert((((size_t)dst) & 7) == 0);
    return _InterlockedCompareExchange64(dst, compare, value);
}

inline void* set_conditional(void* volatile* dst, void* compare, void* value)
{
    return (void*)_InterlockedCompareExchange((long volatile *)dst, (long)compare, (long)value);
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
    return _InterlockedCompareExchange64((volatile long long*)t, m, ticket.asLongLong) == ticket.asLongLong;
}

}}

/*****************************************************************************/
#endif
