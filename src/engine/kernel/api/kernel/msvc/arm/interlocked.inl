/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_KERNEL_MSVC_ARM_INTERLOCKED_INL_
#define BE_KERNEL_MSVC_ARM_INTERLOCKED_INL_
/**************************************************************************************************/
extern "C"
{
    long __cdecl _InterlockedCompareExchange(long volatile* dest, long exchange, long comp);
    long long __cdecl _InterlockedCompareExchange64(long long volatile* dest, long long exchange, long long comp);
    long __cdecl _InterlockedExchange(long volatile* target, long value);
    long __cdecl _InterlockedExchangeAdd(long volatile * addend, long value);
}
#pragma intrinsic(_InterlockedExchange)
#pragma intrinsic(_InterlockedExchangeAdd)
#pragma intrinsic(_InterlockedCompareExchange)
#pragma intrinsic(_InterlockedCompareExchange64)

#pragma warning(push)
#pragma warning(disable:4521) //multiple copy constructor

namespace _Kernel
{

template< unsigned size >
struct InterlockedType;

template<>
struct InterlockedType<4>
{
    typedef __declspec(align(4)) long value_t;
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
        typedef __declspec(align(4)) void*  value_t;
        typedef __declspec(align(4)) long   counter_t;
        typedef long incr_t;
        typedef tagged_t    tag_t;
        union
        {
            __declspec(align(8)) struct _taggedvalue
            {
                volatile counter_t   tag;
                volatile value_t     value;
            } taggedvalue;
            __declspec(align(8)) volatile long long asLongLong;
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

}

#pragma warning(pop)

/**************************************************************************************************/
#endif
