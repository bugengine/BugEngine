/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_INTERLOCKED_MSVC_X86_INTERLOCKED_INL_
#define BE_MINITL_INTERLOCKED_MSVC_X86_INTERLOCKED_INL_
/*****************************************************************************/
#include    <intrin.h>
#pragma intrinsic(_InterlockedExchange)
#pragma intrinsic(_InterlockedExchangeAdd)
#pragma intrinsic(_InterlockedCompareExchange)
#pragma intrinsic(_InterlockedCompareExchange64)

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
    typedef BE_SET_ALIGNMENT(4) long value_t;
    typedef long incr_t;
    static inline value_t fetch_and_add(volatile value_t *p, incr_t incr)
    {
        return _InterlockedExchangeAdd(p, incr);
    }
    static inline value_t fetch_and_sub(volatile value_t *p, incr_t incr)
    {
        return _InterlockedExchangeAdd(p, -incr);
    }
    static inline value_t fetch_and_set(volatile value_t *p, incr_t v)
    {
        return _InterlockedExchange(p, v);
    }
    static inline value_t set_conditional(volatile value_t *p, incr_t v, incr_t condition)
    {
        return _InterlockedCompareExchange(p, v, condition);
    }
    static inline value_t set_and_fetch(volatile value_t *p, incr_t v)
    {
        _InterlockedExchange(p, v);
        return v;
    }


    struct tagged_t
    {
        typedef BE_SET_ALIGNMENT(4) long        value_t;
        typedef BE_SET_ALIGNMENT(4) long        counter_t;
        typedef long incr_t;
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
        tagged_t(incr_t value = 0)
        {
            taggedvalue.tag = 0;
            taggedvalue.value = value;
        }
        tagged_t(incr_t tag, incr_t value)
        {
            taggedvalue.tag = tag;
            taggedvalue.value = value;
        }
        tagged_t(const tagged_t& other)
            :   asLongLong(other.asLongLong)
        {
        }
        inline incr_t value() { return taggedvalue.value; }
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
    static inline bool set_conditional(tagged_t *p, incr_t v, tagged_t::tag_t& condition)
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

StaticAssert(sizeof(interlocked_impl::InterlockedType<4>::tagged_t) == 8);

}}
#pragma warning(pop)
/*****************************************************************************/
#endif
