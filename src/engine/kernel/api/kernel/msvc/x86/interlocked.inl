/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_MINITL_INTERLOCKED_MSVC_X86_INTERLOCKED_INL_
#define BE_MINITL_INTERLOCKED_MSVC_X86_INTERLOCKED_INL_
/*****************************************************************************/

extern "C"
{
long  __cdecl _InterlockedCompareExchange(long volatile* Dest, long Exchange, long Comp);
long  __cdecl _InterlockedExchange(long volatile* Target, long Value);
long  __cdecl _InterlockedExchangeAdd(long volatile* Addend, long Value);
__int64  __cdecl _InterlockedCompareExchange64(__int64 volatile* Dest, __int64 Exchange, __int64 Comp);
#pragma intrinsic(_InterlockedExchange)
#pragma intrinsic(_InterlockedExchangeAdd)
#pragma intrinsic(_InterlockedCompareExchange)
#if _MSC_VER >= 1400
# pragma intrinsic(_InterlockedCompareExchange64)
#else
# define WIN32_LEAN_AND_MEAN
# define NOMINMAX
# include <windows.h>
# define _InterlockedCompareExchange64 InterlockedCompareExchange64
#endif
}

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
        typedef __declspec(align(4)) long        value_t;
        typedef __declspec(align(4)) long        counter_t;
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
    /*    bool result;
        _asm
        {
            mov         edi,ebp
            push        ebx
            mov         ecx, condition
            mov         eax, [ecx]
            mov         edx, [ecx+4]
            mov         esi, p
            mov         ebx, eax
            inc         ebx
            mov         ecx, v
            // lock        cmpxch8b esi;
            _emit 0xF0
            _emit 0x0F
            _emit 0xC7
            _emit 0x0E
            setz        [result];
            mov         ebp, edi
            pop         ebx;
        }
        return result;*/
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

/*****************************************************************************/
#endif
