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

#ifndef BE_MINITL_INTERLOCKED_MSVC_AMD64_INTERLOCKED_INL_
#define BE_MINITL_INTERLOCKED_MSVC_AMD64_INTERLOCKED_INL_
/*****************************************************************************/
#include    <intrin.h>
#pragma intrinsic(_InterlockedExchange)
#pragma intrinsic(_InterlockedExchangeAdd)
#pragma intrinsic(_InterlockedCompareExchange)
#pragma intrinsic(_InterlockedExchange64)
#pragma intrinsic(_InterlockedExchangeAdd64)
#pragma intrinsic(_InterlockedCompareExchange64)
#if _MSC_VER >= 1500
# define TAG_LONG 1
# pragma intrinsic(_InterlockedCompareExchange128)
#else
# define TAG_SHORT 1
# pragma intrinsic(_InterlockedCompare64Exchange128)
#endif


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



    struct tagged_t
    {
        typedef long long   value_t;
        typedef long long   counter_t;
    #ifdef TAG_LONG
        typedef tagged_t    tag_t;
    #else
        typedef counter_t   tag_t;
    #endif
        BE_SET_ALIGNMENT(16)    volatile counter_t   m_tag;
        BE_SET_ALIGNMENT(8)     volatile value_t     m_value;
        tagged_t(value_t value = 0)
            :   m_tag(0)
            ,   m_value(value)
        {
        }
        tagged_t(counter_t tag, value_t value)
            :   m_tag(tag)
            ,   m_value(value)
        {
        }
        tagged_t(const tagged_t& other)
            :   m_tag(other.m_tag)
            ,   m_value(other.m_value)
        {
        }
        tagged_t& operator=(const tagged_t& other)
        {
            m_tag = other.m_tag;
            m_value = other.m_value;
            return *this;
        }
        inline value_t value() { return m_value; }
        inline bool operator==(tagged_t& other) { return m_tag == other.m_tag && m_value == other.m_value; }
    };
    static inline tagged_t::tag_t get_ticket(const tagged_t &p)
    {
        #ifdef TAG_LONG
            return p;
        #else
            return p.m_tag;
        #endif
    }
    static inline bool set_conditional(volatile tagged_t *p, value_t v, tagged_t::tag_t& condition)
    {
        #ifdef TAG_LONG
            return _InterlockedCompareExchange128((volatile i64*)p, v, p->m_tag+1, (i64*)&condition) != 0;
        #else
            return _InterlockedCompare64Exchange128((volatile i64*)p, v, condition+1, condition) == condition;
        #endif
    }
};

StaticAssert(sizeof(interlocked_impl::InterlockedType<8>::tagged_t) == 16);

}}
#pragma warning(pop)
/*****************************************************************************/
#endif
