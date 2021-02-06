/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_KERNEL_MSVC_ARM64_INTERLOCKED_INL_
#define BE_KERNEL_MSVC_ARM64_INTERLOCKED_INL_
/**************************************************************************************************/
#include <bugengine/kernel/stdafx.h>

#include <intrin.h>
#define be_InterlockedCompareExchange128(addr, v1, v2, c)                                          \
    return _InterlockedCompareExchange128(addr, v1, v2, c) != 0

#pragma warning(push)
#pragma warning(disable : 4521)  // multiple copy constructor

namespace _Kernel {

template < unsigned size >
struct InterlockedType;

template <>
struct InterlockedType< 4 >
{
    typedef __declspec(align(4)) long value_t;
    typedef long          incr_t;
    static inline value_t fetch_and_add(volatile value_t* p, incr_t incr)
    {
        return _InterlockedExchangeAdd(p, incr);
    }
    static inline value_t fetch_and_sub(volatile value_t* p, incr_t incr)
    {
        return _InterlockedExchangeAdd(p, -incr);
    }
    static inline value_t fetch_and_set(volatile value_t* p, incr_t v)
    {
        return _InterlockedExchange(p, v);
    }
    static inline value_t set_conditional(volatile value_t* p, incr_t v, incr_t condition)
    {
        return _InterlockedCompareExchange(p, v, condition);
    }
    static inline value_t set_and_fetch(volatile value_t* p, incr_t v)
    {
        _InterlockedExchange(p, v);
        return v;
    }
};
template <>
struct InterlockedType< 1 > : public InterlockedType< 4 >
{
};
template <>
struct InterlockedType< 2 > : public InterlockedType< 4 >
{
};
template <>
struct InterlockedType< 8 >
{
    typedef __declspec(align(8)) long long value_t;
    typedef long long     incr_t;
    static inline value_t fetch_and_add(volatile value_t* p, incr_t incr)
    {
        return _InterlockedExchangeAdd64(p, incr);
    }
    static inline value_t fetch_and_sub(volatile value_t* p, incr_t incr)
    {
        return _InterlockedExchangeAdd64(p, -incr);
    }
    static inline value_t fetch_and_set(volatile value_t* p, incr_t v)
    {
        return _InterlockedExchange64(p, v);
    }
    static inline value_t set_conditional(volatile value_t* p, incr_t v, incr_t condition)
    {
        return _InterlockedCompareExchange64(p, v, condition);
    }
    static inline value_t set_and_fetch(volatile value_t* p, incr_t v)
    {
        _InterlockedExchange64(p, v);
        return v;
    }

    struct tagged_t
    {
        typedef __declspec(align(8)) void* value_t;
        typedef __declspec(align(8)) long long counter_t;
        typedef tagged_t tag_t;
        __declspec(align(16)) volatile counter_t m_tag;
        __declspec(align(8)) volatile value_t m_value;
        tagged_t(value_t value = 0) : m_tag(0), m_value(value)
        {
        }
        tagged_t(counter_t tag, value_t value) : m_tag(tag), m_value(value)
        {
        }
        tagged_t(const tagged_t& other) : m_tag(other.m_tag), m_value(other.m_value)
        {
        }
        tagged_t& operator=(const tagged_t& other)
        {
            m_tag   = other.m_tag;
            m_value = other.m_value;
            return *this;
        }
        inline value_t value()
        {
            return m_value;
        }
        inline bool operator==(tagged_t& other)
        {
            return m_tag == other.m_tag && m_value == other.m_value;
        }
    };
    static inline tagged_t::tag_t get_ticket(const tagged_t& p)
    {
        return p;
    }
    static inline bool set_conditional(volatile tagged_t* p, tagged_t::value_t v,
                                       const tagged_t::tag_t& condition)
    {
        be_InterlockedCompareExchange128((volatile i64*)p, (i64)v, p->m_tag + 1, (i64*)&condition);
    }
};

}  // namespace _Kernel

#pragma warning(pop)

/**************************************************************************************************/
#endif
