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
#pragma intrinsic(_InterlockedCompareExchange)
#pragma intrinsic(_InterlockedCompareExchange64)
#define _InterlockedExchange64          InterlockedExchange64
#define _InterlockedExchangeAdd64       InterlockedExchangeAdd64

#include    <core/debug/assert.hh>

#pragma warning(push)
#pragma warning(disable:4521) //multiple copy constructor

namespace minitl { namespace interlocked_impl
{

template< unsigned size >
struct InterlockedType;

template<>
struct InterlockedType<4>
{
    typedef long value_t;
    static inline value_t fetch_and_add(volatile value_t *p, value_t incr)
    {
        return _InterlockedExchangeAdd(p, incr);
    }
    static inline value_t fetch_and_sub(volatile value_t *p, value_t incr)
    {
        return InterlockedExchangeAdd(p, -incr);
    }
    static inline value_t fetch_and_set(volatile value_t *p, value_t v)
    {
        return _InterlockedExchange(p, v);
    }
    static inline value_t set_conditional(volatile value_t *p, value_t v, value_t condition)
    {
        return _InterlockedCompareExchange(p, v, condition);
    }
    static inline value_t set_and_fetch(volatile value_t *p, value_t v)
    {
        _InterlockedExchange(p, v);
        return v;
    }


    struct tagged_t
    {
        typedef long        value_t;
        typedef long        counter_t;
        typedef tagged_t    tag_t;
        union
        {
            BE_SET_ALIGNMENT(8) struct _taggedvalue
            {
                volatile counter_t   tag;
                volatile value_t     value;
            } taggedvalue;
            BE_SET_ALIGNMENT(8) volatile long long asLongLong;
        };
        tagged_t(long long value)
            :   asLongLong(value)
        {
        }
        tagged_t(value_t value = 0)
        {
            taggedvalue.tag = 0;
            taggedvalue.value = value;
        }
        tagged_t(counter_t tag, value_t value)
        {
            taggedvalue.tag = tag;
            taggedvalue.value = value;
        }
        tagged_t(const tagged_t& other)
            :   asLongLong(other.asLongLong)
        {
        }
        inline value_t value() { return taggedvalue.value; }
        tagged_t& operator=(const tagged_t& other)
        {
            taggedvalue.tag = other.taggedvalue.tag;
            taggedvalue.value = other.taggedvalue.value;
            return *this;
        }
        inline bool operator==(tagged_t& other) { return asLongLong == other.asLongLong; }
    };
    static inline tagged_t::tag_t get_ticket(const tagged_t &p)
    {
        return p;
    }
    static inline bool set_conditional(tagged_t *p, value_t v, tagged_t::tag_t& condition)
    {
        tagged_t r(condition.taggedvalue.tag+1, v);
        return _InterlockedCompareExchange64(&(p->asLongLong), r.asLongLong, condition.asLongLong) == condition.asLongLong;
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
template<>
struct InterlockedType<8>
{
    typedef long long value_t;
    static inline value_t fetch_and_add(volatile value_t *p, value_t incr)
    {
        return InterlockedExchangeAdd64(p, incr);
    }
    static inline value_t fetch_and_sub(volatile value_t *p, value_t incr)
    {
        return InterlockedExchangeAdd64(p, -incr);
    }
    static inline value_t fetch_and_set(volatile value_t *p, value_t v)
    {
        return _InterlockedExchange64(p, v);
    }
    static inline value_t set_conditional(volatile value_t *p, value_t v, value_t condition)
    {
        return _InterlockedCompareExchange64(p, v, condition);
    }
    static inline value_t set_and_fetch(volatile value_t *p, value_t v)
    {
        _InterlockedExchange64(p, v);
        return v;
    }
};

StaticAssert(sizeof(interlocked_impl::InterlockedType<4>::tagged_t) == 8);

}}
#pragma warning(pop)
/*****************************************************************************/
#endif
